#include <sys/types.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <sys/thr.h>
#include <time.h>
#include <ps4-offsets/kernel.h>

#if defined(__9_00__)
asm("ps4kexec:\n.incbin \"ps4-kexec-900-baikal/kexec.bin\"\nps4kexec_end:\n");
#include "magic.h"
#elif defined(__9_03__)
asm("ps4kexec:\n.incbin \"ps4-kexec-903-baikal/kexec.bin\"\nps4kexec_end:\n");
#include "magic.h"
#elif defined(__9_60__)
asm("ps4kexec:\n.incbin \"ps4-kexec-960-baikal/kexec.bin\"\nps4kexec_end:\n");
#include "magic.h"
#elif defined(__10_00__)
asm("ps4kexec:\n.incbin \"ps4-kexec-1000-baikal/kexec.bin\"\nps4kexec_end:\n");
#include "magic.h"
#elif defined(__10_50__)
asm("ps4kexec:\n.incbin \"ps4-kexec-1050-baikal/kexec.bin\"\nps4kexec_end:\n");
#include "magic.h"
#elif defined(__11_00__)
asm("ps4kexec:\n.incbin \"ps4-kexec-1100-baikal/kexec.bin\"\nps4kexec_end:\n");
#include "magic.h"
#elif defined(__11_50__)
asm("ps4kexec:\n.incbin \"ps4-kexec-1150-baikal/kexec.bin\"\nps4kexec_end:\n");
#include "magic.h"
#elif defined(__12_00__)
asm("ps4kexec:\n.incbin \"ps4-kexec-1200-baikal/kexec.bin\"\nps4kexec_end:\n");
#include "magic.h"
#else
#error "unsupported firmware"
#endif

extern char ps4kexec[];
extern char ps4kexec_end[];

void kexec(void* f, void* u);

unsigned long long get_syscall(void)
{
    unsigned int eax, ecx, edx;
    ecx = 0xc0000082;
    asm volatile("rdmsr":"=a"(eax),"=d"(edx):"c"(ecx));
    return ((unsigned long long)edx) << 32 | eax;
}

void kernel_main()
{
    unsigned long long kernel_base = get_syscall() - kernel_offset_xfast_syscall;
    asm volatile("cli\nmov %%cr0, %%rax\nbtc $16, %%rax\nmov %%rax, %%cr0":::"rax");
    *(char*)(kernel_base + kernel_patch_kmem_alloc_1) = 0x07;
    *(char*)(kernel_base + kernel_patch_kmem_alloc_2) = 0x07;
    //set pstate before shutdown, needed for PS4 Pro console
    *(char*)(kernel_base + kern_off_pstate_before_shutdown) = 0x03;
    asm volatile("mov %%cr0, %%rax\nbts $16, %%rax\nmov %%rax, %%cr0\nsti":::"rax");
        
    unsigned long long early_printf = kernel_base + kernel_offset_printf;
    unsigned long long kmem_alloc = kernel_base + kernel_offset_kmem_alloc;
    unsigned long long kernel_map = kernel_base + kernel_offset_kernel_map;
    char* new_ps4_kexec = ((char*(*)(unsigned long long, unsigned long long))kmem_alloc)(*(unsigned long long*)kernel_map, ps4kexec_end-ps4kexec);
    for(int i = 0; ps4kexec + i != ps4kexec_end; i++)
        new_ps4_kexec[i] = ps4kexec[i];
    ((void(*)(void*, void*))new_ps4_kexec)((void*)early_printf, NULL);
}

asm("kexec_load:\nmov %rcx, %r10\nmov $153, %rax\nsyscall\nret");

int kexec_load(char* kernel, unsigned long long kernel_size, char* initrd, unsigned long long initrd_size, char* cmdline, int vram_gb);

