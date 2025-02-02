/*************************************************************************
 * Shattered Lands: Towers of Perdition                                  *
 * C opyright (C) 2019  Justin (tuxinator2009) Davis                     *
 *                                                                       *
 * This program is free software: you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation, either version 3 of the License, or     *
 * (at your option) any later version.                                   *
 *                                                                       *
 * This program is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * You should have received a copy of the GNU General Public License     *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *************************************************************************/

#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include "Arduboy2Ext.h"

#define READ_WORD(x) __builtin_bswap16(pgm_read_word(x))
#define READ_DWORD(x) __builtin_bswap32(pgm_read_dword(x))

#define CHAR_SPACE	0
#define CHAR_A			1
#define CHAR_B			2
#define CHAR_C			3
#define CHAR_D			4
#define CHAR_E			5
#define CHAR_F			6
#define CHAR_G			7
#define CHAR_H			8
#define CHAR_I			9
#define CHAR_J			10
#define CHAR_K			11
#define CHAR_L			12
#define CHAR_M			13
#define CHAR_N			14
#define CHAR_O			15
#define CHAR_P			16
#define CHAR_Q			17
#define CHAR_R			18
#define CHAR_S			19
#define CHAR_T			20
#define CHAR_U			21
#define CHAR_V			22
#define CHAR_W			23
#define CHAR_X			24
#define CHAR_Y			25
#define CHAR_Z			26
#define CHAR_0			27
#define CHAR_1			28
#define CHAR_2			29
#define CHAR_3			30
#define CHAR_4			31
#define CHAR_5			32
#define CHAR_6			33
#define CHAR_7			34
#define CHAR_8			35
#define CHAR_9			36
#define CHAR_DASH		37
#define CHAR_EXCL		38
#define CHAR_PLUS		39
#define CHAR_QUOTE	40
#define CHAR_COMMA	41
#define CHAR_DOT		42
#define CHAR_QUES		43
#define CHAR_SLASH	44
#define CHAR_OK			45
#define CHAR_NONE		46
#define CHAR_SWORD	47
#define CHAR_SHIELD	48
#define CHAR_BOOT		49
#define CHAR_WAND		50
#define CHAR_NULL		62
#define CHAR_LINE		63

#define STATE_TITLE				0
#define STATE_PLAYING			1
#define STATE_BATTLE			2
#define STATE_MENU_MAIN		3
#define STATE_MENU_ITEMS	4
#define STATE_MENU_STATS	5
#define STATE_UPLOAD			6
#define STATE_STORY				7
#define STATE_PURCHASE		8
#define STATE_NAMEENTRY		9

#define BATTLE_CHOOSEACTION			0
#define BATTLE_SELECT_MONSTER		1
#define BATTLE_SELECT_ITEM			2
#define BATTLE_SELECT_SPELL			3
#define BATTLE_MONSTER_ACTIONS	4
#define BATTLE_RESOLVE_COMBAT		5
#define BATTLE_VICTORY					6

#define IMMUNE_CURE	0x1
#define IMMUNE_NUKE	0x4
#define IMMUNE_FIRE	0x10
#define IMMUNE_AQUA	0x40
#define IMMUNE_ROCK	0x100
#define IMMUNE_AERO	0x400
#define IMMUNE_DARK	0x1000
#define IMMUNE_HOLY	0x4000

#define LARGE_CHEST_TILE	1472

#define EVENT_STEP_ACTIVATED		0x00
#define EVENT_BUTTON_ACTIVATED	0x80

#define FLAG_USE_MESSAGE_BUFFER		1
#define FLAG_SHOW_GOLD						2
#define FLAG_SHOW_YESNO						4
#define FLAG_SHOW_MESSAGE_BUFFER	8
#define FLAG_HOLD_MESSAGE_BUFFER	16
#define FLAG_BATTLE_ESCAPE				32

#define RANDOM_CHANCE(chance) (chance > random(100000))

static const uint8_t DIR_UP = 0;
static const uint8_t DIR_DOWN = 1;
static const uint8_t DIR_LEFT = 2;
static const uint8_t DIR_RIGHT = 3;

Arduboy2Ext arduboy;

const uint8_t *currentMonsterGroup = 0;
uint8_t globalCounter = 0;
int8_t selection = 0;
int8_t lineStartX = 0;
int8_t cursorX = 0;
int8_t cursorY = 0;
uint8_t gameState = STATE_TITLE;
uint8_t previousState = STATE_TITLE;
uint8_t battleState = BATTLE_CHOOSEACTION;
uint8_t battleFlashSprite = 0x00;
int8_t playerSteps = 0;
uint8_t battleRenderSprite = 0xFF;
int16_t cost = 0;
const uint8_t *bitReaderByte = 0;
uint8_t bitReaderValue = 0;
uint8_t bitReaderBit = 0;
uint8_t tilesetID = 0;
uint8_t numChunks = 0;
int8_t playerXOff;
int8_t playerYOff;
uint8_t flags = 0;
uint8_t chunkIDs[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
const uint8_t *pMap = 0;
const uint8_t *pMapEvents = 0;
const uint8_t *pChunk = 0;

static const uint8_t PROGMEM itemCosts[] = {10, 30, 60, 15, 30, 60, 100, 200};
static const uint8_t PROGMEM healItems[] = {30, 120, 250, 3, 10, 30};

void waitButtons(uint8_t buttons)
{
	arduboy.pollButtons();
	while (!arduboy.justPressed(buttons))
		arduboy.pollButtons();
	arduboy.pollButtons();
}

void bitReaderInit(const uint8_t *startByte, uint8_t bitNumber)
{
	bitReaderByte = startByte;
	bitReaderValue = pgm_read_byte(startByte);
	bitReaderBit = 0x80 >> bitNumber;
}

bool bitReaderNextBit()
{
	bool bitSet = bitReaderValue & bitReaderBit;
	bitReaderBit >>= 1;
	if (bitReaderBit == 0)
	{
		++bitReaderByte;
		bitReaderValue = pgm_read_byte(bitReaderByte);
		bitReaderBit = 0x80;
	}
	return bitSet;
}

uint8_t bitReaderRead8(int8_t numBits)
{
	uint8_t result = 0;
	while (numBits > 0)
	{
		result = (result * 2) + (bitReaderNextBit() ? 1:0);
		--numBits;
	}
	return result;
}

uint16_t bitReaderRead16(int8_t numBits)
{
	uint16_t result = 0;
	while (numBits > 0)
	{
		result = (result * 2) + (bitReaderNextBit() ? 1:0);
		--numBits;
	}
	return result;
}

uint32_t bitReaderRead32(int8_t numBits)
{
	uint32_t result = 0;
	while (numBits > 0)
	{
		result = (result * 2) + (bitReaderNextBit() ? 1:0);
		--numBits;
	}
	return result;
}

#endif //GLOBALS_H
