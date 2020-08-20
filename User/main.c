/*
 ******************************************************************
 *                          ������ͷ�ļ�
 ******************************************************************
 */
#include "board.h"
#include "rtthread.h"
#include <string.h>

/*
 ******************************************************************
 *                          ����
 ******************************************************************
 */
// �����߳̿��ƿ�
static rt_thread_t key_thread = RT_NULL;
static rt_thread_t usart_thread = RT_NULL;
// ������Ϣ���п��ƿ�
rt_mq_t test_mq = RT_NULL;
// �����ź������ƿ�
rt_sem_t test_sem = RT_NULL;

/*
 ******************************************************************
 *                          ȫ�ֱ�������
 ******************************************************************
 */

// �궨��
extern char Usart_Rx_Buf[USART_RBUFF_SIZE];

/*
 ******************************************************************
 *                          ��������
 ******************************************************************
 */
static void key_thread_entry(void *parameter);
static void usart_thread_entry(void *parameter);


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
	rt_kprintf("����һ��[Ұ��]-STM32 ȫϵ�п����� RTT �жϹ���ʵ�飡\n");
	rt_kprintf("���� KEY1 | KEY2 �����жϣ�\n");
	rt_kprintf("���ڷ������ݴ����жϣ��̴߳������ݣ�\n");

	// ����һ����Ϣ����
	test_mq = rt_mq_create("test_mq",
							4,
							2,
							RT_IPC_FLAG_FIFO);
	if (test_mq != RT_NULL)
	{
		rt_kprintf("��Ϣ���д����ɹ���\n\n");
	}

	// ����һ���ź���
	test_sem = rt_sem_create("test_sem",
							0,
							RT_IPC_FLAG_FIFO);
	if (test_sem != RT_NULL)
	{
		rt_kprintf("�ź��������ɹ���\n\n");
	}
	
	key_thread = rt_thread_create("key",
									key_thread_entry,
									RT_NULL,
									512,
									1,
									20);
	// �����̣߳���������
	if (key_thread != RT_NULL)
	{
		rt_thread_startup(key_thread);
	}
	else
	{
		return -1;
	}

	usart_thread = rt_thread_create("usart",
									usart_thread_entry,
									RT_NULL,
									512,
									2,
									20);
	// �����̣߳���������
	if (usart_thread != RT_NULL)
	{
		rt_thread_startup(usart_thread);
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
static void key_thread_entry(void *parameter)
{
	rt_err_t uwRet = RT_EOK;
	uint32_t r_queque;
	while (1)
	{
		uwRet = rt_mq_recv(test_mq,
							&r_queque,
							sizeof(r_queque),
							RT_WAITING_FOREVER);
		if (RT_EOK == uwRet)
		{
			rt_kprintf("�����жϵ���KEY%d", r_queque);
		}
		else
		{
			rt_kprintf("���ݽ��ճ���������룺0x%lx\n", uwRet);
		}
		LED1_TOGGLE;
	}
}

static void usart_thread_entry(void *parameter)
{
	rt_err_t uwRet = RT_EOK;
	while (1)
	{
		uwRet = rt_sem_take(test_sem, 0);
		if (RT_EOK == uwRet)
		{
			rt_kprintf("�յ����ݣ�%s\n", Usart_Rx_Buf);
			memset(Usart_Rx_Buf, 0, USART_RBUFF_SIZE);
		}
		
	}
	
}

