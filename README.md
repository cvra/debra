CVRA NIOS 2 Code & libraries
============================

To build on Linux (should work on any UNIX, but not tested) :

    cd /path/to/repos/
    mkdir build
    cd build && cmake ..
    make

To build just for a single robot :

    make debra
    make nastya

Cross compiling for the robot is more complicated, as for now we dont have any
standard makefiles to cross compile. For now you should create a blank project in
Nios II SBT, and then add all the C files to the build by right clicking them and
choosing "Add to NIOS II build...". Then you can add the subfolders of `modules`
to the include path (look in the Makefile for `ALT_INCLUDE_DIRS`). In the long run
we will probably setup our own system to be able to cross compile with nios2-gcc
only, without using any other Altera tools.

Organisation of the source code
===============================
The source code is organised like this :

    nios2
    |-- debra
    |-- doc
    |-- include
    |-- modules
    |-- nastya
    |-- nios_files
    |-- tools


* The `debra` folder contains all the source specific to our first robot (the one with the arms).
* The `nastya` folder contains all the source for our holonomic robot.
* The `include` folder contains a few generic includes.
* The `modules` folder contains all generic modules and a lot of code taken from Aversive.
* The `tools` folder contains various tools for developpers.
* The `nios_files` folder contains all the binary files generated by Quartus II from VHDL.

Useful warnings
===============
* When building for the NIOS 2 system, do not assume that the compiler will zero out memory before giving it to you. It _won't_.
* Do not do any rebase / rollback of already pushed commits.
* The NIOS 2 compiler hates you. Really.

Priority list
=============
Here is a list of all tasks that are running on this specific app.
Remember : The lower the priority number, the higher the priority (i.e., its importance).
**This list should always be maintained up-to-date.**
Really bad things can happen when you don't set your priority correctly.

| Task name     | Priority | Notes
|---------------|----------|-------
| Init          | 20       | Is self-deleted once init is complete.
| Motor control | 21       | Main motors (wheels) control. Updates robot system and blocking detection.
| Odometry      | 22       | Computes the robot position from robot system.
| Trajectory    | 23       | Computes the current consigns needed to go to target point.
| Shell         | 40       | Serial shell used for debug and config.
| Strategy      | 50       | Must be background task because it doesn't use IPC properly yet.
