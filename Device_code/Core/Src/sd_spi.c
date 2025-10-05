#include "sd_spi.h"
//#include "spi.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1;

void SD_Select(void) {
    HAL_GPIO_WritePin(SD_CS_PORT, SD_CS_PIN, GPIO_PIN_RESET);
}

void SD_Deselect(void) {
    HAL_GPIO_WritePin(SD_CS_PORT, SD_CS_PIN, GPIO_PIN_SET);
}

uint8_t SD_Receive(uint8_t *data, uint16_t len) {
    return HAL_SPI_Receive(&hspi1, data, len, HAL_MAX_DELAY);
}

uint8_t SD_Send(uint8_t *data, uint16_t len) {
    return HAL_SPI_Transmit(&hspi1, data, len, HAL_MAX_DELAY);
}

uint8_t SD_ReadBlock(uint8_t *buff, uint32_t sector) {
    // Dummy read logic placeholder
    // Normally here you'd implement CMD17 command sequence for SD read
    SD_Select();
    HAL_SPI_Receive(&hspi1, buff, 512, HAL_MAX_DELAY);
    SD_Deselect();
    return 0;
}

uint8_t SD_WriteBlock(const uint8_t *buff, uint32_t sector) {
    // Dummy write logic placeholder
    SD_Select();
    HAL_SPI_Transmit(&hspi1, (uint8_t *)buff, 512, HAL_MAX_DELAY);
    SD_Deselect();
    return 0;
}

uint8_t SD_InitCard(void) {
    SD_Deselect();
    HAL_Delay(10);
    return 0;
}
