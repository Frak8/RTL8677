
#Static Routing ���ɲ�������

#����ʵ��, û���쳣���
if {$TestCaseNo == 1} {

#��������
set MsgList(1.1.1) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000001 $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.Layer3Forwarding.Forwarding.\0}}" 
set AckMatchList(1.1.1) ""

}

#����WAN ���Ӳ���
if {$TestCaseNo == 2} {

set MsgList(2.1.1) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_WAN_ADD_INST} 
{1 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0}}" 
set AckMatchList(2.1.1) ""

set MsgList(2.1.2) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_WAN_ADD_INST} 
{1 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.2.\0}}" 
set AckMatchList(2.1.2) ""

set MsgList(2.1.3) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_WAN_ADD_INST} 
{1 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.3.\0}}" 
set AckMatchList(2.1.3) ""
}

#����WAN����
if {$TestCaseNo == 3} {

set MsgList(3.1.1) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_WAN_CONN_EST} 
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0}
   {InterfaceName=ppp0\0}
   {RemoteIPAddress=172.16.2.10\0}}" 
set AckMatchList(3.1.1) ""

set MsgList(3.1.2) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_WAN_CONN_EST} 
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.2.\0}
   {InterfaceName=ppp1\0}
   {RemoteIPAddress=172.16.2.10\0}}" 
set AckMatchList(3.1.2) ""

set MsgList(3.1.3) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_WAN_CONN_EST} 
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.3.\0}
   {InterfaceName=ppp2\0}
   {RemoteIPAddress=172.16.2.10\0}}" 
set AckMatchList(3.1.3) ""
}

#SET��Ϣ �� commit��Ϣ
if {$TestCaseNo == 4} {

#��������
#1. ����3��ʵ����3��pppoe����ʵ��
set MsgList(4.1.1) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000001 $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.Layer3Forwarding.Forwarding.\0}}" 
set AckMatchList(4.1.1) ""

set MsgList(4.1.2) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000001 $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.Layer3Forwarding.Forwarding.\0}}" 
set AckMatchList(4.1.2) ""

set MsgList(4.1.3) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000001 $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.Layer3Forwarding.Forwarding.\0}}" 
set AckMatchList(4.1.3) ""

set MsgList(4.1.4) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(4.1.4) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"

set MsgList(4.1.5) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(4.1.5) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"

set MsgList(4.1.6) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(4.1.6) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"


#2.������������
set MsgList(4.2.1) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   7 
    {InternetGatewayDevice.Layer3Forwarding.DefaultConnectionService=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.Enable=0\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.DestIPAddress=192.168.2.1\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.DestSubnetMask=255.255.255.0\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.GatewayIPAddress=172.16.2.1\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.Interface=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.2.\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.ForwardingMetric=100\0}
}"
set AckMatchList(4.2.1) ""


#3.���ܲ���
#����ʵ��0 1��Ч, 2��Ч
set MsgList(4.3.1) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   6 
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.Enable=0\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.DestIPAddress=192.168.2.1\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.DestSubnetMask=255.255.255.0\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.GatewayIPAddress=172.16.2.1\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.Interface=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.ForwardingMetric=100\0}
}"
set AckMatchList(4.3.1) ""

set MsgList(4.3.2) "{$MID_CMM $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(4.3.2) ""
    
set MsgList(4.3.3) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   6 
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.2.Enable=0\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.2.DestIPAddress=192.168.2.30\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.2.DestSubnetMask=255.255.255.0\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.2.GatewayIPAddress=\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.2.Interface=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.2.ForwardingMetric=100\0}
}"
set AckMatchList(4.3.3) ""

set MsgList(4.3.4) "{$MID_CMM $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(4.3.4) ""

#ʵ��1�󶨽ӿڸı�
set MsgList(4.3.5) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.2.Interface=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.2.\0}
}"
set AckMatchList(4.3.5) ""

set MsgList(4.3.6) "{$MID_CMM $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(4.3.6) ""

#����Чʵ��2����enable = 1
set MsgList(4.3.7) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1 
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.3.Enable=1\0}
}"
set AckMatchList(4.3.7) ""

set MsgList(4.3.8) "{$MID_CMM $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(4.3.8) ""

#��ʵ��0����enable=1
set MsgList(4.3.9) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1 
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.Enable=1\0}
}"
set AckMatchList(4.3.9) ""

set MsgList(4.3.10) "{$MID_CMM $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(4.3.10) ""

#��ʵ��0���þֲ�enable=0
set MsgList(4.3.11) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.Enable=0\0}
}"
set AckMatchList(4.3.11) ""

set MsgList(4.3.12) "{$MID_CMM $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(4.3.12) ""

#����Ĭ��·��Ϊpppoe0
set MsgList(4.3.13) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.DefaultConnectionService=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0}
}"
set AckMatchList(4.3.13) ""

set MsgList(4.3.14) "{$MID_CMM $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(4.3.14) ""

#����Ĭ��·��Ϊpppoe1
set MsgList(4.3.15) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.DefaultConnectionService=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.2.\0}
}"
set AckMatchList(4.3.15) ""

set MsgList(4.3.16) "{$MID_CMM $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(4.3.16) ""

#����Ĭ��·��Ϊ��
set MsgList(4.3.17) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.DefaultConnectionService=\0}
}"
set AckMatchList(4.3.17) ""

