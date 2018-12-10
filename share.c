#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h> //for open
#include <unistd.h> //for read
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

int main(int argc, char *argv[]) {

  key_t key;
  int shmid;
  char *data;
  int mode;
  // int shmget(key_t key, size_t size, int shmflg);
  // returns id for shared memory segment
  // size = size in bytes of shared memory segment
  // shmflg sets permissions
  shmid = shmget(0, 1024, 0777 | IPC_CREAT);
  printf("Id of shared memory segment: %d\n", shmid);
  // void *shmat(int shmid, void *shmaddr, int shmflg);
  // smhaddr specifies which address to use
  // shmflg- set to SHM_RDONLY if only read, 0 otherwise
  // returns a void pointer

  data = shmat(shmid, (void *)0, 0);
  if (data == (char *) -1)
    printf ("%s", strerror((int)data));

  printf("shared contents: %s\n", data);
  if (data) {
    printf("wooopwoop shared memory created!\n");
  }
  char string[256];
  printf("===========================================================\n");
  while(1){

    printf("Do you wanna change some data? [y/n]\n");
    scanf("%s", string);
    string[strlen(string)] = 0;
    printf("Your input: %s\n", string);
    if (strcmp(string, "y") == 0) {
      printf("Please write a new string:\n");
      scanf("%s", string);
      string[strlen(string)] = 0;
      printf("String entered: %s\n", string);
      strcpy(data,string);
      printf("String in shared memory: %s\n", data);
    }
    if (strcmp(string, "n") == 0) {
      printf("Nevermind then. On to the next one!\n");
      printf("===========================================================\n");

      printf("Do you want to delete the segment?[y/n]\n");
      scanf("%s", string);
      string[strlen(string)] = 0;
      printf("Your input: %s\n", string);

      if (strcmp(string, "y") == 0) {
        printf("Detaching segment...\n");
        int x = shmdt(data);
        if (x == 1)
          printf ("%s\n", strerror(x));

        printf("Shared memory destroyed.\n");
        exit(1);
      }
      if (strcmp(string, "n") == 0) {
        printf("Well what DO you want??\n");
        exit(1);
      }
    }
  }
  shmctl(shmid, IPC_RMID, NULL);

  return 0;
}
