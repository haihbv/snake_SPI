#include "button.h"
#include "delay.h"

Btn_Driver_t button;

static const int TIME_DEBOUNCE = 20;
#define GPIO_Pin_Mask GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7
#define BTN_PORT GPIOB

static const uint16_t btnPins[BTN_COUNT] = {
	GPIO_Pin_4,	// LEN
	GPIO_Pin_5, // XUONG
	GPIO_Pin_6, // TRAI
	GPIO_Pin_7, // PHAI
	GPIO_Pin_3	// RESET
};

static uint8_t btn_cur_state[BTN_COUNT];
static uint8_t btn_last_state[BTN_COUNT];
static uint32_t btn_debounce_time[BTN_COUNT] = {0};

void Button_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef BTN_InitStruct;
	BTN_InitStruct.GPIO_Pin = GPIO_Pin_Mask;
	BTN_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	BTN_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BTN_PORT, &BTN_InitStruct);
	
	for (int i = 0; i < BTN_COUNT; i++)
	{
		btn_cur_state[i] = 1;
		btn_last_state[i] = 1;
	}
}

static _Bool Button_Read(Button_e btn)
{
	return GPIO_ReadInputDataBit(BTN_PORT, btnPins[btn]);
}

_Bool Button_IsPressed(Button_e btn)
{
	uint8_t read_btn = Button_Read(btn);
	_Bool ret_val = 0;
	
	if (read_btn != btn_cur_state[btn])
	{
		btn_debounce_time[btn] = millis();
		btn_cur_state[btn] = read_btn;
	}
	
	if ((millis() - btn_debounce_time[btn]) > TIME_DEBOUNCE)
	{
		if (btn_last_state[btn] != btn_cur_state[btn])
		{
			btn_last_state[btn] = btn_cur_state[btn];
			if (btn_cur_state[btn] == 0)
			{
				ret_val = 1;
			}
		}
	}
	return ret_val;
}

void Button_BasicInit(void) __attribute__((constructor));

void Button_BasicInit(void)
{
	button.Init = Button_Init;
	button.IsPressed = Button_IsPressed;
}
