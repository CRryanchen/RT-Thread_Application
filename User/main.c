/*
 ******************************************************************
 *                          ������ͷ�ļ�
 ******************************************************************
 */
#include "board.h"
#include "rtthread.h"

/*
 ******************************************************************
 *                          ����
 ******************************************************************
 */
// ���������ʱ�����ƿ�
static rt_timer_t swtmr1 = RT_NULL;
static rt_timer_t swtmr2 = RT_NULL;

/*
 ******************************************************************
 *                          ȫ�ֱ�������
 ******************************************************************
 */
static uint32_t TmrCb_Count1 = 0;
static uint32_t TmrCb_Count2 = 0;

/*
 ******************************************************************
 *                          ��������
 ******************************************************************
 */
static void swtmr1_callback(void *parameter);
static void swtmr2_callback(void *parameter);


/*
 ******************************************************************
 *                          main ����
 ******************************************************************
 */

/**
 * @brief ������
 * @return int 
 */
int main(void)
{
	/*
	 * ������Ӳ����ʼ����RTTϵͳ��ʼ���Ѿ���main����֮ǰ��ɣ�
	 * ���� component.c �ļ��е� rtthread_startup() �����������
	 * ������ main �����У�ֻ��Ҫ�����̺߳������̼߳���
	 */
	rt_kprintf("����һ��[Ұ��]-STM32 ȫϵ�п����� RTT �����ʱ��ʵ�飡\n");
	rt_kprintf("��ʱ����ʱ����1 ִֻ��һ�ξͱ�����\n");
	rt_kprintf("��ʱ����ʱ����2 ��ѭ��ִ��\n");

	// ����һ�������ʱ��
	swtmr1 = rt_timer_create("swtmr1_callback",									// �����ʱ������
							swtmr1_callback,									// �����ʱ���ĳ�ʱ����
							0,													// ��ʱ��������ڲ���
							5000,												// ��ʱ����ʱʱ��
							RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER);	// �����ʱ����һ��ģʽ
	if (swtmr1 != RT_NULL)
	{
		rt_timer_start(swtmr1);
	}

	// ����һ�������ʱ��
	swtmr2 = rt_timer_create("swtmr2_callback",									// �����ʱ������
							swtmr2_callback,									// �����ʱ���ĳ�ʱ����
							0,													// ��ʱ��������ڲ���
							1000,												// ��ʱ����ʱʱ��
							RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);	// �����ʱ����һ��ģʽ
	if (swtmr2 != RT_NULL)
	{
		rt_timer_start(swtmr2);
	}

}

/*
 ******************************************************************
 *                          �̶߳���
 ******************************************************************
 */
static void swtmr1_callback(void *parameter)
{
	rt_uint32_t tick_num1;
	
	TmrCb_Count1++;

	tick_num1 = (uint32_t)rt_tick_get();

	rt_kprintf("swtmr1_callback ����ִ�� %d ��\n", TmrCb_Count1);
	rt_kprintf("�δ�ʱ����ֵ=%d\n", tick_num1);
}

static void swtmr2_callback(void *parameter)
{
	rt_uint32_t tick_num2;
	
	TmrCb_Count2++;

	tick_num2 = (uint32_t)rt_tick_get();

	rt_kprintf("swtmr2_callback ����ִ�� %d ��\n", TmrCb_Count2);
	rt_kprintf("�δ�ʱ����ֵ=%d\n", tick_num2);
}

