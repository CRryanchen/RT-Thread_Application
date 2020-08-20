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
// �����߳̿��ƿ�
static rt_thread_t alloc_thread = RT_NULL;
static rt_thread_t free_thread = RT_NULL;
// ���������ڴ��ָ��
static rt_uint32_t *p_test = RT_NULL;

/*
 ******************************************************************
 *                          ȫ�ֱ�������
 ******************************************************************
 */

// �궨��
#define TEST_SIZE			100			// �ڴ��С

/*
 ******************************************************************
 *                          ��������
 ******************************************************************
 */
static void alloc_thread_entry(void *parameter);
static void free_thread_entry(void *parameter);


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
	rt_kprintf("����һ��[Ұ��]-STM32 ȫϵ�п����� RTT ��̬�ڴ����ʵ�飡\n");

	alloc_thread = rt_thread_create("alloc",
									alloc_thread_entry,
									RT_NULL,
									512,
									1,
									20);
	// �����̣߳���������
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
	// �����̣߳���������
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
 *                          �̶߳���
 ******************************************************************
 */
static void alloc_thread_entry(void *parameter)
{
	rt_kprintf("�������ڴ�������ڴ�......\n");

	p_test = rt_malloc(TEST_SIZE);
	if (p_test == RT_NULL)
	{
		rt_kprintf("��̬�ڴ�����ʧ��!\n");
	}
	else
	{
		rt_kprintf("��̬�ڴ�����ɹ�����ַ%d!\n\n", p_test);
	}
	rt_kprintf("������p_test д������......\n");
	*p_test = 1234;
	rt_kprintf("�Ѿ�д��p_test��ַ������\n");
	rt_kprintf("*p_test=%.4d, ��ַΪ%d\n\n", *p_test, p_test);

	while (1)
	{
		LED2_TOGGLE;
		rt_thread_delay(1000);
	}
}

static void free_thread_entry(void *parameter)
{
	rt_kprintf("�����ͷ��ڴ�......\n");
	rt_free(p_test);
	rt_kprintf("�ͷ��ڴ�ɹ���\n\n");

	while (1)
	{
		LED1_TOGGLE;
		rt_thread_delay(500);
	}
	
}

