#pragma once
#define kernel_offset_xfast_syscall 0x1c0
#define kernel_offset_allproc 0x1b906e0
#define kernel_offset_vmspace_acquire_ref 0x7b9e0
#define kernel_offset_vmspace_free 0x7b810
#define kernel_offset_printf 0xb79e0
#define kernel_offset_kmem_alloc 0x37a070
#define kernel_offset_kernel_map 0x2264d48
#define kernel_offset_sysent 0x10fc310
#define kernel_offset_proc_rwmem 0x41ca70
#define kernel_offset_copyin 0x271320

#define kernel_patch_kmem_alloc_1 0x37a13c
#define kernel_patch_kmem_alloc_2 0x37a144
