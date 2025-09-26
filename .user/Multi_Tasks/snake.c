#include "snake.h"
#include "st7735.h"
#include "delay.h"
#include <string.h>
#include <stdlib.h>

Snake_Driver_t snake;

static Point_t snakeBody[SNAKE_MAX_LEN];
static uint8_t snake_length;
static SnakeDirection_e direction;
static Point_t food;
static Point_t tail;
GameState_e gameState;

#define GRID_W (ST7735_WIDTH / BLOCK_SIZE)
#define GRID_H (ST7735_HEIGHT / BLOCK_SIZE)
#define CELL(x) ((x) * BLOCK_SIZE)

static uint8_t s_rng_seeded = 0;

static void Snake_Random_Food(void);
static inline void draw_food_cell(int fx, int fy, uint16_t color);

void Snake_Init(void)
{
	if (s_rng_seeded == 0)
	{
		srand(millis());
		s_rng_seeded = 1;
	}

	snake_length = 3;
	snakeBody[0] = (Point_t){5, 5};
	snakeBody[1] = (Point_t){4, 5};
	snakeBody[2] = (Point_t){3, 5};
	direction = SNAKE_RIGHT;

	gameState = GAME_RUNNING;

	/* Random moi khi start game */
	int16_t x_max = ST7735_WIDTH / BLOCK_SIZE;
	int16_t y_max = ST7735_HEIGHT / BLOCK_SIZE;
	food.x = rand() % x_max;
	food.y = rand() % y_max;

	/* Ve snake ban dau */
	for (int i = 0; i < snake_length; i++)
	{
		uint16_t color = (i == 0) ? GREEN : YELLOW;
		st7735_FillRect((uint8_t)snakeBody[i].x * BLOCK_SIZE, (uint8_t)snakeBody[i].y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, color);
	}
	draw_food_cell(food.x, food.y, RED);
}
void Snake_SetDirection(SnakeDirection_e dir)
{
	/* Chan quay dau 180 do */
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
	draw_food_cell(food.x, food.y, RED);
}

/* Ve food */
static inline void draw_food_cell(int fx, int fy, uint16_t color)
{
	const uint8_t cell_x0 = (uint8_t)CELL(fx);
	const uint8_t cell_y0 = (uint8_t)CELL(fy);
	const uint8_t cell_x1 = cell_x0 + BLOCK_SIZE - 1;
	const uint8_t cell_y1 = cell_y0 + BLOCK_SIZE - 1;

	const uint8_t cx = cell_x0 + BLOCK_SIZE / 2;
	const uint8_t cy = cell_y0 + BLOCK_SIZE / 2;

	uint8_t r = BLOCK_SIZE / 2;
	if (r > 0)
		r -= 1;
	st7735_FillCircleInBox(cx, cy, r, cell_x0, cell_y0, cell_x1, cell_y1, color);
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

	/* wrapp theo bien */
	if (x_new < 0)
	{
		x_new = GRID_W - 1;
	}
	else if (x_new >= GRID_W)
	{
		x_new = 0;
	}
	if (y_new < 0)
	{
		y_new = GRID_H - 1;
	}
	else if (y_new >= GRID_H)
	{
		y_new = 0;
	}

	/* update snake head*/
	snakeBody[0].x = (uint8_t)x_new;
	snakeBody[0].y = (uint8_t)y_new;

	/* tu can -> game over */
	for (int i = 1; i < snake_length; i++)
	{
		if (snakeBody[0].x == snakeBody[i].x && snakeBody[0].y == snakeBody[i].y)
		{
			gameState = GAME_OVER;
			return;
		}
	}

	/* an food */
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
	if (gameState == GAME_OVER)
	{
		st7735_PutString((ST7735_WIDTH - 9 * 11) / 2, (ST7735_HEIGHT - 18) / 2, "GAME OVER", Font_11x18, WHITE, BLACK);
		return;
	}

	if (!(snakeBody[0].x == food.x && snakeBody[0].y == food.y))
	{
		st7735_FillRect((uint8_t)tail.x * BLOCK_SIZE, (uint8_t)tail.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, BLACK);
	}

	/* ve lai food */
	if (!(snakeBody[0].x == food.x && snakeBody[0].y == food.y))
	{
		draw_food_cell(food.x, food.y, RED);
	}

	/* ve lai than */
	if (snake_length > 1)
	{
		st7735_FillRect((uint8_t)snakeBody[1].x * BLOCK_SIZE, (uint8_t)snakeBody[1].y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, YELLOW);
	}

	/* ve lai dau */
	st7735_FillRect((uint8_t)snakeBody[0].x * BLOCK_SIZE, (uint8_t)snakeBody[0].y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, GREEN);
}

void Snake_Reset(void)
{
	Snake_Init();
}

/**********************************************
 * Snake driver
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
