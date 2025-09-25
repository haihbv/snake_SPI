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
	BTN_RESET,
	BTN_COUNT // so luong nut
} Button_e;

void Button_Init(void);
_Bool Button_IsPressed(Button_e btn);

#ifdef __cplusplus
}
#endif

#endif /* BUTTON_H */
