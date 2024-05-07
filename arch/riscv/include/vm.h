/*
 * @Author: TroyMitchell
 * @Date: 2024-04-23
 * @LastEditors: TroyMitchell
 * @LastEditTime: 2024-05-07
 * @FilePath: /caffeinix/arch/riscv/include/vm.h
 * @Description: 
 * Words are cheap so I do.
 * Copyright (c) 2024 by TroyMitchell, All Rights Reserved. 
 */
#ifndef __CAFFEINIX_ARCH_RISCV_VM_H
#define __CAFFEINIX_ARCH_RISCV_VM_H

#include <riscv.h>

/* For kernel */
void kvm_create(void);
void kvm_init(void);
/* For general */
int vm_map(pagedir_t pgdir, uint64 va, uint64 pa, uint64 size, int perm);
void vm_unmap(pagedir_t pgdir, uint64 va, uint64 npages, int do_free);
pte_t *PTE(pagedir_t pgdir, uint64 va, int flag);
/* For page-table */
pagedir_t pagedir_alloc(void);
void pagedir_free(pagedir_t pgdir);

int copyout(pagedir_t pgdir, uint64 dstva, char* src, uint64 len);
int copyin(pagedir_t pgdir, char* dst, uint64 srcva, uint64 len);
int copyinstr(pagedir_t pgdir, char *dst, uint64 srcva, uint64 max);
#endif