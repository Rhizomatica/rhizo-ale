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
    int shmid = shmget(1, size, 0666|IPC_CREAT);

    char *str = (char*) shmat(shmid,(void*)0,0);

    if (sizeof(str) == 8)
        sprintf(str, "gollaaaaazooooooo, 0x%"PRIx64"\n", str);
    if (sizeof(str) == 4)
        sprintf(str, "gollaaaaazooooooo, 0x%"PRIx32"\n", str);

    printf("%s", str);

    shmdt(str);

    return 0;

}
