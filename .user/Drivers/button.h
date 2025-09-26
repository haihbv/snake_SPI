#ifndef BUTTON_H
#define BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"

typedef enum
{
	BTN_UP = 0,
	BTN_DOWN,
	BTN_LEFT,
	BTN_RIGHT,
	BTN_COUNT // so luong nut
} Button_e;

typedef struct
{
	void (*Init)(void);
	_Bool (*IsPressed)(Button_e btn);
	void (*ResetInit)(void);
	_Bool (*ResetGame)(void);
} Btn_Driver_t;

extern Btn_Driver_t button;

#define RST_GAME_PORT GPIOA
#define RST_GAME_PIN  GPIO_Pin_8

void Button_Init(void);
_Bool Button_IsPressed(Button_e btn);

void Button_Reset_Init(void);
_Bool Button_IsResetPressed(void);

#ifdef __cplusplus
}
#endif

#endif /* BUTTON_H */
