/*=========================================================================
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 �ļ����� : flash_layout.c
 �ļ����� : flash layout��ز���

˵������ǰ�ļ������ܶ������Ϣ��������#if 0 #endif��ʽ�رգ�����Ϊ����
			���㣬���ò�������Ҫ���Ķ������Ե�����Ϣ��ʱ����

 �޶���¼ :
          1 ���� : ������
            ���� : 2008-7-22
            ���� :

          2 �޶�: ������
             ����: 2008-9-10
             ����: ��������Ϣ��Ϊ��Ŀ����

          3 �޶�: ������
             ����: 2008-11-24
             ����: ��SPI���͵Ķ�������Ϊ��ӳ��ӿ�

           4 �޶�: ������
             ����: 2009-2-18
             ����: ������BIN image�ĺ����������ļ�,������CRCУ��

           5 �޶�: ������
             ����: 2009-3-16
             ����: �Ż���Ŀ�Ĳ���

           6 �޶�: ������
             ����: 2009-5-6
             ����: �������ʱ����Ӧ�ò�����

           7 �޶�: �ĳ���
             ����: 2011-08-08
             ����: �ض����ļ�β����Ϣ�Լ�ϵͳ������Ϣ             

=========================================================================*/

#include <common.h>
#include <command.h>
#include <flash_layout_private.h>
#include <crc.h>
#include <tbs_common.h>
#include <malloc.h>

int	is_sysdata(void *sys_data)
{
	struct sys_cfg *syscfg = (struct sys_cfg *)sys_data;
	int ret_val = -1;
	
	if((strncmp((char *)(syscfg->cfg_version), SYSTEM_CONFIG_VERSION, VERSION_LEN) == 0) && (strncmp((char *)(syscfg->tag), "sysc", 4) == 0))
		{
		ret_val = 1;
		}

	return ret_val;
}

int sysdata_save(void *sys_data)
{
	int ret_val = ERROR_CONFIG_LOST;
	
	gd->syscfg_ver = is_sysdata(sys_data);
	fldebug("\nsyscfg_version=[%d]\n", gd->syscfg_ver);
	if((1 == gd->syscfg_ver) && (item_save(sys_data, LLCONFIG_NAME, sizeof(struct sys_cfg)) == ERROR_ITEM_OK))
		{
		ret_val = ERROR_OK;
		}

	return ret_val;
}


void get_sys_ip(unsigned long *ip)
{
	struct sys_cfg *syscfg;
	#ifdef __BIG_ENDIAN
	const unsigned int default_ip = 0xc0a80101;
	#else	
	const unsigned int default_ip = 0x0101a8c0;
	#endif
	
	if(1 == gd->syscfg_ver)
		{
		syscfg = (struct sys_cfg *)(gd->syscfg_addr);
		*ip = syscfg->ip;
		}
	else
		{
		*ip = default_ip;
		}
}

/*=========================================================================
 Function Description:		���ÿ����Ŀ��������,�������������Ӧ�Ĵ���
 Data Accessed:
 Data Updated:
 Input:
 Output:
 Return:			ERROR_ITEM_OK
				ERROR_CONFIG_LOST
 Others:
=========================================================================*/

int item_map_and_check(void)
{
	item_t *item;
	unsigned short checksum;
	unsigned int i = 0;
	unsigned int sum = 0;
	unsigned int block_size;
	unsigned char *item_sdram;
	char *temp = NULL;
	int ret_val = ERROR_CONFIG_LOST;

	item_sdram = (unsigned char *)(CONFIG_DRAM_BASE + gd->bi_memsize - BOOT_RUNTIME_SIZE + ITEM_MAP_OFFSET);

	/* �ҵ�Flash��������������������ʼ��ַ*/
	while((info->start[i] - info->start[0] ) < BOOTCODE_LENGTH)
		{
		i++;
		}
	block_size = info->start[i+1] - info->start[i];
	/* ����������ȫ�������ڴ�*/
	debug("Item maped to %#x, blocksize is %#x, memsize is %#x", (unsigned int)item_sdram, block_size, gd->bi_memsize);
	flash_read_buf(info, (char *)info->start[i], &item_sdram, block_size);
	debug("Global date points to %#x\n", (unsigned int)gd);
	/*ͨ�����������ͷ�����ж������������Ƿ����*/
	if(config_is_ok(item_sdram) < 0)
		{
		/*���������ݲ�����,�����ת�������Ƿ����*/
		#ifdef CONFIG_FLASH_TRANSFER
		/* ����ת������ȫ�������ڴ�*/
		flash_read_buf(info, (char *)info->start[i+1], &item_sdram, block_size);
		if(config_is_ok(item_sdram) < 0)
			{/*��ת������Ҳ������,���ö�ʧ,���ش���*/
			printf("The config data has lost!\n");
			goto out;
			}
		else
			{
			printf("Restore config data from backups\n");
			/*��ת����������,�޸�������*//*˳��:��������-> ������ת����Ŀ���ݵ�������*/
			flash_erase(info, i , i);
			flash_write((char *)(info->start[i+1]), info->start[i], block_size);
			}
		#else
		printf("The config data has lost!\n");
		goto out;
		#endif
		}
	sum = CONFIG_MARK_LEN;
	temp = malloc(ITEM_HEAD_LEN);
	if(NULL == temp)
		{
		printf("Failed to allocate memory!\n");
		goto out;
		}
	else
		{
		while((sum + ITEM_HEAD_LEN) <= block_size)
			{/*ɨ��������Ŀ*/
			item = (item_t *)(item_sdram + sum);
			fldebug("check item at %x, state:%02X, len:%x\n", sum, item->hdr.avail, item->hdr.len);
			switch(item->hdr.avail)
				{
			case ITEM_UNAVAIL:/*��Ŀ��Ч,ָ����һ��Ŀ*/
				sum += ITEM_SIZE(item->hdr.len);
				break;
			case ITEM_AVAIL:
				crc16(item->data, item->hdr.len, &checksum);
				if(checksum == item->hdr.crc)
					{/*��Ŀ����,����ɨ����һ��Ŀ*/
					sum += ITEM_SIZE(item->hdr.len);
					break;
					}
				else
					{/*��Ŀ������,�ô���ĿΪ��Ч,���ҿ��Կ϶��������һ����Ŀ*/
					fldebug("Item checksum %x is bad, expecting %x!\n", checksum, item->hdr.crc);
					temp[0] = ITEM_UNAVAIL;
					flash_write(temp, info->start[i] + (unsigned int)&(item->hdr.avail) - (unsigned int)item_sdram, 0x1);
					sum = block_size + ITEM_HEAD_LEN;
					goto scan_finish;
					}
				
			case ITEM_BAD:/*��Ŀ�Ѿ��ǻ���,ָ����һ��Ŀ*/
				sum += ITEM_SIZE(0);
				break;
			case ITEM_NULL:
				memset(temp, 0xFF, ITEM_HEAD_LEN);
				if(memcmp(item, temp, ITEM_HEAD_LEN) != 0)
					{/*��鵽��Ŀֻд��ͷ����һ����,�ô���ĿΪ����,���ҿ��Կ϶��������һ����Ŀ*/
					temp[0] = ITEM_BAD;
					flash_write(temp, info->start[i] + (unsigned int)&(item->hdr.avail) - (unsigned int)item_sdram, 0x1);
					fldebug("Item at %x has been marked bad\n", sum);
					}
				goto scan_finish;
			default:
				goto scan_finish;
				}
			}
scan_finish:		
		free(temp);
		ret_val = ERROR_ITEM_OK;
		}

out:
	return ret_val;
}

