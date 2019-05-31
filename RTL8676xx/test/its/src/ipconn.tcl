#
# ����IPCONN����
#


#����ȫ�ֱ��� 
set WAN_PATH "InternetGatewayDevice.WANDevice"
set WAN_INST "InternetGatewayDevice.WANDevice.1"

set WANCONN_PATH "InternetGatewayDevice.WANDevice.1.WANConnectionDevice"
set WANCONN_INST "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1"

set IPCONN_PATH "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection"
set IPCONN_INST "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1"

set NAME        "Name"
set ENABLE      "Enable"
set ADDRTYPE    "AddressingType"
set EXTIPADDR   "ExternalIPAddress"
set MASK        "SubnetMask"
set GATEWAY     "DefaultGateway"

set DeviceName  "X_TWSZ-COM_DeviceName"
set DNSSRV      "DNSServers"
set CONNSTATUS  "ConnectionStatus"
set LASTERROR   "LastConnectionError"

set INT        int
set UINT       unsignedInt
set STR        string
set BOOL       boolean

set i 1
set j 1

#
# ���Ա��˵��:
#   1.������нڵ�,��ϵͳ������������ִ��,��ֻ������1��,��ΪҪƥ�����ʵ����ʵ����
#   2.���û������ȼ��Ķ���
#   3.���û����������ƵĶ���
#   4.���û���IP��Ԫ��ķ������
#   5.���û���DSCP�ķ������
#   6.����������
#   7.ɾ����������a,������Ա���������,��ΪҪƥ��ʵ����
#
#   ���������ı��δ���ĵ������ֹ���ţ����²��Ե���ʱ���Ѳ��ҳ�����������������޸�һ�¡�

################################################################################



