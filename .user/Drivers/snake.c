#include "snake.h"
#include "st7735.h"
#include <stdlib.h>

Snake_Driver_t snake;

static Point_t snakeBody[SNAKE_MAX_LEN];
static uint8_t snake_length;
static SnakeDirection_e direction;
static Point_t food;
static Point_t tail;
GameState_e gameState;

void Snake_Init(void)
{
	snake_length = 3;
	snakeBody[0].x = 5;
	snakeBody[0].y = 5;
	snakeBody[1].x = 4;
	snakeBody[1].y = 5;
	snakeBody[2].x = 3;
	snakeBody[2].y = 5;

	direction = SNAKE_RIGHT;
	gameState = GAME_RUNNING;

	/* Random moi khi start game */
	int16_t x_max = ST7735_WIDTH / BLOCK_SIZE;
	int16_t y_max = ST7735_HEIGHT / BLOCK_SIZE;
	food.x = rand() % x_max;
	food.y = rand() % y_max;

	st7735_FillScreen_Fast(BLACK);

	/* Ve snake ban dau */
	for (int i = 0; i < snake_length; i++)
	{
		if (i == 0)
		{
			st7735_FillRect((uint8_t)snakeBody[i].x * BLOCK_SIZE, (uint8_t)snakeBody[i].y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, MAGENTA);
		}
		else
		{
			st7735_FillRect((uint8_t)snakeBody[i].x * BLOCK_SIZE, (uint8_t)snakeBody[i].y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, GREEN);
		}
	}

	/* Ve food */
	// st7735_FillRect((uint8_t)food.x * BLOCK_SIZE, (uint8_t)food.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, BLUE);
	st7735_FillCircle((uint8_t)food.x * BLOCK_SIZE + BLOCK_SIZE / 2, (uint8_t)food.y * BLOCK_SIZE + BLOCK_SIZE / 2, BLOCK_SIZE / 2, BLUE);
}
void Snake_SetDirection(SnakeDirection_e dir)
{
	if ((dir == SNAKE_UP && direction == SNAKE_DOWN) || (dir == SNAKE_DOWN && direction == SNAKE_UP) || (dir == SNAKE_LEFT && direction == SNAKE_RIGHT) || (dir == SNAKE_RIGHT && direction == SNAKE_LEFT))
	{
		return;
	}
	direction = dir;
}

/* Sinh moi moi, khong dung voi snake */
static void Snake_Random_Food(void)
{
	int16_t x_max = ST7735_WIDTH / BLOCK_SIZE;
	int16_t y_max = ST7735_HEIGHT / BLOCK_SIZE;
	_Bool found_valid_position = 0;

	while (!found_valid_position)
	{
		food.x = rand() % x_max;
		food.y = rand() % y_max;
		found_valid_position = 1;
		for (int i = 0; i < snake_length; i++)
		{
			if (snakeBody[i].x == food.x && snakeBody[i].y == food.y)
			{
				found_valid_position = 0;
				break;
			}
		}
	}

	// ve food theo pos moi
	// st7735_FillRect((uint8_t)food.x * BLOCK_SIZE, (uint8_t)food.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, BLUE);
	st7735_FillCircle((uint8_t)food.x * BLOCK_SIZE + BLOCK_SIZE / 2, (uint8_t)food.y * BLOCK_SIZE + BLOCK_SIZE / 2, BLOCK_SIZE / 2, BLUE);
}

