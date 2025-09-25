#include "snake.h"
#include "st7735.h"
#include <stdlib.h>

static Point_t snake[SNAKE_MAX_LEN];
static uint8_t snake_length;
static SnakeDirection_e direction;
static Point_t food;
GameState_e gameState;

void Snake_Init(void)
{
	snake_length = 3;
	snake[0].x = 5; snake[0].y = 5;
	snake[1].x = 4; snake[1].y = 5;
	snake[2].x = 3; snake[2].y = 5;
	
	direction = SNAKE_RIGHT;
	gameState = GAME_RUNNING;
	
	/* Random moi khi start game */
	int16_t x_max = ST7735_WIDTH / BLOCK_SIZE;
	int16_t y_max = ST7735_HEIGHT / BLOCK_SIZE;
	food.x = rand() % x_max;
	food.y = rand() % y_max;
}
void Snake_SetDirection(SnakeDirection_e dir)
{
	if ((dir == SNAKE_UP && direction == SNAKE_DOWN) || (dir == SNAKE_DOWN && direction == SNAKE_UP) 
		|| (dir == SNAKE_LEFT && direction == SNAKE_RIGHT) || (dir == SNAKE_RIGHT && direction == SNAKE_LEFT))
	{
    return;
	}
	direction = dir;
}

/* Sinh moi moi, khong dung voi snake */
static void Snake_Generate_Food(void)
{
	int16_t x_max = ST7735_WIDTH / BLOCK_SIZE;
	int16_t y_max = ST7735_HEIGHT / BLOCK_SIZE;
	_Bool check_bait_same_as_snake = 0;
	
	while (!check_bait_same_as_snake)
	{
		food.x = rand() % x_max;
		food.y = rand() % y_max;
		check_bait_same_as_snake = 1;
		for (int i = 0; i < snake_length; i++)
		{
			if (snake[i].x == food.x && snake[i].y == food.y)
			{
				check_bait_same_as_snake = 0;
				break;
			}
		}
	}
}

void Snake_Update(void)
{
	if (gameState == GAME_OVER) return;
	
	for (int i = snake_length - 1; i > 0; i--)
	{
		snake[i] = snake[i - 1];
	}
	
	int16_t x_new = snake[0].x;
	int16_t y_new = snake[0].y;
	
	switch (direction)
	{
		case SNAKE_UP:
			y_new--; break;
		case SNAKE_DOWN:
			y_new++; break;
		case SNAKE_LEFT:
			x_new--; break;
		case SNAKE_RIGHT:
			x_new++; break;
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
	snake[0].x = (uint8_t)x_new;
  snake[0].y = (uint8_t)y_new;
	
	/* can vao duoi - game over */
	for (int i = 1; i < snake_length; i++)
	{
		if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
		{
			gameState = GAME_OVER;
			return;
		}
	}
	
	/* an moi */
	if (snake[0].x == food.x && snake[0].y == food.y)
	{
		if (snake_length < SNAKE_MAX_LEN)
		{
			snake[snake_length] = snake[snake_length - 1];
			snake_length++;
		}
		Snake_Generate_Food(); // random moi tiep theo
	}
}

void Snake_Draw(void)
{
	st7735_FillScreen_Fast(BLACK);
	
	if (gameState == GAME_OVER)
	{
		st7735_PutString(20, 70, "GAME OVER", Font_11x18, WHITE, BLACK);
		return;
	}
	
	/* ve moi */
	st7735_FillRect((uint8_t)(food.x * BLOCK_SIZE), (uint8_t)(food.y * BLOCK_SIZE), BLOCK_SIZE, BLOCK_SIZE, BLUE);
	
	/* ve ran */
	for (int i = 0; i < snake_length; i++)
	{
		st7735_FillRect((uint8_t)(snake[i].x * BLOCK_SIZE), (uint8_t)(snake[i].y * BLOCK_SIZE), BLOCK_SIZE, BLOCK_SIZE, GREEN);
	}
}

void Snake_Reset(void)
{
	snake_length = 3;
	snake[0].x = 5; snake[0].y = 5;
	snake[1].x = 4; snake[1].y = 5;
	snake[2].x = 3; snake[2].y = 5;
	
	direction = SNAKE_RIGHT;
	gameState = GAME_RUNNING;
	
	int16_t x_max = ST7735_WIDTH / BLOCK_SIZE;
  int16_t y_max = ST7735_HEIGHT / BLOCK_SIZE;
	food.x = rand() % x_max;
  food.y = rand() % y_max;
}
