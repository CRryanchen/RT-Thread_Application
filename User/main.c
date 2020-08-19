/*
 ******************************************************************
 *                          包含的头文件
 ******************************************************************
 */
#include "board.h"
#include "rtthread.h"

/*
 ******************************************************************
 *                          变量
 ******************************************************************
 */
// 定义线程控制块指针
static rt_thread_t receive_thread = RT_NULL;
static rt_thread_t send_thread = RT_NULL;
// 定义信号量控制块
static rt_sem_t test_sem = RT_NULL;


/*
 ******************************************************************
 *                          全局变量声明
 ******************************************************************
 */
uint8_t ucValue[2] = {0x00, 0x00};


/*
 ******************************************************************
 *                          函数声明
 ******************************************************************
 */
static void receive_thread_entry(void *parameter);
static void send_thread_entry(void *parameter);


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
	rt_kprintf("这是一个[野火]-STM32 全系列开发板 RTT 二值信号量同步实验！\n");
	rt_kprintf("同步成功输出 Successful，反之输出 Fail\n");

	// 创建一个信号量
	test_sem = rt_sem_create("test_sem",			// 信号量名字
							1,					// 信号量初始值，默认有一个信号量
							RT_IPC_FLAG_FIFO);	// 队列模式 FIFO
	if (test_sem != RT_NULL)
	{
		rt_kprintf("信号量创建成功！\n\n");
	}

	receive_thread = 									// 线程控制块指针
	rt_thread_create("receive",							// 线程名字
					receive_thread_entry,				// 线程入口函数
					RT_NULL,							// 线程入口函数参数
					512,								// 线程栈大小
					3,									// 线程的优先级
					20);								// 线程时间片

	// 启动线程，开启调度
	if (receive_thread != RT_NULL)
	{
		rt_thread_startup(receive_thread);
	}
	else
	{
		return -1;
	}

	send_thread = 									// 线程控制块指针
	rt_thread_create("send",						// 线程名字
					send_thread_entry,				// 线程入口函数
					RT_NULL,						// 线程入口函数参数
					512,							// 线程栈大小
					2,								// 线程的优先级
					20);							// 线程时间片

	// 启动线程，开启调度
	if (send_thread != RT_NULL)
	{
		rt_thread_startup(send_thread);
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
static void receive_thread_entry(void *parameter)
{
	while (1)
	{
		rt_sem_take(test_sem,				// 获取信号量
					RT_WAITING_FOREVER);	// 等待时间：一直等
		if ( ucValue[0] == ucValue[1])
		{
			rt_kprintf("Successful\n");
		}
		else
		{
			rt_kprintf("Fail\n");
		}
		rt_sem_release(test_sem);

		rt_thread_delay(1000);
	}
}

static void send_thread_entry(void *parameter)
{
	while (1)
	{
		rt_sem_take(test_sem,				// 获取信号量
					RT_WAITING_FOREVER);	// 等待时间：一直等
		ucValue[0]++;
		rt_thread_delay(100);				// 延时 100 ms
		ucValue[1]++;
		rt_sem_release(test_sem);			// 释放二元信号量
		rt_thread_yield();					// 放弃剩余时间片，进行一次线程切换
	}
}

