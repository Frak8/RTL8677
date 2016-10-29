/*NEW3G Created by scb at 2011-7-22*/

/* ------------Wan Config-----------*/
var G_ATM = [];
var kk = 0;
var G_stModemCfg = [];
<?objget :InternetGatewayDevice.WANDevice.2.WANConnectionDevice. "X_TWSZ-COM_VLANID"
`	G_ATM[kk] = ["InternetGatewayDevice.WANDevice.2.WANConnectionDevice.$00.",//Path
				"$00" //X_TWSZ-COM_VLANID
				];
	kk++;
`?>

<?mget :InternetGatewayDevice.WANDevice.2.X_TWSZ-COM_WAN3GInterfaceConfig. "Status Enable Inform"
`	var G_3gInform	= "$01";
	var G_Enable		= "$02";
	var G_3gStatus		= "$03";
`?>

/*let the next page known whitch is the previous action*/
var act = '<?echo $var:WAN3GVIEW_act?>';

var MID_NEW3G = '0x0499';

<?mget :InternetGatewayDevice.WANDevice.2.X_TWSZ-COM_WAN3GInterfaceConfig. "Enable BackupInterval APN DialNumber  defaultwan  pingenble pingintf RecoveryInterval AllowUnlockLowTimesOfPIN AllowUnlockLowTimesOfPUK NetType" 
`	G_stModemCfg['enable']                 =       "$01";
	G_stModemCfg['idletime']               =	 "$02";	
	G_stModemCfg['apn']            		   =	 "$03";
	G_stModemCfg['dialnumber']               =	 "$04";	
	G_stModemCfg['defaultwan']               =	 "$05";	
	G_stModemCfg['pingenble']               =	 "$06";	
	G_stModemCfg['pingintf']               =	 "$07";
	G_stModemCfg['revcoverytime']               =	 "$08";
	G_stModemCfg['timepin']               =	 "$09";
	G_stModemCfg['timepuk']               =	 "$0a";
	G_stModemCfg['NetType']               =	 "$0b";
`?>

<?mget :InternetGatewayDevice.WANDevice.2.X_TWSZ-COM_WAN3GInterfaceConfig. "pingintf pingaddr pingperiod pingtimeout pingtolerance Status hotplugdelay switchdelay CountryNum ProfileNum" 
`	G_stModemCfg['pingintf']                =       "$01"; 
	G_stModemCfg['pingaddr']               =	 "$02";
	G_stModemCfg['pingperiod']            =	 "$03";	
	G_stModemCfg['pingtimeout']          =	 "$04";
	G_stModemCfg['pingtolerance']        =	 "$05";
	G_stModemCfg['status']                  =	 "$06";
	G_stModemCfg['hotplugdelay']         =	 "$07";
	G_stModemCfg['switchdelay']         	=	 "$08";
	G_stModemCfg['CountryNum']         	=	 "$09";
	G_stModemCfg['ProfileNum']         	=	 "$0a";
`?>
var G_Conns = [];
var n = 0;

<?objget :InternetGatewayDevice.WANDevice.2.WANConnectionDevice. "WANPPPConnectionNumberOfEntries"
`	<?if gt $11 0
		`	<?objget :InternetGatewayDevice.WANDevice.2.WANConnectionDevice.$20.WANPPPConnection. "Name Username  Password ConnectionTrigger IdleDisconnectTime"
				  ` G_Conns[n] = [n,
								  "$01",
								  "$02",
								  "**********",
								  "$04",
								  "$05"
									];
					 n++;			 
			`?>
	`?>
`?>

var TOTAL_COUNTRY = 4;

function cList(name, entryNum)
{
   this.name = name;
   this.entryNum = entryNum;
   this.profile = new Array(7);
}

function profile(dialNo, apn, userName, passWord, name, nettype)
{
   this.dialNo = dialNo;
   this.apn = apn;
   this.userName = userName;
   this.passWord = passWord;
   this.name = name;
   this.nettype = nettype;
}

