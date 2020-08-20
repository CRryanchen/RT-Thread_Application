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
// 定义内存池控制块
static rt_mp_t test_mp = RT_NULL;
// 定义申请内存的指针
static rt_uint32_t *p_test = RT_NULL;

/*
 ******************************************************************
 *                          全局变量声明
 ******************************************************************
 */

// 宏定义
#define BLOCK_COUNT			20			// 内存块数量
#define	BLOCK_SIZE			3			// 内存块大小
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
	rt_kprintf("这是一个[野火]-STM32 全系列开发板 RTT 静态内存管理实验！\n");
	rt_kprintf("正在创建一个内存池......\n");

	// 创建一个静态内存池
	test_mp = rt_mp_create("test_mp",				// 内存池名字
							BLOCK_COUNT,
							BLOCK_SIZE);
	if (test_mp != RT_NULL)
	{
		rt_kprintf("静态内存池创建成功！\n");
	}

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

	p_test = rt_mp_alloc(test_mp, 0);
	if (p_test == RT_NULL)
	{
		rt_kprintf("静态内存申请失败!\n");
	}
	else
	{
		rt_kprintf("静态内存申请成功，地址%d!\n\n", p_test);
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
	rt_err_t uwRet = RT_EOK;
	rt_kprintf("正在释放内存......\n");
	rt_mp_free(p_test);
	rt_kprintf("释放内存成功！\n\n");
	rt_kprintf("正在删除内存.......\n");
	uwRet = rt_mp_delete(test_mp);
	if (uwRet == RT_EOK)
	{
		rt_kprintf("删除内存成功！\n");
	}

	while (1)
	{
		LED1_TOGGLE;
		rt_thread_delay(500);
	}
	
}

