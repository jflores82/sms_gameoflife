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
unsigned char prevGenGrid[32][24];
unsigned char bgGrid[32][24];
char buffer[8];
unsigned char newstate = 0;
unsigned char gamestate = 1;
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
void fontLoad(unsigned char bg0, unsigned char bg1);
void fontSpriteLoad(unsigned char bg0, unsigned char bg1);
//void spriteLoad(void);
void bgLoad(void);
void bgBlank(void);

void gridInit(unsigned char pattern);
void gridGen(void);
void gridDrawFull(void);
void gridDrawCell(unsigned char x, unsigned char y);
void gridScan(void);
void gridDrawBorders(void);

void cellDraw(unsigned char x, unsigned char y);
int  cellCheckNeighboor(unsigned char x, unsigned char y, unsigned char cellStatus);
int  cellCountAlive(void);

void generationProc(void);



void controllerCheck(void);

void statsRender(unsigned char alive);
void titleScreenRender(void);
void hudDraw(void);

#define PATTERN_TOTAL 9

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

void fontLoad(unsigned char bg0, unsigned char bg1) { 
    SMS_setBGPaletteColor(0, bg0);
    SMS_setBGPaletteColor(1, bg1);
    SMS_loadPSGaidencompressedTiles(outrunner_font_psgcompr, 128);
    SMS_configureTextRenderer(96);
}

void fontSpriteLoad(unsigned char bg0, unsigned char bg1) { 
    SMS_setSpritePaletteColor(0, bg0);
    SMS_setSpritePaletteColor(1, bg1);
    SMS_loadPSGaidencompressedTiles(outrunner_font_psgcompr, 255);
    SMS_configureTextRenderer(223);
}

// grid functions //
void gridInit(unsigned char pattern) {

    // half: x:15 / y:11 //

    // random //
    if(pattern == 0) { 
        unsigned char cell = 0;
        for(unsigned char x = 2; x <= MAPW - 1; x++) { 
            for(unsigned char y = 1; y <= MAPH - 1; y++) { 
                cell = rand_num(1,5);
                if(cell == 1) { 
                    grid[x][y] = 1;
                }
            }
        }
    }

    // blinker // 
    if(pattern == 1) { 
        grid[15][11] = 1;
        grid[15][12] = 1;
        grid[15][13] = 1;
    }

    // glider //
    if(pattern == 2) { 
        grid[3][1] = 1;
        grid[4][2] = 1;
        grid[2][3] = 1;
        grid[3][3] = 1;
        grid[4][3] = 1;
    }

    // pulsar //
    if(pattern == 3) { 
        grid[10][6] = 1;
        grid[11][6] = 1;
        grid[12][6] = 1;

        grid[16][6] = 1;
        grid[17][6] = 1;
        grid[18][6] = 1;

        grid[8][8] = 1;
        grid[13][8] = 1;
        grid[15][8] = 1;
        grid[20][8] = 1;

        grid[8][9] = 1;
        grid[13][9] = 1;
        grid[15][9] = 1;
        grid[20][9] = 1;

        grid[8][10] = 1;
        grid[13][10] = 1;
        grid[15][10] = 1;
        grid[20][10] = 1;

        grid[10][11] = 1;
        grid[11][11] = 1;
        grid[12][11] = 1;

        grid[16][11] = 1;
        grid[17][11] = 1;
        grid[18][11] = 1;

        grid[10][11] = 1;
        grid[11][11] = 1;
        grid[12][11] = 1;

        grid[16][11] = 1;
        grid[17][11] = 1;
        grid[18][11] = 1;

        grid[10][13] = 1;
        grid[11][13] = 1;
        grid[12][13] = 1;

        grid[16][13] = 1;
        grid[17][13] = 1;
        grid[18][13] = 1;

        grid[8][14] = 1;
        grid[13][14] = 1;
        grid[15][14] = 1;
        grid[20][14] = 1;

        grid[8][15] = 1;
        grid[13][15] = 1;
        grid[15][15] = 1;
        grid[20][15] = 1;

        grid[8][16] = 1;
        grid[13][16] = 1;
        grid[15][16] = 1;
        grid[20][16] = 1;

        grid[10][18] = 1;
        grid[11][18] = 1;
        grid[12][18] = 1;

        grid[16][18] = 1;
        grid[17][18] = 1;
        grid[18][18] = 1;

        grid[10][18] = 1;
        grid[11][18] = 1;
        grid[12][18] = 1;

        grid[16][18] = 1;
        grid[17][18] = 1;
        grid[18][18] = 1;
    }

    // bee hive //
    if(pattern == 4) { 
        grid[11][10] = 1;
        grid[12][10] = 1;
        grid[10][11] = 1;
        grid[13][11] = 1;
        grid[11][12] = 1;
        grid[12][12] = 1;
    }

    // Toad //
    if(pattern == 5) {
        grid[11][10] = 1;
        grid[12][10] = 1;
        grid[13][10] = 1;
        grid[10][11] = 1;
        grid[11][11] = 1;
        grid[12][11] = 1;
    }

    // Pulsar Cousin //
    if(pattern == 6) { 
        grid[12][8] = 1;
        grid[17][8] = 1;
        grid[12][10] = 1;
        grid[17][10] = 1;

        grid[10][9] = 1;
        grid[11][9] = 1;
        grid[13][9] = 1;
        grid[14][9] = 1;
        grid[15][9] = 1;
        grid[16][9] = 1;
        grid[18][9] = 1;
        grid[19][9] = 1;

    }

    //LWSS - Light Weight Space Ship //
    if(pattern == 7) { 
        grid[27][10] = 1;
        grid[30][10] = 1;
        grid[26][11] = 1;
        grid[26][12] = 1;
        grid[30][12] = 1;
        grid[26][13] = 1;
        grid[27][13] = 1;
        grid[28][13] = 1;
        grid[29][13] = 1;
    }

    //DieHard//
    if(pattern == 8) { 
        grid[16][10] = 1;

        grid[10][11] = 1;
        grid[11][11] = 1;

        grid[11][12] = 1;
        grid[15][12] = 1;
        grid[16][12] = 1;
        grid[17][12] = 1;
    }

    //r-pantomino//
    if(pattern == 9) {
        grid[6][5] = 1;
        grid[7][5] = 1;

        grid[5][6] = 1;
        grid[6][6] = 1;

        grid[6][7] = 1;
    }

}