var countryList = new Array(TOTAL_COUNTRY);



countryList[0] = new cList("Egypt",4);
countryList[0].profile[0] = new profile("*99#", "mobinilweb", "", "", "MobiNil", "Auto");
countryList[0].profile[1] = new profile("*99#", "internet.vodafone.net", "internet", "internet", "Vodafone EG", "Auto");
countryList[0].profile[2] = new profile("*99#", "interent.etisalat", "", "", "Etisalat EG", "Auto");
countryList[0].profile[3] = new profile("*99#", "internet", "", "", "other", "Auto");

countryList[1] = new cList("Nigeria",6);
countryList[1].profile[0] = new profile("*99#", "internet.ng.airtel.com", "internet", "internet", "Airtel Nigeria", "Auto");
countryList[1].profile[1] = new profile("*99#", "web.gprs.mtnnigeria.net", "web", "web", "MTN-NG", "Auto");
countryList[1].profile[2] = new profile("*99#", "gloflat", "flat", "flat", "globacom", "Auto");
countryList[1].profile[3] = new profile("*99#", "glosecure", "gprs", "gprs", "Globdirect", "Auto");
countryList[1].profile[4] = new profile("*99#", "etisalat", "", "", "Etisalat", "Auto");
countryList[1].profile[5] = new profile("*99#", "internet", "", "", "other", "Auto");

countryList[2] = new cList("United Arab Emirates",3);
countryList[2].profile[0] = new profile("*99#", "etisalat.ae", "", "", "Etisalat UAE", "Auto");
countryList[2].profile[1] = new profile("*99#", "du", "", "", "DU", "Auto");
countryList[2].profile[2] = new profile("*99#", "internet", "", "", "other", "Auto");

countryList[3] = new cList("other",1);
countryList[3].profile[0] = new profile("*99#", "internet", "", "", "other", "Auto");
function createCountryList(curCountry){
	var _text = [],_value = [];

	_text.push("(Click to Select)");
	_value.push(-1);
	
	for(var i = 0; i < TOTAL_COUNTRY; i++){	
		_text.push(countryList[i].name);
		_value.push(countryList[i].name);
	}
	
	$S('SELECT_3G_CountryList',_text,_value);
	$('SELECT_3G_CountryList').value=curCountry;
}

function appConutrySelect(value){
	var _text = [],_value = [];
	var countryNum = 0;

	_text.push("(Click to Select)");
	_value.push(-1);

	if (-1 != value){
		for(var i = 0; i < TOTAL_COUNTRY; i++){	
			if(value == countryList[i].name){
				countryNum = i;
				break;
			}
		}
	
		for(var i = 0; i < countryList[countryNum].entryNum; i++){	
			_text.push(countryList[countryNum].profile[i].name);
			_value.push(i);
		}
	}
	
	$S('SELECT_3G_ProfileList',_text,_value);
	$('SELECT_3G_ProfileList').value = -1;
}

function appProfile(value){
	var countryNum;
	var country = $('SELECT_3G_CountryList').value;
	
	for(var i = 0; i < TOTAL_COUNTRY; i++){	
		if(country == countryList[i].name){
			countryNum = i;
			break;
		}
	}
	if (-1 != value){
		setJSONValue({
			'INPUT_3G_Account'				:	countryList[countryNum].profile[value].userName||'',
			'INPUT_3G_Psw'					:	(countryList[countryNum].profile[value].userName == "")?"":countryList[countryNum].profile[value].passWord || '',
			'INPUT_3G_DialNumber'				:	countryList[countryNum].profile[value].dialNo||'',
			'SELECT_3G_NetType'				:	countryList[countryNum].profile[value].nettype||'Auto',
			'INPUT_3G_Apn'					:	countryList[countryNum].profile[value].apn||''
		});
	}
}

