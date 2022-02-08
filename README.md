# parallel sand simulator
Implementation of a material simulator in C, user can add material and see how they interact over time.

## Dependencies
* GCC
* GNU Make
* SDL2

## Installation
* `git clone https://github.com/joaquin-rossi/celullar-automata`
* `cd celullar-automata`
* `make`
* `./bin/main`

## Available automata
* [Langton's ant](https://en.wikipedia.org/wiki/Langton%27s_ant)
* [Conway's game of life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)
* [Brian's brain](https://en.wikipedia.org/wiki/Brian%27s_Brain)
* [Wireworld](https://en.wikipedia.org/wiki/Wireworld)

## Extra info
* Press spacebar to toggle between paused and unpaused.
* Press left click on a cell to alter its value.
* Edit src/logic.h (pre-processor #defines) to change speed, size, etc.

## Demo
![Langton's ant](demo.gif)
