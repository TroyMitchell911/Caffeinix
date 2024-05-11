/*
 * @Author: TroyMitchell
 * @Date: 2024-05-08
 * @LastEditors: TroyMitchell
 * @LastEditTime: 2024-05-11
 * @FilePath: /caffeinix/user/init.c
 * @Description: 
 * Words are cheap so I do.
 * Copyright (c) 2024 by TroyMitchell, All Rights Reserved. 
 */
#include "user.h"
#include "../kernel/include/myfcntl.h"

#define CONSOLE                 1  
int main(void){
        int ret;
        ret = open("console", O_RDWR);
        if(ret == -1) {
                ret = mknod("console", 1, 0);
                if(ret == 0) {
                        ret = open("console", O_RDWR);
                }
        }
        for(;;);
        return 0;
}