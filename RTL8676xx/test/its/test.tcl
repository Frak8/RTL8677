#!/usr/bin/tclsh
########################################################
#               ������Ա���޸����µĲ���               #
########################################################

#set EndianType  "little"    ;#����С���ֽ�����������
set EndianType "big"         ;#���ڴ���ֽ�����������

set host      "192.168.1.1"   ;#�������˵�ip��ַ
set port      "7130"          ;#�������˵Ķ˿�
set timeout   "1000"          ;#���ӳ�ʱֵ(ms)

########################################################
set ScriptPath [file dirname [info script]]

if {$EndianType == "little"} {
    set int i
    set short s
} else {
    set int I
    set short S
}

###����ģ��ID����Ϣ���Ͷ���ű�
#source "$ScriptPath/lib/msg-head.tcl"

###���������ű�
source "$ScriptPath/lib/command-set.tcl"
##################################################################################
if {$argc == 0} {
    set LogFileId [CreateMonLog]
    MonMsg
    close $LogFileId
    
} elseif {$argc == 3} {
    set ModArg [string toupper [lindex $argv 0] 0 end]
    
    set module [ParseMod $ModArg]
    if {$module == -1} {
        return
    }
            
    if {[lindex $argv 1] == "-c"} {
        set TestCaseType  1                  ;#������������1�����Ƶĵ�ģ�鼯�ɲ�������
    } elseif {[lindex $argv 1] == "-f"} {
        set TestCaseType  2                  ;#������������2�����ƵĹ��ܼ��ɲ�������
    } elseif {[lindex $argv 1] == "-u"} {
        set TestCaseType  3                  ;#������������3�����û��Զ���ļ��ɲ�������          
    } else {
        puts "error! the second argument must be '-c'��'-u' or '-f'"
        return 
    }
        
    set MsgIndex [string trimleft [lindex $argv 2] "0"]                                             
    set LogFileId [CreateLog $module $TestCaseType $MsgIndex]                
    set result [StartTest $module $TestCaseType $MsgIndex]
        
    if {$result == -1} {
        close $LogFileId
        return
    }
        
    close $LogFileId
} else {
    puts "error! the num of argument should be 0 or 3"
    return
}


