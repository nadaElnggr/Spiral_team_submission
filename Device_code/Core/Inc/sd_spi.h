/*
 * sd_spi.h
 *
 *  Created on: Oct 5, 2025
 *      Author: Lenovo
 */
#ifndef __SD_SPI_H
#define __SD_SPI_H

#include "stm32f4xx_hal.h"

#define SD_CS_PORT GPIOB
#define SD_CS_PIN  GPIO_PIN_12

void SD_Select(void);
void SD_Deselect(void);
uint8_t SD_Receive(uint8_t *data, uint16_t len);
uint8_t SD_Send(uint8_t *data, uint16_t len);
uint8_t SD_ReadBlock(uint8_t *buff, uint32_t sector);
uint8_t SD_WriteBlock(const uint8_t *buff, uint32_t sector);
uint8_t SD_InitCard(void);

#endif
