/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 �ļ����� : tbs_common.h
 �ļ����� : ��������

 �޶���¼ :
          1 ���� : ������
            ���� : 2008-9-17
            ���� : �����ĵ�


**********************************************************************/

#ifndef __TBS_COMMON_H__
#define __TBS_COMMON_H__

int config_is_ok(void *data);
int ConvertEndian4(int a);
int ConvertEndian2(short a);
int check_addr_null( unsigned int *addr, unsigned int len );

#endif
