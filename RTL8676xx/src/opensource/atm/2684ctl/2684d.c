#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <string.h>
#include <syslog.h>
#include <fcntl.h>
#include <netinet/in.h>
#include "atm.h"
#include "autoconf.h"
#include <linux/atmdev.h>
#ifdef CONFIG_WAN_VLAN_SUPPORT
#include <linux/types.h>
#endif
#include <linux/atmbr2684.h>
#include <linux/atmrt2684.h>

#define PROGRAM_NAME	   "2684ctl"
#define CMD_FILE           "/tmp/2684cmd"
#define DAEMON_PID_FILE    "/var/run/2684d.pid"

struct vcc_2684 {
    char ifname[IFNAMSIZ];
	int iftype;
	int sockfd;
	struct vcc_2684 *next;
};

struct vcc_2684 *vcc_head = NULL;
int last_sock, last_iftype;
char last_ifname[IFNAMSIZ];

/*����pid�ļ�/var/run/2684d.pid*/
int create_pid_file(void)
{
    FILE *fp;
    
    fp = fopen(DAEMON_PID_FILE, "w");
    if (fp == NULL) {
        syslog(LOG_INFO, "Error : Couldn't open file [%s] in write mode!", DAEMON_PID_FILE);
	    return -1;
    } else {
        fprintf(fp, "%d", getpid()); /*���ļ���д�����ID*/
        fclose(fp);
    }

    return 0;
}

/*ɾ��pid�ļ�*/
void del_pid_file(void)
{
	unlink(DAEMON_PID_FILE);
	return;
}

void usage(char *s)
{   
	printf("Usage:\n");
	printf("\tCreate: %s [-b] [-p 0|1] [-s sndbuf] [-q qos] [-c number] [-e 0|1] [-a [itf.]vpi.vci]\n", s);
	printf("\tClose: %s -k <nas_index>\n", s);
    exit(1);
}

/*����EOA/IPOA�豸�ӿ�, �豸�����ɴ���Ĳ���ָ��*/
int create_if(char *ifname, int *fd)
{
	int err;

	if(*fd<0) {
		*fd = socket(PF_ATMPVC, SOCK_DGRAM, ATM_AAL5);
	}
	if (*fd<0) {
		syslog(LOG_ERR, "socket creation failed: %s",strerror(errno));
	} else {
		/* create the device with ioctl: */		
		struct atm_newif_br2684 ni_br;
		void *ni;

		memset(&ni_br, 0, sizeof(ni_br));
		ni_br.backend_num = ATM_BACKEND_BR2684;
		ni_br.media = BR2684_MEDIA_ETHERNET;
		ni_br.mtu = 1500;
		strncpy(ni_br.ifname, ifname, IFNAMSIZ-1);
		//ni = (void *)&ni_br;

		/*�����µĽӿ�(EOA/IPOA)*/
		err= ioctl(*fd, ATM_NEWBACKENDIF, &ni_br);
		if (err == 0)
		{
			syslog(LOG_NOTICE, "Interface \"%s\" created sucessfully\n", ifname);
			printf("Interface \"%s\" created sucessfully\n", ifname);
		}
		else
			syslog(LOG_INFO, "Interface \"%s\" could not be created, reason: %s\n",
					         ifname, strerror(errno));

		/* even if we didn't create, because existed, assign_vcc wil want to know it! */ 
		strncpy(last_ifname, ifname, IFNAMSIZ-1);
	}
	return 0;
}

/*���豸�ӿ���vcc��������, ͬʱ����qos�Ȳ���*/
int assign_vcc(char *astr, int encap, int bufsize, struct atm_qos qos, int fd)
{
	int err=0;
	struct sockaddr_atmpvc addr;
	struct atm_backend_br2684 be_br;
	void *be;

	memset(&addr, 0, sizeof(addr));
	err=text2atm(astr,(struct sockaddr *)(&addr), sizeof(addr), T2A_PVC);
	if (err!=0)
		syslog(LOG_ERR,"Could not parse ATM parameters (error=%d)\n",err);

	syslog(LOG_INFO,"Communicating over ATM %d.%d.%d, encapsulation: %s\n", 
	        addr.sap_addr.itf, addr.sap_addr.vpi,
			addr.sap_addr.vci, encap ? "LLC" : "VC mux");
	if (fd < 0 && (( fd = socket(PF_ATMPVC, SOCK_DGRAM, ATM_AAL5)) < 0))
	{
		syslog(LOG_ERR,"failed to create socket %d, reason: %s", errno,strerror(errno));		
	}

    /*����Ĭ�ϵ�qos����*/
	if (qos.aal == 0)
	{
		qos.aal = ATM_AAL5;
    }
    if (qos.txtp.traffic_class == ATM_NONE)
    {
        qos.txtp.traffic_class = ATM_UBR;
    }
    if (qos.txtp.pcr == 0)
    {		
        qos.txtp.pcr = 0x1770;
    }

	qos.txtp.max_sdu = 1524;		
	qos.rxtp = qos.txtp;

    /* ���÷��ͻ�������С */
	if ((err=setsockopt(fd,SOL_SOCKET,SO_SNDBUF, &bufsize ,sizeof(bufsize))))
		syslog(LOG_ERR,"setsockopt SO_SNDBUF: (%d) %s\n",err, strerror(err));

    /* ����qos*/
	if (setsockopt(fd, SOL_ATM, SO_ATMQOS, &qos, sizeof(struct atm_qos)) < 0)
		syslog(LOG_ERR,"setsockopt SO_ATMQOS %d", errno);

    /* ����pvc */
	err = connect(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_atmpvc));
	if (err < 0) {		
		syslog(LOG_ERR,"br2684ctld : Connect Error!!!");
		return err;
	}
    
	/* attach the vcc to device: */
    be_br.backend_num = ATM_BACKEND_BR2684;
    be_br.ifspec.method = BR2684_FIND_BYIFNAME;
    strncpy(be_br.ifspec.spec.ifname, last_ifname, IFNAMSIZ-1);
    be_br.fcs_in = BR2684_FCSIN_NO;
    be_br.fcs_out = BR2684_FCSOUT_NO;
    be_br.fcs_auto = 0;
    be_br.encaps = encap;
    be_br.has_vpiid = 0;
    be_br.send_padding = 0;
    be_br.min_size = 0;
    be = (void *)&be_br;
	
    err=ioctl(fd, ATM_SETBACKEND, be);	
	if (err == 0)
		syslog (LOG_INFO,"Interface configured");
	else {
		syslog (LOG_ERR,"Could not configure interface:%s", strerror(errno));
		exit(2);
	}
	
	return fd;
}

