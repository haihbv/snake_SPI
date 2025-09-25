#include "main.h"
#include "task.h"

void Init(void)
{
	SystemInit();
	delay.Init();
	spi1.Init();
	st7735_Init();
	st7735_InvertColors(DISABLE);
	button.Init();
	snake.Init();
	
	TaskFunction();
}
void Loop(void)
{
	WordAssignment();
}

int main(void)
{
	Init();
	while (1)
	{
		Loop();
	}
}
