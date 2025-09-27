#include "button.h"
#include "delay.h"

Btn_Driver_t button;

static const uint32_t TIME_DEBOUNCE = 20;

static const uint16_t ctrlPins[CTRL_BTN_COUNT] = {
	CTRL_BTN_UP_PIN,	// LEN
	CTRL_BTN_DOWN_PIN,	// XUONG
	CTRL_BTN_LEFT_PIN,	// TRAI
	CTRL_BTN_RIGHT_PIN, // PHAI
};

static uint8_t ctrlPrev[CTRL_BTN_COUNT];
static uint8_t ctrlCurr[CTRL_BTN_COUNT];
static uint32_t ctrlTstamp[CTRL_BTN_COUNT];

typedef struct __attribute__((packed))
{
	GPIO_TypeDef *Port;
	uint16_t Pin;
} PinDef_t;

static const PinDef_t gamePins[GAME_BTN_COUNT] = {
	{START_BTN_PORT, START_BTN_PIN},
	{RST_BTN_PORT, RST_BTN_PIN}};

static uint8_t gamePrev[GAME_BTN_COUNT];
static uint8_t gameCurr[GAME_BTN_COUNT];
static uint32_t gameTstamp[GAME_BTN_COUNT];

static inline uint8_t Read_Pin(GPIO_TypeDef *GPIOx, uint16_t Pin)
{
	return GPIO_ReadInputDataBit(GPIOx, Pin);
}

static void Ctrl_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef BTN_InitStruct = {0};
	BTN_InitStruct.GPIO_Pin = CTRL_BTN_MASK;
	BTN_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	BTN_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CTRL_BTN_PORT, &BTN_InitStruct);

	for (int i = 0; i < CTRL_BTN_COUNT; i++)
	{
		uint8_t readPin = Read_Pin(CTRL_BTN_PORT, ctrlPins[i]);
		ctrlPrev[i] = readPin;
		ctrlCurr[i] = readPin;
		ctrlTstamp[i] = 0;
	}
}

static _Bool Ctrl_Pressed(CtrlBtn_e btn)
{
	uint8_t reading = Read_Pin(CTRL_BTN_PORT, ctrlPins[btn]);
	_Bool ret = 0;

	if (reading != ctrlCurr[btn])
	{
		ctrlTstamp[btn] = millis();
		ctrlCurr[btn] = reading;
	}

	if (millis() - ctrlTstamp[btn] > TIME_DEBOUNCE)
	{
		if (ctrlPrev[btn] != ctrlCurr[btn])
		{
			ctrlPrev[btn] = ctrlCurr[btn];
			if (ctrlCurr[btn] == 0)
			{
				ret = 1;
			}
		}
	}
	return ret;
}

static void Game_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	for (int i = 0; i < GAME_BTN_COUNT; i++)
	{
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.GPIO_Pin = gamePins[i].Pin;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(gamePins[i].Port, &GPIO_InitStruct);

		uint8_t readPin = Read_Pin(gamePins[i].Port, gamePins[i].Pin);
		gamePrev[i] = readPin;
		gameCurr[i] = readPin;
		gameTstamp[i] = 0;
	}
}

static _Bool Game_Pressed(GameBtn_e gbtn)
{
	uint8_t reading = Read_Pin(gamePins[gbtn].Port, gamePins[gbtn].Pin);
	_Bool ret = 0;

	if (reading != gameCurr[gbtn])
	{
		gameTstamp[gbtn] = millis();
		gameCurr[gbtn] = reading;
	}

	if (millis() - gameTstamp[gbtn] > TIME_DEBOUNCE)
	{
		if (gamePrev[gbtn] != gameCurr[gbtn])
		{
			gamePrev[gbtn] = gameCurr[gbtn];
			if (gameCurr[gbtn] == 0)
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
	button.CtrlInit = Ctrl_Init;
	button.CtrlPressed = Ctrl_Pressed;

	button.GameInit = Game_Init;
	button.GamePressed = Game_Pressed;
}