function CheckShowIpCheckDiv() {
	var ucBackupMechanism = $('SELECT_3G_BackupMechanism').value;
	var iIsDSL = false;

	if (ucBackupMechanism == 'DSL' )
		iIsDSL = true;
	
	disCtrl('div_IpCheck', !iIsDSL);	
}

function CheckShowSetupDiv() {
	var checked = false;

	if($('INPUT_Enable3G').checked)
		checked = true;
		
	disCtrl('div_Setup3g', checked);

	if (checked)
		CheckShowIpCheckDiv();	
}

function InitDate() {
	var iBackupMechanism = G_stModemCfg['pingenble'] == '1'? 'IP_CHECK':'DSL';
	for(var i=0;i<G_Conns.length;i++)
	{
		if(G_Conns[i][1]=="pppo3g")
		{
	        G_stModemCfg['username']               =	G_Conns[i][2];
	        G_stModemCfg['passwd']               =	 G_Conns[i][3];
		    G_stModemCfg['demand']               =	G_Conns[i][4] == 'OnDemand' ? '1':'0';
	        G_stModemCfg['dialdelay']               =	 G_Conns[i][5];
			break;
		}
	}

	setJSONValue({
		'INPUT_Enable3G'					:	G_stModemCfg['enable']||'0',
		'INPUT_3G_Account'				:	G_stModemCfg['username']||'',
		'INPUT_3G_Psw'					:	(G_stModemCfg['username'] == "")?"":G_stModemCfg['passwd'] || '',
		'INPUT_3G_DialNumber'				:	G_stModemCfg['dialnumber']||'',
		'SELECT_3G_AuthMethod'			:	G_stModemCfg['authmethod']||'AUTO',
		'SELECT_3G_NetType'				:	G_stModemCfg['NetType']||'Auto',
		'INPUT_3G_Apn'					:	G_stModemCfg['apn']||'',
		'INPUT_EnableOnDemand'			:	G_stModemCfg['demand']||'0',
		'INPUT_InaTimeout'				:	G_stModemCfg['dialdelay'] ||'60',
		
		'INPUT_3G_MTU'   					: 	'1492',
		'INPUT_DelayTimeout'   			:	 G_stModemCfg['idletime'] || '0',
		'SELECT_3G_BackupMechanism'   		:	 iBackupMechanism,
		'INPUT_3G_CheckIpAddr'   			: 	G_stModemCfg['pingaddr'] || '8.8.8.8',
		'INPUT_3G_TimeOut'				:	G_stModemCfg['pingtimeout'] || '1',
		'INPUT_3G_CheckPeriod'   			:	 G_stModemCfg['pingperiod'] || '1',
		'INPUT_3G_CheckFailTolerance'   		: 	G_stModemCfg['pingtolerance'] || '1',

		'item_hotplug_delay_value'   		: 	G_stModemCfg['hotplugdelay'] || '20',
		'item_switch_delay_value'   		: 	G_stModemCfg['switchdelay'] || '20',

		'INPUT_RecoveryTimeout'   		: 	G_stModemCfg['revcoverytime'] || '10',
		'INPUT_timepin'   		: 	G_stModemCfg['timepin'] || '3',
		'INPUT_timepuk'   		: 	G_stModemCfg['timepuk'] || '10'

		});
}



