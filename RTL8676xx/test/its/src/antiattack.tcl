##
## ����Anti Attack
##

##����ȫ�ֱ��� 
set ANTI_PATH "InternetGatewayDevice.X_TWSZ-COM_AntiAttack"

set i 1
set j 1




####################################################################################
### 11. ����DOS ENABLE״̬(AntiDosEnable)
if {$TestCaseNo == 11} {
    set i $TestCaseNo

    ########### 11.1 ����Ϸ��Լ��######

    set comment($i.$j) "#ֵΪ��"
    set MsgList($i.$j) "{$MID_CMM  $MID_ANTIATTACK 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$ANTI_PATH.AntiDosEnable=\0}}"
    set AckMatchList($i.$j) "^$MID_ANTIATTACK$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    set comment($i.$j) "#boolֵ�Ƿ�:On"
    set MsgList($i.$j) "{$MID_CMM  $MID_ANTIATTACK 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$ANTI_PATH.AntiDosEnable=0n\0}}"
    set AckMatchList($i.$j) "^$MID_ANTIATTACK$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    set comment($i.$j) "#boolֵ�Ƿ�:1024"
    set MsgList($i.$j) "{$MID_CMM  $MID_ANTIATTACK 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$ANTI_PATH.AntiDosEnable=1024\0}}"
    set AckMatchList($i.$j) "^$MID_ANTIATTACK$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    set comment($i.$j) "#boolֵ�Ƿ�:����"
    set MsgList($i.$j) "{$MID_CMM  $MID_ANTIATTACK 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$ANTI_PATH.AntiDosEnable=[string repeat "1" 1024]\0}}"
    set AckMatchList($i.$j) "^$MID_ANTIATTACK$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    ## 11.2 �������

    set comment($i.$j) "#set AntiDosEnable=1"
    set MsgList($i.$j) "$MID_CMM  $MID_ANTIATTACK 0x00000001 $MSG_CMM_SET_VAL
    5 
    {$ANTI_PATH.AntiDos.AntiDosEnable=1\0}
    {$ANTI_PATH.AntiDos.SynCookieEnable=0\0}
    {$ANTI_PATH.AntiDos.IcmpEchoIgnrBcastEnable=0\0}
    {$ANTI_PATH.AntiDos.AntiFraggleEnable=0\0}
    {$ANTI_PATH.AntiDos.AntiIPLandEnable=0\0}
    "
    set AckMatchList($i.$j) "^$MID_ANTIATTACK$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set comment($i.$j) "#�ύ"
    set MsgList($i.$j) "$MID_CMM  $MID_ANTIATTACK  0x00000001  $MSG_CMM_COMMIT
    {}"
    set AckMatchList($i.$j) ""
    incr j

    set comment($i.$j) "#set AntiDosEnable=0"
    set MsgList($i.$j) "$MID_CMM  $MID_ANTIATTACK 0x00000001 $MSG_CMM_SET_VAL 
    5
    {$ANTI_PATH.AntiDos.AntiDosEnable=0\0}
    {$ANTI_PATH.AntiDos.SynCookieEnable=1\0}
    {$ANTI_PATH.AntiDos.IcmpEchoIgnrBcastEnable=1\0}
    {$ANTI_PATH.AntiDos.AntiFraggleEnable=1\0}
    {$ANTI_PATH.AntiDos.AntiIPLandEnable=1\0}
    "
    set AckMatchList($i.$j) "^$MID_ANTIATTACK$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set comment($i.$j) "#�ύ"
    set MsgList($i.$j) "$MID_CMM  $MID_ANTIATTACK  0x00000001  $MSG_CMM_COMMIT
    {}"
    set AckMatchList($i.$j) ""
    incr j

}




