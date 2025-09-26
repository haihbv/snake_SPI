#ifndef SNAKE_H
#define SNAKE_H

#include <stdint.h>

#define BLOCK_SIZE 8
#define SNAKE_MAX_LEN 100

typedef struct
{
	int16_t x;
	int16_t y;
} Point_t;

typedef enum
{
	SNAKE_UP,
	SNAKE_DOWN,
	SNAKE_LEFT,
	SNAKE_RIGHT
} SnakeDirection_e;

typedef enum 
{
	GAME_WAIT_START,
  GAME_RUNNING,
  GAME_OVER
} GameState_e;

extern GameState_e gameState;

typedef struct
{
	void (*Init)(void);
	void (*Update)(void);
	void (*Draw)(void);
	void (*Direction)(SnakeDirection_e dir);
	void (*Reset)(void);
} Snake_Driver_t;

extern Snake_Driver_t snake;

void Snake_Init(void);
void Snake_Update(void);
void Snake_Draw(void);
void Snake_SetDirection(SnakeDirection_e dir);
void Snake_Reset(void);

#endif /* SNAKE_H */