set MsgList(4.3.18) "{$MID_CMM $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(4.3.18) ""


#4.�쳣����
#enable��������
set MsgList(4.4.1) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.Enable=2\0}
}"
set AckMatchList(4.4.1) ""

#enable����Ϊ��
set MsgList(4.4.2) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.Enable=\0}
}"
set AckMatchList(4.4.2) ""

#dest IP��������1
set MsgList(4.4.3) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.DestIPAddress=172.16.3.\0}
}"
set AckMatchList(4.4.3) ""

#dest IP��������2
set MsgList(4.4.4) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.DestIPAddress=256.16.3.3\0}
}"
set AckMatchList(4.4.4) ""

#dest IP��������3
set MsgList(4.4.5) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.DestIPAddress=172.16.3.1.\0}
}"
set AckMatchList(4.4.5) ""

#dest IP��������4
set MsgList(4.4.6) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.DestIPAddress=172.16.3.2a\0}
}"
set AckMatchList(4.4.6) ""

#dest IP��������5
set MsgList(4.4.7) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.DestIPAddress=1111.16.3.\0}
}"
set AckMatchList(4.4.7) ""

#dest IP��������6
set MsgList(4.4.8) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.DestIPAddress=172..16.3.2\0}
}"
set AckMatchList(4.4.8) ""

#dest IP��������7
set MsgList(4.4.9) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.DestIPAddress=172..16.3\0}
}"
set AckMatchList(4.4.9) ""

#dest IP����Ϊ��
set MsgList(4.4.10) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.DestIPAddress=\0}
}"
set AckMatchList(4.4.10) ""


#����Ľӿ�·��, Ϊ��
set MsgList(4.4.11) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.DefaultConnectionService=\0}
}"
set AckMatchList(4.4.11) ""

#����Ľӿ�·��, û�д�·��
set MsgList(4.4.12) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.DefaultConnectionService=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.5.\0}
}"
set AckMatchList(4.4.12) ""

#ForwardingMetric����1
set MsgList(4.4.13) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.2.ForwardingMetric=-1\0}
}"
set AckMatchList(4.4.13) ""

#ForwardingMetric����2
set MsgList(4.4.14) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.2.ForwardingMetric=1000\0}
}"
set AckMatchList(4.4.14) ""

#ForwardingMetric����, Ϊ��
set MsgList(4.4.15) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.2.ForwardingMetric=\0}
}"
set AckMatchList(4.4.15) ""


}






#����LAN ���Ӳ���
if {$TestCaseNo == 5} {

set MsgList(5.1.1) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_LAN_ADD_INST} \
{1 {Path=InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.\0}}"
set AckMatchList(5.1.1) ""

set MsgList(5.1.2) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_LAN_ADD_INST} \
{1 {Path=InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.2.\0}}" 
set AckMatchList(5.1.2) ""

set MsgList(5.1.3) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_LAN_ADD_INST} \
{1 {Path=InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.3.\0}}"
set AckMatchList(5.1.3) ""

}



#�Ͽ�WAN����
if {$TestCaseNo == 6} {

set MsgList(6.1.1) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_WAN_CONN_FIN} 
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0}
   {InterfaceName=\0}
   {RemoteIPAddress=\0}}" 
set AckMatchList(6.1.1) ""

set MsgList(6.1.2) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_WAN_CONN_FIN} 
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.2.\0}
   {InterfaceName=\0}
   {RemoteIPAddress=\0}}" 
set AckMatchList(6.1.2) ""

set MsgList(6.1.3) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_WAN_CONN_FIN} 
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.3.\0}
   {InterfaceName=\0}
   {RemoteIPAddress=\0}}" 
set AckMatchList(6.1.3) ""
}


#ɾ��WAN����
if {$TestCaseNo == 7} {

set MsgList(7.1.1) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_WAN_DEL_INST} 
{1 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0}}" 
set AckMatchList(7.1.1) ""

set MsgList(7.1.2) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_WAN_DEL_INST} 
{1 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.2.\0}}" 
set AckMatchList(7.1.2) ""

set MsgList(7.1.3) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_WAN_DEL_INST} 
{1 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.3.\0}}" 
set AckMatchList(7.1.3) ""

}

#ɾ��LAN ���Ӳ���
if {$TestCaseNo == 8} {

set MsgList(8.1.1) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_LAN_DEL_INST} 
{2 {Path=InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.\0}
   {InterfaceName=eth0\0}}" 
set AckMatchList(8.1.1) ""

set MsgList(8.1.2) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_LAN_DEL_INST} 
{1 {Path=InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.2.\0}
   {InterfaceName=eth2\0}}" 
set AckMatchList(8.1.2) ""

set MsgList(8.1.3) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_LAN_DEL_INST} 
{1 {Path=InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.3.\0}
   {InterfaceName=eth3\0}}" 
set AckMatchList(8.1.3) ""

}


#ɾ��ʵ������
if {$TestCaseNo == 9} {

#��������
set MsgList(9.1.1) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000001 $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.Layer3Forwarding.Forwarding.1.\0}}" 
set AckMatchList(9.1.1) ""

set MsgList(9.1.2) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000001 $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.Layer3Forwarding.Forwarding.2.\0}}" 
set AckMatchList(9.1.2) ""

set MsgList(9.1.3) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000001 $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.Layer3Forwarding.Forwarding.3.\0}}" 
set AckMatchList(9.1.3) ""

}


