#this tcl script includes test cases for module "pppoe"

####################################################################################
#set ��Ϣ
if {$TestCaseNo == 1} {

set MsgList(1.1.1.1) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(1.1.1.1) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"


###1.1 ��������
#���ò���
set MsgList(1.2.1.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{4 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Username=alex\0InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Password=alex\0InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Name=PPPOE\0InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.ConnectionTrigger=Manual\0}}"
set AckMatchList(1.2.1.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^0$} {^0$}}"

set MsgList(1.2.1.2) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
set AckMatchList(1.2.1.2) ""

#ʹ�ܴ�
set MsgList(1.2.2.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Enable=1\0}}"
set AckMatchList(1.2.2.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^0$} {^0$}}"

set MsgList(1.2.2.2) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
set AckMatchList(1.2.2.2) ""

#ʹ�ܹر�
set MsgList(1.2.3.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Enable=0\0}}"
set AckMatchList(1.2.3.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^0$} {^0$}}"

set MsgList(1.2.3.2) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
set AckMatchList(1.2.3.2) ""

###1.2 ����Ϸ��Լ��

#�ı�ڵ��������ʵ�ʸ���
set MsgList(1.3.1.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{5 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Username=alex\0InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Password=alex\0InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Name=PPPOE\0InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.ConnectionTrigger=Manual\0}}"
set AckMatchList(1.3.1.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9003$} {^9003$}}"

#�ı�ڵ����С��ʵ�ʸ���
set MsgList(1.3.2.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{3 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Username=alex\0InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Password=alex\0InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Name=PPPOE\0InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.ConnectionTrigger=Manual\0}}"
set AckMatchList(1.3.2.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9003$} {^9003$}}"

#��Ϣ��Ϊ��
set MsgList(1.3.3.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {\0}}"
set AckMatchList(1.3.3.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

#·������,����
set MsgList(1.3.4.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.=1\0}}"
set AckMatchList(1.3.4.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

#·������,��������1
set MsgList(1.3.5.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Username.Username=1\0}}"
set AckMatchList(1.3.5.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

#·������,��������2
set MsgList(1.3.6.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Username.ABC=1\0}}"
set AckMatchList(1.3.6.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

#·������,�пո�
set MsgList(1.3.7.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1. Username=1\0}}"
set AckMatchList(1.3.7.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

#·������,ʵ��������
set MsgList(1.3.8.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.99999.Username=1\0}}"
set AckMatchList(1.3.8.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

#ConnectionTriggerֵΪ��
set MsgList(1.3.9.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.ConnectionTrigger=\0}}"
set AckMatchList(1.3.9.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#ConnectionTriggerֵ����
set MsgList(1.3.10.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.ConnectionTrigger=ManualManual\0}}"
set AckMatchList(1.3.10.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#usernameֵΪ��
set MsgList(1.3.11.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Username=\0}}"
set AckMatchList(1.3.11.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#username����
set MsgList(1.3.12.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Username=abcd[string repeat "abcd" 17]\0}}"
set AckMatchList(1.3.12.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#username�����Ϲ��
set MsgList(1.3.13.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Username=a@f!dd\0}}"
set AckMatchList(1.3.13.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#PasswordֵΪ��
set MsgList(1.3.14.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Password=\0}}"
set AckMatchList(1.3.14.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#Password����
set MsgList(1.3.15.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Password=abcd[string repeat "abcd" 17]\0}}"
set AckMatchList(1.3.15.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#Password�����Ϲ��
set MsgList(1.3.16.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Password=a@f!dd\0}}"
set AckMatchList(1.3.16.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#Nameֵ����
set MsgList(1.3.17.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Name=abc[string repeat "abc" 100]\0}}"
set AckMatchList(1.3.17.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#Name�����Ϲ��
set MsgList(1.3.18.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Name=a@f!dd\0}}"
set AckMatchList(1.3.18.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#MRU��С
set MsgList(1.3.19.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.MaxMRUSize=0\0}}"
set AckMatchList(1.3.19.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#MRU����
set MsgList(1.3.20.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.MaxMRUSize=1541\0}}"
set AckMatchList(1.3.20.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#MRU�����Ϲ��
set MsgList(1.3.21.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.MaxMRUSize=a33\0}}"
set AckMatchList(1.3.21.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#·������
set MsgList(1.3.22.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.MaxMRUSize.[string repeat "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.MaxMRUSize." 40]\0}}"
set AckMatchList(1.3.22.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#PPPAuthenticationProtocolΪ��
set MsgList(1.3.23.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.PPPAuthenticationProtocol=\0}}"
set AckMatchList(1.3.23.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#PPPAuthenticationProtocol����
set MsgList(1.3.24.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.PPPAuthenticationProtocol=unknow\0}}"
set AckMatchList(1.3.24.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#X_TWSZ-COM_ConnectionActionΪ��
set MsgList(1.3.25.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.X_TWSZ-COM_ConnectionAction=\0}}"
set AckMatchList(1.3.25.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#X_TWSZ-COM_ConnectionAction����
set MsgList(1.3.26.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.X_TWSZ-COM_ConnectionAction=unknow\0}}"
set AckMatchList(1.3.26.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#EnableΪ��
set MsgList(1.3.27.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Enable=\0}}"
set AckMatchList(1.3.27.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#Enable����
set MsgList(1.3.28.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Enable=unknow\0}}"
set AckMatchList(1.3.28.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"


}


