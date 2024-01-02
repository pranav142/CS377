# Lab 1 - Closh Clone: Command Line Shell Implementation  

## Overview  

In this lab, I developed a command-line shell clone named Closh that can execute commands multiple times, either in parallel or sequentially, and with an optional timeout feature. The primary objective was to understand process creation and management in Unix-based systems through the implementation of various shell functionalities.  

## Implementation Details  

### Execution of Multiple Commands  

* The core functionality to execute commands multiple times was implemented using the Unix fork() system call. This system call duplicates the current process, creating a child process that is nearly identical to the parent but differs in the return value of fork().  
* As an example, if a user wants to run the command echo hello world 3 times, the parent process creates three child processes to handle this task.

### Sequential and Parallel Execution
* **Sequential Execution**: For running commands sequentially, the parent process creates a new child process only after the previous one has finished executing or has been terminated. This ensures that each command runs one after the other, in a sequential manner.  
* **Parallel Execution**: In contrast, for parallel execution, the parent process creates all child processes at once. Then it waits for all of them to complete. This allows multiple commands to run simultaneously, in parallel.  

### Timeout Implementation  

* The timeout feature is implemented using the Unix sleep() system call. The program essentially sleeps for the specified timeout duration and then terminates the processes that are still running after this period.  
* If the timeout is set to 0, the program uses the waitpid() call to wait for each process to finish completely, allowing each command to run for an indefinite amount of time until it completes on its own.  
