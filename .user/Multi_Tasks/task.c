#include "task.h"
#include "ui.h"

Task_t ButtonTaskStruct;
Task_t SnakeTaskStruct;

static uint16_t SnakeSpeed(uint16_t speed)
{
	return speed;
}

static GameState_e prevMode = GAME_WAIT_START;

void ButtonTask(void)
{
	// <1. START tu man cho>
	if (gameState != prevMode)
	{
		if (gameState == GAME_OVER)
		{
			ui.ShowOverScreen();
		}
		else if (gameState == GAME_WIN)
		{
			ui.ShowWinScreen();
		}
		prevMode = gameState;
	}
	
	if (gameState == GAME_WAIT_START)
	{
		if (button.GamePressed(GAME_BTN_START))
		{
			ui.ClearToGameBg();
			snake.Reset();
			gameState = GAME_RUNNING;
		}
		return;
	}

	// <2. Control khi dang choi>
	if (gameState == GAME_RUNNING)
	{
		if (button.CtrlPressed(CTRL_BTN_UP))
		{
			snake.Direction(SNAKE_UP);
		}
		if (button.CtrlPressed(CTRL_BTN_DOWN))
		{
			snake.Direction(SNAKE_DOWN);
		}
		if (button.CtrlPressed(CTRL_BTN_LEFT))
		{
			snake.Direction(SNAKE_LEFT);
		}
		if (button.CtrlPressed(CTRL_BTN_RIGHT))
		{
			snake.Direction(SNAKE_RIGHT);
		}
	}

	// <3. RESET khi game OVER -> quay lai man hinh cho>
	if (gameState == GAME_OVER || gameState == GAME_WIN)
	{
		if (gameState == GAME_OVER)
		{
			if (button.GamePressed(GAME_BTN_RESET))
			{
				gameState = GAME_WAIT_START;
				ui.ShowStartScreen();
			}
		}
		else if (gameState == GAME_WIN)
		{
			if (button.GamePressed(GAME_BTN_START))
			{
				gameState = GAME_WAIT_START;
				ui.ShowWinScreen();
			}
		}
		
	}
}
void FlushSnakeTask(void)
{
	if (gameState == GAME_WAIT_START)
	{
		ui.UpdateStartBlink();
		return;
	}

	if (gameState == GAME_RUNNING)
	{
		snake.Update();
		snake.Draw();
		return;
	}

	if (gameState == GAME_OVER)
	{
		ui.UpdateOverBlink();
		return;
	}
	
	if (gameState == GAME_WIN)
	{
		ui.UpdateWinBlink();
		return;
	}
}

void TaskFunction(void)
{
	ButtonTaskStruct.LastTick = 0;
	ButtonTaskStruct.Period = 10;
	ButtonTaskStruct.TaskHandler = ButtonTask;

	SnakeTaskStruct.LastTick = 0;
	SnakeTaskStruct.Period = SnakeSpeed(100);
	SnakeTaskStruct.TaskHandler = FlushSnakeTask;

	gameState = GAME_WAIT_START;
	ui.ShowStartScreen();
}

static Task_t *TaskList[] = {
	&ButtonTaskStruct,
	&SnakeTaskStruct};

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
