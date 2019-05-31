
#1. ����ʵ������  MSG_CMM_ADD_NODE

#1.1 �쳣����
if {$TestCaseNo == 1} {
#����·��Ϊ��
set MsgList(1.1.1) "{$MID_CMM  $MID_WAN  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{}"
set AckMatchList(1.1.1) "{{^$MID_WAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_ADD_NODE_ACK$}} \
{{^67371524$} {^67371524$} {^1$} {^ =67371524.*\0$}}"

#����·������
set MsgList(1.1.2) "{$MID_CMM  $MID_WAN  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevi.\0}}"
set AckMatchList(1.1.2) "{$MID_WAN  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^67371524$} {^67371524$} {^1$} {^InternetGatewayDevice.WANDevice.1.WANConnectionDevi.=67371524.*\0$}}"

#����·���ж��ಿ��
set MsgList(1.1.3) "{$MID_CMM  $MID_WAN  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.DevName.xxxx\0}}"
set AckMatchList(1.1.3) "{$MID_WAN  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^67371524$} {^67371524$} {^1$} {^InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.DevName.xxxx=67371524.*\0$}}"

#����·��δ��.����
set MsgList(1.1.4) "{$MID_CMM  $MID_WAN  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice\0}}"
set AckMatchList(1.1.4) "{$MID_WAN  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^67371524$} {^67371524$} {^1$} {^InternetGatewayDevice.WANDevice.1.WANConnectionDevice=67371524.*\0$}}"

#����·�������Լ�ģ��
set MsgList(1.1.5) "{$MID_CMM  $MID_WAN  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.\0}}"
set AckMatchList(1.1.5) "{$MID_WAN  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^67371524$} {^67371524$} {^1$} {^InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.=67371524.*\0$}}"

#����·����������������
set MsgList(1.1.6) "{$MID_CMM  $MID_WAN  0x0b4a6f9c  $MSG_CMM_ADD_NODE}\
{{InternetGatewayDevice.WANDevice.222.WANConnectionDevice.\0}}"
set AckMatchList(1.1.6) "{$MID_WAN  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^67371524$} {^67371524$} {^1$} {^InternetGatewayDevice.WANDevice.222.WANConnectionDevice.=67371524.*\0$}}"

#�������
set MsgList(1.2.1) "{$MID_WEB  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.\0}}"
set AckMatchList(1.2.1) "{$MID_CMM  $MID_WEB  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{0 2}"

set MsgList(1.2.2) "{$MID_CMM  $MID_WAN  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.\0}}"
set AckMatchList(1.2.2) "{$MID_WAN  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"

}

#2. ɾ��ʵ������  MSG_CMM_DEL_NODE
if {$TestCaseNo == 2} {

#�쳣���

#����·��Ϊ��
set MsgList(2.1.1) "{$MID_CMM  $MID_WAN  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{\0}}"
set AckMatchList(2.1.1) "{$MID_WAN  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

#����·������
set MsgList(2.1.2) "{$MID_CMM  $MID_WAN  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.xxxxx.\0}}"
set AckMatchList(2.1.2) "{$MID_WAN  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

#����·���ж��ಿ��
set MsgList(2.1.3) "{$MID_CMM  $MID_WAN  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.DevName.xxxx\0}}"
set AckMatchList(2.1.3) "{$MID_WAN  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

#����·��δ��.����
set MsgList(2.1.4) "{$MID_CMM  $MID_WAN  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1\0}}"
set AckMatchList(2.1.4) "{$MID_WAN  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

#����·�������Լ�ģ��
set MsgList(2.1.5) "{$MID_CMM  $MID_WAN  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.\0}}"
set AckMatchList(2.1.5) "{$MID_WAN  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

#�������
set MsgList(2.2.1) "{$MID_WEB  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.31.\0}}"
set AckMatchList(2.2.1) "{$MID_CMM  $MID_WEB  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{0 0}"

}

