# Conway's Game of Life - Sega Master System
Conway's Game of Life - Sega Master System port.

Coded by [tibonev](http://classicgames.com.br)

using the [SMSDevkit by sverx](https://github.com/sverx/devkitSMS)

"outrunner" font by [DamienG](https://damieng.com/typography/zx-origins/)

compiled using [SDCC](https://sdcc.sourceforge.net)

web emulator version using [EmulatorJS](https://emulatorjs.org)

This project is 100% human made, absolutely no AI of any kind was used for any part of the code, assets or documentation.

## Build from the Code
To build it, you need a working copy of SDCC and SMSDevkit, with crt0, lib(SMSlib) and PSGlib inside their own folders, relative to main.c, if you're in a windows enviroment, then all you need to do is run build.bat.
Otherwise you might need to execute the commands yourself.

## ScreenShots 
![TitleScreen](http://classicgames.com.br/temp/gameoflife0.png)
![Random](http://classicgames.com.br/temp/gameoflife2.png)
![Random](http://classicgames.com.br/temp/gameoflife3.png)

## Controls 
To select CUSTOM / EDIT mode, select Pattern 12 (CUSTOM), autoGen is disabled for CUSTOM.
Button 1 - in STEP or CUSTOM mode: Next Generation - hold for animations;
Button 2 - in CUSTOM mode only: Place / Remove Cell under the cursor.
D-Pad - in CUSTOM mode only: move the cursor.

## What is "Game of Life" ?
Game of Life (GoL) is a celullar automaton, zero-player game, developed by mathematian John Conway in 1970. The algorithm requires no proper input from the player, with the initial state determining how the game will evolve. 
Other than computation limits, there are no generational limit and there are no win conditions. It can achieve a stale condition where no evolution is possible. 

For the Sega Master System game, it is not a infinite grid, the grid is limited to 32 x 24, due to the limitations of the SMS Video Display Processor (VDP), up for a max of a 768 cells in total. 

Each cell can only be _alive_ or _dead_ there are no intermediate state. 
Each cell state is only determined by Moore's neighboorhood population number.

And then each generation is calculated as such: 
- Any alive cell with 2 or less neighboors dies by underpopulation.
- Any alive cell with 3 or 4 neighboors lives.
- Any alive cell with more than 4 neighboors die by overpopulation.
- Any dead cell with exactly 3 alive neighboors is alive by reproduction.

### Bonus - One Dimension (1D) mode / rule set
The 1D mode takes place on a single row/column, it was created as a subset algorithim for old machines using text-mode or with teletypes, popularized by Stephen Wolfram. 
The ruleset says each cell can only be _alive_ or _dead_ without any intermediate state.
Each cell state is only determined by a 5-neighboor population (the cell itself plus 2 neighboors to the left and 2 neighboors to the right).

And then each generation is calculated as such:
- Any alive cell with 2 or 4 neighboors survives.
- Any alive cell with 3 neighboors dies.
- Any dead cell with 2 or 3 neighboors becomes alive.

I've implemented this as a bonus on this version, its not the main focus.

## Features
- 30 x 22 (660 cells) grid;
- Sonic-like Graphics;
- 4 major conditions implemented;
- CUSTOM mode, allows player to draw directly on the grid on any genenation;
- SFX;

### Preset Patterns 
- 0 : Random cells across the screen
- 1 : Blinker (2 period oscillator)
- 2 : Glider (Glider diagonal down)
- 3 : Pulsar (3 period oscillator)
- 4 : BeeHive (Still Life)
- 5 : Toad (2 periodo oscillator)
- 6 : Pulsar Cousin (15 period oscillator)
- 7 : LWSS (Light Weight Spaceship)
- 8 : DieHard (dies in 150 generations)
- 9 : R-Pantomino (Chaotic Evolution)
- 10: Acorn (Chaotic Evolution)
- 11: B-Hexomino (Chaotic Evolution)
- 12: Custom (You create the pattern)

## Todo
- Music;
- Linux build script;
- Code Cleanup;

## Version History
### 20260721
- fixed bug with CUSTOM Mode showing as Pattern 12.
- added ROM Headers to get the build released.
- added 1D (One Dimension) Mode w/ vertical wrap
- added my name on the title screen (how could i forget _that_ ?)

### 20260720
- fixed bug with the cursor colors (background was not mapped as palette index 0).
- added menu sfx.
- fixed bug, pressing 2 would throw the game into EDIT/CUSTOM mode at any time. (might be a feature? would anyone like that?)
- added "new_gen" sfx. 
- changed algorithm so the grid is only read when there's a need to process a new generation, not every frame.

### 20260717
- added acorn and b-hexomino patterns.
- changed menu to accomodate EDIT/CUSTOM mode.
- added spriteLoad(), spriteDraw() to load a cursor for CUSTOM mode.
- added controls into controllerCheck().
- cursor is moving correctly.
- adding / removing cells is adding / removing of the grid, correctly.

### 20260716
- Changed a bunch of unneeded ints to chars (and unsigned chars).
- added several preset patterns.
- added simple hud
- removed "stats screen".

### 20260715
- Changed gridDraw to be gridGen and a split gridDraw into gridDrawCell and gridDrawFull, also created gridDrawBorders, this way, i can keep a map of the tiles in the checkerboard, and won't need to update the entire board each new generation, only the tiles are were occupied, but are now dead. Did a test and it is indeed faster, even if ram usage is larger.
- Added first 2 preset patterns.

### 20260714 
- Added preliminary title/options screen;
- Added new font "outrunner" from DamienG; 
- Renamed bgBlack to bgBlank;
- Added randomSeed based on frames in titleScreen; 
- Fixed cell generation rules, a typo on number of neighboors needed to keep alive; 
- Added first version of options screen;
- Implemented autogen logic;

### 20260713 
- Full seperation of simulation / render;
- Fixed major bug with grid being written mid-generation, now its completely separated;

### 20260712 
- Added rules for dead cells to become alive; 
- Added "sonic" graphic for the alive cells; 
- Optimized grid;

### 20260529 
- Tested with random patterns, most rules implemented;

### 20260528 
- Fixed cell placement, fixed out of bounds grid error;

### 20260527 
- Fixed grid drawing;

### 20260526 
- Added random pattern generation of cells;

### 20260525 
- Added background grid / checkerboard with sonic green hill colors;

### 20260522
 - Changed from sequential grid to Cartesian grid, with separated value for x/y;

### 20260521 
- Initial Version;

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