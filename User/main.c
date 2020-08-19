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
	rt_kprintf("这是一个[野火]-STM32 全系列开发板 RTT 计数信号量同步实验！\n");
	rt_kprintf("车位默认值为5个，按下K1申请车位，按下K2释放车位！\n\n");

	// 创建一个信号量
	test_sem = rt_sem_create("test_sem",			// 信号量名字
							5,					// 信号量初始值，默认有一个信号量
							RT_IPC_FLAG_FIFO);	// 队列模式 FIFO
	if (test_sem != RT_NULL)
	{
		rt_kprintf("计数信号量创建成功！\n\n");
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
	rt_err_t uwRet = RT_EOK;
	while (1)
	{
		if (Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON)
		{
			// 获取一个计数信号量
			uwRet = rt_sem_take(test_sem,
								0);				// 等待时间：0
			if (uwRet == RT_EOK)
			{
				rt_kprintf( "KEY1被单击：成功申请到停车位。\r\n" );
			}
			else
			{
				rt_kprintf( "KEY1被单击：不好意思，现在停车场已满！\r\n" );
			}
			
		}
		rt_thread_delay(20);			// 每20ms扫描一次
	}
}

static void send_thread_entry(void *parameter)
{
	rt_err_t uwRet = RT_EOK;
	while (1)
	{
		if (Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON)
		{
			// 释放一个计数信号量
			uwRet = rt_sem_release(test_sem);
			if (uwRet == RT_EOK)
			{
				rt_kprintf ( "KEY2被单击：释放1个停车位。\r\n" );
			}
			else
			{
				rt_kprintf ( "KEY2被单击：但已无车位可以释放！\r\n" );
			}
			
		}
		rt_thread_delay(20);			// 每20ms扫描一次
	}
}

