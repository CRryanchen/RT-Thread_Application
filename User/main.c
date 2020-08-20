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
static rt_thread_t receive_thread = RT_NULL;
static rt_thread_t send_thread = RT_NULL;
// ����������ƿ�
static rt_mailbox_t test_mail = RT_NULL;

/*
 ******************************************************************
 *                          ȫ�ֱ�������
 ******************************************************************
 */
char test_str1[] = "This is a mail test 1";
char test_str2[] = "This is a mail test 2";

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
	rt_kprintf("����һ��[Ұ��]-STM32 ȫϵ�п����� RTT ������Ϣʵ�飡\n");
	rt_kprintf("���� K1 | K2 ��������ʵ����ԣ�\n");

	// ����һ������
	test_mail = rt_mb_create("test_mail",				// ��������
							10,							// �����С
							RT_IPC_FLAG_FIFO);			// ����ģʽ��FIFO
	if (test_mail != RT_NULL)
	{
		rt_kprintf("���䴴���ɹ���\n");
	}

	receive_thread = rt_thread_create("receive",
										receive_thread_entry,
										RT_NULL,
										512,
										3,
										20);
	// �����̣߳���������
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
	rt_err_t uwRet = RT_EOK;
	char *r_str;
	while (1)
	{
		// �ȴ�������Ϣ
		uwRet = rt_mb_recv(test_mail,
							(rt_uint32_t*)&r_str,
							RT_WAITING_FOREVER);
		if (RT_EOK == uwRet)
		{
			rt_kprintf("����������ǣ�%s\n\n", r_str);
		}
		else
		{
			rt_kprintf("������մ��󣡴�������0x%x\n", uwRet);
		}
	}
}

static void send_thread_entry(void *parameter)
{
	rt_err_t uwRet = RT_EOK;
	while (1)
	{
		// ��� KEY1 ������
		if (Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON)
		{
			rt_kprintf("KEY1 ������\n");
			// ����һ��������Ϣ
			uwRet = rt_mb_send(test_mail, (rt_uint32_t)&test_str1);
			if (RT_EOK ==uwRet)
			{
				rt_kprintf("������Ϣ���ͳɹ�\n");
			}
			else
			{
				rt_kprintf("������Ϣ����ʧ��\n");
			}
		}
		
		
		// ��� KEY2 ������
		if (Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON)
		{
			rt_kprintf("KEY2 ������\n");
			// ����һ��������Ϣ
			uwRet = rt_mb_send(test_mail, (rt_uint32_t)&test_str2);
			if (RT_EOK ==uwRet)
			{
				rt_kprintf("������Ϣ���ͳɹ�\n");
			}
			else
			{
				rt_kprintf("������Ϣ����ʧ��\n");
			}
		}
		rt_thread_delay(20);
	}
}

