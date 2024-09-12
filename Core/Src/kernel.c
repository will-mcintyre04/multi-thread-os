#include <main.h>
#include <stdio.h>
#include <stdbool.h>
#include <kernel.h>

#define STACKSIZE 0x400

uint32_t* MSP_INIT;

uint32_t threads_created = 0;
uint32_t current_thread_index = 0;

struct k_thread threads[16]; //I think we can have 16 threads?

/* Extracts system call number and prints a message if it is # 17 */
void SVC_Handler_Main(unsigned int *svc_args){
	// Tell the C file to look for the runFirstThread function in an assembly file
	extern void runFirstThread(void);

	// Declare an integer for the svc number
	unsigned int svc_number;

	/*
	 * Stack contains:
	 * r0, r1, r2, r3, r12, r14, the return address and xPSR
	 * First argument, r0 is svc_args[0]
	 */
	//Think right to left. Convert svc_args[6] to a pointer to char array
	// Then we can go back 2 bytes to get halfway through r14 (where the number is)
	svc_number = ((char*)svc_args[6])[-2];
	switch(svc_number)
	{
		case 17: //17 is sort of arbitrarily chosen
			printf("Success\r\n");
			break;
		case 65:
			printf("Failure\r\n");
			break;
		// If the SVC number is RUN_FIRST_THREAD
		case 2:
			// Set PSP to the first thread's stack pointer
			// Casts the memory location to an integer value to work
			__set_PSP((uint32_t)threads[0].sp);
			runFirstThread();
			break;
		// THIS IS YIELD
		case 69:
			//Pend an interrupt to do the context switch
			_ICSR |= 1<<28;
			__asm("isb");

			// After context switch reset the runtime to the timeslice
			threads[current_thread_index].runtime = threads[current_thread_index].timeslice;
			break;
		default:
			break;
	}
}

uint32_t* allocate_new_stack() {
    uint32_t* new_stack;
    // Allocate stack based on initial MSP location and stack #
    // Decrement MSP_INIT (the address it is pointed to) by the stacksize * stack  #
    threads_created++;
    new_stack= (uint32_t*)((uint32_t)MSP_INIT - threads_created * STACKSIZE);

    // If the stack is expanded beyond the limit then return null
    if((threads_created * STACKSIZE) > 0x4000){
    	return NULL;
    }

    return new_stack;
}

// Function to create a new thread
bool osCreateThread(void* thread_function, void* args) {
	struct k_thread new_thread;
	new_thread.thread_function = thread_function;
	uint32_t* stackptr = allocate_new_stack();

	// Check if stack allocation was successful
	if (stackptr == NULL) {
		return false; // Unable to allocate stack, return false
	}

	// Fill up xPSR
	*(--stackptr) = 1<<24; //Magic Number, this is xPSR

	// Fill up PC
	*(--stackptr) = (uint32_t)thread_function; //The function name


	for(int i = 0; i < 14; i++){
		if(i == 5){
			*(--stackptr) = (uint32_t)args;
		}
		else{
			*(--stackptr) = 0xA; //An arbitrary number
		}
	}
	new_thread.sp = stackptr;

	threads[threads_created-1] = new_thread;
	return true;
}

// Function to create a new thread
bool osCreateThreadWithDeadline(void* thread_function, void* args, int runtime_and_timeslice) {
	struct k_thread new_thread;
	new_thread.thread_function = thread_function;

	new_thread.runtime = runtime_and_timeslice;
	new_thread.timeslice= runtime_and_timeslice;

	uint32_t* stackptr = allocate_new_stack();

	// Check if stack allocation was successful
	if (stackptr == NULL) {
		return false; // Unable to allocate stack, return false
	}

	// Fill up xPSR
	*(--stackptr) = 1<<24; //Magic Number, this is xPSR

	// Fill up PC
	*(--stackptr) = (uint32_t)thread_function; //The function name


	for(int i = 0; i < 14; i++){
		// Add arguments to the R0 register to be passed as arguments to the thread_function
		if(i == 5){
			*(--stackptr) = args;
		}
		else{
			*(--stackptr) = 0xA; //An arbitrary number
		}
	}
	new_thread.sp = stackptr;

	threads[threads_created-1] = new_thread;
	return true;
}

void osKernelInitialize() {
	MSP_INIT = *(uint32_t**)0x0;
	//set the priority of PendSV to almost the weakest
	SHPR3 |= 0xFE << 16; //shift the constant 0xFE 16 bits to set PendSV priority
	SHPR2 |= 0xFD << 24; //Set the priority of SVC higher than PendSV
}

void osKernelStart() {
	__asm("SVC 2");
}

void osSched(){
	// Save the stack pointer of the current thread in that thread’s struct.
	threads[current_thread_index].sp = (uint32_t*)(__get_PSP() - 8*4);

	// Change the current thread to the next thread
	current_thread_index = (current_thread_index + 1)%threads_created;

	// Set PSP to the current thread's stack pointer
	__set_PSP((uint32_t)threads[current_thread_index].sp);
}