/*=========================================================================
 Function Description:		��ȡָ����Ŀ������
 Data Accessed:
 Data Updated:
 Input:			*item_name:	 ��Ŀ����
 				*data:		��Ŀ���ݵĴ��ָ��
 Output:			*len:		��Ŀ��Ч���ݵĳ���
 Return:			ERROR_ITEM_OK
				ERROR_ITEM_NOT_FIND
 Others:
=========================================================================*/

int item_get( void *data , char *item_name ,unsigned short *len )
{
	item_t *item;
	unsigned int i = 0;
	unsigned int sum = 0;
	unsigned int block_size;
	unsigned char *item_sdram;
	int ret_val = ERROR_ITEM_NOT_FIND;

	item_sdram = (unsigned char *)(CONFIG_DRAM_BASE + gd->bi_memsize - BOOT_RUNTIME_SIZE + ITEM_MAP_OFFSET);
	/* �ҵ�Flash��������������������ʼ��ַ*/
	while((info->start[i] - info->start[0]) < BOOTCODE_LENGTH)
		{
		i++;
		}
	block_size = info->start[i+1] - info->start[i];
	/* ����������ȫ�������ڴ�*/
	flash_read_buf(info, (char *)info->start[i], &item_sdram, block_size);
	sum = CONFIG_MARK_LEN;
	/* ������Ŀ*/
	while((sum + ITEM_HEAD_LEN) <= block_size)
		{
		item = (item_t *)(item_sdram + sum);
		fldebug("get item at %x, state: %02x, len: %x, offset: 0x%08x\n", sum, item->hdr.avail, item->hdr.len, info->start[i]);
		switch(item->hdr.avail)
			{
			case ITEM_AVAIL: /* �ҵ���Ч��Ŀ*//* �Ա���Ŀ����,���ж��Ƿ�ΪҪѰ�ҵ�*/
				if(memcmp(item->data, item_name, strlen(item_name)) == 0)
					{/*�����,�����ݶ�ȡ����*/
					memcpy(data, item->data + strlen(item_name ) + 1 , item->hdr.len - strlen( item_name ) - 1);
					*len = item->hdr.len - strlen(item_name) - 1;
					ret_val = ERROR_ITEM_OK;
					goto out;
					}
				else
					{/*�������,����Ѱ��*/
					fldebug("Item at %x is not the one we need!\n", sum);
					sum += ITEM_SIZE( item->hdr.len);
					break;
					}
			case ITEM_UNAVAIL: /*��Ŀ��Ч,ָ����һ��Ŀ*/
				sum += ITEM_SIZE( item->hdr.len);
				break;

			case ITEM_BAD: /*��Ŀ�Ѿ��ǻ���,ָ����һ��Ŀ*/
				sum += ITEM_SIZE(0);
				break;

			default: /*���ÿռ�Ѱ�����*/
				goto out;
			}
		}
out:
	return ret_val;
}

