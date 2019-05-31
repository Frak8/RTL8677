#####################################
#DNS���ɲ�������
#####################################


set i 1
set j 1


##��������
if {$TestCaseNo == 0 || $TestCaseNo == 9} {

    #���� X_TWSZ-COM_DNSRelayEnable=0, ����/�ر�dns������
	set MsgList(9.1.1.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
	{1 InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_DNSRelayEnable=1\0}" 
	set AckMatchList(9.1.1.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
	{{^0$} }"
    
    #�ύ
	set MsgList(9.1.2.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9d $MSG_CMM_COMMIT} {}" 
	set AckMatchList(9.1.2.1) ""

}

###1.1 �������

##1 DNS������������
if {$TestCaseNo == 0 || $TestCaseNo == 1} {

###1.1 �������

    #2 ����·��Ϊ��
    set MsgList(1.1.2.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1 \0}" 
    set AckMatchList(1.1.2.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^9007$} {^9007$} {^1$} {^.*$}}" 

    #3 ����·����ȱ�ٵȺ�
    set MsgList(1.1.3.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1 InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_DNSRelayEnable\0}" 
    set AckMatchList(1.1.3.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^9007$} {^9007$} {^1$} {^.*$}}"

    #4 ���ڵ����ó���ֵ
    set MsgList(1.1.4.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1  InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_DNSRelayEnable=aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\0}" 
    set AckMatchList(1.1.4.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^9007$} {^9007$} {^1$} {^.*$}}"

    #5 ����ĳ���·��
    set MsgList(1.1.5.2) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1 InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_DNSRelayEnable.aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa=1\0}" 
    set AckMatchList(1.1.5.2) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^9005$} {^9005$} {^1$} {^.*$}}"
    
    #6 �����ڵĽڵ�·��
    set MsgList(1.1.6.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1 InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_DNSRelayEnableABC=1\0}" 
    set AckMatchList(1.1.6.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^9005$} {^9005$} {^1$} {^.*$}}"
   
    #7 ����Ҷ�ӽڵ�����ֵ
    set MsgList(1.1.7.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1 InternetGatewayDevice.LANDevice.1.LANHostConfigManagement=1\0}" 
    set AckMatchList(1.1.7.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^9005$} {^9005$} {^1$} {^.*$}}"


    #9 ����� X_TWSZ-COM_DNSRelayEnable ֵ
    set MsgList(1.1.9.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1 InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_DNSRelayEnable=x\0}" 
    set AckMatchList(1.1.9.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^66462511$} {^66462511$} {^1$} {^.*$}}"

    #9 �յĵ� X_TWSZ-COM_DNSRelayEnable ֵ
    set MsgList(1.1.9.2) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1 InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_DNSRelayEnable=\0}" 
    set AckMatchList(1.1.9.2) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^66462511$} {^66462511$} {^1$} {^.*$}}"

    #9 ������ X_TWSZ-COM_DNSRelayEnable ֵ
    set MsgList(1.1.9.3) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1 InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_DNSRelayEnable=aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\0}" 
    set AckMatchList(1.1.9.3) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^66462511$} {^66462511$} {^1$} {^.*$}}"

    #10 ����� WANPPPConnection.1.DNSOverrideAllowed ֵ
    set MsgList(1.1.10.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1 InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.DNSOverrideAllowed=x\0}" 
    set AckMatchList(1.1.10.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^66462511$} {^66462511$} {^1$} {^.*$}}"

    #11 ����� WANIPConnection.1.DNSServers ֵ
    set MsgList(1.1.11.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1 InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.DNSServers=das\0}" 
    set AckMatchList(1.1.11.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^9007$} {^9007$} {^1$} {^.*$}}"

    #11 ������ WANIPConnection.1.DNSServers ֵ
    set MsgList(1.1.11.2) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1 InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.DNSServers=aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\0}" 
    set AckMatchList(1.1.11.2) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^9007$} {^9007$} {^1$} {^.*$}}"
    
    #12 ����� WANPPPConnection.1.DNSServers ֵ
    set MsgList(1.1.12.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1 InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.DNSServers=188.146.888.999\0}" 
    set AckMatchList(1.1.12.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^9007$} {^9007$} {^1$} {^.*$}}"

    #13  �����WANIPConnection.1.X_TWSZ-COM_UsrDNSServersֵ
    set MsgList(1.1.13.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1 InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.X_TWSZ-COM_UsrDNSServers=188.146.888.999\0}" 
    set AckMatchList(1.1.13.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^9007$} {^9007$} {^1$} {^.*$}}"

    #14 ����� WANPPPConnection.1.X_TWSZ-COM_UsrDNSServers ֵ
    set MsgList(1.1.14.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1 InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.X_TWSZ-COM_UsrDNSServers=da\0}" 
    set AckMatchList(1.1.14.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^9007$} {^9007$} {^1$} {^.*$}}"

    #15 �������Ϣ����
    set MsgList(1.1.15.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_MON_INTF_STATUS_QUERY} \
    {1 \0}"
    set AckMatchList(1.1.15.1) ""

}

