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
// ���廥�������ƿ�
static rt_mutex_t test_mux = RT_NULL;


/*
 ******************************************************************
 *                          ȫ�ֱ�������
 ******************************************************************
 */
uint8_t ucValue [ 2 ] = { 0x00, 0x00 };

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
	rt_kprintf("����һ��[Ұ��]-STM32 ȫϵ�п����� RTT ������ͬ��ʵ�飡\n");
	rt_kprintf("ͬ���ɹ������Successful,��֮���Fail\n");

	// ����һ���ź���
	test_mux = rt_mutex_create("test_mux",			// ����������
							RT_IPC_FLAG_PRIO);	// ����ģʽ FIFO
	if (test_mux != RT_NULL)
	{
		rt_kprintf("�����������ɹ���\n\n");
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
	while (1)
	{
		rt_mutex_take(test_mux, RT_WAITING_FOREVER);
		if (ucValue[0] == ucValue[1])
		{
			rt_kprintf ( "Successful\n" );
		}
		else
		{
			rt_kprintf ( "Fail\n" );
		}
		rt_mutex_release( test_mux ); 	//�ͷŻ�����
		
		rt_thread_delay(1000);
	}
}

static void send_thread_entry(void *parameter)
{
	rt_err_t uwRet = RT_EOK;
	while (1)
	{
		rt_mutex_take(test_mux, RT_WAITING_FOREVER);		// ��ȡ������
		ucValue[0]++;
		rt_thread_delay(100);								// ��ʱ100ms
		ucValue[1]++;
		rt_mutex_release( test_mux ); 						// �ͷŻ�����
		rt_thread_yield();									// ����ʣ��ʱ��Ƭ������һ���߳��л�
	}
}