/*=========================================================================
 Function Description:		������ת��Ϊ��Ŀ����
 Data Accessed:
 Data Updated:
 Input:			*item_name:	 ��Ŀ����
 				*data:		Ҫ��ŵ���Ŀ���ݵ�ָ��

 Output:			*len:		��Ŀ��Ч���ݵĳ���
 Return:			ERROR_ITEM_OK
 				ERROR_ITEM_BIG
 Others:
=========================================================================*/
int item_save(void *data, char *item_name, unsigned short len)
{
	item_t *item = NULL;
	item_t *item_data = NULL;
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int sum = 0;
	unsigned int block_size;
	unsigned short checksum;
	char avail;
	unsigned char *item_sdram;
	unsigned int item_repeat_addr[5];

	item_sdram = (unsigned char *)(CONFIG_DRAM_BASE + gd->bi_memsize - BOOT_RUNTIME_SIZE + ITEM_MAP_OFFSET);
	/* �ҵ�Flash��������������������ʼ��ַ*/
	while((info->start[i] - info->start[0]) < BOOTCODE_LENGTH)
		{
		i++;
		}
	block_size = info->start[i+1] - info->start[i];
	flash_read_buf(info, (char *)info->start[i], &item_sdram, block_size);
	sum = CONFIG_MARK_LEN;	
	while((sum + ITEM_HEAD_LEN) <= block_size)
		{/* Ѱ�ҿ��õ����ÿռ�*/
		item = (item_t *)(item_sdram + sum);
		fldebug("save item at %x, state: %02x, len: %x\n", sum, item->hdr.avail, item->hdr.len);
		switch(item->hdr.avail)
			{
			case ITEM_UNAVAIL:
				sum += ITEM_SIZE(item->hdr.len);
				break;
				
			case ITEM_BAD:
				sum += ITEM_SIZE(0);
				break;
				
			case ITEM_AVAIL:
				if(memcmp(item->data, item_name, strlen(item_name)) == 0)
					{/*���ֽ�Ҫ��Ϊ��Ч����Ŀ,���������ĵ�ַ*/
					item_repeat_addr[j++] = info->start[i] + (unsigned int)&(item->hdr.avail) - (unsigned int)item_sdram;
					if(j == 2)
						{/*���������ظ�����Ŀ,��ǰһ����Ŀ��Ϊ��Ч*/
						avail = ITEM_UNAVAIL;
						flash_write((char *)&avail, item_repeat_addr[0], 0x1);
						item_repeat_addr[0] = item_repeat_addr[1];
						j = 1;
						}
					}
				sum += ITEM_SIZE(item->hdr.len);
				break;
				
			default:
				goto found_space;
			}
		}

found_space:
	/*��������Ϣ�ϳ���Ŀ*/
	item_data = (item_t *)malloc(strlen(item_name) + 1 + len);
	if(NULL == item_data)
		{
		printf("Fail to allocate memory for save item %s\n", item_name);
		return ERROR_ITEM_MALLOC;
		}
	item_data->hdr.avail = ITEM_AVAIL;
	memcpy(item_data->data , item_name , strlen(item_name));
	memcpy(item_data->data + strlen(item_name), "\0", 1);
	memcpy(item_data->data + strlen(item_name) + 1, data, len);
	item_data->hdr.len = strlen(item_name) + 1 + len;
	crc16(item_data->data, item_data->hdr.len, &checksum);
	item_data->hdr.crc = checksum;
	
	/*�ж�����Ŀ�Ĵ�С�Ƿ񳬳���������ʣ��ռ�*/
	if(ITEM_SIZE(item_data->hdr.len) > (block_size - sum))
		{/*�����,ͨ����ת�ķ�ʽ���������е���Ч�ͻ���Ŀ����*/
		item_recycle();
		flash_read_buf(info, (char *)info->start[i], &item_sdram, block_size);
		sum = CONFIG_MARK_LEN;
		j = 0;

		while((sum + ITEM_HEAD_LEN ) <= block_size)
			{
			item = (item_t *)(item_sdram + sum);
			/*�ҵ�FLASH�յ�ַ*/
			if( item->hdr.avail == ITEM_NULL)
				{
				break;
				}
			if(item->hdr.avail == ITEM_AVAIL)
				{/*���ֽ�Ҫ��Ϊ��Ч����Ŀ,���������ĵ�ַ*/
				if(memcmp(item->data, item_name, strlen(item_name)) ==0)
					{
					item_repeat_addr[j++] = info->start[i] + ( unsigned int )&( item->hdr.avail ) - ( unsigned int )item_sdram;
					}
				sum += ITEM_SIZE( item->hdr.len );
				continue;
				}
			}
		/*�ٴ��ж��Ƿ񳬳��ռ�*/
		if(ITEM_SIZE(item_data->hdr.len) > (block_size - sum))
			{/*��Ŀ̫��,�޷�����,���ش���*/
			printf("Item %s is too big to save in flash\n", item_name);
			free(item_data);
			return ERROR_ITEM_BIG;
			}
		}

	/* ���ϳɵ�������Ŀд��Flash*/
	flash_write((char *)item_data, info->start[i] + (unsigned int)item - (unsigned int)item_sdram, ITEM_HEAD_LEN + item_data->hdr.len );
	if( j == 1 )
		{	/* ������Ŀ��Ϊ��Ч*/
		avail = ITEM_UNAVAIL;
		flash_write((char *)&avail , item_repeat_addr[0] , 0x1);
		}
	free(item_data);

	return ERROR_ITEM_OK;
}

