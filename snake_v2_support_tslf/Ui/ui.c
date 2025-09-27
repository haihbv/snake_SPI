#include "ui.h"
#include "st7735.h"
#include "fonts.h"
#include "delay.h"
#include <string.h>
#include <stdio.h>

/* =========================================================
 * UI Driver Instance
 * ========================================================= */
UI_Driver_t ui;

/* =========================================================
 * State Blink + Score
 * ========================================================= */
static uint32_t s_blink_ts = 0;
static _Bool s_blink_on = 1;
static uint16_t s_blink_y = 0; // vi tri dong nhap nhay
uint16_t s_final_score = 0;

/* =========================================================
 * Helper
 * ========================================================= */
static inline void draw_frame(uint16_t color)
{
    for (int t = 0; t < UI_FRAME_THICK; ++t)
    {
        lcd.DrawHLine(0, (uint8_t)t, ST7735_WIDTH, color);
        lcd.DrawHLine(0, (uint8_t)(ST7735_HEIGHT - 1 - t), ST7735_WIDTH, color);
        lcd.DrawVLine((uint8_t)t, 0, ST7735_HEIGHT, color);
        lcd.DrawVLine((uint8_t)(ST7735_WIDTH - 1 - t), 0, ST7735_HEIGHT, color);
    }
}

static inline void text_center(uint16_t y, const char *str, Font_Define_t f, uint16_t fg, uint16_t bg)
{
    const uint16_t W = ST7735_WIDTH;
    const uint16_t px = (uint16_t)strlen(str) * f.width;
    const uint16_t x = (W > px) ? (uint16_t)((W - px) / 2) : 0;
    lcd.PutString((uint8_t)x, (uint8_t)y, str, f, fg, bg);
}

static inline void text_center_highlight(uint16_t y, const char *left, const char *mid, const char *right, Font_Define_t f, uint16_t fg, uint16_t midColor, uint16_t bg)
{
    const uint16_t total_chars = (uint16_t)(strlen(left) + strlen(mid) + strlen(right));
    const uint16_t total_px = total_chars * f.width;
    uint16_t x = (ST7735_WIDTH > total_px) ? (uint16_t)((ST7735_WIDTH - total_px) / 2) : 0;

    lcd.PutString((uint8_t)x, (uint8_t)y, left, f, fg, bg);
    x += (uint16_t)strlen(left) * f.width;
    lcd.PutString((uint8_t)x, (uint8_t)y, mid, f, midColor, bg);
    x += (uint16_t)strlen(mid) * f.width;
    lcd.PutString((uint8_t)x, (uint8_t)y, right, f, fg, bg);
}

static inline void divider(uint16_t y, uint16_t color)
{
    lcd.DrawHLine(6, (uint8_t)y, ST7735_WIDTH - 12, color);
}

static inline void blink_reset(void)
{
    s_blink_ts = millis();
    s_blink_on = 1;
}

/* Vi tri Blink gan voi vien */
static inline uint16_t bottom_blink_y(void)
{
    const uint16_t h = UI_FONT_TEXT.height;
    return (ST7735_HEIGHT > h + 1) ? (uint16_t)(ST7735_HEIGHT - h - 1) : 0;
}

static inline void blink_center_line(uint16_t y, const char *left, const char *mid, const char *right)
{
    const uint32_t now = millis();
    if (now - s_blink_ts > UI_BLINK_PERIOD_MS)
    {
        s_blink_ts = now;
        s_blink_on = !s_blink_on;
    }

    if (s_blink_on)
        text_center_highlight(y, left, mid, right, UI_FONT_TEXT, UI_COLOR_TEXT, UI_COLOR_ACCENT, UI_COLOR_BG);
    else
        text_center_highlight(y, left, mid, right, UI_FONT_TEXT, UI_COLOR_BG, UI_COLOR_BG, UI_COLOR_BG);
}

/* =========================================================
 * Public: set final score
 * ========================================================= */
void UI_SetFinalScore(uint16_t score)
{
    s_final_score = score;
}

/* =========================================================
 * Screens
 * ========================================================= */
