// Conway's Game of Life (or something close to it) //
// Coded by tibonev using sverx's sms devkit //
// Graphics Made using paint dot net //
// Music created with Reaper / Mod2PSG2 //
// tibonev: http://classicgames.com.br //
// All assets, code, music etc: made by tibonev //
// Check README.MD for more information //
// Everything is 100% human-made, zero AI used //

// SDCC Includes //
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// SMS Includes //
#include "SMSlib/SMSlib.h"           // Main devkit //
#include "PSGlib/PSGlib.h"           // Sound Stuff //
#include "vdp.h"                     // VDP Stuff //

// Data Includes //
#include "bank2.h"

// Game Constants / Variables //
#define MAPW 31
#define MAPH 23
#define TILE_CELL 0
#define TILE_BG_DARK 1
#define TILE_BG_LIGHT 2
#define TILE_BLANK 16
unsigned char grid[32][24];
unsigned char newGenGrid[32][24];
char buffer[8];
int newstate = 0;
int gamestate = 1;
int gen = 0;
int aliveNum = 0;
unsigned int randomSeed = 0;

typedef struct {
    unsigned char screenIndex;
    unsigned char pattern;
    unsigned char autogen;
} options;
options op;



// Functions Prototypes //
int rand_num(int lb, int ub);
//void spriteLoad(void);
void bgLoad(void);
void bgBlank(void);
void gridInit(void);
void gridDraw(void);
void fontLoad(int bg0, int bg1);
char intToString(int number);

void cellScanGrid(void);
void cellDraw(int x, int y);
int cellCheckNeighboor(int x, int y, int cellStatus);
int cellCountAlive(void);
void generationProc(void);

void controllerCheck(void);

void statsRender(int alive);
void titleScreenRender(void);


// sprite stuff //
/*void spriteLoad(void) {
    SMS_loadPSGaidencompressedTiles(spr_tiles_psgcompr, 256);
	SMS_loadSpritePalette(spr_tiles_bin);
    return;
} */

void bgLoad(void) {
    SMS_loadPSGaidencompressedTiles(spr_tiles_psgcompr, 0);
    SMS_loadBGPalette(spr_tiles_bin);
}

void bgBlank(void) { 
    for(int x = 0; x <= MAPW; x++) {
        for(int y = 0; y <= MAPH; y++) { 
            SMS_setTileatXY(x,y,TILE_BLANK);
        }
    }
}

void fontLoad(int bg0, int bg1) { 
    SMS_setBGPaletteColor(0, bg0);
    SMS_setBGPaletteColor(1, bg1);
    SMS_loadPSGaidencompressedTiles(outrunner_font_psgcompr, 128);
    SMS_configureTextRenderer(96);
}

// grid functions //
void gridInit(void) {
    int cell = 0;
    for(int x = 0; x <= MAPW; x++) { 
        for(int y = 0; y <= MAPH; y++) { 
            cell = rand_num(1,5);
            if(cell == 1) { 
                grid[x][y] = 1;
            }
        }
    }
}

void gridDraw(void) {
    int i = TILE_BG_DARK;
    for(int x = 0; x <= 32; x++) {
        for(int y = 0; y <= 24; y++) {
            SMS_setTileatXY(x,y,i);
            i++;
            if(i == TILE_BG_LIGHT + 1) { i = TILE_BG_DARK; }
        }
    }
}

// cell functions //
void cellScanGrid(void) {
    for(int x = 0; x <= MAPW; x++) { 
        for(int y = 0; y <= MAPH; y++) { 
            if(grid[x][y] != 0) { 
                cellDraw(x,y);
            }
        }
    }
}

void cellDraw(int x, int y) {
    SMS_setTileatXY(x,y,TILE_CELL);
}

int cellCheckNeighboor(int x, int y, int cellStatus) { 
    int alive = 0;
    int neighboors = 0;

    // Check line above cell //
    if(x-1 >= 0 && y-1 >= 0 && grid[x-1][y-1] == 1) { neighboors += 1; }
    if(y-1 >= 0 && grid[x][y-1] == 1) { neighboors += 1; }
    if(x + 1 <= MAPW && y-1 >= 0 && grid[x+1][y-1] == 1 ) { neighboors += 1; }
    
    // Check left / right of the cell //
    if(x-1 >= 0 && grid[x-1][y] == 1) { neighboors += 1; }
    if(x+1 <= MAPW && grid[x+1][y] == 1) { neighboors += 1; }

    //Check line below cell //
    if(x-1 >= 0 && y + 1 <= MAPH && grid[x-1][y+1] == 1) { neighboors += 1; }
    if(y+1 <= MAPH && grid[x][y+1] == 1) { neighboors += 1; }
    if(x+1 <= MAPW && y+1 <= MAPH && grid[x+1][y+1] == 1) {neighboors += 1; }

    // Rules that Apply for a Living Cell //
    if(cellStatus == 1) {
        if(neighboors < 2) { alive = 0; } // underpopulated //
        if(neighboors == 2 || neighboors == 3) { alive = 1; } // cell is alive //
        if(neighboors > 3) { alive = 0; } // overpopulated //
        return alive;
    }
    
    // Rules that apply for a Dead Cell //
    if(cellStatus == 0) { 
        if(neighboors == 3) { alive = 1; } // if a deadcell has exactly 3 neighboors, life is created //
        return alive;
    }

    return alive;
}

int cellCountAlive(void) { 
    int alive = 0;
    for(int x = 0; x <= MAPW; x++) { 
        for(int y = 0; y <= MAPH; y++) { 
            if(grid[x][y] != 0) { 
                alive += 1;
            }
        }
    }
    return alive;
}

