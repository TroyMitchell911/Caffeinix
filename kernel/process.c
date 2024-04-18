#include <process.h>
#include <palloc.h>
#include <mem_layout.h>
#include <kernel_config.h>
#include <vm.h>
#include <scheduler.h>
#include <string.h>

/* From trampoline.S */
extern char trampoline[];

static struct spinlock pid_lock;
static struct process proc[NPROC];
static int next_pid = 1;

static pagedir_t proc_pagedir(process_t p)
{
        int ret;
        /* Malloc memory for page-talble */
        pagedir_t pgdir = (pagedir_t)palloc();
        if(!pgdir) {
                return 0;
        }
        /* Clear the memory */
        memset(pgdir, 0, PGSIZE);
        /* Map highest address to trampoline */
        ret = vm_map(pgdir, TRAMPOLINE, (uint64)trampoline, PTE_R | PTE_X, 1);
        if(ret) {
                /* Something */
        }
        /* Map address of under trampoline to trapframe */
        ret = vm_map(pgdir, TRAPFRAME, (uint64)p->trapframe, PTE_W | PTE_R, 1);
        if(ret){
                /* Something */
        }
        return pgdir;
}

/* This function is the first when a process first start */
static void proc_first_start(void)
{
        /* The function scheduler will acquire the lock */
        spinlock_release(&cur_proc()->lock);
}

/* All processes have a alone id, pid */
static int pid_alloc(void)
{
        int pid;
        spinlock_acquire(&pid_lock);
        pid = next_pid ++;
        spinlock_release(&pid_lock);
        return pid;
}

/* Alloc a process */
process_t process_alloc(void)
{
        process_t process;
        for(process = proc; process != &proc[NPROC - 1]; process++) {
                spinlock_acquire(&process->lock);
                if(process->state == UNUSED) {
                        goto found;
                }
                spinlock_release(&process->lock);
        }
found:
        /* Alloc memory for trapframe */
        process->trapframe = (trapframe_t)palloc();
        if(!process->trapframe) {
                goto r1;
        }
        /* Alloc memory for page-table */
        process->pagetable = proc_pagedir(process);
        if(!process->pagetable) {
                goto r2;
        }
        
        /* Alloc pid */
        process->pid = pid_alloc();
        /* Set the address of kernel stack */
        process->kstack = KSTACK((int)(process - proc));
        /* Clear the context of process */
        memset(&process->context, 0, sizeof(struct context));
        /* Set the context of stack pointer */
        process->context.sp = process->kstack + PGSIZE;
        /* Set the context of return address */
        process->context.ra = (uint64)(proc_first_start);

        return process;
r2:
        pfree(process->trapframe);
r1:
        spinlock_release(&process->lock);
        return 0;
}

void process_map_kernel_stack(pagedir_t pgdir)
{
        process_t p = proc;
        uint64 pa;
        uint64 va;
        /* Assign kernel stack space to each process and map it */
        for(; p <= &proc[NCPU - 1]; p++) {
                pa = (uint64)palloc();
                if(!pa) {
                        PANIC("process_map_kernel_stack");
                }
                va = KSTACK((int)(p - proc));
                vm_map(pgdir, va, pa, PGSIZE, PTE_R | PTE_W);
        }
}

void process_init(void)
{
        process_t p = proc;
        /* Init the spinlock */
        spinlock_init(&pid_lock, "pid_lock");
        /* Set the state and starting kernel stack address of each process */
        for(; p <= &proc[NCPU - 1]; p++) {
                spinlock_init(&p->lock, "proc");
                p->state = UNUSED;
                p->kstack = KSTACK((int)(p - proc));
        }
}