/*�յ��ź�SIGTERM��Ĵ�����*/
void terminate(int signum) 
{
	syslog(LOG_INFO,"2684d : Received SIGTERM or no interface configured.");
	del_pid_file();
	exit(0);
}

/*ɾ��EOA/IPOA�豸*/
int del_vcc_dev(char *ifname)
{
    struct vcc_2684 *ptrcur = vcc_head, *ptrpre = vcc_head;
    
    while (ptrcur) {
        if(!strcmp(ptrcur->ifname, ifname))
			break;
		ptrpre = ptrcur;	
		ptrcur = ptrcur->next;
    }

    if(ptrcur) {
		syslog(LOG_INFO,"2684d : Closing socket for %s", ifname);
		close(ptrcur->sockfd);
		ptrpre->next = ptrcur->next;
		if(ptrcur == vcc_head)
			vcc_head = ptrcur->next;
		free(ptrcur);
	} else {
		syslog(LOG_INFO,"2684d: Could not find socket for %s", ifname);
	}

	return 0;    
}

/*�����ò������н������õĺ���*/
int vcc_2684_main(int argc, char **argv)
{
	int c, background=0, encap=0, sndbuf=8192;

	struct atm_qos reqqos;
	last_sock = -1;	
	last_iftype = NULL_2684;
	int fd = -1;

	memset(last_ifname, 0, IFNAMSIZ);
	/* st qos to 0 */
	memset(&reqqos, 0, sizeof(reqqos));
	optind = 0;
	if (argc>1)
    {
		while ((c = getopt(argc, argv,"a:bc:e:s:p:q:t:k:?h")) !=EOF)
		{
			switch (c) {
				case 'b':
					background=1;
					break;
				case 't': /*-t ����Ϊ�����豸����, 0:EOA, 1:IPOA*/
				    last_iftype = atoi(optarg);
				    if (last_iftype < 0) {
                        syslog(LOG_ERR, "Invalid link dev type: %s\n", optarg);
                        last_iftype = EOA_2684;  /*Ĭ��ΪEOA*/
				    }
				    break; 	
				case 'c':  /*-c ����Ϊ�豸��, -t�����������-c����֮ǰ*/
					create_if(optarg, &fd);
					break;
				case 'e':  /*-e ����Ϊ��װ����0:e_vc, 1:e_llc, 3:r_vc, 4:r_llc*/
					encap=(atoi(optarg));
					if(encap<0){
						syslog (LOG_ERR, "invalid encapsulation: %s:\n",optarg);
						encap = 0;
					}
					break;
				case 's': /*-s ����Ϊ���ͻ�������С*/
					sndbuf=(atoi(optarg));
					if(sndbuf<0){
						syslog(LOG_ERR, "Invalid sndbuf: %s, using size of 8192 instead\n",optarg);
						sndbuf = 8192;
					}
					break;
				case 'q': /*-q ����Ϊqos������صĲ���*/
					printf ("optarg : %s\n",optarg);
					if (text2qos(optarg,&reqqos,0)) fprintf(stderr,"QOS parameter invalid\n");
					break;      
				case 'a': /*-a ����Ϊpvc, -a ��������������*/
					fd = assign_vcc(optarg, encap, sndbuf, reqqos, fd);
					break;
				case 'k': /*-k ����ΪҪɾ�����豸���, ��-t�������ʹ��*/
				    del_vcc_dev(optarg);
				    fd = 0;
				    break;
				case '?':
				case 'h':
				default:
					usage(argv[0]);
			}
		}
	}
	else
		usage(argv[0]);

	if (argc != optind) 
	    usage(argv[0]);

	if(last_sock > 0) 
	    close(last_sock);

	return fd;

#if 0
	if (background) {
		pid_t pid;

		pid=fork();
		if (pid < 0) {
			fprintf(stderr,"Error detaching\n");
			exit(2);
		} else if (pid)
			exit(0); // This is the parent

		// Become a process group and session group leader
		if (setsid()<0) {
			fprintf (stderr,"Could not set process group\n");
			exit(2);
		}

		// Fork again to let process group leader exit
		pid = fork();
		if (pid < 0) {
			fprintf(stderr,"Error detaching during second fork\n");
			exit(2);
		} else if (pid)
			exit(0); // This is the parent

		// Now we're ready for buisness
		chdir("/");            // Don't keep directories in use
		close(0); close(1); close(2);  // Close stdin, -out and -error
		/*
		   Note that this implementation does not keep an open
		   stdout/err.
		   If we need them they can be opened now
		   */

	}

	sprintf(itf_name,"%s%d",BR2684_DEV_PREFIX, unit_id);
	create_pid_file(itf_name);

	syslog (LOG_INFO, "RFC 1483/2684 bridge daemon started\n");
	atexit (exitFunc);

	while (1) sleep(30);    /* to keep the sockets... */
	return 0;
#endif
}