void generationProc(void) { 
    int live = 0;
    int cellStatus = 0;
    gen += 1;

    for(int x = 0; x <= MAPW; x++) { 
        for(int y = 0; y <= MAPH; y++) { 
            cellStatus = grid[x][y];
            live = cellCheckNeighboor(x,y,cellStatus);
            newGenGrid[x][y] = live;
        }
    }
    memcpy(grid, newGenGrid, sizeof(grid));
}

// controller Functions //
void controllerCheck(void) { 
    unsigned int key=SMS_getKeysStatus();
    unsigned int keyp = SMS_getKeysPressed();

	//PORT_A_KEY_UP
	//PORT_A_KEY_DOWN
    //PORT_A_KEY_LEFT
	//PORT_A_KEY_RIGHT

    // title screen //
    if(gamestate == 0) { 
        if(key & PORT_A_KEY_1)	{
            srand(randomSeed);
            gridInit();
            newstate = 1;
            gamestate = 1;
            return;
        }

        if(keyp & PORT_A_KEY_DOWN) { 
            if(op.screenIndex < 1) { op.screenIndex += 1; }
        }
        if(keyp & PORT_A_KEY_UP) { 
            if(op.screenIndex > 0) { op.screenIndex -= 1; }
        }
        if(keyp & PORT_A_KEY_LEFT) { 
            if(op.screenIndex == 0 && op.pattern > 0) { 
                op.pattern -= 1;
            }
            if(op.screenIndex == 1 && op.autogen > 0) { 
                op.autogen -= 1;
            }
        }
        if(keyp & PORT_A_KEY_RIGHT) {
            if(op.screenIndex == 0 && op.pattern < 2) { 
                op.pattern += 1;
            }
            if(op.screenIndex == 1 && op.autogen < 2) { 
                op.autogen += 1;
            }
        }
    }

    // cell view //
    if(gamestate == 1) { 
        if(key & PORT_A_KEY_1 && op.autogen == 0)	{ 
            generationProc();
            gridDraw();
        }

        if(key & PORT_A_KEY_2) {
            newstate = 1;
            gamestate = 2;
            return;
        }
    }

    // stats view // 
    if(gamestate == 2) { 
        if(key & PORT_A_KEY_2) { 
            newstate = 1;
            gamestate = 1;
            return;
        }
    }
}

// Stats Screens //
void statsRender(int alive) {
    SMS_printatXY(4,4,"Generations:");
    sprintf(buffer, "%u", gen);
    SMS_printatXY(20,4, buffer);
    SMS_printatXY(4,6, "Cells Alive:");
    sprintf(buffer, "%u", alive);
    SMS_printatXY(20,6, buffer);
}


// title screen render // 
void titleScreenRender(void) { 
    int cursorY = 20;
    int cursorBlank = 22;

    SMS_printatXY(10,8,"GAME OF LIFE");
    SMS_printatXY(10,10,"PRESS 1");

    SMS_printatXY(5,20, "PATTERN:");
    SMS_printatXY(5,22, "AUTOGEN:");

    if(op.pattern == 0) { 
        SMS_printatXY(15,20,"RANDOM"); 
    } else { 
        SMS_printatXY(15,20,"      ");
        sprintf(buffer, "%u", op.pattern);
        SMS_printatXY(15,20, buffer);
    }

    if(op.autogen == 0) { 
        SMS_printatXY(15,22,"NO  ");
    } else { 
        SMS_printatXY(15,22,"AUTO");
    }

    if(op.screenIndex == 0) { cursorY = 20; cursorBlank = 22; } else { cursorY = 22; cursorBlank = 20; }

    SMS_printatXY(14,cursorY,">");
    SMS_setTileatXY(14,cursorBlank,TILE_BLANK);

    //SMS_debugPrintf("pattern: %u, gen: %u, index: %u\n", op.pattern, op.autogen, op.screenIndex);
    randomSeed += 1; 
    if(randomSeed > 65000) { randomSeed = 0; }
}


// misc functions //
int rand_num(int lb, int ub) {
	int ret;
	ret = rand() % (ub - lb + 1) + lb;
	return ret;
}

// main game loop //
void main(void) {
    vdpConfig();
    vdpVRAMClear();
    vdpResetDisplay();
    
    gamestate = 0;
    newstate = 1;

    while(true) {

        // Title Screen //
        if(gamestate == 0) { 
            if(newstate == 1) {
                bgBlank();
                fontLoad(0, 45); 
                op.screenIndex = 0;
                op.pattern = 0;
                op.autogen = 0;
                newstate = 0;
            }
            titleScreenRender();
            controllerCheck();
            SMS_waitForVBlank();
        }

        // Main Cells View //
        if(gamestate == 1) { 
            if(newstate == 1) {
                bgLoad();
                gridDraw();
                newstate = 0;
            }
            controllerCheck(); // Call generationProc() if button pressed is A //

            //autoge is on //
            if(op.autogen == 1) { 
                generationProc();
                gridDraw();
            }

            cellScanGrid();
            SMS_waitForVBlank();
        }

        // Stats Screen //
        if(gamestate == 2) { 
            if(newstate == 1) { 
                bgBlank();
                fontLoad(0,15);
                newstate = 0;
            }
            //SMS_initSprites();
            controllerCheck();
            aliveNum = cellCountAlive();
            statsRender(aliveNum);
            SMS_waitForVBlank();
            //SMS_copySpritestoSAT();
        }
    }
}