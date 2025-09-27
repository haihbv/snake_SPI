#include "spi.h"
#include "stm32f10x_spi.h"

static SPI_TypeDef *pSPI = SPI1;
SPI_Driver_t spi1;
SPI_Driver_t spi2;

void spi_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	SPI_InitTypeDef SPI_InitStruct = {0};

	if (pSPI == SPI1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);

		// PA5, PA7: SCK, MOSI - AF_PP
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStruct);

		// PA6: MISO - INPUT_FLOATING
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStruct);

		// PA4: CS
		GPIO_InitStruct.GPIO_Pin = CS_Pin;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(CS_Port, &GPIO_InitStruct);
	}
	else if (pSPI == SPI2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

		// PB13, PB15: SCK, MOSI - AF_PP
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStruct);

		// PB14: MISO - INPUT_FLOATING
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB, &GPIO_InitStruct);

		// PB12: CS
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
	else
	{
		return;
	}

	// SPIx->CR1 = 0;
	// SPIx->CR1 |= 1 << 2;
	// SPIx->CR1 |= (0x02 << 3);
	// SPIx->CR1 |= (1 << 9);
	// SPIx->CR1 |= (1 << 8);
	// SPIx->CR1 |= (1 << 6);
	SPI_I2S_DeInit(pSPI);
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_Init(pSPI, &SPI_InitStruct);
	SPI_Cmd(pSPI, ENABLE);

	GPIO_SetBits(CS_Port, CS_Pin);
}

void spi_Master_Transmit(uint8_t txData)
{
	while (!(pSPI->SR & (1 << 1)))
		;
	pSPI->DR = txData;
	while (!(pSPI->SR & (1 << 0)))
		;
	uint16_t rxData = pSPI->DR;
	(void)rxData;
	while ((pSPI->SR & (1 << 7)))
		;
}

/* Simulate OOP using C */

static void spi1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);

	// PA5, PA7: SCK, MOSI - AF_PP
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	// PA6: MISO - INPUT_FLOATING
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	// PA4: CS
	GPIO_InitStruct.GPIO_Pin = CS_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(CS_Port, &GPIO_InitStruct);

	SPI1->CR1 = 0;
	SPI1->CR1 |= 1 << 2;
	SPI1->CR1 |= (0x02 << 3);
	SPI1->CR1 |= (1 << 9);
	SPI1->CR1 |= (1 << 8);
	SPI1->CR1 |= (1 << 6);

	GPIO_SetBits(CS_Port, CS_Pin);
}

static void spi2_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	// PB13, PB15: SCK, MOSI - AF_PP
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	// PB14: MISO - INPUT_FLOATING
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	// PB12: CS
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	SPI2->CR1 = 0;
	SPI2->CR1 |= 1 << 2;
	SPI2->CR1 |= (0x02 << 3);
	SPI2->CR1 |= (1 << 9);
	SPI2->CR1 |= (1 << 8);
	SPI2->CR1 |= (1 << 6);

	GPIO_SetBits(GPIOB, GPIO_Pin_12);
}

static void spi1_transfer(uint8_t txData)
{
	while (!(SPI1->SR & (1 << 1)))
		;
	pSPI->DR = txData;
	while (!(SPI1->SR & (1 << 0)))
		;
	uint16_t rxData = pSPI->DR;
	(void)rxData;
	while ((SPI1->SR & (1 << 7)))
		;
}
static void spi2_transfer(uint8_t txData)
{
	while (!(SPI2->SR & (1 << 1)))
		;
	pSPI->DR = txData;
	while (!(SPI2->SR & (1 << 0)))
		;
	uint16_t rxData = pSPI->DR;
	(void)rxData;
	while ((SPI2->SR & (1 << 7)))
		;
}

void SPI_AutoInit(void) __attribute__((constructor));

void SPI_AutoInit(void)
{
	spi1.Init = spi1_init;
	spi1.Transfer = spi1_transfer;

	spi2.Init = spi2_init;
	spi2.Transfer = spi2_transfer;
}