#8. �������ӱ仯֪ͨ���� MSG_MON_INTF_STATUS_INFORM
if {$TestCaseNo == 3} {

#�쳣����
#����MON��WANģ�鷢��MSG_MON_INTF_STATUS_INFORM��Ϣ��֪ͨ�����Ϊ0�����
set MsgList(3.1.1) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{0 {}}"
set AckMatchList(3.1.1) ""

#����MON��WANģ�鷢��MSG_MON_INTF_STATUS_INFORM��Ϣ��֪ͨ�����С��ʵ�ʵ�֪ͨ����������
set MsgList(3.1.2) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{1 {Path=InternetGatewayDevice.WANDevice.1.\0Name=eth1\0Status=Connected\0}}"
set AckMatchList(3.1.2) ""

#����MON��WANģ�鷢��MSG_MON_INTF_STATUS_INFORM��Ϣ��֪ͨ���������ʵ�ʵ�֪ͨ����������
set MsgList(3.1.3) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{4 {Path=InternetGatewayDevice.WANDevice.1.\0Name=eth1\0Status=Connected\0}}"
set AckMatchList(3.1.3) ""

#����MON��WANģ�鷢��MSG_MON_INTF_STATUS_INFORM��Ϣ��֪ͨ��ֵ���ܱ�ʶ������
set MsgList(3.1.4) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{3 {Path=InternetGatewayDevice.WANDevice.1.\0Name=eth1\0Status=Down\0}}"
set AckMatchList(3.1.4) ""

#����MON��WANģ�鷢��MSG_MON_INTF_STATUS_INFORM��Ϣ��Path֪ͨ����ڵ����
set MsgList(3.1.5) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{2 {Name=eth1\0Status=Connected\0}}"
set AckMatchList(3.1.5) ""

#����MON��WANģ�鷢��MSG_MON_INTF_STATUS_INFORM��Ϣ��Path֪ͨ�����1�������
set MsgList(3.1.6) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{4 {Path=InternetGatewayDevice.WANDevice.1.\0Path=InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.\0\
Name=eth1\0Status=Connected\0}}"
set AckMatchList(3.1.6) ""

#����MON��WANģ�鷢��MSG_MON_INTF_STATUS_INFORM��Ϣ��Path֪ͨ��ֵΪ�յ����
set MsgList(3.1.7) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{3 {Path=\0Name=eth1\0Status=Connected\0}}"
set AckMatchList(3.1.7) ""

#����MON��WANģ�鷢��MSG_MON_INTF_STATUS_INFORM��Ϣ��Path֪ͨ��ֵ���������
set MsgList(3.1.8) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{3 {Path=InternetGatewayDevice.WANDevice.1.[string repeat "Enabled" 100].\0Name=eth1\0Status=Connected\0}}"
set AckMatchList(3.1.8) ""

#����MON��WANģ�鷢��MSG_MON_INTF_STATUS_INFORM��Ϣ��Path֪ͨ��ֵ��������
set MsgList(3.1.9) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANDevice.1.\0Name=eth1\0Status=Connected\0}}"
set AckMatchList(3.1.9) ""

#����MON��WANģ�鷢��MSG_MON_INTF_STATUS_INFORM��Ϣ��Path֪ͨ��ֵδ��"."���������
set MsgList(3.1.10) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{3 {Path=InternetGatewayDevice.WANDevice.1\0Name=eth1\0Status=Connected\0}}"
set AckMatchList(3.1.10) ""

#����MON��WANģ�鷢��MSG_MON_INTF_STATUS_INFORM��Ϣ��Name֪ͨ����ڵ����
set MsgList(3.1.11) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{2 {Path=InternetGatewayDevice.WANDevice.1.\0Status=Connected\0}}"
set AckMatchList(3.1.11) ""

#����MON��WANģ�鷢��MSG_MON_INTF_STATUS_INFORM��Ϣ��Name֪ͨ�����һ�������
set MsgList(3.1.12) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{4 {Path=InternetGatewayDevice.WANDevice.1.\0Name=eth0\0Name=eth1\0Status=Connected\0}}"
set AckMatchList(3.1.12) ""

#zhujianwen 13
#����MON��WANģ�鷢��MSG_MON_INTF_STATUS_INFORM��Ϣ��Name֪ͨ��ֵΪ�յ����
set MsgList(3.1.13) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{3 {Path=InternetGatewayDevice.WANDevice.1.\0Name=\0Status=Connected\0}}"
set AckMatchList(3.1.13) ""

#����MON��WANģ�鷢��MSG_MON_INTF_STATUS_INFORM��Ϣ��Name֪ͨ��ֵ�Ƿ������
#set MsgList(3.1.14) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
#{3 {Path=InternetGatewayDevice.WANDevice.1.\0Name=fth0\0Status=Connected\0}}"
#set AckMatchList(3.1.14) ""

