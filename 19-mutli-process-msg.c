#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>

#include <errno.h>


void msg_show_attr(int, struct msqid_ds);

int main(void)
{
	int ret = -1;
	int msg_flags, msg_id;
	key_t key;
	struct msgmbuf{
		int mtype;
		char mtext[10];
		};

	struct msqid_ds msg_info;
	struct msgmbuf msg_mbuf;
	int msg_sflags, msg_rflags;
	char *msgpath = "./";

	key = ftok(msgpath, 'b');
	if(key != -1)
	{
		printf("成功创建key\n");
	}
	else
	{
		printf("创建key失败\n");
	}

	msg_flags = IPC_CREAT|IPC_EXCL;
	msg_id = msgget(key, msg_flags | 0666);
	if(msg_id == -1)
	{
		printf("创建消息失败\n");
		return -1;
	}
	msg_show_attr(msg_id, msg_info);

	msg_sflags = IPC_NOWAIT;
	msg_mbuf.mtype = 10;
	memcpy(msg_mbuf.mtext, "abc", sizeof("abc"));
	ret = msgsnd(msg_id, (void *)&msg_mbuf, 10, msg_sflags);
	if(-1 == ret)
	{
		printf("%s\n", strerror(errno));
		printf("发送消息失败\n");
	}
	msg_show_attr(msg_id, msg_info);

	msg_rflags = IPC_NOWAIT|MSG_NOERROR;
	ret = msgrcv(msg_id, &msg_mbuf, 10, 10, msg_rflags);
	if(-1 == ret)
	{
		printf("%s\n", strerror(errno));
		printf("接收消息失败\n");
	}
	else
	{
		printf("接收消息成功，长度：%d\n", ret);
	}

	msg_show_attr(msg_id, msg_info);

	ret = msgctl(msg_id, IPC_RMID, NULL);
	if(ret == -1)
	{
		printf("删除消息失败\n");
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
		printf("获取消息信息失败\n");
		return;	
	}

	printf("\n");
	printf("队列中的字节数: %ld\n", msg_info.msg_cbytes);
	printf("队列中的消息数: %d\n", (int)msg_info.msg_qnum);
	printf("队列中的最大字节数: %ld\n", (int)msg_info.msg_qbytes);
	printf("最后发送消息进程的PID：%d\n", msg_info.msg_lspid);
	printf("最后接收消息进程的PID：%d\n", msg_info.msg_lrpid);
	printf("最后发送消息的时间: %s\n", ctime(&(msg_info.msg_stime)));
	printf("最后接收消息的时间：%s\n", ctime(&(msg_info.msg_rtime)));
	printf("最后变化的时间：%s\n", ctime(&(msg_info.msg_ctime)));
	printf("消息的UID是:%d\n", msg_info.msg_perm.uid);
	printf("消息的GID是：%d\n", msg_info.msg_perm.gid);
	return;
}

