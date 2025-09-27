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

typedef struct
{
	void (*Init)(void);
	void (*Transfer)(uint8_t txData);
} SPI_Driver_t;

extern SPI_Driver_t spi1;
extern SPI_Driver_t spi2;

void spi_Init(void);
void spi_Master_Transmit(uint8_t txData);

void spi_Init_DMA(void);
void spi_Master_Transmit_DMA(uint8_t txData);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SPI_H */
