#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <errno.h>
#include <string.h>
#include <sys/msg.h>
#include <unistd.h>

#define BUF_LEN 1024

/*
* 进程创建的消息不能被自己接收，创建子进程就可以
*/




void msg_stat(int, struct msqid_ds);

int main()
{
    int gflags, sflags, rflags;
    key_t key;
    int msgid;
    int reval;
    pid_t pid;
    struct msgsbuf
    {
        long int mtype;
        char mtext[BUF_LEN];
    }msg_sbuf;

    struct msgmbuf
    {
        long int mtype;
        char mtext[BUF_LEN];
    }msg_rbuf;

    struct msqid_ds msg_ginfo, msg_sinfo;
    char *msgpath="./";

	//create messge
    key = ftok(msgpath, 'a');
    gflags = IPC_CREAT;
    //gflags = IPC_CREAT|IPC_EXCL;
    msgid = msgget(key, gflags|00666);

    if(msgid == -1)
    {
        printf("msg create error\n");
        printf("result : %s\n", strerror(errno));
        return -1;
    }

    pid = fork();
    if(pid < 0)
    {
        printf("fork fail , %s\n", strerror(errno));
        return -1;
    }
    else if(pid == 0)
    {

	    sflags = IPC_NOWAIT;

	    msg_sbuf.mtype = 10;
	    strcpy(msg_sbuf.mtext, "hello world");
	    //msg_sbuf.mtext[0] = 'a';

	    reval = msgsnd(msgid, &msg_sbuf, sizeof(msg_sbuf.mtext), sflags);
	    if(reval == -1)
	    {
	        printf("message send error: %s\n", strerror(errno));
	        return reval;
	    }

	    msg_stat(msgid, msg_ginfo);
		return 0;
    }
    else
    {
    	//rflags = IPC_NOWAIT|MSG_NOERROR;

		msg_rbuf.mtype = 10;
	    reval = msgrcv(msgid, &msg_rbuf, sizeof(msg_sbuf.mtext), msg_rbuf.mtype, 0);
	    if(reval == -1)
	    {
	        printf("message recive error: %s\n", strerror(errno));
	        return reval;
	    }
	    else
	    {
	        printf("recive message success, size: %d, msg: %s\n", reval, msg_rbuf.mtext);
	    }
		//delete message
		reval = msgctl(msgid, IPC_RMID, NULL);
		if(reval == -1)
		{
			printf("unlink msg queue error, %s\n", strerror(errno));
			return reval;
		}
		return 0;
    }
}

void msg_stat(int msgid, struct msqid_ds msg_info)
{
    return;
}