/*=========================================================================
 Function Description: ���������ռ䲻��ʱͨ����ת����ת���ݣ���ת��������
 				flashһ���������ڴ�
 				Flash��ʽ��ת:
 				1.������ת��;2.������������Ч��Ŀ��������ת��;3.����������;
 				4.����ת����Ŀ����������.
 				�ڴ淽ʽ��ת:
 				1.������������Ч��Ŀ�������ڴ���;2.����������;
 				3.���ڴ�����Ŀ����������.
 				Flash��ʽ��ת���ڴ淽ʽ��ת��ȫ�ȸ�
 Data Accessed:
 Data Updated:
 Input:			��
 Output:			��
 Return:			ERROR_ITEM_REPEAT_OK
 Others:
=========================================================================*/
int item_recycle(void)
{
	item_t *item;
	unsigned int i = 0;
	unsigned int sum = 0;
	unsigned int len = 0;
	unsigned int block_size;
	unsigned char *item_sdram;
	unsigned int addr;

	item_sdram = (unsigned char *)(CONFIG_DRAM_BASE + gd->bi_memsize - BOOT_RUNTIME_SIZE + ITEM_MAP_OFFSET);
	while((info->start[i] - info->start[0] ) < BOOTCODE_LENGTH)
		{/* �ҵ�Flash��������������������ʼ��ַ*/
		i++;
		}
	block_size = info->start[i+1] - info->start[i];
#ifdef CONFIG_FLASH_TRANSFER
	flash_erase(info, i+1, i+1); /* ������ת�� */
	addr = info->start[i+1] + CONFIG_MARK_LEN;
#else
	addr = CONFIG_DRAM_BASE + gd->bi_memsize - BOOT_RUNTIME_SIZE + ITEM_CACHE_OFFSET;
#endif
	flash_read_buf(info, (char *)(info->start[i]), &item_sdram, block_size);
	sum = CONFIG_MARK_LEN;

	while((sum + ITEM_HEAD_LEN) <= block_size)
		{	/* ������������Ч��Ŀ��������ת��*/
		item = (item_t *)(item_sdram + sum);
		fldebug("repeat item state: %02x, len: %x\n", item->hdr.avail, item->hdr.len);
		switch(item->hdr.avail)
			{
			case ITEM_AVAIL:
				#ifdef CONFIG_FLASH_TRANSFER
				flash_write((char *)item, addr, ITEM_SIZE(item->hdr.len));
				#else
				memcpy((unsigned int *)addr, item, ITEM_SIZE(item->hdr.len));
				#endif
				sum += ITEM_SIZE(item->hdr.len);
				addr += ITEM_SIZE(item->hdr.len);
				len += ITEM_SIZE(item->hdr.len);
				break;

			case ITEM_UNAVAIL:
				sum += ITEM_SIZE(item->hdr.len);
				break;

			case ITEM_BAD:
				sum += ITEM_SIZE(0);
				break;

			default:
				goto recovery_done;
			}
		}
recovery_done:
	#ifdef CONFIG_FLASH_TRANSFER
	flash_write(CONFIG_MARK, info->start[i+1], CONFIG_MARK_LEN);
	#endif
	flash_erase(info, i, i);/*����������*/
	/* ����Ŀд��������*/
	#ifdef CONFIG_FLASH_TRANSFER
	flash_read_buf(info, (char *)(info->start[i + 1]), &item_sdram, len + CONFIG_MARK_LEN);
	flash_write((char *)(item_sdram + CONFIG_MARK_LEN), info->start[i] + CONFIG_MARK_LEN, len);
	#else
	addr = CONFIG_DRAM_BASE + gd->bi_memsize - BOOT_RUNTIME_SIZE + ITEM_CACHE_OFFSET;
	flash_write((char *)addr, info->start[i] + CONFIG_MARK_LEN, len);
	#endif
	flash_write(CONFIG_MARK, info->start[i], CONFIG_MARK_LEN);

	return ERROR_ITEM_REPEAT_OK;
}


int system_config_map(unsigned long addr)
{
	unsigned short	item_len;
	int syscfg_version = -1;

	if(item_map_and_check() == ERROR_ITEM_OK)
		{
		debug("FUNC:%s,LINE:%d\n", __func__, __LINE__);
		if(item_get((void *)addr, LLCONFIG_NAME, &item_len) == ERROR_ITEM_OK)
			{
			debug("FUNC:%s,LINE:%d\n", __func__, __LINE__);
			syscfg_version = is_sysdata((void *)addr);
			}
		}

	return syscfg_version;
}

static inline int image_checksum_ok(int first_second_flag, unsigned char *buf)
{
    unsigned long crc_checksum;
    unsigned int len;
    int ret_val = -ERROR_CRC;
    struct sys_cfg *syscfg;

	syscfg = (struct sys_cfg *)(gd->syscfg_addr);
	if(0 == first_second_flag)
		{ // �ӵ�һ��image����
		len = syscfg->first_rootfs_offset + syscfg->first_rootfs_len - syscfg->first_kernel_offset;
		fldebug("first_rootfs_offset=%08x first_rootfs_len=%08x first_kernel_offset=%08x firlen=%08x\n",
			syscfg->first_rootfs_offset, syscfg->first_rootfs_len, syscfg->first_kernel_offset, len);
		flash_read_buf(info, (unsigned long *)(CONFIG_FLASH_BASE + syscfg->first_kernel_offset), &buf, len);
		tbs_calc_sum_addr(buf, &crc_checksum, len);
		if(syscfg->first_image_checksum == crc_checksum)
			{
			ret_val = ERROR_OK;
			}
		else
			{
			printf("First image CRC validation failed\n");
			fldebug("first_image_checksum=0x%08x, crc_checksum=0x%08x\n", syscfg->first_image_checksum, crc_checksum);
			}
		}
	else
		{ // �ӵڶ���image����
		len = syscfg->second_rootfs_offset + syscfg->second_rootfs_len - syscfg->second_kernel_offset;
		fldebug("second_rootfs_offset=%08x second_rootfs_len=%08x second_kernel_offset=%08x seclen=%08x\n",
			syscfg->second_rootfs_offset, syscfg->second_rootfs_len, syscfg->second_kernel_offset, len);
		flash_read_buf(info, (unsigned long *)(CONFIG_FLASH_BASE + syscfg->second_kernel_offset), &buf, len);
		tbs_calc_sum_addr(buf, &crc_checksum, len);
		if(syscfg->second_image_checksum == crc_checksum)
			{
			ret_val = ERROR_OK;
			}
		else
			{
			printf("Second image CRC validation failed\n");
			fldebug("second_image_checksum=0x%08x, crc_checksum=0x%08x\n", syscfg->second_image_checksum, crc_checksum);
			}
		}		
	
    return ret_val;
}

