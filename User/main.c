/*
 ******************************************************************
 *                          包含的头文件
 ******************************************************************
 */
#include "board.h"
#include "rtthread.h"
#include <string.h>

/*
 ******************************************************************
 *                          变量
 ******************************************************************
 */
// 定义线程控制块
static rt_thread_t key_thread = RT_NULL;
static rt_thread_t usart_thread = RT_NULL;
// 定义消息队列控制块
rt_mq_t test_mq = RT_NULL;
// 定义信号量控制块
rt_sem_t test_sem = RT_NULL;

/*
 ******************************************************************
 *                          全局变量声明
 ******************************************************************
 */

// 宏定义
extern char Usart_Rx_Buf[USART_RBUFF_SIZE];

/*
 ******************************************************************
 *                          函数声明
 ******************************************************************
 */
static void key_thread_entry(void *parameter);
static void usart_thread_entry(void *parameter);


/*
 ******************************************************************
 *                          main 函数
 ******************************************************************
 */

/**
 * @brief 主函数
 * @return int 
 */
int main(void)
{
	/*
	 * 开发板硬件初始化，RTT系统初始化已经在main函数之前完成，
	 * 即在 component.c 文件中的 rtthread_startup() 函数中完成了
	 * 所以在 main 函数中，只需要创建线程和启动线程即可
	 */
	rt_kprintf("这是一个[野火]-STM32 全系列开发板 RTT 中断管理实验！\n");
	rt_kprintf("按下 KEY1 | KEY2 触发中断！\n");
	rt_kprintf("串口发送数据触发中断，线程处理数据！\n");

	// 创建一个消息队列
	test_mq = rt_mq_create("test_mq",
							4,
							2,
							RT_IPC_FLAG_FIFO);
	if (test_mq != RT_NULL)
	{
		rt_kprintf("消息队列创建成功！\n\n");
	}

	// 创建一个信号量
	test_sem = rt_sem_create("test_sem",
							0,
							RT_IPC_FLAG_FIFO);
	if (test_sem != RT_NULL)
	{
		rt_kprintf("信号量创建成功！\n\n");
	}
	
	key_thread = rt_thread_create("key",
									key_thread_entry,
									RT_NULL,
									512,
									1,
									20);
	// 启动线程，开启调度
	if (key_thread != RT_NULL)
	{
		rt_thread_startup(key_thread);
	}
	else
	{
		return -1;
	}

	usart_thread = rt_thread_create("usart",
									usart_thread_entry,
									RT_NULL,
									512,
									2,
									20);
	// 启动线程，开启调度
	if (usart_thread != RT_NULL)
	{
		rt_thread_startup(usart_thread);
	}
	else
	{
		return -1;
	}
	

}

/*
 ******************************************************************
 *                          线程定义
 ******************************************************************
 */
static void key_thread_entry(void *parameter)
{
	rt_err_t uwRet = RT_EOK;
	uint32_t r_queque;
	while (1)
	{
		uwRet = rt_mq_recv(test_mq,
							&r_queque,
							sizeof(r_queque),
							RT_WAITING_FOREVER);
		if (RT_EOK == uwRet)
		{
			rt_kprintf("触发中断的是KEY%d", r_queque);
		}
		else
		{
			rt_kprintf("数据接收出错，错误代码：0x%lx\n", uwRet);
		}
		LED1_TOGGLE;
	}
}

static void usart_thread_entry(void *parameter)
{
	rt_err_t uwRet = RT_EOK;
	while (1)
	{
		uwRet = rt_sem_take(test_sem, 0);
		if (RT_EOK == uwRet)
		{
			rt_kprintf("收到数据：%s\n", Usart_Rx_Buf);
			memset(Usart_Rx_Buf, 0, USART_RBUFF_SIZE);
		}
		
	}
	
}

