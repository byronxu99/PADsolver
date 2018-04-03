# PADsolver
A solver for the "puzzle" part of the mobile game [Puzzle &amp; Dragons (PAD)](https://en.wikipedia.org/wiki/Puzzle_%26_Dragons). Made with [Qt 5](https://www.qt.io/) and C++.

## GUI application
The GUI portion of the solver can be compiled with the standard Qt 5 build process.

![Screenshot](https://i.imgur.com/9CbBz5I.png)

Here is the program in action. A random board is generated and displayed. The solution found is overlaid on the board. Click "New Board" to generate a new board. The Showing/Hiding Path button toggles the visibility of the solution.

Click and drag an orb to move it. As you move the orb, combos that are formed are highlighted, including "cascade" combos that occur as a result of other combos being cleared out. Click "Reset" to return to the original board.

## Command line solver
The command line solver is compiled with CMake (`cmake && make`). To use, run

`./padsolver ORBCODE`

where `ORBCODE` is a 30-character string consisting of R, G, B, L, D, H, J, P, representing the respective orb types in the game. Running the program without an orbcode generates and solves a random orb configuration.

## Additional Resources
[Dawnglare PAD Simulator](http://pad.dawnglare.com/) is great for practicing the puzzle and generating orbcodes for the command-line solver.