unsigned int prepare_kernel(void)
{
    unsigned char *buf = (unsigned char *)(CONFIG_DRAM_BASE + KERNEL_TMP_OFFSET);
    struct sys_cfg *syscfg = NULL;
	int flag = 0;
	unsigned int addr = 0;

	printf("Checking version %d image... ", gd->syscfg_ver);		
	syscfg = (struct sys_cfg *)(gd->syscfg_addr);
	if(syscfg->boot_img_flag == 1)
		{
		if((0 != syscfg->second_kernel_len) && (0 != syscfg->second_rootfs_len) && (image_checksum_ok(1, buf) == ERROR_OK))
			{
			flag = 2;
			}
		else if((0 != syscfg->first_kernel_len) && (0 != syscfg->first_rootfs_len) && (image_checksum_ok(0, buf) == ERROR_OK))
			{
			syscfg->boot_img_flag = 0;
			flag = (1 | 4);
			}
		}
	else
		{
		if((0 != syscfg->first_kernel_len) && (0 != syscfg->first_rootfs_len) && (image_checksum_ok(0, buf) == ERROR_OK))
			{
			flag = 1;
			}
		else if((0 != syscfg->second_kernel_len) && (0 != syscfg->second_rootfs_len) && (image_checksum_ok(1, buf) == ERROR_OK))
			{
			syscfg->boot_img_flag = 1;
			flag = (2 | 4);
			}
		}
	
	fldebug("Prepare kernel flag is %#x\n", flag);
	if(4 & flag)
		{
		sysdata_save((void *)(gd->syscfg_addr));					
		}
	if(2 & flag)
		{
		addr = (unsigned int)buf;
		printf("Second image OK\n");
		}
	else if(1 & flag)
		{
		addr = (unsigned int)buf;
		printf("First image OK\n");
		}
	else
		{
		printf("Failed!\n");
		}

	return addr;		
}

void prepare_kernel_commandline(char *arg)
{
#ifdef CONFIG_CMDLINE_TAG
	char *commandline = { CONFIG_BOOTARGS };
	char *p, *r;
	char partinfo[512] = {'\0'};
	char buf[256] = {'\0'};
	struct sys_cfg *syscfg;
	int len = 0;
	
	syscfg = (struct sys_cfg *)(gd->syscfg_addr);

	/* pass tags for Linux kernel */    
	gd->bi_boot_params = (unsigned long)(CONFIG_DRAM_BASE + BOOTARGS_OFFSET);
	sprintf(buf, commandline, CONFIG_BAUDRATE, gd->bi_memsize >> 20);
	
	/* eat leading white space */
	for(p = buf; *p == ' '; p++);
	/* skip non-existent command lines so the kernel will still use its default command line. */
	if (*p == '\0')
		{
		printf("please define CONFIG_BOOTARGS in bootloader header file.\n");
		return;
		}
	if(!arg)
		{		
		strcpy(partinfo, p);
		memset(buf, 0x00, sizeof(buf));
		if(1 == gd->syscfg_ver)
			{
			
			if((0 == syscfg->boot_img_flag) && (0 != syscfg->first_kernel_len) && (0 != syscfg->first_rootfs_len))
				{
		 		sprintf(buf, "%d(boot),%d(kernel),%d(rootfs),-(data)", syscfg->first_kernel_offset, 
					syscfg->first_rootfs_offset - syscfg->first_kernel_offset, 
					syscfg->first_rootfs_len);
				}
			else if((1 == syscfg->boot_img_flag) && (0 != syscfg->second_kernel_len) && (0 != syscfg->second_rootfs_len))
				{
				sprintf(buf, "%d(boot),%d(kernel),%d(rootfs),-(data)", syscfg->second_kernel_offset, 
					syscfg->second_rootfs_offset - syscfg->second_kernel_offset, 
					syscfg->second_rootfs_len);
				}
			}
		else
			{
			printf("Unkown system configuration version!\n");
			}
		strcat(partinfo, buf);
		}
	else
		{
		for(r = p; r - p < 50; r++)
			{
			if(0 == strncmp(r, "root=", 5))
				{
				len = r - p;
				break;
				}
			}
		memcpy(partinfo, buf, len);
		memcpy((char *)(partinfo + len), arg, 255);
		}
	memcpy((char *)gd->bi_boot_params, partinfo, sizeof(partinfo));
   	fldebug("bootargs:%s \n", (char *)gd->bi_boot_params);
#endif
}

static int offset2sector(unsigned int offset)
{
	int i;

	if((offset < 0) || (offset > info->size))
		{
		printf("Offset out of range!\n");
		i = -1;
		goto err;
		}
	else
		{
		for(i=0; i< info->sector_count; i++)
			{
			if(info->start[i] > (info->start[0] + offset))
				break;
			}
		i -= 1;
		}
	
err:	
	return i;
}

static int update_sys_cfg(unsigned int cfg_sector, int force_erase)
{
	int ret_val;

	printf("Done\nUpdate system configuration ");
	if(1 == force_erase)
		{
		fldebug("\nErase configuration area, cfg_sector=%d\n", cfg_sector);
		#ifdef CONFIG_FLASH_TRANSFER
		flash_erase(info, cfg_sector, cfg_sector + 1);
		#else
		flash_erase(info, cfg_sector, cfg_sector);
		#endif			
		}
	else
		{
		item_save("clear", TBS_APP_CFG, 5); 			/*���Ӧ�ò�����*/
		}
	ret_val = sysdata_save((void *)(gd->syscfg_addr));
	if(ret_val == ERROR_OK)
		{
		flash_write(CONFIG_MARK, info->start[cfg_sector], CONFIG_MARK_LEN);
		printf("Done\n");
		}

	return ret_val;
}

