#this tcl script includes test cases for module "AUTH"


####################################################################################

#AUTH���ɲ�������

##1 AUTH ���õĴ������

if {$TestCaseNo == "all" || $TestCaseNo == 1} {

###1.2 ��������

    #1 �޸��û���
	set i 1.1.1.1
	set MsgList($i) "{$MID_WEB  $MID_CMM 0x00000001 $MSG_CMM_SET_VAL} \
	{1 InternetGatewayDevice.X_TWSZ-COM_Authentication.1.UserName=admin1\0}" 
	set AckMatchList($i) "{{^$MID_CMM$} {^$MID_WEB$} {^0x00000001$} {^$MSG_CMM_SET_VAL_ACK$}} {0 0}" 

    #1 check result
	set i 1.1.1.2
	set MsgList($i) "{$MID_WEB  $MID_CMM 0x00000001 $MSG_CMM_GET_VAL} \
	{1 InternetGatewayDevice.X_TWSZ-COM_Authentication.1.UserName\0}" 
	set AckMatchList($i) "{{^$MID_CMM$} {^$MID_WEB$} {^0x00000001$} {^$MSG_CMM_GET_VAL_ACK$}} {{^0$} {^1$} {^string InternetGatewayDevice.X_TWSZ-COM_Authentication.1.UserName=admin1\0$}}" 

    #5 �޸�����
	set i 1.1.1.3
	set MsgList($i) "{$MID_WEB  $MID_CMM 0x00000003 $MSG_CMM_SET_VAL} \
	{1 InternetGatewayDevice.X_TWSZ-COM_Authentication.1.Password=admin1\0}" 
	set AckMatchList($i) "{{^$MID_AUTH$} {^$MID_CMM$} {^0x00000003$} {^$MSG_CMM_SET_VAL_ACK$}} {0 0}" 

    #5 check result
	set i 1.1.1.4
	set MsgList($i) "{$MID_WEB  $MID_CMM 0x00000001 $MSG_CMM_GET_VAL} \
	{1 InternetGatewayDevice.X_TWSZ-COM_Authentication.1.Password\0}" 
	set AckMatchList($i) "{{^$MID_CMM$} {^$MID_WEB$} {^0x00000001$} {^$MSG_CMM_GET_VAL_ACK$}} {{^0$} {^1$} {^string InternetGatewayDevice.X_TWSZ-COM_Authentication.1.Password=admin1\0$}}" 

    #9 �޸Ķ������
	set i 1.1.1.7
	set MsgList($i) "{$MID_WEB  $MID_CMM 0x00000005 $MSG_CMM_SET_VAL} \
	{2 
	{InternetGatewayDevice.X_TWSZ-COM_Authentication.2.UserName=user1\0}
	{InternetGatewayDevice.X_TWSZ-COM_Authentication.2.Password=user1\0}
	}" 
	set AckMatchList($i) "{{^$MID_CMM$} {^$MID_WEB$} {^0x00000005$} {^$MSG_CMM_SET_VAL_ACK$}} {0 0}" 

    #9 check result
	set i 1.1.1.8
	set MsgList($i) "{$MID_WEB  $MID_CMM 0x00000001 $MSG_CMM_GET_VAL} \
	{2
	{InternetGatewayDevice.X_TWSZ-COM_Authentication.2.UserName\0}
	{InternetGatewayDevice.X_TWSZ-COM_Authentication.2.Password\0} 
	}"
	set AckMatchList($i) "{{^$MID_CMM$} {^$MID_WEB$} {^0x00000001$} {^$MSG_CMM_GET_VAL_ACK$}} {{^0$}
	{^2$}
	{string InternetGatewayDevice.X_TWSZ-COM_Authentication.2.UserName=user1\0}
	{string InternetGatewayDevice.X_TWSZ-COM_Authentication.2.Password=user1\0}
	}" 


    #13 ��ԭ
	set i 1.1.1.11
	set MsgList($i) "{$MID_WEB  $MID_CMM 0x00000007 $MSG_CMM_SET_VAL} \
	{6 
	{InternetGatewayDevice.X_TWSZ-COM_Authentication.1.UserName=admin\0}
	{InternetGatewayDevice.X_TWSZ-COM_Authentication.1.Password=admin\0}
	{InternetGatewayDevice.X_TWSZ-COM_Authentication.2.UserName=user\0}
	{InternetGatewayDevice.X_TWSZ-COM_Authentication.2.Password=user\0}
	{InternetGatewayDevice.X_TWSZ-COM_Authentication.3.UserName=guest\0}
	{InternetGatewayDevice.X_TWSZ-COM_Authentication.3.Password=guest\0}
	}" 
	set AckMatchList($i) "{{^$MID_CMM$} {^$MID_WEB$} {^0x00000007$} {^$MSG_CMM_SET_VAL_ACK$}} {0 0}" 

    #13 check result
	set i 1.1.1.12
	set MsgList($i) "{$MID_WEB  $MID_CMM 0x00000001 $MSG_CMM_GET_VAL} \
	{6
	{InternetGatewayDevice.X_TWSZ-COM_Authentication.1.UserName\0}
	{InternetGatewayDevice.X_TWSZ-COM_Authentication.1.Password\0}
	{InternetGatewayDevice.X_TWSZ-COM_Authentication.2.UserName\0}
	{InternetGatewayDevice.X_TWSZ-COM_Authentication.2.Password\0}
	{InternetGatewayDevice.X_TWSZ-COM_Authentication.3.UserName\0}
	{InternetGatewayDevice.X_TWSZ-COM_Authentication.3.Password\0}
	}"
	set AckMatchList($i) "{{^$MID_CMM$} {^$MID_WEB$} {^0x00000001$} {^$MSG_CMM_GET_VAL_ACK$}} {{^0$}
	{^6$}
	{InternetGatewayDevice.X_TWSZ-COM_Authentication.1.UserName=admin\0}
	{InternetGatewayDevice.X_TWSZ-COM_Authentication.1.Password=admin\0}
	{InternetGatewayDevice.X_TWSZ-COM_Authentication.2.UserName=user\0}
	{InternetGatewayDevice.X_TWSZ-COM_Authentication.2.Password=user\0}
	{InternetGatewayDevice.X_TWSZ-COM_Authentication.3.UserName=guest\0}
	{InternetGatewayDevice.X_TWSZ-COM_Authentication.3.Password=guest\0}
	}" 

}




