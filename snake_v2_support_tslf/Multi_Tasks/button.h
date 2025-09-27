#ifndef BUTTON_H
#define BUTTON_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f10x.h"

typedef enum
{
	CTRL_BTN_UP = 0,
	CTRL_BTN_DOWN,
	CTRL_BTN_LEFT,
	CTRL_BTN_RIGHT,
	CTRL_BTN_COUNT	
} CtrlBtn_e;

typedef enum
{
	GAME_BTN_START = 0,
	GAME_BTN_RESET,
	GAME_BTN_COUNT
} GameBtn_e;

typedef struct
{
	void (*CtrlInit)(void);
	_Bool (*CtrlPressed)(CtrlBtn_e);
	
	void (*GameInit)(void);
	_Bool (*GamePressed)(GameBtn_e);
} Btn_Driver_t;

extern Btn_Driver_t button;

#ifndef CTRL_BTN_PORT
#define CTRL_BTN_PORT      GPIOB
#endif /* CTRL_BTN_PORT */

#ifndef CTRL_BTN_MASK
#define CTRL_BTN_MASK     (GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7)
#endif /* CTRL_BTN_MASK */


#ifndef CTRL_BTN_PINS_DEFINED
#define CTRL_BTN_UP_PIN     GPIO_Pin_4
#define CTRL_BTN_DOWN_PIN   GPIO_Pin_5
#define CTRL_BTN_LEFT_PIN   GPIO_Pin_6
#define CTRL_BTN_RIGHT_PIN  GPIO_Pin_7
#endif /* CTRL_BTN_PINS_DEFINED */


#ifndef START_BTN_PORT
#define START_BTN_PORT   GPIOA
#define START_BTN_PIN    GPIO_Pin_9
#endif /* START_BIN_PORT */


#ifndef RST_BTN_PORT
#define RST_BTN_PORT     GPIOA
#define RST_BTN_PIN      GPIO_Pin_8
#endif /* RST_BIN_PORT */

#ifdef __cplusplus
}
#endif

#endif /* BUTTON_H */
