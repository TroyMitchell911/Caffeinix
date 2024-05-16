/*
 * @Author: TroyMitchell
 * @Date: 2024-04-25 09:22
 * @LastEditors: TroyMitchell
 * @LastEditTime: 2024-05-16
 * @FilePath: /caffeinix/kernel/include/process.h
 * @Description: 
 * Words are cheap so I do.
 * Copyright (c) 2024 by ${TroyMitchell}, All Rights Reserved. 
 */
#ifndef __CAFFEINIX_KERNEL_PROCESS_H
#define __CAFFEINIX_KERNEL_PROCESS_H

#include <thread.h>
#include <spinlock.h>
#include <riscv.h>
#include <file.h>

#define MAXNAME                         16

/* TODO:Delete this macro */
// #define PROCESS_NO_SCHED                1

typedef struct inode *inode_t;

typedef struct trapframe {
        /* kernel page table */
        /*   0 */ uint64 kernel_satp; 
        /* top of process's kernel stack */
        /*   8 */ uint64 kernel_sp; 
        /* usertrap() */   
        /*  16 */ uint64 kernel_trap; 
        /* saved user program counter */  
        /*  24 */ uint64 epc; 
        /* saved kernel tp */      
        /*  32 */ uint64 kernel_hartid; 
        /*  40 */ uint64 ra;
        /*  48 */ uint64 sp;
        /*  56 */ uint64 gp;
        /*  64 */ uint64 tp;
        /*  72 */ uint64 t0;
        /*  80 */ uint64 t1;
        /*  88 */ uint64 t2;
        /*  96 */ uint64 s0;
        /* 104 */ uint64 s1;
        /* 112 */ uint64 a0;
        /* 120 */ uint64 a1;
        /* 128 */ uint64 a2;
        /* 136 */ uint64 a3;
        /* 144 */ uint64 a4;
        /* 152 */ uint64 a5;
        /* 160 */ uint64 a6;
        /* 168 */ uint64 a7;
        /* 176 */ uint64 s2;
        /* 184 */ uint64 s3;
        /* 192 */ uint64 s4;
        /* 200 */ uint64 s5;
        /* 208 */ uint64 s6;
        /* 216 */ uint64 s7;
        /* 224 */ uint64 s8;
        /* 232 */ uint64 s9;
        /* 240 */ uint64 s10;
        /* 248 */ uint64 s11;
        /* 256 */ uint64 t3;
        /* 264 */ uint64 t4;
        /* 272 */ uint64 t5;
        /* 280 */ uint64 t6;
}*trapframe_t;

typedef enum process_state{
        UNUSED,
        ALLOCED,
        RUNNABLE,
        RUNNING,
        SLEEPING,
        ZOMBIE,
}process_state_t;

typedef struct process{
        char name[MAXNAME];
        int pid;
        struct spinlock lock;  
        process_state_t state;
        uint64 kstack;
        uint64 sz;
        pagedir_t pagetable;
        trapframe_t trapframe;
        struct context context;
        inode_t cwd;
        file_t ofile[NOFILE];
        void *sleep_chan;
        struct process *parent;
}*process_t;

void process_map_kernel_stack(pagedir_t pgdir);
void process_init(void);
pagedir_t process_pagedir(process_t p);
void process_freepagedir(pagedir_t pgdir, uint64 sz);
int process_grow(int n);

void sleep(void* chan, spinlock_t lk);
void wakeup(void* chan);
void sleep_(void* chan, spinlock_t lk);
void wakeup_(void* chan);
int either_copyout(int user_dst, uint64 dst, void* src, uint64 len);
int either_copyin(void *dst, int user_src, uint64 src, uint64 len);
/* User init for first process */
void userinit(void);
#endif