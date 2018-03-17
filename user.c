#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <ctype.h>
#include <time.h>
#include "oss.h"


pid_t pid;



const int NANO_SECONDS_PER_SECOND = 1000000000;
int random_duration;
int count_duration = 0;
int add_to_clock = 0; 
int random;
Clock* clock_shared_memory;//this pointer will point to clock struct and be used to modify attributes of clock
int shm_id;//id for shmdata struct/shared mem ->shmget

int main(int argc, char* argv[]) {
 
printf("Hello from the user.\n"); 


 printf("About to allocate shared memory in user...\n");
 //Allocate shared memory  
 shm_id = get_shared_memory();   
 
printf("About to attach to shared memory in user...\n"); 
 //Attach to shmData segment
   ShmData shm;
   ShmData *shmPtr;

   shmPtr =  shmat(shm_id, 0, 0);
   
    if(shmPtr == -1) {
        perror("could not attach to memory....shmat error");
        exit(1);
   }
   printf("successfully attached to shared memory in user...\n");



   
  // int shmSecpnds1 = shm_time_ptr->seconds;
  // int shmNano1= shm_time_ptr->nano_seconds;

  // printf("this is seconds from shared memory in the user.c %d \n", shmSecpnds1);
  // printf("this is nano_seconds from shared memory in the user.c %d \n", shmNano1);
 
   
 
	
	//Generate scaled random time to update the simulated clock
  //long this_time = (rand() % 500000) + 1; //I will be adding this to the clock
  //shm_time_ptr->nano_seconds = this_time;
	

    
  return 0;
  
 }

/*****Shared memory*****/
/*Allocating shared memory for a shmData struct.
This struct is in oss.h and it holds: a clock structure,
an array of 18 PCBs, a process counter (int), an time quantum (int).
to ensure I use the same key in oss and user, a shared memory key
was defined in oss.h and as a global. I am also setting permissions*/

int get_shared_memory(void){
 shm_id = shmget ( SHMKEY, sizeof(ShmData), 0777 | IPC_CREAT );
  if (shm_id == -1) {
    perror("Failed to get shared memory for shmData");
    exit(1);
  }
   printf("Created shared memory for ShmData struct... shmget succeeded.\n");
 
  return shm_id;
}