#����MON��WANģ�鷢��MSG_MON_INTF_STATUS_INFORM��Ϣ��STATUS֪ͨ����ڵ����
set MsgList(3.1.15) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{2 {Path=InternetGatewayDevice.WANDevice.1.\0Name=eth1\0}}"
set AckMatchList(3.1.15) ""

#����MON��WANģ�鷢��MSG_MON_INTF_STATUS_INFORM��Ϣ��STATUS֪ͨ�����һ�������
set MsgList(3.1.16) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{4 {Path=InternetGatewayDevice.WANDevice.1.\0Name=eth1\0Status=Connected\0Status=Disconnected\0}}"
set AckMatchList(3.1.16) ""

#����MON��WANģ�鷢��MSG_MON_INTF_STATUS_INFORM��Ϣ��STATUS֪ͨ��ֵΪ�յ����
set MsgList(3.1.17) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{3 {Path=InternetGatewayDevice.WANDevice.1.\0Name=eth1\0Status=\0}}"
set AckMatchList(3.1.17) ""

#����MON��WANģ�鷢��MSG_MON_INTF_STATUS_INFORM��Ϣ��STATUS֪ͨ��ֵ�Ƿ������
set MsgList(3.1.18) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{3 {Path=InternetGatewayDevice.WANDevice.1.\0Name=eth1\0Status=EST\0}}"
set AckMatchList(3.1.18) ""

#�������
#����MON��WANģ�鷢��MSG_MON_INTF_STATUS_INFORM��Ϣ��Connected�����
set MsgList(3.2.1) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{3 {Path=InternetGatewayDevice.WANDevice.1.\0Name=eth1\0Status=Connected\0}}"
set AckMatchList(3.2.1) ""

#����MON��WANģ�鷢��MSG_MON_INTF_STATUS_INFORM��Ϣ��Disconnected�����
set MsgList(3.2.2) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{3 {Path=InternetGatewayDevice.WANDevice.1.\0Name=eth1\0Status=Disconnected\0}}"
set AckMatchList(3.2.2) ""

#����MON��WANģ�鷢��MSG_MON_INTF_STATUS_INFORM��Ϣ��Unknow�����
set MsgList(3.2.3) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{3 {Path=InternetGatewayDevice.WANDevice.1.\0Name=eth1\0Status=UNKNOW\0}}"
set AckMatchList(3.2.3) ""

}