void gridGen(void) {
    unsigned char i = TILE_BG_DARK;
    for(unsigned char x = 2; x <= MAPW -1; x++) {
        if(x % 2 == 0) { i = TILE_BG_LIGHT; } else { i = TILE_BG_DARK; }
        for(unsigned char y = 1; y <= MAPH -1; y++) {
            //SMS_setTileatXY(x,y,i);
            bgGrid[x][y] = i;
            i++;
            if(i == TILE_BG_LIGHT + 1) { i = TILE_BG_DARK; }
        }
    }
}

void gridDrawFull(void) { 
    gridDrawBorders();
    for(unsigned char x = 2; x <= MAPW - 1; x++) {
        for(unsigned char y = 1; y <= MAPH - 1; y++) {
            unsigned char tile = bgGrid[x][y];
            SMS_setTileatXY(x,y, tile);
            if(grid[x][y] == 1) { cellDraw(x,y); }
        }
    }
}

void gridDrawBorders(void) {
    unsigned char tile = 3; 
    for(unsigned char x = 1; x <= MAPW; x++) { 
        SMS_setTileatXY(x,0,tile);
        SMS_setTileatXY(x,23,tile);
    }
    for(unsigned char y = 0; y <= MAPH; y++) { 
        SMS_setTileatXY(1,y,tile);
        SMS_setTileatXY(31,y,tile);
    }
}

void gridDrawCell(unsigned char x, unsigned char y) { 
    unsigned char tile = bgGrid[x][y];
    SMS_setTileatXY(x,y,tile);
}

void gridScan(void) {
    for(unsigned char x = 2; x <= MAPW -1; x++) { 
        for(unsigned char y = 1; y <= MAPH -1; y++) {
            if(grid[x][y] != prevGenGrid[x][y]) {
                if(grid[x][y] == 0) { gridDrawCell(x,y); }
                if(grid[x][y] == 1) { cellDraw(x,y); }
            }
        }
    }
}

// cell functions //
void cellDraw(unsigned char x, unsigned char y) {
    SMS_setTileatXY(x,y,TILE_CELL);
}