void Snake_Update(void)
{
	if (gameState == GAME_OVER)
		return;

	tail = snakeBody[snake_length - 1];

	for (int i = snake_length - 1; i > 0; i--)
	{
		snakeBody[i] = snakeBody[i - 1];
	}

	int16_t x_new = snakeBody[0].x;
	int16_t y_new = snakeBody[0].y;

	switch (direction)
	{
	case SNAKE_UP:
		y_new--;
		break;
	case SNAKE_DOWN:
		y_new++;
		break;
	case SNAKE_LEFT:
		x_new--;
		break;
	case SNAKE_RIGHT:
		x_new++;
		break;
	}

	/* wrapp */
	int16_t x_max = ST7735_WIDTH / BLOCK_SIZE;
	int16_t y_max = ST7735_HEIGHT / BLOCK_SIZE;

	if (x_new < 0)
	{
		x_new = x_max - 1;
	}
	else if (x_new >= x_max)
	{
		x_new = 0;
	}

	if (y_new < 0)
	{
		y_new = y_max - 1;
	}
	else if (y_new >= y_max)
	{
		y_new = 0;
	}

	/* update snake head*/
	snakeBody[0].x = (uint8_t)x_new;
	snakeBody[0].y = (uint8_t)y_new;

	/* can vao duoi - game over */
	for (int i = 1; i < snake_length; i++)
	{
		if (snakeBody[0].x == snakeBody[i].x && snakeBody[0].y == snakeBody[i].y)
		{
			gameState = GAME_OVER;
			return;
		}
	}

	/* an thuc an */
	if (snakeBody[0].x == food.x && snakeBody[0].y == food.y)
	{
		if (snake_length < SNAKE_MAX_LEN)
		{
			snakeBody[snake_length] = snakeBody[snake_length - 1];
			snake_length++;
		}
		Snake_Random_Food();
	}
}

void Snake_Draw(void)
{
	// st7735_FillScreen_Fast(BLACK);

	if (gameState == GAME_OVER)
	{
		st7735_PutString((ST7735_WIDTH - 9 * 11) / 2, (ST7735_HEIGHT - 18) / 2, "GAME OVER", Font_11x18, WHITE, BLACK);
		return;
	}

	/* Version 1.0
	<ve food>
	st7735_FillRect((uint8_t)(food.x * BLOCK_SIZE), (uint8_t)(food.y * BLOCK_SIZE), BLOCK_SIZE, BLOCK_SIZE, BLUE);

	<ve ran>
	for (int i = 0; i < snake_length; i++)
	{
		st7735_FillRect((uint8_t)(snakeBody[i].x * BLOCK_SIZE), (uint8_t)(snakeBody[i].y * BLOCK_SIZE), BLOCK_SIZE, BLOCK_SIZE, GREEN);
	}
	*/

	/* <Version 2.0> */
	/*
	 * @brief ko dung FillScreen_Fast moi lan Draw nua
	 */
	if (!(snakeBody[0].x == food.x && snakeBody[0].y == food.y))
	{
		st7735_FillRect((uint8_t)tail.x * BLOCK_SIZE, (uint8_t)tail.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, BLACK);
	}

	/* ve lai food */
	st7735_FillCircle((uint8_t)food.x * BLOCK_SIZE + BLOCK_SIZE / 2, (uint8_t)food.y * BLOCK_SIZE + BLOCK_SIZE / 2, BLOCK_SIZE / 2, BLUE);

	/* ve lai than */
	if (snake_length > 1)
	{
		st7735_FillRect((uint8_t)snakeBody[1].x * BLOCK_SIZE, (uint8_t)snakeBody[1].y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, GREEN);
	}

	/* ve lai dau */
	st7735_FillRect((uint8_t)snakeBody[0].x * BLOCK_SIZE, (uint8_t)snakeBody[0].y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, MAGENTA);
}

void Snake_Reset(void)
{
	snake_length = 3;
	snakeBody[0].x = 5;
	snakeBody[0].y = 5;
	snakeBody[1].x = 4;
	snakeBody[1].y = 5;
	snakeBody[2].x = 3;
	snakeBody[2].y = 5;

	direction = SNAKE_RIGHT;
	gameState = GAME_RUNNING;

	int16_t x_max = ST7735_WIDTH / BLOCK_SIZE;
	int16_t y_max = ST7735_HEIGHT / BLOCK_SIZE;
	food.x = rand() % x_max;
	food.y = rand() % y_max;

	st7735_FillScreen_Fast(BLACK);
}

/**********************************************
 * Snake
 **********************************************/
void Game_SnakeInit(void) __attribute__((constructor));

void Game_SnakeInit(void)
{
	snake.Init = Snake_Init;
	snake.Update = Snake_Update;
	snake.Draw = Snake_Draw;
	snake.Direction = Snake_SetDirection;
	snake.Reset = Snake_Reset;
}
