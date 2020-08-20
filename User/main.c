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
// 定义线程控制块
static rt_thread_t receive_thread = RT_NULL;
static rt_thread_t send_thread = RT_NULL;
// 定义邮箱控制块
static rt_mailbox_t test_mail = RT_NULL;

/*
 ******************************************************************
 *                          全局变量声明
 ******************************************************************
 */
char test_str1[] = "This is a mail test 1";
char test_str2[] = "This is a mail test 2";

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
	rt_kprintf("这是一个[野火]-STM32 全系列开发板 RTT 邮箱消息实验！\n");
	rt_kprintf("按下 K1 | K2 进行邮箱实验测试！\n");

	// 创建一个邮箱
	test_mail = rt_mb_create("test_mail",				// 邮箱名字
							10,							// 邮箱大小
							RT_IPC_FLAG_FIFO);			// 邮箱模式：FIFO
	if (test_mail != RT_NULL)
	{
		rt_kprintf("邮箱创建成功！\n");
	}

	receive_thread = rt_thread_create("receive",
										receive_thread_entry,
										RT_NULL,
										512,
										3,
										20);
	// 启动线程，开启调度
	if (receive_thread != RT_NULL)
	{
		rt_thread_startup(receive_thread);
	}
	else
	{
		return -1;
	}

	send_thread = rt_thread_create("send",
									send_thread_entry,
									RT_NULL,
									512,
									2,
									20);
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
	char *r_str;
	while (1)
	{
		// 等待邮箱消息
		uwRet = rt_mb_recv(test_mail,
							(rt_uint32_t*)&r_str,
							RT_WAITING_FOREVER);
		if (RT_EOK == uwRet)
		{
			rt_kprintf("邮箱的内容是：%s\n\n", r_str);
		}
		else
		{
			rt_kprintf("邮箱接收错误！错误码是0x%x\n", uwRet);
		}
	}
}

static void send_thread_entry(void *parameter)
{
	rt_err_t uwRet = RT_EOK;
	while (1)
	{
		// 如果 KEY1 被单击
		if (Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON)
		{
			rt_kprintf("KEY1 被单击\n");
			// 发送一个邮箱消息
			uwRet = rt_mb_send(test_mail, (rt_uint32_t)&test_str1);
			if (RT_EOK ==uwRet)
			{
				rt_kprintf("邮箱消息发送成功\n");
			}
			else
			{
				rt_kprintf("邮箱消息发送失败\n");
			}
		}
		
		
		// 如果 KEY2 被单击
		if (Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON)
		{
			rt_kprintf("KEY2 被单击\n");
			// 发送一个邮箱消息
			uwRet = rt_mb_send(test_mail, (rt_uint32_t)&test_str2);
			if (RT_EOK ==uwRet)
			{
				rt_kprintf("邮箱消息发送成功\n");
			}
			else
			{
				rt_kprintf("邮箱消息发送失败\n");
			}
		}
		rt_thread_delay(20);
	}
}