#9. �½�����֪ͨ MSG_WAN_CONN_EST
if {$TestCaseNo == 4} {

#����Ϸ��Լ��
#����IPCONN��WANģ�鷢��MSG_WAN_CONN_EST��Ϣ��֪ͨ���������ʵ�����������
set MsgList(4.1.1) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1\0ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(4.1.1) ""

#����IPCONN��WANģ�鷢��MSG_WAN_CONN_EST��Ϣ��֪ͨ���������ʵ�����������
set MsgList(4.1.2) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{2 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1\0ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(4.1.2) ""

#����IPCONN��WANģ�鷢��MSG_WAN_CONN_EST��Ϣ����Ϣ��������ʶ���֪ͨ������
set MsgList(4.1.3) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{1 {Nat=Enable\0}}"
set AckMatchList(4.1.3) ""

#����IPCONN��WANģ�鷢��MSG_WAN_CONN_EST��Ϣ��Path֪ͨ����ڵ����
set MsgList(4.1.4) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{2 {ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(4.1.4) ""

#����IPCONN��WANģ�鷢��MSG_WAN_CONN_EST��Ϣ��Path֪ͨ��ֵΪ�յ����
set MsgList(4.1.5) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{3 {Path=\0ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(4.1.5) ""

#����IPCONN��WANģ�鷢��MSG_WAN_CONN_EST��Ϣ��Path֪ͨ��ֵ�Ƿ������
set MsgList(4.1.6) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{3 {Path=ITU.CPE.IPCONN.1\0ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(4.1.6) ""

#�������
#����IPCONN��WANģ�鷢��MSG_WAN_CONN_EST��Ϣ
set MsgList(4.2.1) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1\0ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(4.2.1) ""

#����PPPOE��WANģ�鷢��MSG_WAN_CONN_EST��Ϣ
set MsgList(4.2.2) "{$MID_PPPOE  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1\0ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(4.2.2) ""
}

#10. �½�����֪ͨ MSG_WAN_CONN_FIN
if {$TestCaseNo == 5} {

#����Ϸ��Լ��
#����IPCONN��WANģ�鷢��MSG_WAN_CONN_FIN��Ϣ��֪ͨ���������ʵ�����������
set MsgList(5.1.1) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1\0ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(5.1.1) ""

#����IPCONN��WANģ�鷢��MSG_WAN_CONN_FIN��Ϣ��֪ͨ���������ʵ�����������
set MsgList(5.1.2) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{2 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1\0ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(5.1.2) ""

#����IPCONN��WANģ�鷢��MSG_WAN_CONN_FIN��Ϣ����Ϣ��������ʶ���֪ͨ������
set MsgList(5.1.3) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{1 {Nat=Enable\0}}"
set AckMatchList(5.1.3) ""

#����IPCONN��WANģ�鷢��MSG_WAN_CONN_FIN��Ϣ��Path֪ͨ����ڵ����
set MsgList(5.1.4) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{2 {ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(5.1.4) ""

#����IPCONN��WANģ�鷢��MSG_WAN_CONN_FIN��Ϣ��Path֪ͨ��ֵΪ�յ����
set MsgList(5.1.5) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{3 {Path=\0ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(5.1.5) ""

#����IPCONN��WANģ�鷢��MSG_WAN_CONN_FIN��Ϣ��Path֪ͨ��ֵ�Ƿ������
set MsgList(5.1.6) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{3 {Path=ITU.CPE.IPCONN.1\0ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(5.1.6) ""

#�������
#����IPCONN��WANģ�鷢��MSG_WAN_CONN_EST��Ϣ
set MsgList(5.2.1) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1\0ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(5.2.1) ""

#����PPPOE��WANģ�鷢��MSG_WAN_CONN_EST��Ϣ
set MsgList(5.2.2) "{$MID_PPPOE  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1\0ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(5.2.2) ""

#����IPCONN��WANģ�鷢��MSG_WAN_CONN_FIN��Ϣ
set MsgList(5.2.3) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1\0ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(5.2.3) ""

#����PPPOE��WANģ�鷢��MSG_WAN_CONN_FIN��Ϣ
set MsgList(5.2.4) "{$MID_PPPOE  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1\0ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(5.2.4) ""

}

#11. �½�����֪ͨ MSG_WAN_CONN_ENABLE_UPDATE
if {$TestCaseNo == 6} {

#֪ͨ���������ʵ�����������
set MsgList(6.1.1) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_ENABLE_UPDATE} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Enable=1\0}}"
set AckMatchList(6.1.1) ""

#֪ͨ�����С��ʵ�����������
set MsgList(6.1.2) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_ENABLE_UPDATE} \
{1 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Enable=1\0}}"
set AckMatchList(6.1.2) ""

#֪ͨ�������������
set MsgList(6.1.3) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_ENABLE_UPDATE} \
{1 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0}}"
set AckMatchList(6.1.3) ""

#·��Ϊ�յ����
set MsgList(6.1.4) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_ENABLE_UPDATE} \
{2 {Path=\0Enable=1\0}}"
set AckMatchList(6.1.4) ""

#·���Ƿ�
set MsgList(6.1.5) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_ENABLE_UPDATE} \
{2 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.fas.fasf\0Enable=1\0}}"
set AckMatchList(6.1.5) ""

#·����������
set MsgList(6.1.6) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_ENABLE_UPDATE} \
{2 {Path=InternetGatewayDevice.WANDevice.1.\0Enable=1\0}}"
set AckMatchList(6.1.6) ""

#û��path�����
set MsgList(6.1.7) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_ENABLE_UPDATE} \
{1 {Enable=1\0}}"
set AckMatchList(6.1.7) ""

#enable����
set MsgList(6.1.8) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_ENABLE_UPDATE} \
{2 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Enable=fasdf\0}}"
set AckMatchList(6.1.8) ""

#û�еȺŵ����
set MsgList(6.1.9) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_ENABLE_UPDATE} \
{3 {PathInternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Enable=1\0Status=Conneted\0}}"
set AckMatchList(6.1.9) ""

#�������
set MsgList(6.2.1) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_ENABLE_UPDATE} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Enable=1\0Status=Conneted\0}}"
set AckMatchList(6.2.1) ""

}


