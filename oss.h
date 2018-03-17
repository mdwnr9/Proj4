#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#define SHMKEY  85904


/***************************
* Struct for a system clock*
***************************/
typedef struct system_clock {
   unsigned int seconds; //time in seconds
   unsigned int nano_seconds;//time in nanoseconds
} Clock;


/**********************************
* Struct for process control block*
**********************************/
typedef struct  processControlBlock{

    //total CPU time used. Feeds from myClock
    int total_cpu_time;

    //total time in the system. Feeds from myClock
    int total_sys_time;

    //time during last bust in nanoseconds
    unsigned int last_burst_time;

    //0 for "real-time process" and 1 for "normal-user process"
    int priority;

    //Local simulated pid
    int pid;

    //a bit vector to help track whether process id is taken
    //bool vector;

} PCB;

/*************************************
* Struct to hold a clock struct, an  *
* array of PCB structs and other data*
*that needs to be in shared memory   *
*************************************/
typedef struct ShmData{

   //simulated system clock. Will be adding to seconds and nanoseconds
   Clock Myclock;

   //struct of PCBs
   struct processControlBlock PCB[18];

   //Keep count of processes
   int currentProcessCount;

   //Indivisible amount of time to run for
   int time_quantum;

}ShmData;

//global variables for pointer to shared memory
ShmData shm;
extern ShmData *shmPtr;


/**************
* Prototypes  *
**************/

int get_shared_memory(void);

/**********************************
* Prototypes for clock functions  *
**********************************/

