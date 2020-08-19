#ifndef __BOARD_H
#define __BOARD_H

#ifdef __cplusplus
extern "C"{
#endif

/*
 ******************************************************************
 *                          包含的头文件
 ******************************************************************
 */
// STM32 固件库头文件
#include "stm32f10x.h"								// Device header

// 开发板硬件 bsp 头文件
#include "bsp_led.h"
#include "bsp_usart.h"

/*
 ******************************************************************
 *                          包含的头文件
 ******************************************************************
 */
void rt_hw_board_init(void);
void SysTick_Handler(void);
void rt_hw_console_output(const char *str);

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_H */
