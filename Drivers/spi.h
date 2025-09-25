#ifndef SPI_H
#define SPI_H

#ifdef __cplusplus
extern "C" 
{
#endif

#include "stm32f10x.h"
#include "gpio.h"

#define CS_Pin      GPIO_Pin_4
#define CS_Port     GPIOA

void spi_Init(void);
void spi_Master_Transmit(uint8_t txData);

void spi_Init_DMA(void);
void spi_Master_Tramsmit_DMA(uint8_t txData);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* SPI_H */
