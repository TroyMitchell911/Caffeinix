/*
 * @Author: TroyMitchell
 * @Date: 2024-05-08
 * @LastEditors: TroyMitchell
 * @LastEditTime: 2024-05-14
 * @FilePath: /caffeinix/user/init.c
 * @Description: 
 * Words are cheap so I do.
 * Copyright (c) 2024 by TroyMitchell, All Rights Reserved. 
 */
#include "user.h"
#include "../kernel/include/myfcntl.h"

#define CONSOLE                 1  
int main(void){
        int ret, fd, pid;
        char buf[128];
        
        fd = open("console", O_RDWR);
        if(fd == -1) {
                ret = mknod("console", 1, 0);
                if(ret == 0) {
                        fd = open("console", O_RDWR);
                }
        }
        if(fd != -1)
                fd = dup(fd);

        /* For pid test */
        pid = getpid();
        printf("Get pid: %d\n", pid);
        for(;;) {
                if(fd != -1) {
                        ret = read(fd, buf, 128);
                        buf[ret] = '\0';
                        if(ret != 0) {
                                fprintf(fd, "From user: %s", buf);
                        }
                }  
        }
        return 0;
}