int read_file(char* path, char** ptr, unsigned long long* sz)
{
    int fd = open(path, O_RDONLY);
    if(fd < 0)
        return -1;
    *sz = lseek(fd, 0, SEEK_END);
    *ptr = mmap(NULL, *sz, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    char* p = *ptr;
    unsigned long long l = *sz;
    lseek(fd, 0, SEEK_SET);
    while(l)
    {
        unsigned long long chk = read(fd, p, l);
        if(chk <= 0)
            return -1;
        p += chk;
        l -= chk;
    }
    close(fd);
    return 0;
}

int evf_open(char*);
void evf_cancel(int, unsigned long long, unsigned long long);
void evf_close(int);

void reboot_thread(void* _)
{
    nanosleep((const struct timespec*)"\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", NULL);
    int evf = evf_open("SceSysCoreReboot");
    evf_cancel(evf, 0x4000, 0);
    evf_close(evf);
    kill(1, SIGUSR1);
}

void* dlopen(const char*, int);
void* dlsym(void*, const char*);

void alert(const char* msg)
{
    static int(*sceSysUtilSendSystemNotificationWithText)(int, const char*);
    if(!sceSysUtilSendSystemNotificationWithText)
    {
        void* handle = dlopen("/system/common/lib/libSceSysUtil.sprx", 0);
        sceSysUtilSendSystemNotificationWithText = dlsym(handle, "sceSysUtilSendSystemNotificationWithText");
    }
    sceSysUtilSendSystemNotificationWithText(222, msg);
}

int my_atoi(const char *s)
{
    int ret = 0;
    int neg = 0;
    while(*s == ' ')
        s++;
    neg = (*s == '-') ? 1 : 0;
    for(; *s; s++)
    {
        char c = *s;
        if('0' <= c && c <= '9')
        {
            ret *= 10;
            ret += c-'0';
        }
        else
            break;
    }
    return (neg) ? (-ret) : (ret);
}

#ifndef VRAM_GB_DEFAULT
#define VRAM_GB_DEFAULT 1
#endif

#ifndef VRAM_GB_MIN
#define VRAM_GB_MIN 1
#endif

#ifndef VRAM_GB_MAX
#define VRAM_GB_MAX 5
#endif

#ifndef HDD_BOOT_PATH
#define HDD_BOOT_PATH "/data/linux/boot/"
#endif

int main()
{
    struct sigaction sa = {
        .sa_handler = SIG_IGN,
        .sa_flags = 0,
    };
    // note: overriding SIGSTOP and SIGKILL requires a kernel patch
    sigaction(SIGSTOP, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGKILL, &sa, NULL);
    char* kernel = NULL;
    unsigned long long kernel_size = 0;
    char* initrd = NULL;
    unsigned long long initrd_size = 0;
    char* cmdline = NULL;
    unsigned long long cmdline_size = 0;
    char* vramstr = NULL;
    unsigned long long vramstr_size = 0;
    int vramgb = 0;

#define L(name, where, wheresz, is_fatal)\
    if(read_file("/mnt/usb0/" name, where, wheresz)\
    && read_file("/mnt/usb1/" name, where, wheresz)\
    && read_file(HDD_BOOT_PATH name, where, wheresz)\
    && is_fatal)\
    {\
        alert("Failed to load file: " name ".\nPaths checked:\n/mnt/usb0/" name "\n/mnt/usb1/" name "\n" HDD_BOOT_PATH name);\
        return 1;\
    }
    L("bzImage", &kernel, &kernel_size, 1);
    L("initramfs.cpio.gz", &initrd, &initrd_size, 1);

    L("bootargs.txt", &cmdline, &cmdline_size, 0);

    if(cmdline && cmdline_size)
    {
        for(int i = 0; i < cmdline_size; i++)
            if(cmdline[i] == '\n')
            {
                cmdline[i] = '\0';
                break;
            }
    }
    else
        cmdline = "panic=0 clocksource=tsc consoleblank=0 net.ifnames=0 radeon.dpm=0 amdgpu.dpm=0 drm.debug=0 console=ttyS0,115200n8 console=tty0 video=HDMI-A-1:1920x1080@60";

    // video=HDMI-A-1:1920x1080-24@60      drm.edid_firmware=edid/1920x1080.bin  drm.edid_firmware=edid/my_edid.bin

    L("vram.txt", &vramstr, &vramstr_size, 0);
    if(vramstr && vramstr_size)
    {
        vramgb = my_atoi(vramstr);
        if(vramgb < VRAM_GB_MIN || vramgb > VRAM_GB_MAX)
            vramgb = VRAM_GB_DEFAULT;
    }
    else
        vramgb = VRAM_GB_DEFAULT;

    kexec(kernel_main, (void*)0);
    long x, y;
    struct thr_param thr = {
        .start_func = reboot_thread,
        .arg = NULL,
        .stack_base = mmap(NULL, 16384, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0),
        .stack_size = 16384,
        .tls_base = NULL,
        .tls_size = 0,
        .child_tid = &x,
        .parent_tid = &y,
        .flags = 0,
        .rtp = NULL
    };
    thr_new(&thr, sizeof(thr));
    kexec_load(kernel, kernel_size, initrd, initrd_size, cmdline, vramgb);
    for(;;);
}
