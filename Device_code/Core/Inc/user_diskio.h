#ifndef __USER_DISKIO_H
#define __USER_DISKIO_H

#include "diskio.h"
#include "ff.h"
#include "stm32f4xx_hal.h"
#include "main.h"
#include "ff_gen_drv.h"

extern const Diskio_drvTypeDef  USER_Driver;

// Initialize SPI handle (extern from MX generated code)
extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart2;

// Disk I/O API
DSTATUS USER_initialize(BYTE pdrv);
DSTATUS USER_status(BYTE pdrv);
DRESULT USER_read(BYTE pdrv, BYTE* buff, DWORD sector, UINT count);
DRESULT USER_write(BYTE pdrv, const BYTE* buff, DWORD sector, UINT count);
DRESULT USER_ioctl(BYTE pdrv, BYTE cmd, void* buff);

#endif // USER_DISKIO_H
