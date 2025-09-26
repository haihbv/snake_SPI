#include "ui.h"
#include "st7735.h"
#include "fonts.h"
#include "delay.h"
#include <string.h>

#ifndef UI_FONT_TITLE
#define UI_FONT_TITLE Font_11x18
#endif /* UI_FONT_TITLE */

#ifndef UI_FONT_TEXT
#define UI_FONT_TEXT Font_7x10
#endif /* UI_FONT_TEXT */

#define GAME_BG_COLOR BLACK
#define GAME_TITLE_COLOR GREEN
#define GAME_TEXT_COLOR WHITE
#define GAME_ACCENT_COLOR 0xDFE0

static uint32_t blinkTs = 0;
static _Bool blinkOn = 1;

static void Text_Center(uint16_t y, const char *str, Font_Define_t f, uint16_t fg, uint16_t bg)
{
	uint16_t weight = ST7735_WIDTH;
	uint16_t str_px = (uint16_t)(strlen(str) * f.width);
	uint16_t x = (weight > str_px) ? (uint16_t)((weight - str_px) / 2) : 0;
	lcd.PutString((uint8_t)x, (uint8_t)y, str, f, fg, bg);
}

static void Frame(uint16_t color)
{
	st7735_DrawHorizontal_Line(0, 0, ST7735_WIDTH, color);
	st7735_DrawHorizontal_Line(0, ST7735_HEIGHT - 1, ST7735_WIDTH, color);
	st7735_DrawVerical_Line(0, 0, ST7735_HEIGHT, color);
	st7735_DrawVerical_Line(ST7735_WIDTH - 1, 0, ST7735_HEIGHT, color);
}

static void Text_Center_Highlight(uint16_t y,
								  const char *left, const char *mid, const char *right,
								  Font_Define_t f, uint16_t fg, uint16_t midColor, uint16_t bg)
{
	uint16_t total_chars = (uint16_t)(strlen(left) + strlen(mid) + strlen(right));
	uint16_t total_px = total_chars * f.width;
	uint16_t x = (ST7735_WIDTH > total_px) ? (uint16_t)((ST7735_WIDTH - total_px) / 2) : 0;

	lcd.PutString((uint8_t)x, (uint8_t)y, (const char *)left, f, fg, bg);
	x += (uint16_t)strlen(left) * f.width;

	lcd.PutString((uint8_t)x, (uint8_t)y, (const char *)mid, f, midColor, bg);
	x += (uint16_t)strlen(mid) * f.width;

	lcd.PutString((uint8_t)x, (uint8_t)y, (const char *)right, f, fg, bg);
}

void UI_ShowStartScreen(void)
{
	lcd.FillScreen_Fast(GAME_BG_COLOR);
	Frame(GAME_ACCENT_COLOR);

	Text_Center(20, "S N A K E", UI_FONT_TITLE, GAME_TITLE_COLOR, GAME_BG_COLOR);
	Text_Center_Highlight(48, "Press ", "START", " to enter", Font_6x8,
						  GAME_TEXT_COLOR, GAME_ACCENT_COLOR, GAME_BG_COLOR);
	Text_Center(62, "Game", UI_FONT_TEXT, GAME_TEXT_COLOR, GAME_BG_COLOR);

	blinkTs = millis();
	blinkOn = 1;
}

void UI_UpdateStartBlink(void)
{
	uint32_t now = millis();
	if (now - blinkTs > 500)
	{
		blinkTs = now;
		blinkOn = !blinkOn;
		if (blinkOn)
		{
			Text_Center_Highlight(92, ">> Push ", "START", " <<", UI_FONT_TEXT, GAME_TEXT_COLOR, GAME_ACCENT_COLOR, GAME_BG_COLOR);
		}
		else
		{
			Text_Center_Highlight(92, ">> Push ", "START", " <<", UI_FONT_TEXT, GAME_BG_COLOR, GAME_BG_COLOR, GAME_BG_COLOR);
		}
	}
}

void UI_ClearToGameBg(void)
{
	lcd.FillScreen_Fast(GAME_BG_COLOR);
}
