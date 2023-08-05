## Test task for a trainee for the position "C++ Programmer"

The russian version of the task can be found in the repository.

## Contents

1. [Program I](#program-i-client) 
2. [Program II](#program-ii-server)

## General description
This test task consists of two programs to be implemented in C++. The programs must communicate with each other through the use of sockets. The test case is divided into two programs: Program I and Program II.

### Program I (Client)
### Description
Program I consists of two threads and one shared buffer. Thread 1 is responsible for receiving a string that the user enters. The program must check that the string consists of only numbers and does not exceed 64 characters. After checking, the string must be sorted in descending order and all elements whose value is even are replaced by the Latin letters "EF". This string is then placed in a shared buffer and thread 1 waits for further user input. Thread 2 processes the data from the shared buffer, displays the received data on the screen, calculates the total sum of all numeric values of elements and transfers the received sum to Program II. After that, thread 2 waits for the following data.

### Notes
- Interaction of threads must be synchronized without using global variables.
- Program operation must be independent of the start status of Program II. Restarting program II should not cause immediate input problems for the user. If communication between programs is lost, program I should be expected to connect when it is restarted.

### Program II (Server)
### Description
Program II is waiting for data from Program I. When the data is received, the length of the transmitted value is analyzed. If it consists of more than two characters and is a multiple of 32, a message about the received data is output, otherwise an error message is output. The program continues to wait for the next data from Program I.

### Notes
- Program operation should be independent of the start status of Program I. Sudden stopping of Program I should not cause immediate display problems. If communication between programs is lost, program I should be expected to connect when program I is restarted.

## Repository Requirements
- Each of the programs should be in its own folder.
- Building the programs should not require system settings or locating certain files in a specific location.
- The source code should be compiled using CMake or make with GCC to work in Linux environment.
- There should be no junk in the source code folder: unused source code or resource files, intermediate build files, etc.