void UI_ShowStartScreen(void)
{
    lcd.FillScreen_Fast(UI_COLOR_BG);
    draw_frame(UI_COLOR_ACCENT);

    uint16_t y = UI_TOP_MARGIN;

    text_center(y, "S N A K E", UI_FONT_TITLE, UI_COLOR_TITLE, UI_COLOR_BG);
    y += UI_LINE_SPACING + 8;
    divider(y, UI_COLOR_ACCENT);

    y += 8;
    text_center_highlight(y, "Press ", "START", " to", UI_FONT_TEXT, UI_COLOR_TEXT, UI_COLOR_ACCENT, UI_COLOR_BG);

    y += UI_LINE_SPACING;
    text_center(y, "Enter Game", UI_FONT_TEXT, UI_COLOR_TEXT, UI_COLOR_BG);

    y = (uint16_t)(ST7735_HEIGHT - (UI_LINE_SPACING * 3));
    text_center(y, "Use Arrow Buttons", UI_FONT_TEXT, UI_COLOR_MUTED, UI_COLOR_BG);

    s_blink_y = bottom_blink_y();
    blink_reset();
}

void UI_UpdateStartBlink(void)
{
    blink_center_line(s_blink_y, ">> Push ", "START", " <<");
}

void UI_ShowOverScreen(void)
{
    lcd.FillScreen_Fast(UI_COLOR_BG);
    draw_frame(UI_COLOR_ACCENT);

    uint16_t y = UI_TOP_MARGIN;

    text_center(y, "GAME OVER", UI_FONT_TITLE, UI_COLOR_TITLE, UI_COLOR_BG);
    y += UI_LINE_SPACING + 8;
    divider(y, UI_COLOR_ACCENT);

    y += 6;
    char buf[32];
    snprintf(buf, sizeof(buf), "Final Score: %u", (unsigned)s_final_score);
    text_center(y, buf, UI_FONT_TEXT, UI_COLOR_TEXT, UI_COLOR_BG);

    y += UI_LINE_SPACING;
    text_center_highlight(y, "Press ", "RESET", " button", UI_FONT_TEXT, UI_COLOR_TEXT, UI_COLOR_ACCENT, UI_COLOR_BG);

    y += UI_LINE_SPACING;
    text_center(y, "to continue play", UI_FONT_TEXT, UI_COLOR_TEXT, UI_COLOR_BG);

    // Vi tri Blink
    s_blink_y = bottom_blink_y();
    blink_reset();
}

void UI_UpdateOverBlink(void)
{
    blink_center_line(s_blink_y, ">> Push ", "RESET", " <<");
}

void UI_ShowWinScreen(void)
{
    lcd.FillScreen_Fast(UI_COLOR_BG);
    draw_frame(UI_COLOR_ACCENT);

    uint16_t y = UI_TOP_MARGIN;

    text_center(y, "YOU WIN", UI_FONT_TITLE, UI_COLOR_TITLE, UI_COLOR_BG);
    y += UI_LINE_SPACING + 8;
    divider(y, UI_COLOR_ACCENT);

    y += 6;
    char buf[32];
    snprintf(buf, sizeof(buf), "Final Score: %u", (unsigned)s_final_score);
    text_center(y, buf, UI_FONT_TEXT, UI_COLOR_TEXT, UI_COLOR_BG);

    y += UI_LINE_SPACING;
    text_center_highlight(y, "Press ", "RESTART", " ", UI_FONT_TEXT, UI_COLOR_TEXT, UI_COLOR_ACCENT, UI_COLOR_BG);

    y += UI_LINE_SPACING;
    text_center(y, "button to continue", UI_FONT_TEXT, UI_COLOR_TEXT, UI_COLOR_BG);

    // Vi tri blink
    s_blink_y = bottom_blink_y();
    blink_reset();
}

void UI_UpdateWinBlink(void)
{
    blink_center_line(s_blink_y, ">> Push ", "RESTART", " <<");
}

void UI_ClearToGameBg(void)
{
    lcd.FillScreen_Fast(UI_COLOR_BG);
}

/* =========================================================
 * Driver binding
 * ========================================================= */
void UI_AutoInit(void) __attribute__((constructor));
void UI_AutoInit(void)
{
    ui.ShowStartScreen = UI_ShowStartScreen;
    ui.UpdateStartBlink = UI_UpdateStartBlink;

    ui.ShowOverScreen = UI_ShowOverScreen;
    ui.UpdateOverBlink = UI_UpdateOverBlink;

    ui.ShowWinScreen = UI_ShowWinScreen;
    ui.UpdateWinBlink = UI_UpdateWinBlink;

    ui.ClearToGameBg = UI_ClearToGameBg;
    ui.FinalScore = UI_SetFinalScore;
}