static int generate_sys_cfg(void *data, unsigned int len, void *ptr)
{
	int syscfg_version = -1;
	int ret_val = ERROR_IMG_SIZE;
	struct sys_cfg *sys_data = (struct sys_cfg *)(gd->syscfg_addr);
    struct sys_cfg syscfg;
    unsigned int temp;
	struct update_parameters *uppa;
	const unsigned char default_mac[] ={0x00, 0x02, 0x03, 0x04, 0x05, 0x06};
	#ifdef __BIG_ENDIAN
	const unsigned int default_ip = 0xc0a80101;
	#else	
	const unsigned int default_ip = 0x0101a8c0;
	#endif
	#ifdef __BIG_ENDIAN
	const unsigned char sys_endian = 0x42; //"B"
	#else	
	const unsigned char sys_endian = 0x4C; //"L"
	#endif
	
	if(NULL == ptr)
		{
		printf("Update information points to null\n");
		ret_val = ERROR_NO_MEM;
		goto out;
		}
	uppa = (struct update_parameters *)ptr;
	/* Check system data version */
	syscfg_version = is_sysdata(sys_data);
	uppa->syscfg_version = syscfg_version;
    memcpy(&syscfg, sys_data, sizeof(struct sys_cfg));
	switch(uppa->tail.img_type) /* ����������汾�Ƿ����ļ��ĸ�ʽ������粻�������ºϳ� */
		{
		case TYPE_SINGLE_IMG:
		case TYPE_DUAL_IMG:
		case TYPE_BACKUP_IMG:	
		case TYPE_BACKUP_BIMG:
		case TYPE_BACKUP_FIMG:
			if(1 != syscfg_version) /* �������汾��ƥ�䣬��Ҫ���ºϳ� */
				{
				printf("sysdata version [%d] not match with target version[1]\n", syscfg_version);				
				memset(&syscfg, 0x00, sizeof(struct sys_cfg));
				memcpy(syscfg.cfg_version, SYSTEM_CONFIG_VERSION, sizeof(syscfg.cfg_version));
				syscfg.ip = default_ip;
				memcpy(syscfg.mac, default_mac, sizeof(syscfg.mac));
				syscfg.endian = sys_endian;
				memcpy(syscfg.board_id, uppa->tail.board_id, sizeof(syscfg.board_id));
				memcpy(syscfg.product, uppa->tail.product, sizeof(syscfg.product));
				memcpy(syscfg.version, uppa->tail.version, sizeof(syscfg.version));
				memcpy(syscfg.tag, "sysc", sizeof(syscfg.tag));
				}
			else
				{
				printf("sysdata version [%d] exist\n", syscfg_version);
				}
			break;
			
		default:
			break;
		}

	uppa->cfg_sector = offset2sector((unsigned int)BOOTCODE_LENGTH);
	#ifdef CONFIG_FLASH_TRANSFER
	uppa->f_s_sector = uppa->cfg_sector + 2;
	#else
	uppa->f_s_sector = uppa->cfg_sector + 1;
	#endif	
	
	switch(uppa->tail.img_type)  /* ������ļ��������FLASH��ƫ�Ƶ�ַ�������ţ������浽ϵͳ������(RAM��) */
		{
		case TYPE_SINGLE_IMG:
		case TYPE_DUAL_IMG:			
		case TYPE_BACKUP_IMG:	
		case TYPE_BACKUP_BIMG:
		case TYPE_BACKUP_FIMG:
			if(1 == syscfg_version)
				{
				if(((uppa->tail.img_type == TYPE_BACKUP_BIMG) && 
				((syscfg.first_rootfs_offset + syscfg.first_rootfs_len - syscfg.first_kernel_offset) < uppa->tail.image_len)) ||
				((uppa->tail.img_type == TYPE_BACKUP_FIMG) && 
				((syscfg.second_rootfs_offset + syscfg.second_rootfs_len - syscfg.second_kernel_offset) < uppa->tail.image_len)))
					{
					printf("Backup image file size out of range!\n");
					goto out;
					}	
				}
			if(uppa->tail.img_type != TYPE_BACKUP_FIMG)/* �����һ���� */
				{
				syscfg.first_kernel_offset = info->start[uppa->f_s_sector] - info->start[0];
				syscfg.first_kernel_len = uppa->tail.first_kernel_len;
				syscfg.first_rootfs_offset = syscfg.first_kernel_offset + (uppa->tail.first_rootfs_offset - uppa->tail.first_kernel_offset);
				syscfg.first_rootfs_len = uppa->tail.first_rootfs_len;
				syscfg.first_image_checksum = uppa->tail.first_image_checksum;
				fldebug("\nfirst_kernel_offset=0x%x,first_rootfs_offset=0x%x\n", syscfg.first_kernel_offset, syscfg.first_rootfs_offset);
				uppa->f_e_sector = offset2sector((unsigned int)(syscfg.first_rootfs_offset + syscfg.first_rootfs_len));	
				if((uppa->f_s_sector < 0) || (uppa->f_s_sector > info->sector_count) ||
					(uppa->f_e_sector < 0) || (uppa->f_e_sector > info->sector_count))
					{
					printf("Image size not fit for flash!\n");
					goto out;
					}				
				}
			if(uppa->tail.img_type == TYPE_SINGLE_IMG)
				{
				syscfg.second_kernel_len = 0;
				syscfg.second_rootfs_len = 0;
				syscfg.boot_img_flag = 0; /* �ӵ�һ��IMAGE���� */
				}
			if((uppa->tail.img_type != TYPE_SINGLE_IMG) && (uppa->tail.img_type != TYPE_BACKUP_BIMG))/* ����ڶ����� */
				{
          			if(uppa->tail.img_type == TYPE_DUAL_IMG){
        				uppa->tail.second_kernel_offset = uppa->tail.first_kernel_offset;
        				uppa->tail.second_kernel_len = uppa->tail.first_kernel_len;
        				uppa->tail.second_rootfs_offset = uppa->tail.first_rootfs_offset;
        				uppa->tail.second_rootfs_len = uppa->tail.first_rootfs_len;
        				uppa->tail.second_image_checksum = uppa->tail.first_image_checksum;
                        /* ˫����ʱ��ʣ��ռ�԰�� */
                        temp = syscfg.first_kernel_offset + ((info->size - syscfg.first_kernel_offset) >> 1); 
                        uppa->s_s_sector = offset2sector(temp);
                    } else {
        				temp = syscfg.first_rootfs_offset + syscfg.first_rootfs_len;
        				uppa->s_s_sector = offset2sector(temp);
                        temp = offset2sector(sys_data->first_rootfs_offset + sys_data->first_rootfs_len);
                        if(uppa->s_s_sector < temp) {/* �ڶ���img�Ŀ�ʼ���������ԭʼ�ĵ�һimg���������� */
                            uppa->s_s_sector = temp;
                    }
                }
                uppa->s_s_sector += 1;
				syscfg.second_kernel_offset = info->start[uppa->s_s_sector] - info->start[0];
				syscfg.second_kernel_len = uppa->tail.second_kernel_len;
				syscfg.second_rootfs_offset = syscfg.second_kernel_offset + (uppa->tail.second_rootfs_offset - uppa->tail.second_kernel_offset);
				syscfg.second_rootfs_len = uppa->tail.second_rootfs_len;				
				fldebug("\nsecond_kernel_offset=0x%x,second_rootfs_offset=0x%x\n", syscfg.second_kernel_offset, syscfg.second_rootfs_offset);
				syscfg.second_image_checksum = uppa->tail.second_image_checksum;
				uppa->s_e_sector = offset2sector((unsigned int)(syscfg.second_rootfs_offset + syscfg.second_rootfs_len));
				if((uppa->s_s_sector < 0) || (uppa->s_s_sector > info->sector_count) ||	(uppa->s_e_sector < 0) || (uppa->s_e_sector > info->sector_count))
					{
					printf("Image size not fit for flash!\n");
					goto out;
					}				
				}
            memcpy(sys_data, &syscfg, sizeof(struct sys_cfg));
			ret_val = ERROR_OK;
			break;
			
		default:
			if(uppa->tail.image_len < info->size)
				{
				ret_val = ERROR_OK;
				}
			break;
		}
out:	
	return ret_val;
}
/*=========================================================================
 Function Description:	�Խ�Ҫ������image�Ϸ��Լ�������ּ����ɲ���force_flag����
 				1.��image ��PRODUCT���----�˷�ʽΪĬ�Ϸ�ʽ
 				2.ֻ��CRC���ļ����ȵĺϷ��Լ��

 Data Accessed:
 Data Updated:
 Input:			len:����image�ļ�����
 				force_mark:��鼶���:0Ϊȫ����飬1������PRODUCT/CRC���
 Output:			��
 Return:		ERROR_CRC:		crcУ�����,�����ļ�������
				ERROR_PRODUCT:	����image�Ǳ���Ʒ
				ERROR_IMG_SIZE:	image�ļ��ߴ粻����Ҫ��
				ERROR_OK:		�����ɹ�
 Others:
=========================================================================*/
int file_check(void *data, unsigned int len, int force_flag, void *ptr)
{
	unsigned long checksum, file_crc;
	int ret_val = ERROR_IMG_TYPE;
	struct update_parameters *uppa = NULL;

	if((NULL == ptr) || (NULL == data))
		{
		printf("Image tail information or data points to null!\n");
		ret_val = ERROR_NO_MEM;
		goto err;
		}
	else
		{
		uppa = (struct update_parameters *)ptr;
		}
	if(force_flag < 0)
		{
		uppa->tail.img_type = TYPE_BOOTLOADER;
		uppa->tail.image_len = len;
		goto show_type;
		}
	if((len < 1) || ((len % 4) != 0) || (len > info->size) || (((unsigned long)data % 4) != 0))
		{
		printf("Illegal file size!\n");
		ret_val = ERROR_IMG_SIZE;
		goto err;
		}
	if(0 == force_flag) /* Check file CRC by default, jump this step when force enabled */
		{
		file_crc = *((unsigned long *)(data + len - sizeof(checksum)));
		tbs_calc_sum_addr((unsigned char *)data, &checksum, len - sizeof(checksum));
		if(checksum != file_crc)
			{
			printf("File CRC check failed\n");
			fldebug("data=0x%08x, image_checksum=0x%08x, checksum=0x%08x\n",(unsigned int)data, file_crc, checksum);
			ret_val = ERROR_CRC;
			goto err;
			}
		}
		
	/* Check image tail version */
	if(strncmp((char *)(data + len - sizeof(checksum) - VERSION_LEN), IMAGE_TAIL_VERSION, VERSION_LEN) != 0)
		{ /* File tail information not match with the latest version */
		uppa->tail.img_type = TYPE_UNKNOWN;
		}
	else
		{
		memcpy(&(uppa->tail), (struct image_tail *)(data + len - sizeof(struct image_tail)), sizeof(struct image_tail));
		}
	
show_type:	
	printf("File type: ");
	switch(uppa->tail.img_type)
		{
		case TYPE_SINGLE_IMG:
			force_flag = 0;
			printf("new single img file\n");
			break;
		case TYPE_SINGLE_BIN:
			printf("new single bin file\n");
			break;
		case TYPE_DUAL_IMG:
			force_flag = 0;			
			printf("new dual img file\n");
			break;
		case TYPE_DUAL_BIN:
			printf("new dual bin file\n");
			break;
		case TYPE_BACKUP_IMG:
			force_flag = 0;
			printf("new backup img file\n");
			break;
		case TYPE_BACKUP_BIN:
			printf("new backup bin file\n");
			break;
		case TYPE_BACKUP_FIMG:
			force_flag = 0;
			printf("new backup full img file\n");
			break;
		case TYPE_BACKUP_BIMG:
			force_flag = 0;
			printf("new backup backup img file\n");
			break;
		case TYPE_BOOTLOADER:
			printf("new bootloader file\n");
			break;
		default:
			printf("unknown\n");
			break;			
		}

 	if((0 == force_flag) && (strncmp(DIGIT_SIG, (char *)(uppa->tail.product), sizeof(uppa->tail.product)) != 0))
 		{/*check product information */
		printf("Product type %s not match with target system %s!\n", uppa->tail.product, DIGIT_SIG);
		uppa->tail.img_type = TYPE_UNKNOWN;		
		ret_val = ERROR_PRODUCT;
		goto err;
 		}

	if(uppa->tail.img_type != TYPE_UNKNOWN)
		{
		ret_val = generate_sys_cfg(data, len, ptr);
		}
	

err:
	return ret_val;
}