int cellCheckNeighboor(unsigned char x, unsigned char y, unsigned char cellStatus) { 
    int alive = 0;
    int neighboors = 0;

    // Check line above cell //
    /* if(x-1 >= 0 && y-1 >= 0 && grid[x-1][y-1] == 1) { neighboors += 1; }
    if(y-1 >= 0 && grid[x][y-1] == 1) { neighboors += 1; }
    if(x + 1 <= MAPW && y-1 >= 0 && grid[x+1][y-1] == 1 ) { neighboors += 1; }
    
    // Check left / right of the cell //
    if(x-1 >= 0 && grid[x-1][y] == 1) { neighboors += 1; }
    if(x+1 <= MAPW && grid[x+1][y] == 1) { neighboors += 1; }

    //Check line below cell //
    if(x-1 >= 0 && y + 1 <= MAPH && grid[x-1][y+1] == 1) { neighboors += 1; }
    if(y+1 <= MAPH && grid[x][y+1] == 1) { neighboors += 1; }
    if(x+1 <= MAPW && y+1 <= MAPH && grid[x+1][y+1] == 1) {neighboors += 1; } */

    if(grid[x-1][y-1] == 1) { neighboors += 1; }
    if(grid[x][y-1] == 1) { neighboors += 1; }
    if(grid[x+1][y-1] == 1 ) { neighboors += 1; }
    
    // Check left / right of the cell //
    if(grid[x-1][y] == 1) { neighboors += 1; }
    if(grid[x+1][y] == 1) { neighboors += 1; }

    //Check line below cell //
    if(grid[x-1][y+1] == 1) { neighboors += 1; }
    if(grid[x][y+1] == 1) { neighboors += 1; }
    if(grid[x+1][y+1] == 1) {neighboors += 1; }

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
    unsigned char alive = 0;
    for(unsigned char x = 0; x <= MAPW; x++) { 
        for(unsigned char y = 0; y <= MAPH; y++) { 
            if(grid[x][y] != 0) { 
                alive += 1;
            }
        }
    }
    return alive;
}

void generationProc(void) { 
    unsigned char live = 0;
    unsigned char cellStatus = 0;
    gen += 1;

    for(unsigned char x = 2; x <= MAPW -1 ; x++) { 
        for(unsigned char y = 1; y <= MAPH - 1; y++) { 
            cellStatus = grid[x][y];
            live = cellCheckNeighboor(x,y,cellStatus);
            newGenGrid[x][y] = live;
        }
    }
    memcpy(prevGenGrid, grid, sizeof(grid));
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
            if(op.screenIndex == 0 && op.pattern < PATTERN_TOTAL) { 
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
            //gridDrawFull();
        }

        if(key & PORT_A_KEY_2) {
            newstate = 1;
            gamestate = 2;
            return;
        }
    }

    // stats view //
    /* 
    if(gamestate == 2) { 
        if(key & PORT_A_KEY_2) { 
            newstate = 1;
            gamestate = 1;
            return;
        }
    } */
}

// Stats Screens //
void statsRender(unsigned char alive) {
    SMS_printatXY(4,4,"Generations:");
    sprintf(buffer, "%u", gen);
    SMS_printatXY(20,4, buffer);
    SMS_printatXY(4,6, "Cells Alive:");
    sprintf(buffer, "%u", alive);
    SMS_printatXY(20,6, buffer);
}

// title screen render // 
void titleScreenRender(void) { 
    unsigned char cursorY = 20;
    unsigned char cursorBlank = 22;

    SMS_printatXY(10,8,"GAME OF LIFE");
    SMS_printatXY(10,10,"PRESS 1");

    SMS_printatXY(5,20, "PATTERN:");
    SMS_printatXY(5,22, "MODE:");

    if(op.pattern == 0) { 
        SMS_printatXY(15,20,"RANDOM"); 
    } else { 
        SMS_printatXY(15,20,"      ");
        sprintf(buffer, "%u", op.pattern);
        SMS_printatXY(15,20, buffer);
    }

    if(op.autogen == 0) { 
        SMS_printatXY(15,22,"STEP");
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

// hud //
void hudDraw(void) { 
    unsigned char population;
    population = cellCountAlive();
    
    SMS_printatXY(2,23,"GEN:");
    sprintf(buffer, "%u", gen);
    SMS_printatXY(7,23, buffer);

    SMS_printatXY(12,23,"CELLS:");
    sprintf(buffer, "%u", population);
    SMS_printatXY(18,23, buffer);
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

    gridGen();
    
    

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
                if(gen == 0) { gridInit(op.pattern); }
                fontLoad(0,1);
                bgLoad();
                gridDrawFull();
                newstate = 0;
            }
            controllerCheck(); // Call generationProc() if button pressed is A //

            //autoge is on //
            if(op.autogen == 1) { 
                generationProc();
                //gridDrawFull();
            }

            gridScan();
            hudDraw();
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
            //SMS_waitForVBlank();
            //SMS_copySpritestoSAT();
        }
    }
}