if {$TestCaseNo == 2} {

set MsgList(2.1.1) "{$MID_CMM  $MID_WAN  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.\0}}"
set AckMatchList(2.1.1) "{$MID_WAN  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"

#�������
set MsgList(2.2.1) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(2.2.1) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"

#����������
set MsgList(2.3.1) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(2.3.1) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"

set MsgList(2.3.2) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(2.3.2) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"
set MsgList(2.3.3) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(2.3.3) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"
set MsgList(2.3.4) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(2.3.4) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"
set MsgList(2.3.5) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(2.3.5) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"
set MsgList(2.3.6) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(2.3.6) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"
set MsgList(2.3.7) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(2.3.7) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"
set MsgList(2.3.8) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(2.3.8) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"

#�쳣���
#����·��Ϊ��
set MsgList(2.4.1) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{}"
set AckMatchList(2.4.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_ADD_NODE_ACK$}} \
{{^67371524$} {^67371524$} {^1$} {^ =67371524.*\0$}}"

#����·������
set MsgList(2.4.2) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnectio.\0}}"
set AckMatchList(2.4.2) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^67371524$} {^67371524$} {^1$} {^InternetGatewayDevice.WANDevice.1.WANConnectionDevi.=67371524.*\0$}}"

#����·���ж��ಿ��
set MsgList(2.4.3) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.DevName.xxxx\0}}"
set AckMatchList(2.4.3) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^67371524$} {^67371524$} {^1$} {^InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.DevName.xxxx=67371524.*\0$}}"

#����·��δ��.����
set MsgList(2.4.4) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection\0}}"
set AckMatchList(2.4.4) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^67371524$} {^67371524$} {^1$} {^InternetGatewayDevice.WANDevice.1.WANConnectionDevice=67371524.*\0$}}"

#����·����������������
set MsgList(2.4.5) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE}\
{{InternetGatewayDevice.WANDevice.222.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(2.4.5) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^67371524$} {^67371524$} {^1$} {^InternetGatewayDevice.WANDevice.222.WANConnectionDevice.=67371524.*\0$}}"

}

if {$TestCaseNo == 3} {

###��������
set MsgList(3.1.1.1) "$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_PC_PROC_OUTPUT \
 {pppd\0} 1   6   1 {Connect: ppp0\0}  \
                  2 {local  IP address 172.16.2.23\0} \
                  3 {remote IP address 10.2.10.5\0} \
                  4 {primary   DNS address 10.28.100.2\0} \
                  5 {secondary DNS address 10.28.100.7\0} \
                  6 {PPP session is iDex1 12346\0}"
set AckMatchList(3.1.1.1) ""

#ʵ���Ų���ȷ
set MsgList(3.2.1.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_PC_PROC_OUTPUT} \
{{pppd\0} 9   1 {1 {Connect: ppp0\0}}}"
set AckMatchList(3.2.1.1) ""

#ģ��������ȷ
set MsgList(3.2.2.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_PC_PROC_OUTPUT} \
{{pppdd\0} 1   1 {1 {Connect: ppp0\0}}}"
set AckMatchList(3.2.2.1) ""

#����Ϊ��
set MsgList(3.2.6.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_PC_PROC_OUTPUT} \
{{pppd\0} 1  1 {1 \0}}"
set AckMatchList(3.2.6.1) ""

}


if {$TestCaseNo == 4} {

#����5��ʵ��
set MsgList(4.1.1.1) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(4.1.1.1) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"

set MsgList(4.1.2.1) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(4.1.2.1) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"

set MsgList(4.1.3.1) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(4.1.3.1) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"

set MsgList(4.1.4.1) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(4.1.4.1) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"

set MsgList(4.1.5.1) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(4.1.5.1) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"


#�������̲���
#up����
set MsgList(4.2.1.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Name=eth1\0Status=Up\0}}"
set AckMatchList(4.2.1.1) ""
#down����
set MsgList(4.2.2.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Name=eth1\0Status=Down\0}}"
set AckMatchList(4.2.2.1) ""