##3 AUTH ��Ȩ�Ĵ������

if {$TestCaseNo == "all" || $TestCaseNo == 3} {


###3.2 ��������
	#1 ��֤admin
	set i 3.1.1.1
	set MsgList($i) "{$MID_CMM  $MID_AUTH 0x00000001 $MSG_AUTH} \
	{4 
	{sessionid=00000001\0}
	{username=admin\0}
	{password=admin\0}
	{action=check\0}
	}" 
	set AckMatchList($i) "{{^$MID_AUTH$} {^$MID_CMM$} {^0x00000001$} {^$MSG_AUTH_ACK$}} 
	{{^0$} {^2$} {result=0\0} {level=1\0}}"

	#2 ����
	set i 3.1.1.2
	set MsgList($i) "{$MID_CMM  $MID_AUTH 0x00000002 $MSG_AUTH} \
	{2 
	{sessionid=00000001\0}
	{action=control\0}
	}" 
	set AckMatchList($i) "{{^$MID_AUTH$} {^$MID_CMM$} {^0x00000002$} {^$MSG_AUTH_ACK$}} 
	{{^0$} {^2$} {result=0\0} {level=1\0}}"
    
	#3 ��֤user
	set i 3.1.1.3
	set MsgList($i) "{$MID_CMM  $MID_AUTH 0x00000003 $MSG_AUTH} \
	{4 
	{sessionid=00000002\0}
	{username=user\0}
	{password=user\0}
	{action=check\0}
	}" 
	set AckMatchList($i) "{{^$MID_AUTH$} {^$MID_CMM$} {^0x00000003$} {^$MSG_AUTH_ACK$}} 
	{{^0$} {^2$} {result=0\0} {level=2\0}}"

	#4 ����
	set i 3.1.1.4
	set MsgList($i) "{$MID_CMM  $MID_AUTH 0x00000004 $MSG_AUTH} \
	{2 
	{sessionid=00000002\0}
	{action=control\0}
	}" 
	set AckMatchList($i) "{{^$MID_AUTH$} {^$MID_CMM$} {^0x00000004$} {^$MSG_AUTH_ACK$}} 
	{{^0$} {^2$} {result=0\0} {level=2\0}}"

	#5 ��֤guest
	set i 3.1.1.5
	set MsgList($i) "{$MID_CMM  $MID_AUTH 0x00000005 $MSG_AUTH} \
	{4 
	{sessionid=00000003\0}
	{username=guest\0}
	{password=guest\0}
	{action=check\0}
	}" 
	set AckMatchList($i) "{{^$MID_AUTH$} {^$MID_CMM$} {^0x00000005$} {^$MSG_AUTH_ACK$}} 
	{{^0$} {^2$} {result=0\0} {level=3\0}}"

	#6 ����
	set i 3.1.1.6
	set MsgList($i) "{$MID_CMM  $MID_AUTH 0x00000006 $MSG_AUTH} \
	{2 
	{sessionid=00000003\0}
	{action=control\0}
	}" 
	set AckMatchList($i) "{{^$MID_AUTH$} {^$MID_CMM$} {^0x00000006$} {^$MSG_AUTH_ACK$}} 
	{{^0$} {^2$} {result=0\0} {level=3\0}}"
    

