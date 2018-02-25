/************************************************************************* 
    > File Name: get.c
    > Author: ma6174 
    > Mail: ma6174@163.com  
    > Created Time: Sat 13 Jan 2018 05:37:48 AM PST 
 ************************************************************************/  
  
#include "shmfifo.h"  
  
typedef struct stu {  
    int id;  
    char name[32];  
}stu_t;  
  
int main( void )  
{  
    int i;  
    stu_t buf;  
    shmfifo_t *p = shmfifo_init(1234, sizeof(stu_t), 3);  
    //连接到以Key为1234的共享内存shmid. 如果没有则创建，如果有则连接.  
      
    for (i=0; i<3; i++) {  
        //每次拿一个数据.  
        shmfifo_get(p, &buf);  
        printf("%d %s\n", buf.id, buf.name);  
    }  
      
    //shmfifo_destroy(p);  
}