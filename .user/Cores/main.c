#include "main.h"
#include "task.h"

void Init(void)
{
	SystemInit();
	delay.Init();
	spi1.Init();
	lcd.Init();
	lcd.Invert(0);
	button.Init();
	snake.Init();

	TaskFunction();
}
void Loop(void)
{
	WorkAssignment();
}

int main(void)
{
	Init();
	while (1)
	{
		Loop();
	}
}