###1.2 ��������
if {$TestCaseNo == 0 || $TestCaseNo == 2} {

    #���� X_TWSZ-COM_DNSRelayEnable=0, �ر�dns������
	set MsgList(2.1.1.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
	{1 InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_DNSRelayEnable=0\0}" 
	set AckMatchList(2.1.1.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
	{{^0$} }"   
    #�ύ
	set MsgList(2.1.2.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9d $MSG_CMM_COMMIT} {}" 
	set AckMatchList(2.1.2.1) ""
    #�ع�
	set MsgList(2.1.3.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9e $MSG_CMM_CANCEL} {}"
	set AckMatchList(2.1.3.1) ""


    #���� X_TWSZ-COM_DNSRelayEnable=1, ����dns������
	set MsgList(2.1.4.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
	{1 InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_DNSRelayEnable=1\0}" 
	set AckMatchList(2.1.4.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
	{{^0$} {^0$} }"
    #�ύ
	set MsgList(2.1.5.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9d $MSG_CMM_COMMIT} {}"
	set AckMatchList(2.1.5.1) ""
    #�ع�
	set MsgList(2.1.6.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9e $MSG_CMM_CANCEL} {}"
	set AckMatchList(2.1.6.1) ""


####��ʼ���� WANIPConnection
    #���� WANIPConnection �µĽڵ� DNSOverrideAllowed , DNSServers �� X_TWSZ-COM_UsrDNSServers 
    #ʹ���Զ���dns������
	set MsgList(2.1.7.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
	{3 \
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.DNSOverrideAllowed=1\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.DNSServers=10.10.10.10\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.X_TWSZ-COM_UsrDNSServers=10.10.10.20\0}" 
	set AckMatchList(2.1.7.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
	{{^0$} {^0$} }"
    #�ύ
	set MsgList(2.1.8.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9d $MSG_CMM_COMMIT} {}" 
	set AckMatchList(2.1.8.1) ""
    #�ع�
	set MsgList(2.1.9.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9e $MSG_CMM_CANCEL} {}"
	set AckMatchList(2.1.9.1) ""
    #��ȡ���ú��ֵ
    set MsgList(2.1.9.2) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
	{4 \
    InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.DNSServers\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.DNSOverrideAllowed\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.DNSServers\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.X_TWSZ-COM_UsrDNSServers\0}" 
	set AckMatchList(2.1.9.2) ""


####��ʼ���� WANPPPConnection
    #���� WANPPPConnection �µĽڵ� DNSOverrideAllowed , DNSServers �� X_TWSZ-COM_UsrDNSServers
    #ʹ���Զ���ȡ��dns
	set MsgList(2.1.10.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
	{3 \
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.DNSOverrideAllowed=0\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.DNSServers=10.10.10.10\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.X_TWSZ-COM_UsrDNSServers=10.10.10.20\0}" 
	set AckMatchList(2.1.10.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
	{{^0$} {^0$} }"
    #�ύ
	set MsgList(2.1.11.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9d $MSG_CMM_COMMIT} {}"
	set AckMatchList(2.1.11.1) ""
    #�ع�
	set MsgList(2.1.12.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9e $MSG_CMM_CANCEL} {}"
	set AckMatchList(2.1.12.1) ""

    #��ȡ���ú��ֵ
    set MsgList(2.1.12.2) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
	{4 \
    InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.DNSServers\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.DNSOverrideAllowed\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.DNSServers\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.X_TWSZ-COM_UsrDNSServers\0}" 
	set AckMatchList(2.1.12.2) ""



}


#########2 �㲥��Ϣ##############
if {$TestCaseNo == 0 || $TestCaseNo == 3} {

###2.1 �������

    #���� WANPPPConnection.1 �ĳ�ʼֵ
	set MsgList(3.1.1.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
	{3 \
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.DNSOverrideAllowed=0\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.DNSServers=10.10.10.10\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.X_TWSZ-COM_UsrDNSServers=10.10.10.20\0}" 
	set AckMatchList(3.1.1.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
	{{^0$} {^0$} }"
    #�ύ
	set MsgList(3.1.1.2) "{$MID_CMM  $MID_DNS 0x0b4a6f9d $MSG_CMM_COMMIT} {}"
	set AckMatchList(3.1.1.2) ""

    #��ȡ�㲥����֮ǰ����ֵ
    set MsgList(3.1.1.3) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
	{4 \
    InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.DNSServers\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.DNSOverrideAllowed\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.DNSServers\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.X_TWSZ-COM_UsrDNSServers\0}" 
	set AckMatchList(3.1.1.3) ""

    #�յ���Ϣ��
	set MsgList(3.1.1.4) "{$MID_PPPOE  $MID_GRP_WAN_CONN_UPDATE 0x0b4a6f9c $MSG_WAN_CONN_EST} \
	{2 \0}" 
	set AckMatchList(3.1.1.4) ""

    #��Ϣ���в���������
    set MsgList(3.1.2.1) "{$MID_PPPOE  $MID_GRP_WAN_CONN_UPDATE 0x0b4a6f9c $MSG_WAN_CONN_EST} \
	{2 \
    Pathx=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0\
    DNSServers=10.10.10.30\0}" 
	set AckMatchList(3.1.2.1) ""

    #��Ϣ�����ظ��Ĳ���
    set MsgList(3.1.3.1) "{$MID_PPPOE  $MID_GRP_WAN_CONN_UPDATE 0x0b4a6f9c $MSG_WAN_CONN_EST} \
	{2 \
    Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0\
    Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0\
    DNSServers=10.10.10.30\0}" 
	set AckMatchList(3.1.3.1) ""

    #��Ϣ����ȱ�ٲ���
    set MsgList(3.1.4.1) "{$MID_PPPOE  $MID_GRP_WAN_CONN_UPDATE 0x0b4a6f9c $MSG_WAN_CONN_EST} \
	{1 \
    Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0}" 
	set AckMatchList(3.1.4.1) ""

    #��Ϣ���в���Pathֵ����
    set MsgList(3.1.5.1) "{$MID_PPPOE  $MID_GRP_WAN_CONN_UPDATE 0x0b4a6f9c $MSG_WAN_CONN_EST} \
	{2 \
    Path=xx\0\
    DNSServers=10.10.10.30\0}" 
	set AckMatchList(3.1.5.1) ""

    #��Ϣ���в���DNSServersֵ����
    set MsgList(3.1.5.1) "{$MID_PPPOE  $MID_GRP_WAN_CONN_UPDATE 0x0b4a6f9c $MSG_WAN_CONN_EST} \
	{1 \
    Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0\
    DNSServers=xxx\0}" 
	set AckMatchList(3.1.5.1) ""

    #��ȡ�㲥����֮�����ֵ,��ԭ����ֵһ��,˵������ʧ��
    set MsgList(3.1.6.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
	{4 \
    InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.DNSServers\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.DNSOverrideAllowed\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.DNSServers\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.X_TWSZ-COM_UsrDNSServers\0}" 
	set AckMatchList(3.1.6.1) ""

}


###2.2 ��������

if {$TestCaseNo == 4} {
set i $TestCaseNo

if {0} {
set MsgList($i.$j) "$MID_PPPOE  $MID_GRP_WAN_CONN_UPDATE 0x0b4a6f9c $MSG_WAN_CONN_EST 
2 
{Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0}
{DNSServers=10.10.10.30\0}
" 
set AckMatchList($i.$j) ""
incr j
return
}

###�������������¹㲥
if {1} {
set comment($i.$j) "##��DNSģ�鷢��������������Ϣ"
set MsgList($i.$j) "$MID_DHCPS  $MID_DNS  0x00000001  $MSG_LAN_DOMAIN_NAME_UPDATE
{}
"
set AckMatchList($i.$j) ""
incr j
return
}
    

}

