/*
 * @mainpage Main Header File
 * @brief This is TFT LCD ST7735 driver
 * @author haihbv
 * @date   09/2025
 */

#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * Includes
 **********************************************************************/
#include <stdlib.h>
#include "stm32f10x.h"
#include "gpio.h"
#include "delay.h"
#include "spi.h"
#include "st7735.h"
#include "snake.h"
#include "button.h"

void Init(void);
void Loop(void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* MAIN_H */
/*********************************************************************
 * END OF FILE
 **********************************************************************/