function uiSubmit(){
	if(act == 'add')
	{
	var _path = ':InternetGatewayDevice.WANDevice.2.WANConnectionDevice.0.WANPPPConnection.0.';
	}
	else
	{
		var _path = ':InternetGatewayDevice.WANDevice.2.WANConnectionDevice.' +G_ATM[0][1]+'.WANPPPConnection.1.';
	}
	var _path2 = ':InternetGatewayDevice.WANDevice.2.X_TWSZ-COM_WAN3GInterfaceConfig.';

	var enable = $('INPUT_Enable3G').checked? '1':'0';
	var pingenble = ($('SELECT_3G_BackupMechanism').value) != 'DSL'? '1':'0';
	var demand = $('INPUT_EnableOnDemand').checked? '1':'0';
	
	if (act == 'add') {
		$H({
			'obj-action'    : 'set',
			'var:WAN3GVIEW_act'  : act,
			'var:menu'      : G_Menu,
			'var:page'      : 'wan3gview',
			'var:errorpage' : 'wan3gview',
			'getpage'       : 'html/index.html',
			'errorpage'     : 'html/index.html'
		},true);
	} else {
		$H({
			'obj-action'    : 'set',
			'var:WAN3GVIEW_act'  : act,
			'var:menu'      : G_Menu,
			'var:page'      : 'wan3gview',
			'var:subpage'	: 'wan3gview',
			'var:errorpage' : 'wan3gview',
			'getpage'       : 'html/index.html',
			'errorpage'     : 'html/index.html'
		},true);	
	}

	/*let webproc send the set msg to logic_3g module*/
	$F('mid', MID_NEW3G);

	/*let webproc known this page is belong to new3g module*/
	$F('isnew3g', '1');
	
	$F(_path2 + 'Enable', enable);
	if (enable == '1') {
		$F(_path + 'Username', 	$('INPUT_3G_Account').value);
		if($('INPUT_3G_Psw').value != '**********')
		{
		     $F(_path + 'Password',  	$('INPUT_3G_Psw').value);
		}

		if (-1 == $('SELECT_3G_CountryList').value)
		{
			alert("Please select one country!");
			return ;
		}
		if (-1 == $('SELECT_3G_ProfileList').value)
		{
			alert("Please select one profile!");
			return ;
		}
		$F(_path2 + 'CountryNum', $('SELECT_3G_CountryList').value);
		$F(_path2 + 'ProfileNum', $('SELECT_3G_ProfileList').value);
		$F(_path2 + 'DialNumber', $('INPUT_3G_DialNumber').value);
		$F(_path2 + 'APN', $('INPUT_3G_Apn').value);
		$F(_path2 + 'NetType', $('SELECT_3G_NetType').value);
		$F(_path + 'ConnectionTrigger', demand == '1'? "OnDemand":"AlwaysOn");

		/*scb+ 2012-9-20, idle time,must larger than 40*/
        if (demand == '1')
        {
            var var_time = parseInt($('INPUT_InaTimeout').value.valueOf());
            /* 新建的时候会检查pppoe节点，所以超时时间也应该跟pppoe统一为60--65535 */
            if ((var_time != 0) && (isNaN(var_time) ||(var_time < 60  || var_time > 65535 ))) {
                 alert("'Inactivity Timeout' must be in the range of 60-65535");
                 return false;
            }
            if(var_time == 0)
                $F(_path + 'IdleDisconnectTime', 65535);/*if 0, pppoe will said invalid*/
            else
                $F(_path + 'IdleDisconnectTime', $('INPUT_InaTimeout').value);
        }
        var var_delay = parseInt($('INPUT_DelayTimeout').value.valueOf());
        if (var_delay!=0 && (isNaN(var_delay) ||(var_delay < 0  || var_delay > 600 ))) {
            alert("'Backup delay time' must be in the range of 0-600");
            return false;
        }
              
        var var_recovery = parseInt($('INPUT_RecoveryTimeout').value.valueOf());
        if (var_recovery != 0 && (isNaN(var_recovery) ||(var_recovery < 0  || var_recovery > 600 ))) {
            alert("'Recovery delay time' must be in the range of 0-600");
            return false;
        }
			
		$F(_path2 + 'BackupInterval', $('INPUT_DelayTimeout').value);
		$F(_path2 + 'pingenble' , pingenble);
		if (pingenble == '1') {
			$F(_path2 + 'pingaddr' , $('INPUT_3G_CheckIpAddr').value);		
			$F(_path2 + 'pingperiod' , $('INPUT_3G_CheckPeriod').value);
			$F(_path2 + 'pingtimeout', $('INPUT_3G_TimeOut').value);
			$F(_path2 + 'pingtolerance' , $('INPUT_3G_CheckFailTolerance').value);	
		}
		$F(_path2 + 'hotplugdelay' , $('item_hotplug_delay_value').value);
		$F(_path2 + 'switchdelay' , $('item_switch_delay_value').value);

		$F(_path2 + 'RecoveryInterval' , $('INPUT_RecoveryTimeout').value);
	//	$F(_path2 + 'AllowUnlockLowTimesOfPIN' , $('INPUT_timepin').value);
	//	$F(_path2 + 'AllowUnlockLowTimesOfPUK' , $('INPUT_timepuk').value);
	}
	
	$F(_path2 + 'action', act);


	
	$('uiPostForm').submit();
}

