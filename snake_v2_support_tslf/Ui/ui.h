#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /* ===========================
     * UI CONFIG
     * =========================== */
    extern uint16_t s_final_score;

/* Font */
#ifndef UI_FONT_TITLE
#define UI_FONT_TITLE Font_11x18
#endif
#ifndef UI_FONT_TEXT
#define UI_FONT_TEXT Font_7x10
#endif

/* Color (RGB565) */
#ifndef UI_COLOR_BG
#define UI_COLOR_BG BLACK
#endif
#ifndef UI_COLOR_TITLE
#define UI_COLOR_TITLE GREEN
#endif
#ifndef UI_COLOR_TEXT
#define UI_COLOR_TEXT WHITE
#endif
#ifndef UI_COLOR_ACCENT
#define UI_COLOR_ACCENT 0xDFE0
#endif
#ifndef UI_COLOR_MUTED
#define UI_COLOR_MUTED 0x4228
#endif

#ifndef UI_FRAME_THICK
#define UI_FRAME_THICK 1
#endif
#ifndef UI_TOP_MARGIN
#define UI_TOP_MARGIN 6
#endif
#ifndef UI_LINE_SPACING
#define UI_LINE_SPACING 14
#endif
#ifndef UI_BLINK_PERIOD_MS
#define UI_BLINK_PERIOD_MS 220
#endif

typedef struct
{
    void (*ShowStartScreen)(void);
    void (*UpdateStartBlink)(void);
    void (*ShowOverScreen)(void);
    void (*UpdateOverBlink)(void);
    void (*ShowWinScreen)(void);
    void (*UpdateWinBlink)(void);
    void (*ClearToGameBg)(void);
    void (*FinalScore)(uint16_t score);
} UI_Driver_t;
extern UI_Driver_t ui;
/******************************************
 * API
 *******************************************/
void UI_ShowStartScreen(void);
void UI_UpdateStartBlink(void);
void UI_ShowOverScreen(void);
void UI_UpdateOverBlink(void);
void UI_ShowWinScreen(void);
void UI_UpdateWinBlink(void);
void UI_ClearToGameBg(void);
void UI_SetFinalScore(uint16_t score);

#ifdef __cplusplus
}
#endif
