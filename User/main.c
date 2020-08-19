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
// 定义消息队列控制块
static rt_mq_t test_mq = RT_NULL;
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
	rt_kprintf("这是一个[野火]-STM32 全系列开发板 RTT 消息队列实验！\n");
	rt_kprintf("按下 K1 或者 按下K2 发送消息队列\n");
	rt_kprintf("receive 线程接收到消息在串口回显\n");

	// 创建一个消息队列
	test_mq = rt_mq_create("test_mq",			// 消息队列名字
							40,					// 消息的最大长度
							20,					// 消息队列的最大容量
							RT_IPC_FLAG_FIFO);	// 队列模式 FIFO
	if (test_mq != RT_NULL)
	{
		rt_kprintf("消息队列创建成功！\n\n");
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
	uint32_t r_queue;
	while (1)
	{
		// 队列读取，等待时间为一直等待
		uwRet = rt_mq_recv(test_mq,					// 读取队列的ID
							&r_queue,				// 读取的数据位置
							sizeof(r_queue),		// 读取数据的长度
							RT_WAITING_FOREVER);	// 等待时间：一直等
		if (uwRet == RT_EOK)
		{
			rt_kprintf("本次接收到的数据是：%d\n",r_queue);
		}
		else
		{
			rt_kprintf("数据接收出错,错误代码: 0x%lx\n",uwRet);
		}
		rt_thread_delay(200);
		
	}
}

static void send_thread_entry(void *parameter)
{
	rt_err_t uwRet = RT_EOK;
	uint32_t send_data1 = 1;
	uint32_t send_data2 = 2;
	while (1)
	{
		if (Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON)
		{
			// 将数据写入到队列中，等待时间为0
			uwRet = rt_mq_send(test_mq,					// 写入队列的ID
								&send_data1,			// 写入的数据
								sizeof(send_data1));	// 数据的长度
			if (RT_EOK != uwRet)
			{
				rt_kprintf("数据不能发送到消息队列！错误代码: %lx\n",uwRet);
			}		
		}

		if (Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON)
		{
			// 将数据写入到队列中，等待时间为0
			uwRet = rt_mq_send(test_mq,					// 写入队列的ID
								&send_data2,			// 写入的数据
								sizeof(send_data2));	// 数据的长度
			if (RT_EOK != uwRet)
			{
				rt_kprintf("数据不能发送到消息队列！错误代码: %lx\n",uwRet);
			}
		}
		rt_thread_delay(20);
	}
}

