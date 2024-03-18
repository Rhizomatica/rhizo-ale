#include <stdio.h>
#include <inttypes.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define KEY_1 1
#define KEY_2 2



int main()
{
    int size = SHMLBA;
    size = 512;

    

    int shmid = shmget(1, size, 0);

    if (shmid == -1)
    {
        printf("shared memory not created\n");
        return 0;
    }

    char *str = (char*) shmat(shmid,(void*)0,0);

    printf("Data read from memory: %s\n",str);

    shmdt(str);

    shmctl(shmid,IPC_RMID,NULL);

    return 0;

}
