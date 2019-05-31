/**********************************************************************

 Copyright (c), 1991-2011, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

�ļ���	  : 3g-stub.h

 �ļ�����: 

 �޶���¼:

        1. ����: �곣��

           ����: 2011-08-22

           ����: �����ļ�           
              
**********************************************************************/

#ifndef __LOGIC_3G__
#define	__LOGIC_3G__

#ifdef IS_3G_DEBUG
#undef 	IS_3G_DEBUG
#endif

//#define DEBUG_NEW3G

#define EXTERN_USE_3G_MNGR_H
#include "3g-mngr-comm.h"

#define IS_3G_DEBUG() IS_MNGR_DEBUG("/var/3g_debug_wan")


extern int console_fd;
extern FILE *fp;


#define   LOG_STUB(fmt, args...)	\
	if (fp )	\
		fprintf(fp, "[3G_STUB: ] %s:%d: " fmt, __FILE__, __LINE__, ##args)

#ifdef d_pintf
#undef d_pintf
#endif

#ifdef DEBUG_NEW3G
#define  d_pintf(args...)  LOG_STUB(args)
#else
#define  d_pintf(args...)		\
	if (IS_3G_DEBUG())	\
		LOG_STUB(args)
#endif		

#endif