static inline int image_file_write(void *data, struct update_parameters *uppa, int part, int ver)
{
	unsigned int s_sector = uppa->f_s_sector;
	unsigned int e_sector = uppa->f_e_sector;
	char *src = (char *)(data + uppa->tail.first_kernel_offset);
	int force_erase = 0;
	int ret_val;
	
	if(0 != part)
		{
		s_sector = uppa->s_s_sector;
		e_sector = uppa->s_e_sector;
		src = (char *)(data + uppa->tail.second_kernel_offset);
		}
	printf("\nErase flash from sector 0x%02x to sector 0x%02x...\n", s_sector, e_sector);
	flash_erase(info, s_sector, e_sector);
	printf("Done\nBurning part %d to flash...\n", part + 1);
	flash_write(src, uppa->dest_addr, uppa->size);
	if(uppa->syscfg_version != ver)
		{
		force_erase = 1;
		}
	ret_val = update_sys_cfg(uppa->cfg_sector, force_erase);
	
	return ret_val;
				
}


/*=========================================================================
 Function Description:	ʹ��bin�ļ���img�ļ���ϵͳ��������
 Data Accessed:
 Data Updated:
 Input:			len:����image�ļ�����	ptr:�ļ�β���ṹ��ָ��
 Output:			��
 Return:		ERROR_CRC:		crcУ�����,�����ļ�������
				ERROR_PRODUCT:	����image�Ǳ���Ʒ
				ERROR_IMG_SIZE:	image�ļ��ߴ粻����Ҫ��
				ERROR_OK:		�����ɹ�
				ERROR_NO_MEM    �ڴ����
 Others:
=========================================================================*/
int flash_update(void *data, unsigned int len, int flag, void *ptr)
{
	int ret_val;
	struct sys_cfg *sys_data = (struct sys_cfg *)(gd->syscfg_addr);
	struct update_parameters *uppa = NULL;

	if(NULL == ptr)
		{
		printf("Update parameters points to NULL!\n");
		ret_val = ERROR_NO_MEM;
		goto out;
		}
	else
		{
		uppa = (struct update_parameters *)ptr;
		}
	ret_val = file_check(data, len, flag, uppa);
	if(ret_val != ERROR_OK)
		{
		uppa->tail.img_type = TYPE_UNKNOWN;
		}
	switch(uppa->tail.img_type)/* ��дFLASH���� */
		{
		case TYPE_SINGLE_BIN:
		case TYPE_DUAL_BIN:
		case TYPE_BACKUP_BIN:
		case TYPE_BOOTLOADER:			
			printf("\nErase flash\n");
			if(uppa->tail.img_type == TYPE_BOOTLOADER)
				flash_erase(info, 0, 0);
			else /* ���Ų�����������޶ȵı���bootloader����ֹ�ϵ������ */
				flash_erase(info, info->sector_count - 1, 0); 
			printf("Done\nBurning flash\n");
			flash_write((char *)data, info->start[0], (unsigned long)uppa->tail.image_len);
			printf("Done\n");
			ret_val = ERROR_OK;
			break;
		
		case TYPE_SINGLE_IMG:
		case TYPE_BACKUP_BIMG:
			uppa->dest_addr = (unsigned long)(info->start[0] + sys_data->first_kernel_offset);
			uppa->size = (unsigned long)(sys_data->first_rootfs_offset + sys_data->first_rootfs_len - sys_data->first_kernel_offset);
			ret_val = image_file_write(data, uppa, 0, 1);
			break;
			
		case TYPE_DUAL_IMG:
		case TYPE_BACKUP_IMG:
			printf("Current active image is %d\n", sys_data->boot_img_flag + 1);
			if(0 == sys_data->boot_img_flag)
				{
				uppa->dest_addr = (unsigned long)(info->start[0] + sys_data->second_kernel_offset);
				uppa->size = (unsigned long)(sys_data->second_rootfs_offset + sys_data->second_rootfs_len - sys_data->second_kernel_offset);
				sys_data->boot_img_flag = 1;
				ret_val = image_file_write(data, uppa, 1, 1);
				}
			else
				{
				uppa->dest_addr = (unsigned long)(info->start[0] + sys_data->first_kernel_offset);
				uppa->size = (unsigned long)(sys_data->first_rootfs_offset + sys_data->first_rootfs_len - sys_data->first_kernel_offset);
				sys_data->boot_img_flag = 0;
				ret_val = image_file_write(data, uppa, 0, 1);
				}
			break;
			
		case TYPE_BACKUP_FIMG:
			uppa->dest_addr = (unsigned long)(info->start[0] + sys_data->second_kernel_offset);
			uppa->size = (unsigned long)(sys_data->second_rootfs_offset + sys_data->second_rootfs_len - sys_data->second_kernel_offset);
			ret_val = image_file_write(data, uppa, 1, 1);
			break;

		default:
			printf("File type NOT recognized!\n");
			ret_val = ERROR_IMG_TYPE;
			goto out;
		}
	printf("\nSystem update %s!\n", (ret_val == ERROR_OK) ? "Successfully" : "Failed");

out:
	return ret_val;
}