####################################################################################
### 16. ����AntiDos�������
if {$TestCaseNo == 16} {
    set i $TestCaseNo

    # 15.2 �������

    set comment($i.$j) "#����antidos�ܿ���,�����ӹ��ܿ���"
    set MsgList($i.$j) "{$MID_CMM  $MID_ANTIATTACK 0x00000001 $MSG_CMM_SET_VAL} 
    {17 \
    {$ANTI_PATH.AntiDosEnable=1\0} \
    {$ANTI_PATH.AntiDos.SynCookieEnable=1\0} \
    {$ANTI_PATH.AntiDos.IcmpEchoIgnrBcastEnable=1\0} \
    {$ANTI_PATH.AntiDos.AntiFraggleEnable=1\0} \
    {$ANTI_PATH.AntiDos.AntiEchoCargenEnable=1\0} \
    {$ANTI_PATH.AntiDos.AntiIPLandEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanSetSynFinEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanSetSynRstEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanSetFinRstEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanUnAckSetFinEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanUnAckSetPshEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanUnAckSetUrgEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanUnsetAllEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanSetAllEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanForAllSetSynRstAckFinUrgEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanForAllSetFinEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanForAllSetFinUrgPshEnable=1\0} \
    }"
    set AckMatchList($i.$j) "^$MID_ANTIATTACK$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set MsgList($i.$j) "$MID_CMM  $MID_ANTIATTACK  0x00000001  $MSG_CMM_COMMIT
    {}"
    set AckMatchList($i.$j) ""
    incr j


    set comment($i.$j) "#����antidos�ܿ���,�ر��ӹ��ܿ���"
    set MsgList($i.$j) "{$MID_CMM  $MID_ANTIATTACK 0x00000001 $MSG_CMM_SET_VAL} 
    {17 \
    {$ANTI_PATH.AntiDosEnable=1\0} \
    {$ANTI_PATH.AntiDos.SynCookieEnable=0\0} \
    {$ANTI_PATH.AntiDos.IcmpEchoIgnrBcastEnable=0\0} \
    {$ANTI_PATH.AntiDos.AntiFraggleEnable=0\0} \
    {$ANTI_PATH.AntiDos.AntiEchoCargenEnable=0\0} \
    {$ANTI_PATH.AntiDos.AntiIPLandEnable=0\0} \
    {$ANTI_PATH.AntiScan.AntiScanSetSynFinEnable=0\0} \
    {$ANTI_PATH.AntiScan.AntiScanSetSynRstEnable=0\0} \
    {$ANTI_PATH.AntiScan.AntiScanSetFinRstEnable=0\0} \
    {$ANTI_PATH.AntiScan.AntiScanUnAckSetFinEnable=0\0} \
    {$ANTI_PATH.AntiScan.AntiScanUnAckSetPshEnable=0\0} \
    {$ANTI_PATH.AntiScan.AntiScanUnAckSetUrgEnable=0\0} \
    {$ANTI_PATH.AntiScan.AntiScanUnsetAllEnable=0\0} \
    {$ANTI_PATH.AntiScan.AntiScanSetAllEnable=0\0} \
    {$ANTI_PATH.AntiScan.AntiScanForAllSetSynRstAckFinUrgEnable=0\0} \
    {$ANTI_PATH.AntiScan.AntiScanForAllSetFinEnable=0\0} \
    {$ANTI_PATH.AntiScan.AntiScanForAllSetFinUrgPshEnable=0\0} \
    }"
    set AckMatchList($i.$j) "^$MID_ANTIATTACK$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set MsgList($i.$j) "$MID_CMM  $MID_ANTIATTACK  0x00000001  $MSG_CMM_COMMIT
    {}"
    set AckMatchList($i.$j) ""
    incr j

    set comment($i.$j) "#�ر�antidos�ܿ���,�����ӹ��ܿ���"
    set MsgList($i.$j) "{$MID_CMM  $MID_ANTIATTACK 0x00000001 $MSG_CMM_SET_VAL} 
    {17 \
    {$ANTI_PATH.AntiDosEnable=0\0} \
    {$ANTI_PATH.AntiDos.SynCookieEnable=1\0} \
    {$ANTI_PATH.AntiDos.IcmpEchoIgnrBcastEnable=1\0} \
    {$ANTI_PATH.AntiDos.AntiFraggleEnable=1\0} \
    {$ANTI_PATH.AntiDos.AntiEchoCargenEnable=1\0} \
    {$ANTI_PATH.AntiDos.AntiIPLandEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanSetSynFinEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanSetSynRstEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanSetFinRstEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanUnAckSetFinEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanUnAckSetPshEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanUnAckSetUrgEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanUnsetAllEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanSetAllEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanForAllSetSynRstAckFinUrgEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanForAllSetFinEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanForAllSetFinUrgPshEnable=1\0} \
    }"
    set AckMatchList($i.$j) "^$MID_ANTIATTACK$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set MsgList($i.$j) "$MID_CMM  $MID_ANTIATTACK  0x00000001  $MSG_CMM_COMMIT
    {}"
    set AckMatchList($i.$j) ""
    incr j
}

####################################################################################
### 17. �����쳣
if {$TestCaseNo == 17} {
    set i $TestCaseNo

    set comment($i.$j) "#AntiDosEnable ���ݳ����쳣"
    set MsgList($i.$j) "{$MID_CMM  $MID_ANTIATTACK 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$ANTI_PATH.AntiDosEnable=[string repeat "ab" 1024]\0}}"
    set AckMatchList($i.$j) "^$MID_ANTIATTACK$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j
}