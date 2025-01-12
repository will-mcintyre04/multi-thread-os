# Real-Time Operating System (RTOS) for STM32 Nucleo Board

This project implements a real-time operating system (RTOS) with multi-threading capabilities using C for an STM32 Nucleo board. The RTOS enables concurrent execution of tasks with precise timing control, allowing for efficient resource utilization and real-time responsiveness.

## Features

- **Multi-threading**: Enables concurrent execution of multiple tasks, improving system responsiveness.
- **Hybrid Scheduler**: Combines yielding and time-based pre-emptive context switching to optimize CPU utilization.
- **Deadline Management**: Tasks are executed with deadlines, ensuring timely execution of critical tasks.
- **Thread Control Blocks (TCBs)**: Custom memory allocator handles thread initialization, stack allocation, and context storage.
- **Efficient Resource Management**: Optimized memory usage and task isolation for better performance.

## Components

1. **RTOS Core**: Provides the basic functionality for multi-threading, task scheduling, and context switching.
2. **Hybrid Scheduler**: A scheduler that combines yielding-based and time-based preemptive context switching.
3. **Memory Allocator**: Custom allocator for managing thread stacks and context storage in TCBs.
4. **Thread Management**: Supports creating and managing threads with deadlines to ensure tasks meet their time constraints.

## Getting Started

### Prerequisites

- **STM32 Nucleo Board** (or any other STM32 microcontroller board)
- **ARM GCC Toolchain** for compiling C code
- **STM32CubeMX** for configuring the microcontroller and peripherals (optional)

### Setup Instructions

1. Clone or download the repository to your local machine.
2. Open the project in your preferred IDE (e.g., STM32CubeIDE or Keil).
3. Set up the STM32 Nucleo board's clock and GPIO configurations using STM32CubeMX (if not already configured).
4. Compile the project using the ARM GCC toolchain or your chosen IDE.
5. Flash the program onto the STM32 Nucleo board.

### Running the Project

- Once the program is flashed onto the Nucleo board, it will start executing the threads.
- The scheduler will ensure tasks are executed according to their deadlines.
- Threads with different priorities and deadlines will yield based on their execution time and deadlines.

### Key Functions
- `osKernelInitialize()`: Initializes the kernel by setting up internal data structures, initializing the scheduler, and preparing the system to run tasks. It must be called before any threads can be created or executed.
- `osKernelStart()`: Once the kernel is initialized, osKernelStart() is called to start the RTOS kernel. It begins the task scheduling process and starts running the created threads.
- `osCreateThread()`: This function creates a new thread and assigns it a function to execute. Once the thread is created, it begins executing concurrently with other threads in the system.
- `osCreateThreadWithParameter()`: Similar to osCreateThread(), but this function also passes a parameter to the thread at the time of its creation. This allows the thread to use the data in the parameter for its execution.
- `osCreateThreadWithDeadline()`: This function is similar to osCreateThread(), but it allows you to specify a deadline for the thread. The RTOS will ensure that the thread runs within the specified time limit, providing a guarantee that critical tasks will be completed on time.
  

## Concepts

### Hybrid Scheduler
The scheduler uses both yielding and time-based preemptive context switching. Tasks can voluntarily yield the processor when they are done or when they reach a yielding point. Additionally, the scheduler will preempt tasks based on their deadlines to ensure real-time guarantees.

### Thread Control Blocks (TCBs)
Each thread is managed by a TCB that holds information about the thread’s state, stack pointer, and context. The custom memory allocator ensures that each thread's memory is efficiently allocated and managed.

### Deadline Management
Tasks are assigned deadlines, and the scheduler ensures that the highest-priority tasks meet their deadlines. This is especially important for time-sensitive applications.

## Example Threads

- **First Thread**: A simple thread that continuously prints "Thread 1" to the console and yields to other threads.
- **Second Thread**: Similar to the first, but prints "Thread 2".
- **Thread with Parameters**: A thread that accepts parameters and prints them in a loop.

## Contributing

If you'd like to contribute to this project, feel free to fork the repository and submit a pull request. Contributions, bug fixes, and feature suggestions are always welcome!

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- STM32 Nucleo Board for hardware support
- ARM GCC Toolchain for C compilation
- STM32CubeMX for peripheral configuration
