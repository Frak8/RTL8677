#
# ����SNMPD
#

#����ȫ�ֱ��� 
set SNMP_PATH "InternetGatewayDevice.X_TWSZ-COM_SNMPAgent"

set L1 0
set L2 0
set L3 0

incr L1

####################################################################################
##1.1 ����SNMP
####################################################################################
incr L2
set L3 0

if {0 && $TestCaseNo == 1} {

################1.1.1 ����Ϸ��Լ��############

	incr L3
	set L4 0
	
	set comment($L1.$L2.$L3.$L4) "# SetItemCount����ʵ�ʸ���"
	set MsgList($L1.$L2.$L3.$L4) "$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL 
	3 
    $SNMP_PATH.Enable=1\0
	$SNMP_PATH.ROCommunity=public\0
" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# SetItemCount����ʵ�ʸ���"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.Enable=1\0
	   $SNMP_PATH.ROCommunity=public\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9005\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# ·��Ϊ��"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 =1\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9005\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# ·������"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable=1\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9005\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# ��Ҷ�ӽڵ�"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.=1\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9005\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# ·������"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.Enable.X_TWSZ-COM_Enable=1\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9005\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# ·�������ո�"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH. X_TWSZ-COM_Enable=1\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9005\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# ����ֻ���ڵ�Status"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.Status=1\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9005\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# Enable����ֵ�Ƿ�"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.Enable=100\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# ROCommunity����"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.ROCommunity=1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# RWCommunity����"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.RWCommunity=1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# TrapVersionС��1"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.TrapVersion=0\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# TrapVersion����3"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.TrapVersion=4\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# TrapHost���������Ƿ��ַ�"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.TrapHost=gj#com\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# TrapHost����������"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.TrapHost=gj.com.gj.com.gj.com.gj.com.gj.com.gj.com.gj.com.gj.com.gj.com.gj.com.gj.com.\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# TrapHostIP��ַ�Ƿ�"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.TrapHost=192.168.256.1\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# TrapHostIP��ַȫ0"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.TrapHost=0.0.0.0\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# TrapHostIP��ַȫ1"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.TrapHost=255.255.255.255\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# TrapHostIP��һ���ֽ�"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.TrapHost=192.168.1.1.1\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# TrapHostIP��һ���ֽ�"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.TrapHost=192.168.1\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# TrapCommunity����"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.TrapCommunity=1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# һ�����ã����е�һ��Ƿ�"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{4 $SNMP_PATH.Enable=100\0 
	   $SNMP_PATH.RWCommunity=public\0 
	   $SNMP_PATH.TrapCommunity=public\0 
	   $SNMP_PATH.TrapHost=192.168.1.12\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# һ�����ã������м�һ��Ƿ�"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{4 $SNMP_PATH.Enable=1\0 
	   $SNMP_PATH.RWCommunity.ABC=public\0 
	   $SNMP_PATH.TrapCommunity=public\0 
	   $SNMP_PATH.TrapHost=192.168.1.12\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9005\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# һ�����ã��������һ��Ƿ�"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{4 $SNMP_PATH.Enable=1\0 
	   $SNMP_PATH.RWCommunity=public\0 
	   $SNMP_PATH.TrapCommunity=public\0 
	   $SNMP_PATH.=192.168.1.12\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9005\0$}"
}

if {$TestCaseNo == 1} {

####################### 1.1.2�������ܲ���####################
	incr L3
	set L4 0
	
	set comment($L1.$L2.$L3.$L4) "# ����SNMP������"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL}
	{6 $SNMP_PATH.Enable=1\0
	   $SNMP_PATH.RWCommunity=public\0
	   $SNMP_PATH.RWCommunity=private\0
	   $SNMP_PATH.TrapVersion=2\0
	   $SNMP_PATH.TrapHost=192.168.1.12\0
	   $SNMP_PATH.TrapCommunity=public\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{^0$} {^0$}" 
    incr L4

	set comment($L1.$L2.$L3.$L4) "# �ύ"
	set MsgList($L1.$L2.$L3.$L4) "$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_COMMIT" 
	set AckMatchList($L1.$L2.$L3.$L4) ""
    incr L4

	set comment($L1.$L2.$L3.$L4) "# ����Enable=0"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL}
	{1 $SNMP_PATH.Enable=0\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{^0$} {^0$}" 
    incr L4
	
	set comment($L1.$L2.$L3.$L4) "# �ύ"
	set MsgList($L1.$L2.$L3.$L4) "$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_COMMIT" 
	set AckMatchList($L1.$L2.$L3.$L4) ""
    incr L4
}


