/************************************************************************* 
    > File Name: shmfifo.c 
    > Author: ma6174 
    > Mail: ma6174@163.com  
    > Created Time: Sat 13 Jan 2018 05:37:48 AM PST 
 ************************************************************************/  
  
#include <assert.h>  
#include "shmfifo.h"  
  
union semun { int val; };  
  
shmfifo_t *shmfifo_init(key_t key, int blksz, int blks)  
{  
    shmfifo_t *fifo = malloc(sizeof(shmfifo_t));  
    assert(fifo);  
    memset(fifo, 0x00, sizeof(shmfifo_t));  
      
    int shmid = shmget(key, 0, 0);  
    //如果已经存在了，那么shmid返回值不为-1.  
    int size = sizeof(shmhead_t)+blksz*blks;  
    if ( shmid == -1 ) { // shmid不存在，新创建一个我们的基于共享内存的自定义消息队列.  
        shmid = shmget(key, size, IPC_CREAT|0666);  
        if ( shmid == -1 ) ERR_EXIT("shmget");  
        fifo->shmid = shmid;  
        fifo->p_shm = shmat(shmid, NULL, 0);  
        fifo->p_payload = (char*)(fifo->p_shm+1);  
  
        fifo->p_shm->blksize  = blksz;  
        fifo->p_shm->blocks   = blks;  
        fifo->p_shm->rd_index = 0;  
        fifo->p_shm->wr_index = 0;  
          
        fifo->sem_mutex = semget(key, 1, 0644|IPC_CREAT);  
        fifo->sem_full  = semget(key+1, 1, 0644|IPC_CREAT);  
        fifo->sem_empty = semget(key+2, 1, 0644|IPC_CREAT);  
        //初始化shmfifo结构体内容.  
          
        union semun su;  
        su.val = 1;  
        semctl(fifo->sem_mutex, 0, SETVAL, su);  
        su.val = 0;  
        semctl(fifo->sem_empty, 0, SETVAL, su);  
        su.val = blks;  
        semctl(fifo->sem_full, 0, SETVAL, su);  
        //初始化，每一个信号量的_op.   //信号量维护的结构体里面的内容  
          
    } else { // shmid已经存在，连接到那个共享内存块即可.  
        fifo->shmid = shmid;  
        fifo->p_shm = shmat(shmid, NULL, 0);  
        fifo->p_payload = (char*)(fifo->p_shm+1);  
        fifo->sem_mutex = semget(key, 0, 0);  
        fifo->sem_full  = semget(key+1, 0, 0);  
        fifo->sem_empty = semget(key+2, 0, 0);  
    }  
  
    return fifo;  
}  
  
//p操作  
int sem_p(int semid)  
{  
    struct sembuf buf[1] = {0, -1, 0};  
    return semop(semid, buf, 1);  
}  
  
//v操作  
int sem_v(int semid)  
{  
    struct sembuf buf[1] = {0, 1, 0};  
    return semop(semid, buf, 1);  
}  
  
//放置数据  
void shmfifo_put(shmfifo_t *fifo, const void *buf)  
{  
    //防止自己在写的时候，别的生产者也过来写入内容，执行信号量sem_mutex写操作.  
    sem_p(fifo->sem_full);  
    sem_p(fifo->sem_mutex);  
    //每次放置数据之前，让信号量sem_full计数-1，等到该计数为0时，要放置数据时使用sem_P操作，就会被阻塞，等到有空间可以写为止.  
  
    memcpy(fifo->p_payload+fifo->p_shm->wr_index*fifo->p_shm->blksize, buf, fifo->p_shm->blksize);  
    fifo->p_shm->wr_index = (fifo->p_shm->wr_index+1) % fifo->p_shm->blocks;  
    //定义下次写位置.  
  
    //放置数据结束以后，让信号量sem_empty的计数+1，这样的话仓库中就多了一个可以读取的数据.  
    sem_v(fifo->sem_mutex);  
    sem_v(fifo->sem_empty);  
}  
  
void shmfifo_get(shmfifo_t *fifo, void *buf)  
{  
    //防止自己在写的时候，别的生产者也过来写入内容，执行信号量sem_mutex写操作.  
    sem_p(fifo->sem_empty);  
    sem_p(fifo->sem_mutex);  
    //每次读取数据之前，让信号量sem_empty计数-1,等到该计数为0时，要读取数据时使用sem_P操作，就会被阻塞，等到有数据可以读为止.  
  
      
    memcpy(buf, fifo->p_payload+fifo->p_shm->rd_index*fifo->p_shm->blksize, fifo->p_shm->blksize);  
    fifo->p_shm->rd_index = (fifo->p_shm->rd_index+1) % fifo->p_shm->blocks;  
    //定位下次读位置.  
      
    //读取完数据之后，让信号量sem_full+1，这样的话"仓库"中多了一个空间可以放置数据  
    sem_v(fifo->sem_mutex);  
    sem_v(fifo->sem_full);  
}  
  
void shmfifo_destroy(shmfifo_t *fifo)  
{  
    semctl(fifo->sem_mutex, 0, IPC_RMID, 0);  
    semctl(fifo->sem_empty, 0, IPC_RMID, 0);  
    semctl(fifo->sem_full,  0, IPC_RMID, 0);  
    shmdt(fifo->p_shm);  
    shmctl(fifo->shmid, IPC_RMID, 0);  
    free(fifo);  
}  