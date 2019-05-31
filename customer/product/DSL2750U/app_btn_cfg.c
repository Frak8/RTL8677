/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 �ļ����� : app_btn_cfg.c
 �ļ����� : ���ļ�����Ӧ�ò㰴ť������Ϣ�����ļ����ᱻ���ӵ� build/app/ssap/btn Ŀ¼�±���
 �����б� : 

 
 �޶���¼ :
          1 ���� : ����
            ���� : 2008-11-07
            ���� : �����ĵ�

**********************************************************************/

#include <btn.h>
#include <btn_app.h>

#define wlan_wps_in_wlan


struct APP_BTN_CFG AppBtnCfg[] =
{
    {btn_wps,CFG_APP_BTN_MODE_RELEASE_LESS,1,0,APP_BTN_EVEN_WPS},
    {btn_wps,CFG_APP_BTN_MODE_TIMER,1,0,APP_BTN_EVEN_WPS},
#ifdef wlan_wps_in_wlan
    {btn_wlan,CFG_APP_BTN_MODE_RELEASE_LESS,1,0,APP_BTN_EVEN_WLAN},
    {btn_wlan,CFG_APP_BTN_MODE_INTERVAL,2,6,APP_BTN_EVEN_WPS},
     {btn_wlan,CFG_APP_BTN_MODE_RELEASE_GREATER,7,0,APP_BTN_EVEN_WPS_SWITCH},
#else
    {btn_wlan,CFG_APP_BTN_MODE_RELEASE_LESS,1,0,APP_BTN_EVEN_WLAN},
    {btn_wlan,CFG_APP_BTN_MODE_TIMER,1,0,APP_BTN_EVEN_WLAN},
#endif
    {btn_reset,CFG_APP_BTN_MODE_RELEASE_LESS,3,0,APP_BTN_EVEN_REBOOT},  /* RESET��ť:���º��ɿ���С��3�봥��APP_BTN_EVEN_REBOOT�¼� */
    {btn_reset,CFG_APP_BTN_MODE_RELEASE_GREATER,5,0,APP_BTN_EVEN_RESET},/* RESET��ť:���º��ɿ�������5�봥��APP_BTN_EVEN_RESET�¼� */
    {btn_reset,CFG_APP_BTN_MODE_TIMER,1,0,APP_BTN_EVEN_RESET},          /* RESET��ť:���²���8�봥��APP_BTN_EVEN_RESET�¼� */
    {btn_end,0,0,0},                                                  /* btn_end:���������־ */
};

