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
// �����ڴ�ؿ��ƿ�
static rt_mp_t test_mp = RT_NULL;
// ���������ڴ��ָ��
static rt_uint32_t *p_test = RT_NULL;

/*
 ******************************************************************
 *                          ȫ�ֱ�������
 ******************************************************************
 */

// �궨��
#define BLOCK_COUNT			20			// �ڴ������
#define	BLOCK_SIZE			3			// �ڴ���С
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
	rt_kprintf("���ڴ���һ���ڴ��......\n");

	// ����һ����̬�ڴ��
	test_mp = rt_mp_create("test_mp",				// �ڴ������
							BLOCK_COUNT,
							BLOCK_SIZE);
	if (test_mp != RT_NULL)
	{
		rt_kprintf("��̬�ڴ�ش����ɹ���\n");
	}

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

	p_test = rt_mp_alloc(test_mp, 0);
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
	rt_err_t uwRet = RT_EOK;
	rt_kprintf("�����ͷ��ڴ�......\n");
	rt_mp_free(p_test);
	rt_kprintf("�ͷ��ڴ�ɹ���\n\n");
	rt_kprintf("����ɾ���ڴ�.......\n");
	uwRet = rt_mp_delete(test_mp);
	if (uwRet == RT_EOK)
	{
		rt_kprintf("ɾ���ڴ�ɹ���\n");
	}

	while (1)
	{
		LED1_TOGGLE;
		rt_thread_delay(500);
	}
	
}

