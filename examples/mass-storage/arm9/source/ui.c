// SPDX-License-Identifier: MIT
//
// SPDX-FileContributor: Adrian "asie" Siekierka, 2024

#include <stdio.h>
#include <nds.h>
#include <nds/arm9/dldi.h>
#include <nds/arm9/dldi_asm.h>
#include <fat.h>
#include "ui.h"

#include <topLogo.h>

PrintConsole bottomConsole, topConsole;

extern u8 gDldiStub[];

static int bg;

/*DTCM_DATA volatile u16 read_activity_value = 0;
DTCM_DATA volatile u16 write_activity_value = 0;
DTCM_DATA volatile bool last_key_lid = false;
DTCM_DATA volatile bool curr_key_lid = false;


void ui_toggle_blink_activity(void) {
	read_activity_value ^= 0xA000;
}

void ui_toggle_blink_write_activity(void) {
	write_activity_value ^= 0x9000;
}

void CheckLid() {
	last_key_lid = false;
	curr_key_lid = (keysHeld() & KEY_LID);
	if (last_key_lid != curr_key_lid) {
		if (curr_key_lid)
			powerOff(POWER_ALL_2D);
		else
			powerOn(POWER_ALL_2D);
		// swiWaitForVBlank();
		// cothread_yield_irq(IRQ_VBLANK);
		last_key_lid = curr_key_lid;
	}
}


void vblankHandler (void) {
	topConsole.fontBgMap[(23 * 32) + 30] = read_activity_value;
	topConsole.fontBgMap[(23 * 32) + 29] = write_activity_value;
	CheckLid();
}*/

void PrintText(const char* text) {
	printf(text);
}

void UpdateDLDIText(void) {
	consoleSelect(&topConsole);
	consoleClear();
	for (int i = 0; i < 21; i++)printf("\n");
	printf("Target Interface:\n\n");
    printf("\x1b[37;0m%s", ((DLDI_INTERFACE*)gDldiStub)->friendlyName);
	consoleSelect(&bottomConsole);
}

void ui_init(void) {
    videoSetMode(MODE_4_2D);
	videoSetModeSub(MODE_0_2D);
    vramSetPrimaryBanks(VRAM_A_LCD, VRAM_B_LCD, VRAM_C_SUB_BG, VRAM_D_MAIN_BG_0x06000000);
	
	bg = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 1, 0);
	// Load graphics after font or else you get palette conflicts. :P
	decompress(topLogoBitmap, bgGetGfxPtr(bg), LZ77Vram);
	dmaCopy(topLogoPal, BG_PALETTE, 256*2);
	
	consoleInit(&bottomConsole,
        0, BgType_Text4bpp, BgSize_T_256x256, 22, 3, false, true);
    consoleInit(&topConsole,
        0, BgType_Text4bpp, BgSize_T_256x256, 4, 6, true, true); // Reconfigured to allow text and 8bit image.
	
    UpdateDLDIText();
	
    puts("\x1b[2J" "\x1b[23;0H");

    /*for (int i = 0; i < 8; i++) {
        topConsole.fontBgMap[(23 * 32) + 24 + i] = ((uint8_t) '*') + topConsole.fontCharOffset - topConsole.font.asciiOffset;
    }
	
	read_activity_value = topConsole.fontBgMap[(23 * 32) + 30];
	write_activity_value = topConsole.fontBgMap[(23 * 32) + 29];*/
	
	// Enable vblank handler
	/*irqSet(IRQ_VBLANK, vblankHandler);
	irqEnable(IRQ_VBLANK);*/
}

void ui_select_top(void) {
    consoleSelect(&topConsole);
}

void ui_select_bottom(void) {
    consoleSelect(&bottomConsole);
}

