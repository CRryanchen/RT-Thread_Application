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
static rt_thread_t alloc_thread = RT_NULL;
static rt_thread_t free_thread = RT_NULL;
// 定义申请内存的指针
static rt_uint32_t *p_test = RT_NULL;

/*
 ******************************************************************
 *                          全局变量声明
 ******************************************************************
 */

// 宏定义
#define TEST_SIZE			100			// 内存大小

/*
 ******************************************************************
 *                          函数声明
 ******************************************************************
 */
static void alloc_thread_entry(void *parameter);
static void free_thread_entry(void *parameter);


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
	rt_kprintf("这是一个[野火]-STM32 全系列开发板 RTT 动态内存管理实验！\n");

	alloc_thread = rt_thread_create("alloc",
									alloc_thread_entry,
									RT_NULL,
									512,
									1,
									20);
	// 启动线程，开启调度
	if (alloc_thread != RT_NULL)
	{
		rt_thread_startup(alloc_thread);
	}
	else
	{
		return -1;
	}

	free_thread = rt_thread_create("free",
									free_thread_entry,
									RT_NULL,
									512,
									2,
									20);
	// 启动线程，开启调度
	if (free_thread != RT_NULL)
	{
		rt_thread_startup(free_thread);
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
static void alloc_thread_entry(void *parameter)
{
	rt_kprintf("正在向内存池申请内存......\n");

	p_test = rt_malloc(TEST_SIZE);
	if (p_test == RT_NULL)
	{
		rt_kprintf("动态内存申请失败!\n");
	}
	else
	{
		rt_kprintf("动态内存申请成功，地址%d!\n\n", p_test);
	}
	rt_kprintf("正在向p_test 写入数据......\n");
	*p_test = 1234;
	rt_kprintf("已经写入p_test地址的数据\n");
	rt_kprintf("*p_test=%.4d, 地址为%d\n\n", *p_test, p_test);

	while (1)
	{
		LED2_TOGGLE;
		rt_thread_delay(1000);
	}
}

static void free_thread_entry(void *parameter)
{
	rt_kprintf("正在释放内存......\n");
	rt_free(p_test);
	rt_kprintf("释放内存成功！\n\n");

	while (1)
	{
		LED1_TOGGLE;
		rt_thread_delay(500);
	}
	
}

