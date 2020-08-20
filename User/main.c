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
// 定义软件定时器控制块
static rt_timer_t swtmr1 = RT_NULL;
static rt_timer_t swtmr2 = RT_NULL;

/*
 ******************************************************************
 *                          全局变量声明
 ******************************************************************
 */
static uint32_t TmrCb_Count1 = 0;
static uint32_t TmrCb_Count2 = 0;

/*
 ******************************************************************
 *                          函数声明
 ******************************************************************
 */
static void swtmr1_callback(void *parameter);
static void swtmr2_callback(void *parameter);


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
	rt_kprintf("这是一个[野火]-STM32 全系列开发板 RTT 软件定时器实验！\n");
	rt_kprintf("定时器超时函数1 只执行一次就被销毁\n");
	rt_kprintf("定时器超时函数2 则循环执行\n");

	// 创建一个软件定时器
	swtmr1 = rt_timer_create("swtmr1_callback",									// 软件定时器名字
							swtmr1_callback,									// 软件定时器的超时函数
							0,													// 超时函数的入口参数
							5000,												// 定时器超时时间
							RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER);	// 软件定时器、一次模式
	if (swtmr1 != RT_NULL)
	{
		rt_timer_start(swtmr1);
	}

	// 创建一个软件定时器
	swtmr2 = rt_timer_create("swtmr2_callback",									// 软件定时器名字
							swtmr2_callback,									// 软件定时器的超时函数
							0,													// 超时函数的入口参数
							1000,												// 定时器超时时间
							RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);	// 软件定时器、一次模式
	if (swtmr2 != RT_NULL)
	{
		rt_timer_start(swtmr2);
	}

}

/*
 ******************************************************************
 *                          线程定义
 ******************************************************************
 */
static void swtmr1_callback(void *parameter)
{
	rt_uint32_t tick_num1;
	
	TmrCb_Count1++;

	tick_num1 = (uint32_t)rt_tick_get();

	rt_kprintf("swtmr1_callback 函数执行 %d 次\n", TmrCb_Count1);
	rt_kprintf("滴答定时器数值=%d\n", tick_num1);
}

static void swtmr2_callback(void *parameter)
{
	rt_uint32_t tick_num2;
	
	TmrCb_Count2++;

	tick_num2 = (uint32_t)rt_tick_get();

	rt_kprintf("swtmr2_callback 函数执行 %d 次\n", TmrCb_Count2);
	rt_kprintf("滴答定时器数值=%d\n", tick_num2);
}