/*SIGHUP �źŴ�����*/
void config_2684(int signum)
{
	int  cmd_fd = -1;
	char strcmd[256];
	char **argv = NULL;
	int  argc = 0;
	char *ptemp = NULL;
	int  sockfd = -1;
	int  i = 0;

	syslog(LOG_INFO,"2684d SIGHUP received");
	/*�����������ļ�, ����������2684ctl����д��*/
	cmd_fd = open(CMD_FILE, O_RDONLY);
	if(cmd_fd < 0) {
		syslog(LOG_INFO,"2684d : Could not open %s file!!!", CMD_FILE);	
		return ;
	}

	syslog(LOG_INFO,"br2684ctld CMD file read!\n");
	memset(strcmd,0x00,sizeof(strcmd));
	if(read(cmd_fd,strcmd,sizeof(strcmd)) <= 0) {
		syslog(LOG_INFO,"br2684d : No data read from file %s!!!", CMD_FILE);
		close(cmd_fd);
		return;
	}
	close(cmd_fd);	
	unlink(CMD_FILE);

    /*���ļ��е���������ת����argc, argv�Ĳ����б����ʽ*/
	argc++;
	argv = (char **)calloc(1,sizeof(char **));
	argv[argc - 1] = (char *)calloc(1, strlen(PROGRAM_NAME) + 1);
	strncpy(argv[argc - 1],PROGRAM_NAME,strlen(PROGRAM_NAME));
	ptemp = strtok(strcmd, " ");
	while(ptemp) {
		argc++;
		argv = (char **)realloc(argv,argc * sizeof(char **));
		argv[argc - 1] = (char *)calloc(1,strlen(ptemp) + 1);
		strncpy(argv[argc - 1],ptemp,strlen(ptemp));
		ptemp = strtok(NULL, " ");
	}	

	/*�����ò������н���,��ִ�����ù���*/
	sockfd = vcc_2684_main(argc,argv);
	if(sockfd > 0) {
	    /*������ӵĽӿ���Ϣ���浽������*/
		struct vcc_2684 *ptr = NULL;
		ptr = (struct vcc_2684 *)calloc(1, sizeof(*ptr));
		strncpy(ptr->ifname, last_ifname, IFNAMSIZ-1);
		ptr->iftype = last_iftype;
		ptr->sockfd = sockfd;
		ptr->next = vcc_head;
		vcc_head = ptr;
		syslog(LOG_NOTICE,"2684d : Interface %s with sockfd %d", last_ifname, sockfd);
	} 

	for(i = 0; i < argc; i++)
		if(argv[i])
			free(argv[i]);
	free(argv);
	argc = 0;
	argv = NULL;
	return ;
}

/*************************************************************************
����: �������������
����: argc  ������Ŀ
      argv  ����ָ������ָ��
����: �ɹ� -- 0
      ʧ�� -- ����������
��ע:
**************************************************************************/
int main(int argc, char **argv)
{
	struct sigaction sa;

	create_pid_file();

	syslog(LOG_INFO,"2684d started\n");
	printf("2684d started\n");
	memset(&sa, 0, sizeof(sa));
    /*ע���źŴ�����*/
	sa.sa_handler = config_2684;
	sigaction(SIGHUP, &sa, NULL); /*�յ�SIGHUP�źź�����br2684*/
	sa.sa_handler = terminate;
	sigaction(SIGTERM, &sa, NULL); /*�յ�SIGTERM�źź��˳�����*/

    /*2684d����Ϊ�ػ�����, ��ά���˶��2684(EOA, IPOA)�ӿڶ�Ӧ��
      �׽���,�ó���һ���˳�,���е�2684(EOA, IPOA)�豸�ͱ�����*/
	while (1)
	    sleep(10);    /* to keep the sockets... */

	return 0;
}

