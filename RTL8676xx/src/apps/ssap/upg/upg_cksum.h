/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 �ļ����� : upg_cksum.h
 �ļ����� : У��͵�ģ���ͷ�ļ�
 �����б� :
 �޶���¼ :
          1 ���� : ���
            ���� : 2008-04-12
            ���� :

**********************************************************************/
#ifndef __UPGCKSUM_H__
#define __UPGCKSUM_H__

#include <stdio.h>

int tbs_crc_file( FILE *fp , unsigned int offset , unsigned long *checksum );
int tbs_get_sum(FILE *fp, unsigned long *sum);

#endif
