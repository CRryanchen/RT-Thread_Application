#ifndef __BOARD_H
#define __BOARD_H

#ifdef __cplusplus
extern "C"{
#endif

/*
 ******************************************************************
 *                          ������ͷ�ļ�
 ******************************************************************
 */
// STM32 �̼���ͷ�ļ�
#include "stm32f10x.h"								// Device header

// ������Ӳ�� bsp ͷ�ļ�
#include "bsp_led.h"
#include "bsp_usart.h"

/*
 ******************************************************************
 *                          ������ͷ�ļ�
 ******************************************************************
 */
void rt_hw_board_init(void);
void SysTick_Handler(void);
void rt_hw_console_output(const char *str);

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_H */
