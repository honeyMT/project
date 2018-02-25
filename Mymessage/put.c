/************************************************************************* 
    > File Name: put.c 
    > Author: ma6174 
    > Mail: ma6174@163.com  
    > Created Time: Sat 20 Jan 2018 10:42:33 PM PST 
 ************************************************************************/  
  
#include<stdio.h>  
#include "shmfifo.h"  
  
typedef struct stu {  
    int id;  
    char name[32];  
}stu_t;  
  
int main( void )  
{  
    int i;  
    shmfifo_t *p = shmfifo_init(1234, sizeof(stu_t), 3);  
    //创建一个共享内存，每一块数据大小为sizeof(stu_t)，仓库容量为3  
    stu_t buf[8] =   
    {  
            {1, "aaa"},  
            {2, "bbb"},  
            {3, "ccc"},  
            {4, "ddd"},  
            {5, "eee"},  
            {6, "fff"},  
            {7, "ggg"},  
            {8, "hhh"}  
    };  
    for (i=0; i<8; i++) {  
  
        shmfifo_put(p, buf+i);  
        printf("完成! %d\n",i);  
        sleep(1);  
    }  
}  