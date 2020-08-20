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
// �����߳̿��ƿ�ָ��
static rt_thread_t receive_thread = RT_NULL;
static rt_thread_t send_thread = RT_NULL;
// �����¼����ƿ�
static rt_event_t test_event = RT_NULL;


/*
 ******************************************************************
 *                          ȫ�ֱ�������
 ******************************************************************
 */
#define KEY1_EVENT	(0x01 << 0)				// �����¼������λ0
#define KEY2_EVENT	(0X01 << 1)				// �����¼������λ1

/*
 ******************************************************************
 *                          ��������
 ******************************************************************
 */
static void receive_thread_entry(void *parameter);
static void send_thread_entry(void *parameter);


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
	rt_kprintf("����һ��[Ұ��]-STM32 ȫϵ�п����� RTT �¼���־��ʵ�飡\n");

	// ����һ���¼�
	test_event = rt_event_create("test_event",			// �¼���־������
							RT_IPC_FLAG_PRIO);			// �¼�ģʽ FIFO
	if (test_event != RT_NULL)
	{
		rt_kprintf("�¼������ɹ���\n\n");
	}

	receive_thread = 									// �߳̿��ƿ�ָ��
	rt_thread_create("receive",							// �߳�����
					receive_thread_entry,				// �߳���ں���
					RT_NULL,							// �߳���ں�������
					512,								// �߳�ջ��С
					3,									// �̵߳����ȼ�
					20);								// �߳�ʱ��Ƭ

	// �����̣߳���������
	if (receive_thread != RT_NULL)
	{
		rt_thread_startup(receive_thread);
	}
	else
	{
		return -1;
	}

	send_thread = 									// �߳̿��ƿ�ָ��
	rt_thread_create("send",						// �߳�����
					send_thread_entry,				// �߳���ں���
					RT_NULL,						// �߳���ں�������
					512,							// �߳�ջ��С
					2,								// �̵߳����ȼ�
					20);							// �߳�ʱ��Ƭ

	// �����̣߳���������
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
 *                          �̶߳���
 ******************************************************************
 */
static void receive_thread_entry(void *parameter)
{
	rt_uint32_t recved;
	while (1)
	{
		rt_event_recv(test_event, 									// �¼�������
						KEY1_EVENT | KEY2_EVENT,					// ���ո���Ȥ���¼�
						RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,	// ����ѡ��
						RT_WAITING_FOREVER,							// ָ����ʱ�¼�
						&recved);									// ָ����յ����¼�
		if (recved == (KEY1_EVENT | KEY2_EVENT))
		{
			rt_kprintf ( "Key1 �� Key2 ������\n" );
			LED1_TOGGLE;
		}
		else
		{
			rt_kprintf ( "�¼�����\n" );
		}
	}
}

static void send_thread_entry(void *parameter)
{
	while (1)
	{
		if (Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON)
		{
			rt_kprintf( "KEY1 ������\n" );
			// ����һ���¼�
			rt_event_send(test_event, KEY1_EVENT);
		}

		if (Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON)
		{
			rt_kprintf( "KEY2 ������\n" );
			// ����һ���¼�
			rt_event_send(test_event, KEY2_EVENT);
		}
		rt_thread_delay(20);
	}
}

