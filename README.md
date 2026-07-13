# Conway's Game of Life - Sega Master System
Conway's Game of Life - Sega Master System port.

Coded by [tibonev](http://classicgames.com.br)
using the [SMSDevkit by sverx](https://github.com/sverx/devkitSMS)
compiled using [SDCC](https://sdcc.sourceforge.net)

This project is 100% human made, absolutely no AI of any kind was used for any part of the code, assets or documentation.

## Build from the Code
To build it, you need a working copy of SDCC and SMSDevkit, with crt0, lib(SMSlib) and PSGlib inside their own folders, relative to main.c, if you're in a windows enviroment, then all you need to do is run build.bat.
Otherwise you might need to execute the commands yourself.  

## Controls 
Button 1 - Next Generation
Button 2 - Status (press 2 again to go back to "cell view")

## What is "Game of Life" ?
Game of Life (GoL) is a celullar automaton, zero-player game, developed by mathematian John Conway in 1970. The algorithm requires no proper input from the player, with the initial state determining how the game will evolve. 
Other than computation limits, there are no generational limit and there are no win conditions. It can achieve a stale condition where no evolution is possible. 

For the Sega Master System game, it is not a infinite grid, the grid is limited to 32 x 24, due to the limitations of the SMS Video Display Processor (VDP), up for a max of a 768 cells in total. 

Each cell can only be _alive_ or _dead_ there are no intermediate state. 
Each cell state is only determined by Moore's neighboorhood.

And then each generation is calculated as such: 
- Any alive cell with 2 or less neighboors dies by underpopulation.
- Any alive cell with 3 or 4 neighboors lives.
- Any alive cell with more than 4 neighboors die by overpopulation.
- Any dead cell with exactly 3 alive neighboors is alive by reproduction.

## Features
- 32 x 24 (768) grid;
- Sonic-like Graphics;
- 4 major conditions implemented;
- Basic Stats Screen;

## Todo
- Title Screen;
- Music;
- More Stats;
- Pattern Selection;

## Version History 
- 20260713 - Full seperation of simulation / render. Fixed major bug with grid being written mid-generation, now its completely separated;
- 20260712 - Added rules for dead cells to become alive; Added sonic graphic for the alive cells; Optimized grid;
- 20260529 - Tested with random patterns, most rules implemented;
- 20260528 - Fixed cell placement, fixed out of bounds grid error;
- 20260527 - Fixed grid drawing;
- 20260526 - Added random pattern generation of cells;
- 20260525 - Added background grid / checkerboard with sonic green hill colors;
- 20260522 - Changed from sequential grid to Cartesian grid, with separated value for x/y;
- 20260521 - Initial Version;

## Contributor License Agreement (CLA)
By submitting a pull request, you agree that:
- Your contribution is your own original work.
- You didn't use any AI generation for code/graphics/assets or text.
- You grant the project maintainer(s) a perpetual, worldwide, non-exclusive license to use, reproduce, and distribute your contribution under the project's existing license (MIT).
- You understand that your contribution will be made publicly available as part of the project.

## Disclaimer
The software is provided “as is”, without any warranty.
The author shall not be held responsible for any damages, misuse, or modifications of the code or assets.

# Copyright 
All the assets (code, graphics and music) were created by tibonev. 
All the code is licensed under the MIT License (check out LICENSE for more info).
(c) 2026 tibonev.