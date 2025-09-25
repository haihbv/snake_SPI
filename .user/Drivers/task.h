#ifndef TASK_H
#define TASK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

typedef struct
{
	uint32_t LastTick;      	// <Thoi diem chay lan truoc>
	uint32_t Period;         	// <Chu ky chay cua cong viec>
	void (*TaskHandler)(void);	// <Con tro den ham xu ly cong viec - Callback>
} Task_t;

/* Khai bao cac Tast */
extern Task_t ButtonTaskStruct;
extern Task_t SnakeTaskStruct;

/* Ham xu ly tung Task */
void ButtonTask(void);
void FlushSnakeTask(void);
void TaskFunction(void);
void WordAssignment(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TASK_H */
