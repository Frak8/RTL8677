#ifndef __MKIMAGE_H__
#define __MKIMAGE_H__

typedef struct 
{	
	volatile unsigned int image_load;				/*stage2��ڵ�ַ*/
	volatile unsigned int image_len;				/*stage2ѹ����ĳ���*/
	unsigned char tag[3];					        /*  'two'  */
}stage2_hdr_t;

#endif  /*endif __MKIMAGE_H__*/

