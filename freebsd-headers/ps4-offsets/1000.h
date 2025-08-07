#pragma once
#define kernel_offset_xfast_syscall 0x1c0
#define kernel_offset_allproc 0x22D9B40
#define kernel_offset_vmspace_acquire_ref 0x38CEE0
#define kernel_offset_vmspace_free 0x38CD10
#define kernel_offset_printf 0xC50F0
#define kernel_offset_kmem_alloc 0x33B040
#define kernel_offset_kernel_map 0x227BEF8
#define kernel_offset_sysent 0x1102D90
#define kernel_offset_proc_rwmem 0x44DC40
#define kernel_offset_copyin 0x472F10

#define kernel_patch_kmem_alloc_1 0x33b10c
#define kernel_patch_kmem_alloc_2 0x33B114
