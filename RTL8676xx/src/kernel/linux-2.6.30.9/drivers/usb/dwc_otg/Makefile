#
# Makefile for DWC_otg Highspeed USB controller driver
#

ifeq ($(CONFIG_USB_DEBUG),y)
	EXTRA_CFLAGS		+= -DDEBUG
endif

#ifneq ($(KERNELRELEASE),)
#EXTRA_CFLAGS	+= -DDEBUG 

# Use one of the following flags to compile the software in host-only or
# device-only mode.
#EXTRA_CFLAGS        += -DDWC_HOST_ONLY

ifeq ($(CONFIG_USB_OTG_DEVICE_RTL8676),y)
EXTRA_CFLAGS        += -DDWC_DEVICE_ONLY
else
EXTRA_CFLAGS        += -DDWC_HOST_ONLY
endif

EXTRA_CFLAGS	+= -Dlinux -DDWC_HS_ELECT_TST


obj-m	:= dwc_otg.o

#dwc_otg-objs	:= dwc_otg_driver.o dwc_otg_attr.o 
dwc_otg-objs	:= dwc_otg_driver.o
dwc_otg-objs	+= dwc_otg_cil.o dwc_otg_cil_intr.o 
ifeq ($(CONFIG_USB_OTG_DEVICE_RTL8676),y)
dwc_otg-objs	+= dwc_otg_pcd.o dwc_otg_pcd_intr.o 
else
dwc_otg-objs	+= dwc_otg_hcd.o dwc_otg_hcd_intr.o dwc_otg_hcd_queue.o
endif


obj-y	+= lm.o
obj-y	+= dwc_otg.o 


#else

#KDIR		:= /home/brian/DesignWare/linux-2.6.12.2
#PWD		:= $(shell pwd)
#CROSS_COMPILE 	:= /home/brian/DesignWare/toolchain/bin/arm-none-elf-
#ARCH 		:= arm

# Command paths
#CTAGS		:= /usr/bin/ctags
#DOXYGEN		:= /usr/bin/doxygen

#default:
#	$(MAKE) -C$(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

#docs:	$(wildcard *.[hc]) doc/doxygen.cfg
#	$(DOXYGEN) doc/doxygen.cfg

#tags:	$(wildcard *.[hc])
#	$(CTAGS) -e $(wildcard *.[hc]) $(wildcard linux/*.[hc]) $(wildcard $(KDIR)/include/linux/usb*.h)

#endif

clean:
	rm -rf   *.o *.ko .*cmd *.mod.c .tmp_versions
