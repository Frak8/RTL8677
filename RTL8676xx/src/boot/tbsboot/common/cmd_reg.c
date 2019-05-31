/*=========================================================================
 Copyright (c), 1991-2012, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 �ļ����� : cmd_reg.c
 �ļ����� : �Ĵ�����ز���

 �޶���¼ :
          1 �޶�: �ĳ���
            ����: 2012-02-15
            ����: �����ļ�,ʵ�ּĴ�������д����             

=========================================================================*/

#include <common.h>
#include <command.h>

#if (CONFIG_COMMANDS & CFG_CMD_REGINFO)
/*=========================================================================
 Function Description:	�Ĵ�����д���ܺ���
 Data Accessed: register
 Data Updated:
 Input:			-r/w:������д
 				addr:�Ĵ���������ַ
 				length:���޶�ȡģʽ
 Output:		�Ĵ�������
 Return:		0
 Others:
=========================================================================*/
int do_reg(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned long addr;
	unsigned long value;
	int i;
	int ret_val = -1;
	
	if(4 == argc)
		{
		addr = simple_strtoul(argv[2], NULL, 16);
		if(addr % sizeof(unsigned long))
			{
			goto error;
			}
		if(0 == strcmp(argv[1], "-r"))
			{
			value = simple_strtoul(argv[3], NULL, 0);
			for(i = 0; i < value; i += sizeof(unsigned long))
				{
				if(!(i % 16))
					{
					printf("\nAddr:%#x\n", addr);
					}					
				printf("  %08X", REG32(addr));
				addr += i;
				}
			printf("\n");
			ret_val = 0;
			}
		else if(0 == strcmp(argv[1], "-w"))
			{
			value = simple_strtoul(argv[3], NULL, 16);
			REGWRITE32(addr, value);
			printf("Addr:%#x  Value:%08X\n", addr, REG32(addr));
			ret_val = 0;
			}
		else
			{
			goto error;
			}
		}
	
error:
	if(ret_val < 0)
		{
		printf("Usage:\n%s\n", cmdtp->usage);
		}
	
	return ret_val;
}


/* -------------------------------------------------------------------- */

U_BOOT_CMD(
	reg, CFG_MAXARGS, CMD_FLAG_REPEAT,	do_reg,
	"reg     - read[write] register at address 'addr'\n",
	" -r[-w] register_addr length[value]\n"
);
#endif