###3.3 �쳣����
	#1 �û�����
	set i 3.3.1.1
	set MsgList($i) "{$MID_CMM  $MID_AUTH 0x00000001 $MSG_AUTH} \
	{4 
	{sessionid=00000001\0}
	{username=unknown\0}
	{password=admin\0}
	{action=check\0}
	}" 
	set AckMatchList($i) "{{^$MID_AUTH$} {^$MID_CMM$} {^0x00000001$} {^$MSG_AUTH_ACK$}} 
	{{^0$} {^1$} {result=1\0}}"

	#2 �����
	set i 3.3.1.2
	set MsgList($i) "{$MID_CMM  $MID_AUTH 0x00000002 $MSG_AUTH} \
	{4 
	{sessionid=00000002\0}
	{username=admin\0}
	{password=user\0}
	{action=check\0}
	}" 
	set AckMatchList($i) "{{^$MID_AUTH$} {^$MID_CMM$} {^0x00000002$} {^$MSG_AUTH_ACK$}} 
	{{^0$} {^1$} {result=2\0}}"

	#3 action ��
	set i 3.3.1.3
	set MsgList($i) "{$MID_CMM  $MID_AUTH 0x00000003 $MSG_AUTH} \
	{4 
	{sessionid=00000002\0}
	{username=admin\0}
	{password=admin\0}
	{action=what\0}
	}" 
	set AckMatchList($i) "{{^$MID_AUTH$} {^$MID_CMM$} {^0x00000003$} {^$MSG_AUTH_ACK$}} 
	{{^78717743$} {^0$}}"

	#4 δ֪����
	set i 3.3.1.4
	set MsgList($i) "{$MID_CMM  $MID_AUTH 0x00000004 $MSG_AUTH} \
	{5 
	{sessionid=00000004\0}
	{username=admin\0}
	{password=admin\0}
	{action=check\0}
	{what=where\0}
	}" 
	set AckMatchList($i) "{{^$MID_AUTH$} {^$MID_CMM$} {^0x00000004$} {^$MSG_AUTH_ACK$}} 
	{{^9005$} {^0$}}"

	#5 ��½�û�����
	for {set j 0} {$j < 8} {incr j} {
		set i 3.3.2.$j
		set MsgList($i) "{$MID_CMM  $MID_AUTH 0x00000005 $MSG_AUTH} \
		{4 
		{sessionid=0000000$j\0}
		{username=admin\0}
		{password=admin\0}
		{action=check\0}
		}" 
		set AckMatchList($i) "{{^$MID_AUTH$} {^$MID_CMM$} {^0x00000005$} {^$MSG_AUTH_ACK$}} 
		{{^0} {^2$} {result=0\0} {level=1\0}}"
	}

	set i 3.3.2.10
	set MsgList($i) "{$MID_CMM  $MID_AUTH 0x00000013 $MSG_AUTH} \
	{4 
	{sessionid=00000009\0}
	{username=admin\0}
	{password=admin\0}
	{action=check\0}
	}" 
	set AckMatchList($i) "{{^$MID_AUTH$} {^$MID_CMM$} {^0x00000013$} {^$MSG_AUTH_ACK$}} 
	{{^0} {^1$} {result=3\0}}"
}

###4 ʱ�������Ϣ�Ĵ������

if {$TestCaseNo == "all" || $TestCaseNo == 4} {

	###4.1 ��������
	set i 4.1.1.1
	set MsgList($i) "{$MID_CMM  $MID_AUTH 0x00000001 $MSG_SNTP_TIME_CHANGED} {1300000000 1500000000}" 
	set AckMatchList($i) ""

}

