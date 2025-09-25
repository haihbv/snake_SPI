#include "main.h"

void Init(void)
{
	SystemInit();
	Delay.Init();
	spi_Init();
	st7735_Init();
	st7735_InvertColors(DISABLE);
	Button_Init();

	Snake_Init();
}

void Loop(void)
{
	if (Button_IsPressed(BTN_UP))
	{
		Snake_SetDirection(SNAKE_UP);
	}
	if (Button_IsPressed(BTN_DOWN))
	{
		Snake_SetDirection(SNAKE_DOWN);
	}
	if (Button_IsPressed(BTN_LEFT))
	{
		Snake_SetDirection(SNAKE_LEFT);
	}
	if (Button_IsPressed(BTN_RIGHT))
	{
		Snake_SetDirection(SNAKE_RIGHT);
	}
	if (gameState == GAME_OVER && Button_IsPressed(BTN_RESET))
	{
		Snake_Reset();
	}

	static uint32_t last_update = 0;
	if (millis() - last_update > 200)
	{
		Snake_Update();
		Snake_Draw();
		last_update = millis();
	}
}

int main(void)
{
	Init();
	while (1)
	{
		Loop();
	}
}
