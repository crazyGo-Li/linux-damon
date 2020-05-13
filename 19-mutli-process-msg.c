#if 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>

#include <errno.h>

#define BUF_LEN 10

void msg_show_attr(int, struct msqid_ds);

int main(void)
{
	int ret = -1;
	int msg_flags, msg_id;
	key_t key;
	struct msgmbuf{
		int mtype;
		char mtext[BUF_LEN];
		};

	struct msqid_ds msg_info;
	struct msgmbuf msg_mbuf;
	int msg_sflags, msg_rflags;
	char *msgpath = "./";

	key = ftok(msgpath, 'b');
	if(key != -1)
	{
		printf("�ɹ�����key\n");
	}
	else
	{
		printf("����keyʧ��\n");
	}

	msg_flags = IPC_CREAT;
	msg_id = msgget(key, msg_flags | 0666);
	if(msg_id == -1)
	{
		printf("������Ϣʧ��\n");
		return -1;
	}
	msg_show_attr(msg_id, msg_info);

	msg_sflags = IPC_NOWAIT;
	msg_mbuf.mtype = 3;
	memcpy(msg_mbuf.mtext, "abc", sizeof("abc"));
	ret = msgsnd(msg_id, (void *)&msg_mbuf, BUF_LEN, msg_sflags);
	if(-1 == ret)
	{
		printf("%s\n", strerror(errno));
		printf("������Ϣʧ��\n");
	}
	msg_show_attr(msg_id, msg_info);


	memset((void *)&msg_mbuf, 0, sizeof(msg_mbuf));
	msg_rflags = IPC_NOWAIT|MSG_NOERROR;
	//ret = msgrcv(msg_id, (void *)&msg_mbuf, 10, 0, msg_rflags);
	ret = msgrcv(msg_id, (void *)&msg_mbuf, BUF_LEN, 3, msg_rflags);
	if(-1 == ret)
	{
		printf("%s\n", strerror(errno));
		printf("������Ϣʧ��\n");
	}
	else
	{
		printf("������Ϣ�ɹ������ȣ�%d\n", ret);
		printf("���յ�����Ϣ���ͣ�%ld\n", msg_mbuf.mtype);
		printf("���յ�����ϢΪ��%s\n", msg_mbuf.mtext);
	}

	msg_show_attr(msg_id, msg_info);

	ret = msgctl(msg_id, IPC_RMID, NULL);
	if(ret == -1)
	{
		printf("ɾ����Ϣʧ��\n");
		return -1;
	}
	
	exit(0);
}
void msg_show_attr(int msg_id, struct msqid_ds msg_info)
{
	int ret = -1;
	sleep(1);
	ret = msgctl(msg_id, IPC_STAT, &msg_info);
	if(-1 == ret)
	{
		printf("��ȡ��Ϣ��Ϣʧ��\n");
		return;	
	}

	printf("\n");
	printf("�����е��ֽ���: %ld\n", msg_info.msg_cbytes);
	printf("�����е���Ϣ��: %d\n", (int)msg_info.msg_qnum);
	printf("�����е�����ֽ���: %ld\n", (int)msg_info.msg_qbytes);
	printf("�������Ϣ���̵�PID��%d\n", msg_info.msg_lspid);
	printf("��������Ϣ���̵�PID��%d\n", msg_info.msg_lrpid);
	printf("�������Ϣ��ʱ��: %s\n", ctime(&(msg_info.msg_stime)));
	printf("��������Ϣ��ʱ�䣺%s\n", ctime(&(msg_info.msg_rtime)));
	printf("���仯��ʱ�䣺%s\n", ctime(&(msg_info.msg_ctime)));
	printf("��Ϣ��UID��:%d\n", msg_info.msg_perm.uid);
	printf("��Ϣ��GID�ǣ�%d\n", msg_info.msg_perm.gid);
	return;
}
#endif

