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
![Image of pretty consoles](https://i.imgur.com/LeCd7np.png)
>These features may not be available on Windows.

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

*Note that the thread producer sleep time is a dynamic value based on the current queue size. At low capacity it will be very low random value and at high capacity it will be a random value up to the argument provided. Thread consumer is completely random from `[0,TC]`*

### Example sleep time for a queue size of 50 and a producer sleep time of 10
```
Queue Size [0] sleeps for =>  0
Queue Size [1] sleeps for =>  1
Queue Size [2] sleeps for =>  1
Queue Size [3] sleeps for =>  2
Queue Size [4] sleeps for =>  0
Queue Size [5] sleeps for =>  0
Queue Size [6] sleeps for =>  2
Queue Size [7] sleeps for =>  2
Queue Size [8] sleeps for =>  2
Queue Size [9] sleeps for =>  2
Queue Size [10] sleeps for =>  2
Queue Size [11] sleeps for =>  0
Queue Size [12] sleeps for =>  2
Queue Size [13] sleeps for =>  7
Queue Size [14] sleeps for =>  7
Queue Size [15] sleeps for =>  7
Queue Size [16] sleeps for =>  7
Queue Size [17] sleeps for =>  7
Queue Size [18] sleeps for =>  7
Queue Size [19] sleeps for =>  7
Queue Size [20] sleeps for =>  7
Queue Size [21] sleeps for =>  7
Queue Size [22] sleeps for =>  7
Queue Size [23] sleeps for =>  7
Queue Size [24] sleeps for =>  7
Queue Size [25] sleeps for =>  7
Queue Size [26] sleeps for =>  7
Queue Size [27] sleeps for =>  7
Queue Size [28] sleeps for =>  7
Queue Size [29] sleeps for =>  7
Queue Size [30] sleeps for =>  7
Queue Size [31] sleeps for =>  7
Queue Size [32] sleeps for =>  7
Queue Size [33] sleeps for =>  7
Queue Size [34] sleeps for =>  7
Queue Size [35] sleeps for =>  7
Queue Size [36] sleeps for =>  7
Queue Size [37] sleeps for =>  7
Queue Size [38] sleeps for => 10
Queue Size [39] sleeps for => 10
Queue Size [40] sleeps for => 10
Queue Size [41] sleeps for => 10
Queue Size [42] sleeps for => 10
Queue Size [43] sleeps for => 10
Queue Size [44] sleeps for => 10
Queue Size [45] sleeps for => 10
Queue Size [46] sleeps for => 10
Queue Size [47] sleeps for => 10
Queue Size [48] sleeps for => 10
Queue Size [49] sleeps for => 10
```
>The value is generated from the `bonk()` function which takes the current queue capacity and the sleep time provided to the program
>*This function is only used on the producer `(TP)` sleep time*

```cpp
int bonk(int currentQueueCapacity, int currentSleep)
{
    // If the current queue capacity is greater than or equal to 75% full
    bool slowBonk = currentQueueCapacity >= BBQ_CAPACITY * 0.75;      // 75% full
    bool oneFourthBonk = currentQueueCapacity <= BBQ_CAPACITY * 0.25; // 0-25% filled

    if (slowBonk)
    {
        // cout << "75% ";
        // Sleep for a fraction of the size of the currently filled queue -- up tp 100%
        return min((rand() % (currentSleep - 1 * (currentQueueCapacity / BBQ_CAPACITY)) + currentSleep), currentSleep);
    }
    else if (oneFourthBonk)
    {
        // cout << "1/4th ";
        // Below 25% queue occupancy and reaches twice the initial average speed when buffer is empty
        return min((rand() % (currentQueueCapacity + 1)), (int)(currentSleep * 0.25));
    }
    else
    {
        // cout << "25-75 ";
        return min((rand() % (int)(currentQueueCapacity * 0.25) + (int)(currentQueueCapacity * 0.75)), (int)(currentSleep * 0.75)); // Use min in case the currentQueue exceeds the currentSleep time
    }
}
```
>Notice how a large sleep time would not actually work until the queue is up to 75% capacity
#
## Program Demonstration
>Watch on YouTube
[![Program Demonstration](https://img.youtube.com/vi/ExWSE1mOxBA/0.jpg)](https://www.youtube.com/watch?v=ExWSE1mOxBA)