#===============================================================================
#             ����ADD��Ϣ
#===============================================================================
if {$TestCaseNo == 1} {
set i $TestCaseNo

#######����Ϸ��Լ��#############
set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_ADD_NODE��Ϣ���Ƿ�·�������(·��Ϊ��,·��������,����·���ȵ�)"
set comment($i.$j) "#·��Ϊ��"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_ADD_NODE \
    {\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_ADD_NODE_ACK 
    {.*} {.*} {^1$} {.*9005\0$}
"
incr j
set comment($i.$j) "#·��������"
set MsgList($i.$j) "{$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_ADD_NODE} \
    {$IPCONN_PATH.xxxx\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_ADD_NODE_ACK 
    {.*} {.*} {^1$} {.*9005\0$}
"
incr j
set comment($i.$j) "#����·��"
set MsgList($i.$j) "{$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_ADD_NODE} \
    {$IPCONN_PATH=xxxxxx\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_ADD_NODE_ACK 
    {.*} {.*} {^1$} {.*9005\0$}
"
incr j

#######�������#############
set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_ADD_NODE��Ϣ������·�������"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_ADD_NODE 
    {$IPCONN_PATH.\0}
"
set AckMatchList($i.$j)  "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_ADD_NODE_ACK 
    ^0$ ^1$ ^0$
"
incr j

set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_ADD_NODE��Ϣ���ٴ��������,ֻ����һ��"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_ADD_NODE 
    {$IPCONN_PATH.\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_ADD_NODE_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j


}


#===============================================================================
#             ����SET��Ϣ
#===============================================================================
if {$TestCaseNo == 2} {
set i $TestCaseNo

#######����Ϸ��Լ��#############

#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ,·���쳣����ģ���Ѿ����,���ټ��
#set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��·����ȫ����Ҷ�ӽڵ㣩�����"
#set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001 $MSG_CMM_SET_VAL 
#    1 
#    {$IPCONN_PATH.1.=1\0}
#"
#set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
#    {.*} {.*} {^1$} {.*9005\0$}
#"
#incr j
#
#set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��ʵ���Ų����ڵ����"
#set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001 $MSG_CMM_SET_VAL 
#    1 
#    {$IPCONN_PATH.2.$ADDRTYPE=DHCP\0}
#"
#set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
#    {.*} {.*} {^1$} {.*9005\0$}
#"
#incr j
#return

#set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��NATEnabledȡֵ�Ƿ�(���1)�����"
#set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
#    1 
#    {$IPCONN_INST.NATEnabled=On\0}
#"
#set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
#    {.*} {.*} {^1$} {.*9005\0$}
#"
#incr j
#
#set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��NATEnabledȡֵ�Ƿ�(���2)�����"
#set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
#1 
#{$IPCONN_INST.NATEnabled=1024\0}
#"
#set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
#    {.*} {.*} {^1$} {.*9005\0$}
#"
#incr j


set comment($i.$j) "����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ������ֵΪ�յ����"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1 
    {$IPCONN_INST.AddressingType=\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��AddressingTypeȡֵ�Ƿ������"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
1 
{$IPCONN_INST.AddressingType=Undefined\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��Nameȡֵ����(����256�ֽ�)�����"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1 
    {$IPCONN_INST.Name=[string repeat "ipconn1" 100]\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��AddressingTypeȡֵ�Ƿ������"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.AddressingType=Auto\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��ExternalIPAddressȡֵ�Ƿ�(��һ���ֽ�)�����"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.ExternalIPAddress=10.10.10.10.10\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��ExternalIPAddressȡֵ�Ƿ�(��һ���ֽ�)�����"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.ExternalIPAddress=10.10.10\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��ExternalIPAddressȡֵ�Ƿ�(�м���һ���ֽ�)�����"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.ExternalIPAddress=10.10..10\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��ExternalIPAddressȡֵ�Ƿ�(��һ��.)�����"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.ExternalIPAddress=10.10.10.10.\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��ExternalIPAddressȡֵ�Ƿ�(����255)�����"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.ExternalIPAddress=10.10.256.10\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��SubnetMaskȡֵ�Ƿ�(��һ���ֽ�)�����"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.SubnetMask=255.255.255.0.0\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��SubnetMaskȡֵ�Ƿ�(��һ���ֽ�)�����"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.SubnetMask=255.255.0\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��SubnetMaskȡֵ�Ƿ�(�м���һ���ֽ�)�����"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.SubnetMask=255.255..0\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��SubnetMaskȡֵ�Ƿ�(��һ��.)�����"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.SubnetMask=255.255.254.0.\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��SubnetMaskȡֵ�Ƿ�(����255)�����"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.SubnetMask=255.255.256.0\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��DefaultGatewayȡֵ�Ƿ�(��һ���ֽ�)�����"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.DefaultGateway=10.10.10.10.10\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��DefaultGatewayȡֵ�Ƿ�(��һ���ֽ�)�����"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.DefaultGateway=10.10.10\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��DefaultGatewayȡֵ�Ƿ�(�м���һ���ֽ�)�����"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.DefaultGateway=10.10..10\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��DefaultGatewayȡֵ�Ƿ�(��һ��.)�����"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.DefaultGateway=10.10.10.10.\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��DefaultGatewayȡֵ�Ƿ�(����255)�����"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.DefaultGateway=10.10.256.10\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

#set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��DNSServersȡֵ�Ƿ�(����64�ַ�)�����"
#set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
#1 
#{$IPCONN_INST.DNSServers=[string repeat "10.10.10.10" 50]\0}
#"
#set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
#    {.*} {.*} {^1$} {.*9007\0$}
#"
#incr j
#
#set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��DNSServersȡֵ�Ƿ�(��һ���ֽ�)�����"
#set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
#    1
#    {$IPCONN_INST.DNSServers=10.10.10.10.10\0}
#"
#set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
#    {.*} {.*} {^1$} {.*9007\0$}
#"
#incr j
#
#set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��DNSServersȡֵ�Ƿ�(��һ���ֽ�)�����"
#set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
#    1
#    {$IPCONN_INST.DNSServers=10.10.10\0}
#"
#set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
#    {.*} {.*} {^1$} {.*9007\0$}
#"
#incr j
#
#set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��DNSServersȡֵ�Ƿ�(�м���һ���ֽ�)�����"
#set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
#    1
#    {$IPCONN_INST.DNSServers=10.10..10\0}
#"
#set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
#    {.*} {.*} {^1$} {.*9007\0$}
#"
#incr j
#
#set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��DNSServersȡֵ�Ƿ�(��һ��.)�����"
#set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
#    1
#    {$IPCONN_INST.DNSServers=10.10.10.10.\0}
#"
#set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
#    {.*} {.*} {^1$} {.*9007\0$}
#"
#incr j
#
#set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��DNSServersȡֵ�Ƿ�(����256)�����"
#set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
#    1
#    {$IPCONN_INST.DNSServers=10.10.256.10\0}
#"
#set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
#    {.*} {.*} {^1$} {.*9007\0$}
#"
#incr j
#
#set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ��DNSServersȡֵ�Ƿ�(�ָ�������,)�����"
#set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
#    1
#    {$IPCONN_INST.DNSServers=10.10.20.10;10.10.20.20\0}
#"
#set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
#    {.*} {.*} {^1$} {.*9007\0$}
#"
#incr j

}

#===============================================================================
#             ����SET��Ϣ
#===============================================================================
if {$TestCaseNo == 3} {
set i $TestCaseNo

#######�������#############

if {1} {
###����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ������Static IP��ʽ����,���ύ
set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ������Static IP��ʽ����"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    7 
    {$IPCONN_INST.Enable=0\0}
    {$IPCONN_INST.Name=TestInf001\0}
    {$IPCONN_INST.AddressingType=Static\0}
    {$IPCONN_INST.ExternalIPAddress=202.10.44.50\0}
    {$IPCONN_INST.SubnetMask=255.0.0.0\0}
    {$IPCONN_INST.DefaultGateway=202.10.0.1\0}
    {$IPCONN_INST.DNSServers=202.10.0.253,202.10.0.254\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
{^0$} {^0$}
"
incr j

set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_COMMIT��Ϣ���ύ����"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_COMMIT"
set AckMatchList($i.$j) ""
incr j
}

if {1} {
###����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ������DHCPC��ʽ����,���ύ
set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ������DHCPC��ʽ����"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    3 
    {$IPCONN_INST.Enable=1\0}
    {$IPCONN_INST.Name=TestInf002\0}
    {$IPCONN_INST.AddressingType=DHCP\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
{^0$} {^0$}
"
incr j

set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_COMMIT��Ϣ���ύ����"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_COMMIT"
set AckMatchList($i.$j) ""
incr j
}

if {1} {
###����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ������Static IP��ʽ����,��ȡ��
set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ������Static IP��ʽ����"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    7 
    {$IPCONN_INST.Enable=1\0}
    {$IPCONN_INST.Name=TestInf003\0}
    {$IPCONN_INST.AddressingType=Static\0}
    {$IPCONN_INST.ExternalIPAddress=202.10.44.50\0}
    {$IPCONN_INST.SubnetMask=255.0.0.0\0}
    {$IPCONN_INST.DefaultGateway=202.10.0.1\0}
    {$IPCONN_INST.DNSServers=202.10.0.253,202.10.0.254\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
{^0$} {^0$}
"
incr j

set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_CANCEL��Ϣ��ȡ������"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_CANCEL"
set AckMatchList($i.$j) ""
incr j
}

if {1} {
##����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ������DHCPC��ʽ����,��ȡ��
set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ������DHCPC��ʽ����"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    2 
    {$IPCONN_INST.Name=TestInf004\0}
    {$IPCONN_INST.AddressingType=DHCP\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
{^0$} {^0$}
"
incr j

set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_CANCEL��Ϣ��ȡ������"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_CANCEL"
set AckMatchList($i.$j) ""
incr j
}

}



#==============================================================================
#             ����MSG_WAN_LINK_CHG������Ϣ
#==============================================================================
if {$TestCaseNo == 4} {
set i $TestCaseNo


###����Ϸ��Լ��###
if {1} {
set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ��֪ͨ���������ʵ�����������"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    4
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Status=Up\0} 
    {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ��֪ͨ���������ʵ�����������"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    2
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Status=Up\0} 
    {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ��ȱ��Path֪ͨ������"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    2
    {Status=Up\0} 
    {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ��Path֪ͨ���������1�������"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    4 
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Status=Up\0}
    {Name=eth1\0} 
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.2.\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ��Path֪ͨ��ֵΪ�յ����"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    3
    {Path=\0} {Status=Up\0} {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ��Path֪ͨ��ֵ��������"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    3
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.99999.\0} 
    {Status=Up\0} 
    {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ��ȱ��NAME֪ͨ������"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    2
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Status=Up\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ��NAME֪ͨ�����1�������"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    4
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Name=eth1\0} 
    {Status=Up\0} 
    {Name=eth0\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ��NAME֪ͨ��ֵΪ�յ����"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    4
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Name=\0} 
    {Status=Up\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ��NAME֪ͨ��ֵ��������"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    3
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Name=eth99\0} 
    {Status=Up\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ��ȱ��STATUS֪ͨ������"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    2
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ��STATUS֪ͨ�����1�������"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    4
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Name=eth1\0} 
    {Status=Up\0} 
    {Status=Down\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ��STATUS֪ͨ��ֵΪ�յ����"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    3
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Name=eth1\0} 
    {Status=\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ��STATUS֪ͨ��ֵ�Ƿ������"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    3
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Name=eth1\0} 
    {Status=Up\0}
"
set AckMatchList($i.$j) ""
incr j
}

#############�������###############

if {1} {
#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ, Static IP��ʽ��������
set comment($i.$j) "#����IP��ʽ����"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    7 
    {$IPCONN_INST.Enable=1\0}
    {$IPCONN_INST.Name=TestInf001\0}
    {$IPCONN_INST.AddressingType=Static\0}
    {$IPCONN_INST.ExternalIPAddress=202.10.44.50\0}
    {$IPCONN_INST.SubnetMask=255.0.0.0\0}
    {$IPCONN_INST.DefaultGateway=202.10.0.1\0}
    {$IPCONN_INST.DNSServers=202.10.0.253,202.10.0.254\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
{^0$} {^0$}
"
incr j

set comment($i.$j) "#�ύ����"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_COMMIT"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ, Static IP��ʽ��������"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    3
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Status=Up\0} 
    {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j
}

if {1} {
###����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ, DHCPC��ʽ��������
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    3 
    {$IPCONN_INST.Enable=1\0}
    {$IPCONN_INST.Name=TestInf002\0}
    {$IPCONN_INST.AddressingType=DHCP\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
{^0$} {^0$}
"
incr j

set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_COMMIT"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ, DHCPC��ʽ��������"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    3
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Status=Up\0} 
    {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j
}

}



#==============================================================================
#             ����MSG_WAN_LINK_CHG������Ϣ
#==============================================================================
if {$TestCaseNo == 5} {
set i $TestCaseNo


####����Ϸ��Լ��
if {1} {
set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ��֪ͨ���������ʵ�����������"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    4
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Status=Down\0} 
    {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ��֪ͨ���������ʵ�����������"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    2
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Status=Down\0} 
    {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ��ȱ��Path֪ͨ������"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    2
    {Status=Down\0} 
    {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ��Path֪ͨ���������1�������"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    4 
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Status=Down\0}\
    {Name=eth1\0} 
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.2.\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ��Path֪ͨ��ֵΪ�յ����"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    3
    {Path=\0} 
    {Status=Down\0} 
    {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ��Path֪ͨ��ֵ��������"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    3
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.99999.\0} 
    {Status=Down\0} 
    {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ��ȱ��NAME֪ͨ������"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    2
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Status=Down\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ��NAME֪ͨ�����1�������"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    4
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Name=eth1\0} 
    {Status=Down\0} 
    {Name=eth0\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ��NAME֪ͨ��ֵΪ�յ����"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    4
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Name=\0} 
    {Status=Down\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ��NAME֪ͨ��ֵ��������"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    3
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Name=eth99\0} 
    {Status=Down\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ��ȱ��STATUS֪ͨ������"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    2
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ��STATUS֪ͨ�����1�������"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    4
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} {Name=eth1\0} 
    {Status=Down\0} 
    {Status=Up\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ��STATUS֪ͨ��ֵΪ�յ����"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    3
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Name=eth1\0} 
    {Status=\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ��STATUS֪ͨ��ֵ�Ƿ������"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    3
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Name=eth1\0} 
    {Status=Up\0}
"
set AckMatchList($i.$j) ""
incr j
return
}


#############�������###############

###����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ, Static IP��ʽ�Ͽ�����
if {1} {
if {1} {
    set comment($i.$j) "#����IP��ʽ����"
    set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
        7 
        {$IPCONN_INST.Enable=1\0}
        {$IPCONN_INST.Name=TestInf001\0}
        {$IPCONN_INST.AddressingType=Static\0}
        {$IPCONN_INST.ExternalIPAddress=202.10.44.50\0}
        {$IPCONN_INST.SubnetMask=255.0.0.0\0}
        {$IPCONN_INST.DefaultGateway=202.10.0.1\0}
        {$IPCONN_INST.DNSServers=202.10.0.253,202.10.0.254\0}
    "
    set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {^0$} {^0$}
    "
    incr j
    set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_COMMIT"
    set AckMatchList($i.$j) ""
    incr j

    set comment($i.$j) "#��������"
    set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
        3
        {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
        {Status=Up\0} 
        {Name=eth1\0}
    "
    set AckMatchList($i.$j) ""
    incr j
}

set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ, Static IP��ʽ�Ͽ�����"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    3
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Status=Down\0} 
    {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j
}


###����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ, DHCPC��ʽ�Ͽ�����
if {1} {
if {1} {
    set comment($i.$j) "#����DHCP��ʽ����"
    set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
        3 
        {$IPCONN_INST.Enable=1\0}
        {$IPCONN_INST.Name=TestInf002\0}
        {$IPCONN_INST.AddressingType=DHCP\0}
    "
    set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {^0$} {^0$}
    "
    incr j
    set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_COMMIT"
    set AckMatchList($i.$j) ""
    incr j

    set comment($i.$j) "#��������"
    set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
        3
        {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
        {Status=Up\0} 
        {Name=eth1\0}
    "
    set AckMatchList($i.$j) ""
    incr j
}


set comment($i.$j) "#����WAN��IPCONNģ�鷢��WAN_LINK_CHG��Ϣ, DHCPC��ʽ�Ͽ�����"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    3
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Status=Down\0} 
    {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j
return
}

}


#==============================================================================
#             ����MSG_PC_PROC_OUTPUT��Ϣ
#==============================================================================
if {$TestCaseNo == 6} {
set i $TestCaseNo

###����Ϸ��Լ��
set comment($i.$j) "#����PC��IPCONNģ�鷢��MSG_PC_PROC_OUTPUT��Ϣ��û��ƥ��ֵ�����"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_PC_PROC_OUTPUT
{ipconn.1.1\0}
1 
{5}
{1  10.28.100.23\0} 
{2  255.255.0.0\0} 
{3  10.28.100.1\0} 
{4  10.28.100.1\0} 
{5  {}}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����PC��IPCONNģ�鷢��MSG_PC_PROC_OUTPUT��Ϣ"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_PC_PROC_OUTPUT
    {ipconn.1.1\0}
    1 
    5 
    {1  10.28.100.23\0} 
    {2  255.255.0.0\0} 
    {3  10.28.100.1\0} 
    {4  10.28.100.1\0} 
    {5  {Lease obtained, entering bound state\0}}
"
set AckMatchList($i.$j) ""
incr j

}




#===============================================================================
#             ����UPDATE��Ϣ
#===============================================================================
if {$TestCaseNo == 7} {
set i $TestCaseNo

set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_UPDATE��Ϣ����������"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_UPDATE"
set AckMatchList($i.$j) ""
incr j

}


#===============================================================================
#             ����DELETE��Ϣ
#===============================================================================
if {$TestCaseNo == 8} {

#######����Ϸ��Լ��#############
set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_DEL_NODE��Ϣ���Ƿ�·�������(·��Ϊ��,·�������ڵȵ�)"
set comment($i.$j) "#·��Ϊ��"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_DEL_NODE
    {$IPCONN_PATH.xx.\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_DEL_NODE_ACK 
    {.*} {.*} {^1$} {.*9005\0$}
"
incr j

set comment($i.$j) "#·������"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_DEL_NODE
    {$IPCONN_PATH.[string repeat "aaaaa" 100]\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_DEL_NODE_ACK 
    {.*} {.*} {^1$} {.*9005\0$}
"
incr j

set comment($i.$j) "#·��������"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN 0x00000001 $MSG_CMM_DEL_NODE
    {$IPCONN_PATH.2.\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_DEL_NODE_ACK 
    {.*} {.*} {^1$} {.*9005\0$}
"
incr j

set comment($i.$j) "#·��ȱ��."
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN 0x00000001 $MSG_CMM_DEL_NODE
    {$IPCONN_PATH.1\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_DEL_NODE_ACK 
    {.*} {.*} {^1$} {.*9005\0$}
"
incr j

#######�������#############
set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_DEL_NODE��Ϣ������·�������"
set MsgList($i.$j) "$MID_WEB $MID_CMM 0x00000001  $MSG_CMM_DEL_NODE
    {$IPCONN_PATH.1.\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_DEL_NODE��Ϣ��ɾ��WANConn������ʵ��"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_DEL_NODE
    {$IPCONN_PATH.\0}
"
set AckMatchList($i.$j) ""
incr j

}

#===============================================================================
#             ���Ա�����Ϣ
#===============================================================================
if {$TestCaseNo == 9} {
set i $TestCaseNo


set comment($i.$j) "#��������"
set MsgList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001  $MSG_CMM_SAVE_CFG"
set AckMatchList($i.$j) ""
incr j
return

if {1} {
set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_ADD_NODE��Ϣ������·�������"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_ADD_NODE 
    {$IPCONN_PATH.\0}
"
set AckMatchList($i.$j)  "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_ADD_NODE_ACK 
    ^0$ ^1$ ^0$
"
incr j
}

if {1} {
set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ������Static IP��ʽ����"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    7 
    {$IPCONN_INST.Enable=1\0}
    {$IPCONN_INST.Name=TestInf001\0}
    {$IPCONN_INST.AddressingType=Static\0}
    {$IPCONN_INST.ExternalIPAddress=202.10.44.50\0}
    {$IPCONN_INST.SubnetMask=255.0.0.0\0}
    {$IPCONN_INST.DefaultGateway=202.10.0.1\0}
    {$IPCONN_INST.DNSServers=202.10.0.253,202.10.0.254\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
{^0$} {^0$}
"
incr j

set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_COMMIT"
set AckMatchList($i.$j) ""
incr j
}
return

if {0} {
set comment($i.$j) "#����CMM��IPCONNģ�鷢��MSG_CMM_SET_VAL��Ϣ������DHCP��ʽ����"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    7 
    {$IPCONN_INST.Enable=1\0}
    {$IPCONN_INST.Name=TestInf002\0}
    {$IPCONN_INST.AddressingType=DHCP\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
{^0$} {^0$}
"
incr j

set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_COMMIT"
set AckMatchList($i.$j) ""
incr j
}

if {0} {
set comment($i.$j) "##��WANģ�鷢��MSG_MON_INTF_STATUS_INFORM��Ϣ"
set MsgList($i.$j) "$MID_ETHLAN  $MID_WAN  0x00000001  $MSG_MON_INTF_STATUS_INFORM
    3
    {Path=InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.Enable\0}
    {Name=eth1\0}
    {Status=Connected\0}
"
set AckMatchList($i.$j) ""
incr j
}
return

}
