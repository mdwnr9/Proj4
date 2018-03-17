#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/time.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <ctype.h>
#include "oss.h"

/* This flag controls termination of the main loop. */
volatile sig_atomic_t keep_going = 1;

/* The signal handler just clears the flag and re-enables itself. */
void catch_alarm (int sig)
{
  keep_going = 0;
  signal (sig, catch_alarm);
}

//Prototypes
void fork_exec();

/***Variables***/ 

int shm_id;//id for shmdata struct/shared mem ->shmget
int msgflg;

pid_t pid;
int status;

int main(int argc, char* argv[]) {
	
  //Flags for getopt	
  int help_flag = 0;
  int max_children = 5;
  char* log_file = "log";
  int max_run_time = 20;
  int max_sim_time = 2;
  opterr = 0;
  int c;
  
  while ((c = getopt(argc, argv, "hs:l:t:m:")) != -1) {
    switch (c) {
      case 'h':
        help_flag = 1;
        break;
      case 's':
        max_children = atoi(optarg);
        break;
      case 'l':
        log_file = optarg;
        break;
      case 't':
        max_run_time = atoi(optarg);
        break;
      case 'm':
        max_sim_time = atoi(optarg);
        break;
      case '?':
		 if (optopt == 's' || optopt == 'l' || optopt == 't')
                    fprintf(stderr, "Option %c requires an argument.\n", optopt);
                else
                    fprintf(stderr, "Unknown option -%c\n", optopt);
                return -1;
        }
}

	if (help_flag == 1)
    {
        printf("'-h' Help menu\n");
        printf("'-s x' Number of child processes spawned (default value = %d)\n", max_children);
        printf("'-l' This sets the name of the logfile");
        printf("'-t z' Total amounnt of seconds that will pass until the master process kills itself and all the children (default value = %d)\n", max_run_time);
        exit(0);
	}
  
  
  //This is for message
  msgflg = IPC_CREAT|0777;
  size_t message_length;
  
  

/* Establish a handler for SIGALRM signals. */
  signal (SIGALRM, catch_alarm);

/* Set an alarm to go off in a little while. */
  alarm (10);

printf("About to allocate shared memory in master...\n");
 //Allocate shared memory  
 shm_id = get_shared_memory();   
 
printf("About to attach to shared memory in master...\n"); 
 //Attach to shmData segment
   ShmData shm;
   ShmData *shmPtr;

   shmPtr =  shmat(shm_id, 0, 0);
   
    if(shmPtr == -1) {
        perror("could not attach to memory....shmat error");
        exit(1);
   }
   printf("successfully attached to shared memory in user...\n");
   //Initialize everything in shared memory (including simulated clock and PCB)
  
  /*printf("About to fork a child ...\n");
//Fork and exec a child ---> I will change this to several children instead of one child
  for(int i = 0; i < 5; i++){
  fork_exec();    
  sleep(3);
  }*/
  
   
//Detach & remove from shared memory 
wait(&status);
     printf("Child has completed...\n");
     shmdt((void *) shmPtr);
     printf("shmData has detached  its shared memory...\n");
     shmctl(shm_id, IPC_RMID, NULL);
     printf("shmData has removed its shared memory...\n");
     printf("shmData  exits...\n");
     exit(0);
	 
return 0;

}

/*This function forks one child and execs it to user.*/

void fork_exec(){
pid_t pid = fork();
    if (pid < 0) {
      perror("Failed to fork");
      exit(1);
    }
    
    else if(pid == 0){
    printf("I am child in OSS and I am about to exec...\n");
	execlp("./user",(char*) NULL); 	
		perror("Failed to exec");
        exit(-1);
	 }
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