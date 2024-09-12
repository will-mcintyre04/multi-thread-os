#include <main.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef SVC_HANDLER_H
#define SVC_HANDLER_H

#define SHPR2 *(uint32_t*)0xE000ED1C //for setting SVC priority, bits 31-24
#define SHPR3 *(uint32_t*)0xE000ED20 // PendSV is bits 23-16
#define _ICSR *(uint32_t*)0xE000ED04 //This lets us trigger PendSV

// Function prototype for SVC_Handler_Main
void SVC_Handler_Main(unsigned int *svc_args);
uint32_t* allocate_new_stack();

extern void __set_PSP(uint32_t topOfProcStack);

typedef struct k_thread{
	uint32_t* sp; //stack pointer
	void (*thread_function)(void*); //function pointer
	int timeslice;
	int runtime;
	}k_thread;

bool osCreateThread(void (*thread_function), void* args);
bool osCreateThreadWithDeadline(void* thread_function, void* args, int runtime_and_timeslice);
void osKernelInitialize();
void osKernelStart();
extern void runFirstThread(void);

#endif // SVC_HANDLER_H
