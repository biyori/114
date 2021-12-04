# BBQ
This program is a Bounded Buffer Queue created using a `vector<string>` data structure
- FIFO
  - `push_back`
  - `pop_back`
  - `back()`

Using `condition variables` and `mutex locks` we can safely access items in the queue.

#
## Requirements
- C++ 11
- pthreads

#
## Environment
The environment this program was intended for is a `Linux` environment. Extensive testing was in Linux but the program will work in Mac OSX and Cygwin. The program uses pthreads and takes advantage of ANSI color codes. The colors are used to *beautify* events in the console.
>These features may not be available on Windows.
![Image of pretty consoles](image.jpg)

#
## Compilation
To compile the program you may use **g++**, **clang**, or **CMake**. *GCC will not work because this is a C++ program*.
>g++ project1.cpp -std=c++11 -lpthread

If you wish to use compiler optimizations you may enable the `-O` flag.
>g++ project1.cpp -std=c++11 -lpthread -O2

#
## Usage
The program takes in two arguments
1. Thread Consumer Sleep time
2. Thread Producer Sleep time
The arguments are required for the program to execute
![Program Arguments](https://i.imgur.com/6R8DbXX.png)

To run the program define sleep times greater than `1`

>./project1 10 20

*Note that the thread producer sleep time is dynamic and will only sleep up to the argument provided. Thread consumer is completely random from `[0,TC]`*
