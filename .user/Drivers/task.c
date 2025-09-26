#include "task.h"

Task_t ButtonTaskStruct;
Task_t SnakeTaskStruct;

uint16_t SnakeSpeed(uint16_t speed)
{
	return speed;
}


void ButtonTask(void)
{
	if (button.IsPressed(BTN_UP))
	{
		snake.Direction(SNAKE_UP);
	}
	if (button.IsPressed(BTN_DOWN))
	{
		snake.Direction(SNAKE_DOWN);
	}
	if (button.IsPressed(BTN_LEFT))
	{
		snake.Direction(SNAKE_LEFT);
	}
	if (button.IsPressed(BTN_RIGHT))
	{
		snake.Direction(SNAKE_RIGHT);
	}
	
	if (button.ResetGame() && gameState == GAME_OVER)
	{
		Snake_Reset();
	}
}
void FlushSnakeTask(void)
{
	snake.Update();
	snake.Draw();
}

void TaskFunction(void)
{
	ButtonTaskStruct.LastTick = 0;
	ButtonTaskStruct.Period = 10;
	ButtonTaskStruct.TaskHandler = ButtonTask;

	SnakeTaskStruct.LastTick = 0;
	SnakeTaskStruct.Period = SnakeSpeed(100);
	SnakeTaskStruct.TaskHandler = FlushSnakeTask;
}

static Task_t *TaskList[] = {
	&ButtonTaskStruct,
	&SnakeTaskStruct
};

#define SIZE_TASK (sizeof(TaskList) / sizeof(Task_t *))

void WorkAssignment(void)
{
	uint32_t current = millis();
	for (uint8_t i = 0; i < SIZE_TASK; i++)
	{
		if (current - TaskList[i]->LastTick >= TaskList[i]->Period)
		{
			TaskList[i]->TaskHandler();		 // <call Function>
			TaskList[i]->LastTick = current; // <update thoi gian>
		}
	}
}
