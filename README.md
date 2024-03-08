# COP4520_Project - Multithreaded N-Body Simulation

This repo contains all of the code related to our project.

Running the code will launch a window that starts the n-body particle simulation. In it's current state it runs the naive (direct) algorithm and runs a multithreaded benchmark up to 20 threads.

### Installation and Running
To install, clone the repo and navigate to it in your terminal.

To run the simulation, build the `cmake` project:
```
cmake --build build
```

Then simply run the executable:
```
./build/nbody
```