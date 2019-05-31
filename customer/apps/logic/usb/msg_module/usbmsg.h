/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 �ļ�����:usbmsg.h
 �ļ�����:usb Mass Storage �豸��Ϣ����ģ��ͷ�ļ�
 �޶���¼:
          1 ���� : ���_
            ���� : 2008-5-12
            ���� :

          2 �޸� : ����
            ���� : 2009-3-26
            ���� : ���USB���ٻָ����ؽڵ�

**********************************************************************/

#ifndef __USBMSG_H__
#define __USBMSG_H__


/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/

#define CFG_USB_OBJ             "object"

/* node path */
#define PATH_USBDEV             "InternetGatewayDevice.X_TWSZ-COM_USBDevice"
#define PATH_USBDEV_MASSDEV_SUM "InternetGatewayDevice.X_TWSZ-COM_USBDevice.StorageSum"
#define PATH_USBDEV_MASS        "InternetGatewayDevice.X_TWSZ-COM_USBDevice.Storage"

/* CFG Restore */
#define PATH_USBDEV_CFG_RESTORE_ENABLE  "InternetGatewayDevice.DeviceInfo.X_TWSZ-COM_Restore.Enable"


/* ������ñ����ļ��Ƿ������غ궨�� */
/* Device Name */
#define PATH_USBDEV_CFG_DEVICE_NAME     "InternetGatewayDevice.DeviceInfo.ModelName"

#define USB_MASS_MOUNT_PATH             "/mnt"
#define USB_MASS_CFG_RESTORE_DIR        "e8_Config_Backup"


/* Object Storage SubObject*/
/* InternetGatewayDevice.USBDevice.Storage.{i}. */
#define NODE_USB_MASS_VENDOR        "Vendor"
#define NODE_USB_MASS_MODEL         "Model"
#ifdef CONFIG_APPS_LOGIC_TF_GUI
#define NODE_USB_MASS_SERIAL        "serial"
#define NODE_USB_MASS_TYPE          "type"
#define NODE_USB_MASS_VERSION       "version"
#endif
#define NODE_USB_MASS_DISKSIZEMB    "DiskSizeMB"
#define NODE_USB_MASS_PARTITIONSUM  "PartitionSum"
/* Object Partitions */
#define NODE_USB_MASS_PARTITIONS    "Partitions"
/* Partitions SubObject */
/* InternetGatewayDevice.USBDevice.Storage.{i}.Partitions.{*}. */
#define NODE_USB_MASS_PARTS_SDNAME      "SDName"
#define NODE_USB_MASS_PARTS_BOOT        "Boot"
#define NODE_USB_MASS_PARTS_PARTUSEDKB  "PartUsedKB"
#define NODE_USB_MASS_PARTS_PARTSIZEMB  "PartSizeMB"
#define NODE_USB_MASS_PARTS_SYSID       "SysID"
#define NODE_USB_MASS_PARTS_FILESYSTEM  "FileSystem"
#define NODE_USB_MASS_PARTS_PERCENTUSED "PercentUsed"
#define NODE_USB_MASS_PARTS_MOUNTPOINT  "MountPoint"
#define NODE_USB_MASS_PARTS_MOUNTED     "Mounted"
#define NODE_USB_MASS_PARTS_CFGFILE_EXIST  "CfgBackupFileExist"


/******************************************************************************
*                                 END                                        *
******************************************************************************/

#endif /* __USBMSG_H__ */
