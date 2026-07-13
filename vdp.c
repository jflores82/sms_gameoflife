// SDCC Includes //
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// SMS Includes //
#include "SMSlib/SMSlib.h"

// Header //
#include "vdp.h"

// VDP Stuff //
void vdpConfig(void) {
	SMS_VDPturnOnFeature(VDPFEATURE_HIDEFIRSTCOL);
	SMS_VDPturnOnFeature(VDPFEATURE_LEFTCOLBLANK);
	SMS_VRAMmemsetW(0, 0x0000, 16384); // CLEAR VRAM
	// SMS_useFirstHalfTilesforSprites(true); // not used atm //
}

void vdpVRAMClear(void) { 
	SMS_VRAMmemsetW(0, 0x0000, 16384); // Clear VRAM //
}

void vdpClearSprites(void) { 
	SMS_initSprites();
	SMS_copySpritestoSAT();
}

void vdpResetDisplay(void) {
	SMS_displayOff();
	SMS_waitForVBlank();
	SMS_displayOn();
}