function uiGetDefault() {
	var _path = ':InternetGatewayDevice.WANDevice.2.X_TWSZ-COM_WAN3GInterfaceConfig.';
	
	$H({
		'obj-action'    : 'set',
		'var:WAN3GVIEW_act'  : act,
		'var:menu'      : G_Menu,
		'var:subpage'	: 'wan3gview',
		'var:page'      : 'wan3gview',
		'var:errorpage' : 'wan3gview',
		'getpage'       : 'html/index.html',
		'errorpage'     : 'html/index.html'
	},true);

	/*let webproc send the set msg to logic_3g module*/
	$F('mid', MID_NEW3G);
	
	/*let webproc known this page is belong to new3g module*/
	$F('isnew3g', '1');	
	if(act == 'add')
	{
	  $F(_path + 'action', 'defaultset_add');
	}
	else
	{
       $F(_path + 'action', 'defaultset_edit');
	}
	
	$('uiPostForm').submit();
}

function uiCancel() {
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=<?echo $var:menu?>&var:page=<?echo $var:page?>&var:subpage=wan3gview';
}

function uiPageRefresh(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=<?echo $var:menu?>&var:page=<?echo $var:page?>&var:subpage=wan3gcfg';
}

function uiOnload() {
	InitDate();
	CheckShowSetupDiv();
	onchgMode();

	/*
	if (G_3gStatus.indexOf('NO') >= 0 && G_3gStatus.indexOf('CARD') >= 0 )
	{
            $('div_buttonDefault').disabled = 1;
	}
	*/
	
	if (G_3gStatus.indexOf('DISCONNECT') >= 0 || G_3gStatus.indexOf('Disconnect') >= 0)
		$('div_buttonDefault').disabled = 0;
	else
		$('div_buttonDefault').disabled = 1;
	
	//$('3G_BackupMechanism').style.display = 'none';
	//$('lang_BackupMechanism').style.display = 'none';
	//$('div_IpCheck').style.display = 'none';
	//$('div_buttonDefault').style.display = 'none';
	$('item_mtu_name').style.display = 'none';
	$('item_mtu_value').style.display = 'none';
	$('lang_Authentication_Method').style.display = 'none';
	$('item_Authentication_Method').style.display = 'none';

	if ($('INPUT_InaTimeout').value.valueOf() == 65535)
		$('INPUT_InaTimeout').value = '0';

    disCtrl('invalid_timeout',Form.Checkbox('INPUT_EnableOnDemand'));

	createCountryList(G_stModemCfg['CountryNum']);
	appConutrySelect(G_stModemCfg['CountryNum']);
	$('SELECT_3G_ProfileList').value = G_stModemCfg['ProfileNum'];
}
function onchgMode(){
	var check = $('INPUT_EnableOnDemand').checked;

	disCtrl('invalid_timeout', check);
}

function dealWithError(){
	if (G_Error != 1){
		return false; 
	}
	
	var arrayHint = [];
	
	dealErrorMsg(arrayHint, G_Error_Msg);
	
}

addListeners(uiOnload, dealWithError);

