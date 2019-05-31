/*=========================================================================
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 �ļ����� : tbs_common.c
 �ļ����� : �Լ�ʵ���ں���

 �޶���¼ :
          1 ���� : ������
            ���� : 2008-9-27
            ���� :

=========================================================================*/
#include <common.h>
#include <tbs_common.h>
#include <malloc.h>

/*=========================================================================
 Function Description:		ת������Ϊ4�ֽڵ��ֽ���
 Calls:			��
 Data Accessed:
 Data Updated:
 Input:			a:		Ҫת�������� 				 				
 Output:			��
 Return:			b:		ת���Ľ��			
 Others:			
=========================================================================*/

int ConvertEndian4(int a)
{
	unsigned char * byte;
	int b = 0;
	byte = ( unsigned char * )&a;
	b = (byte[3]<<3)|(byte[2]<<2)|(byte[1]<<1)|byte[0];
	return b;
}

/*=========================================================================
 Function Description:		ת������Ϊ2�ֽڵ��ֽ���
 Calls:			��
 Data Accessed:
 Data Updated:
 Input:			a:		Ҫת�������� 				 				
 Output:			��
 Return:			b:		ת���Ľ��			
 Others:			
=========================================================================*/

int ConvertEndian2(short a)
{
	unsigned char * byte;
	short b = 0;
	byte = ( unsigned char * )&a;
	b = (byte[1]<<1)|byte[0];
	return b;
}

int	config_is_ok(void *data)
{
	if(memcmp(data, CONFIG_MARK, CONFIG_MARK_LEN) == 0)
		return 0;
	else
		return -1;
}