#�쳣���̲���,����Ϸ��Լ��
#��������ʵ����Ŀ
set MsgList(4.3.1.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Name=eth1\0Status=Up\0}}"
set AckMatchList(4.3.1.1) ""

#����С��ʵ����Ŀ
set MsgList(4.3.2.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{2 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Name=eth1\0Status=Up\0}}"
set AckMatchList(4.3.2.1) ""

#û��Path�ֶ�
set MsgList(4.3.3.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{2 {Name=eth0\0Status=Up\0}}"
set AckMatchList(4.3.3.1) ""

#�ظ�Path�ֶ�
set MsgList(4.3.4.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Name=eth1\0Status=Up\0}}"
set AckMatchList(4.3.4.1) ""

#Path�ֶ�Ϊ��
set MsgList(4.3.5.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{3 {Path=\0Name=eth0\0Status=Up\0}}"
set AckMatchList(4.3.5.1) ""

#�����Path�ֶ�0
set MsgList(4.3.6.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{3 {Path=xxx.yyy.\0Name=eth0\0Status=Up\0}}"
set AckMatchList(4.3.6.1) ""

#Path�ֶδ���5
set MsgList(4.3.7.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1. \0Name=eth1\0Status=Up\0}}"
set AckMatchList(4.3.7.1) ""

#û��name�ֶ�
set MsgList(4.3.8.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{2 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Status=Up\0}}"
set AckMatchList(4.3.8.1) ""

#�ظ�name�ֶ�
set MsgList(4.3.9.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Name=eth0\0Name=eth1\0Status=Up\0}}"
set AckMatchList(4.3.9.1) ""

#0Name�ֶ�Ϊ��
set MsgList(4.3.10.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Name=\0Status=Up\0}}"
set AckMatchList(4.3.10.1) ""

#Path�ֶδ���4
set MsgList(4.3.11.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.Username\0Name=eth1\0Status=Up\0}}"
set AckMatchList(4.3.11.1) ""

#û��Status�ֶ�
set MsgList(4.3.12.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{2 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Name=eth1\0}}"
set AckMatchList(4.3.12.1) ""

#�ظ�Status�ֶ�
set MsgList(4.3.13.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Name=eth1\0Status=Up\0Status=Down\0}}"
set AckMatchList(4.3.13.1) ""

#Status�ֶ�Ϊ��
set MsgList(4.3.14.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Name=eth1\0Status=\0}}"
set AckMatchList(4.3.14.1) ""

#Status�ֶδ���
set MsgList(4.3.15.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Name=eth1\0Status=CONN\0}}"
set AckMatchList(4.3.15.1) ""

#Path�ֶδ���1
set MsgList(4.3.16.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1\0Name=eth1\0Status=Up\0}}"
set AckMatchList(4.3.16.1) ""

#Path�ֶδ���2
set MsgList(4.3.17.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice\0Name=eth1\0Status=Up\0}}"
set AckMatchList(4.3.17.1) ""

#Path�ֶδ���3
set MsgList(4.3.18.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.99.\0Name=eth1\0Status=Up\0}}"
set AckMatchList(4.3.18.1) ""

}

#2. ɾ��ʵ������  MSG_CMM_DEL_NODE
if {$TestCaseNo == 5} {

#����3��ʵ��
set MsgList(5.1.1) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(5.1.1) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"

set MsgList(5.1.2) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(5.1.2) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"

set MsgList(5.1.3) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(5.1.3) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"


#�쳣���

#����·��Ϊ��
set MsgList(5.2.1) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{\0}}"
set AckMatchList(5.2.1) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

#����·������
set MsgList(5.2.2) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.xxx.\0}}"
set AckMatchList(5.2.2) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

#����·���ж��ಿ��
set MsgList(5.2.3) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Username.xxxx\0}}"
set AckMatchList(5.2.3) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

#����·��δ��.����1
set MsgList(5.2.4) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1\0}}"
set AckMatchList(5.2.4) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

#����·��δ��.����2
set MsgList(5.2.5) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1\0}}"
set AckMatchList(5.2.5) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

#����·��������1
set MsgList(5.2.6) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.99.\0}}"
set AckMatchList(5.2.6) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

#����·��������2
set MsgList(5.2.7) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.99.\0}}"
set AckMatchList(5.2.7) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

#�������
#ɾ��һ��ʵ��
set MsgList(5.3.1) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0}}"
set AckMatchList(5.3.1) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{0 0}"

#ɾ������ʵ��
set MsgList(5.3.2) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0}}"
set AckMatchList(5.3.2) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{0 0}"

}









