#include "button.h"
#include "delay.h"

Btn_Driver_t button;

static const int TIME_DEBOUNCE = 20;
#define GPIO_Pin_Mask GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7
#define BTN_PORT GPIOB

static const uint16_t btnPins[BTN_COUNT] = {
	GPIO_Pin_4, // LEN
	GPIO_Pin_5, // XUONG
	GPIO_Pin_6, // TRAI
	GPIO_Pin_7, // PHAI
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

static _Bool Button_Read_RstGame(void)
{
	return GPIO_ReadInputDataBit(RST_GAME_PORT, RST_GAME_PIN);
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

static uint8_t btn_game_prev = 1;
static uint8_t btn_game_curr = 1;
static uint32_t btn_game_debounce = 0;

void Button_Reset_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Pin = RST_GAME_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RST_GAME_PORT, &GPIO_InitStruct);
	
	btn_game_prev = GPIO_ReadInputDataBit(RST_GAME_PORT, RST_GAME_PIN);
	btn_game_curr = btn_game_prev;
}
_Bool Button_IsResetPressed(void)
{
	uint8_t read_btn_game = Button_Read_RstGame();
	uint8_t ret = 0;
	
	if (read_btn_game != btn_game_curr)
	{
		btn_game_debounce = millis();
		btn_game_curr = read_btn_game;
	}
	
	if ((millis() - btn_game_debounce) > TIME_DEBOUNCE)
	{
		if (btn_game_prev != btn_game_curr)
		{
			btn_game_prev = btn_game_curr;
			if (btn_game_curr == 0)
			{
				ret = 1;
			}
		}
	}
	
	return ret;
}

void Button_BasicInit(void) __attribute__((constructor));

void Button_BasicInit(void)
{
	button.Init = Button_Init;
	button.IsPressed = Button_IsPressed;
	
	button.ResetInit = Button_Reset_Init;
	button.ResetGame = Button_IsResetPressed;
}
