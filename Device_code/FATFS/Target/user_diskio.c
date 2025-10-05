/* USER CODE BEGIN Header */
/**
 ******************************************************************************
  * @file    user_diskio.c
  * @brief   This file includes a diskio driver skeleton to be completed by the user.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
 /* USER CODE END Header */

#ifdef USE_OBSOLETE_USER_CODE_SECTION_0
/*
 * Warning: the user section 0 is no more in use (starting from CubeMx version 4.16.0)
 * To be suppressed in the future.
 * Kept to ensure backward compatibility with previous CubeMx versions when
 * migrating projects.
 * User code previously added there should be copied in the new user sections before
 * the section contents can be deleted.
 */
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */
#endif

/* USER CODE BEGIN DECL */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "ff_gen_drv.h"

#include "user_diskio.h"
#include "main.h"
#include "stm32f4xx_hal_spi.h"
#include "fatfs.h"
#include "string.h"
#include "stdio.h"
#include "sd_spi.h"


// === SD Card Chip Select Definitions ===
#define SD_CS_PORT GPIOA
#define SD_CS_PIN  GPIO_PIN_4


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;

/* USER CODE END DECL */

/* Private function prototypes -----------------------------------------------*/
DSTATUS USER_initialize (BYTE pdrv);
DSTATUS USER_status (BYTE pdrv);
DRESULT USER_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
#if _USE_WRITE == 1
  DRESULT USER_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
  DRESULT USER_ioctl (BYTE pdrv, BYTE cmd, void *buff);
#endif /* _USE_IOCTL == 1 */

Diskio_drvTypeDef  USER_Driver =
{
  USER_initialize,
  USER_status,
  USER_read,
#if  _USE_WRITE
  USER_write,
#endif  /* _USE_WRITE == 1 */
#if  _USE_IOCTL == 1
  USER_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes a Drive
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS USER_initialize (BYTE pdrv) {
    (void)pdrv;
    // configure CS pin as output and set high
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = SD_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(SD_CS_PORT, &GPIO_InitStruct);
    SD_Deselect();
    HAL_Delay(2);
    if (SD_InitCard() != 0) {
        dbg_print("SD init failed\r\n");
        return STA_NOINIT;
    }
    dbg_print("SD init OK\r\n");
    return RES_OK;
}

/**
  * @brief  Gets Disk Status
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
//DSTATUS USER_initialize(BYTE pdrv) {
//    (void)pdrv;
//    // configure CS pin as output and set high
//    GPIO_InitTypeDef GPIO_InitStruct = {0};
//    GPIO_InitStruct.Pin = SD_CS_PIN;
//    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    HAL_GPIO_Init(SD_CS_PORT, &GPIO_InitStruct);
//    SD_Deselect();
//    HAL_Delay(2);
//    if (SD_InitCard() != 0) {
//        dbg_print("SD init failed\r\n");
//        return STA_NOINIT;
//    }
//    dbg_print("SD init OK\r\n");
//    return RES_OK;
//}

DSTATUS USER_status(BYTE pdrv) {
    (void)pdrv;
    // simple: check init by trying to select and read one byte
    SD_Select();
    uint8_t SD_Receive(uint8_t *data, uint16_t len);
    SD_Deselect();
    (void)pdrv;
    return 0;
}

DRESULT USER_read(BYTE pdrv, BYTE* buff, DWORD sector, UINT count) {
    (void)pdrv;
    for (UINT i = 0; i < count; ++i) {
        if (SD_ReadBlock((uint32_t)(sector + i), buff + (i * 512)) != 0) return RES_ERROR;
    }
    return RES_OK;
}

/**
  * @brief  Writes Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT USER_write(BYTE pdrv, const BYTE* buff, DWORD sector, UINT count) {
    (void)pdrv;
    for (UINT i = 0; i < count; ++i) {
        if (SD_WriteBlock((uint32_t)(sector + i), buff + (i * 512)) != 0) return RES_ERROR;
    }
    return RES_OK;
}
  /* USER CODE BEGIN WRITE */
  /* USER CODE HERE */

  /* USER CODE END WRITE */

#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation
  * @param  pdrv: Physical drive number (0..)
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT USER_ioctl(BYTE pdrv, BYTE cmd, void* buff) {
    (void)pdrv;
    switch (cmd) {
        case CTRL_SYNC:
            return RES_OK;
        case GET_SECTOR_COUNT:
            // unknown - return error or set reasonable default if needed
            return RES_OK;
        case GET_SECTOR_SIZE:
            *(DWORD*)buff = 512;
            return RES_OK;
        case CTRL_TRIM:
            return RES_OK;
        default:
            return RES_OK;
    }
}

#endif /* _USE_IOCTL == 1 */

