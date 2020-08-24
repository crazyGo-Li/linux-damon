#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/type.h>
#include <sys/wait.h>

#define BUF_LEN 1024

ssize_t recv_fd(int fd, void *data, size_t bytes, int *recvfd)
{
	struct msghdr msghdr_recv;
	struct iovec iov[1];
	size_t n;
	union{
		struct cmsghdr cm;
		char control[CMSG_SPACE(sizeof(int))];
	}control_un;

	struct cmsghdr *pcmsghdr;

	msghdr_recv.msg_control = control_un.control;
	msghdr_recv.msg_controllen = sizeof(control_un.control);

	msghdr_recv.msg_name = NULL;
	msghdr_recv.msg_namelen = 0;

	iov[0].iov_base = data;
	iov[0].iov_len = bytes;
	msghdr_recv.msg_iov = iov;
	msghdr_recv.msg_iovlen  = 1;
	if((n = recvmsg(fd, &msghdr_recv, 0) ) < 0)
	{
		return n;
	}
	if((pcmsghdr = CMSG_FIRSTHDR(&msghdr_recv)) != NULL && pcmsghdr->cmsg_len == CMSG_LEN(sizeof(int)))
	{
		if(pcmsghdr->cmsg_level != SOL_SOCKET)
		{
			perror("msg levle is not SOL_SOCKET\n");
		}
		if(pcmsghdr->cmsg_type != SCM_RIGHTS)
		{
			perror("msg type is not SCM_RIGHTS\n");
		}
		*recvfd = *((int*)CMSG_DATA(pcmsghdr));
	}
	else
	{
		*recvfd = -1;
	}
	return n;
}

int my_open(const char * pathname, int mode)
{
	int fd, sockfd[2], status;
	pid_t childpid;
	char c, argsocketfd[0], argmode[10];

	socketpair(AF_LOCAL, SOCK_STREAM, 0, sockfd);
	childpid = fork();
	if(childpid == 0)
	{
		close(sockfd[0]);
		snprintf(argsocketfd, sizeof(argsocketfd), "%d", sockfd[1]);
		snprintf(argmode, sizeof(argmode), "%d", mode);

		execl("./47-socket_server", "openfile", argsocketfd, pathname, argmode, (char *)NULL);
		perror("exec system fail");
	}
	else
	{
		close(sockfd[1]);
		waitpid(childpid, &status, 0);

		if(WIFEXITED(status) == 0)
		{
			printf("child did not terminate\n");
		}
		if((status = WEXITSTATUS(status)) == 0)
		{
			recv_fd(sockfd[0], &c, 1, &fd);
		}
		else
		{
			errno = status;
			fd = -1;
		}
		close(sockfd[0]);
		return fd;
	}
}

int main(int argv, char argc*[])
{
	int fd, n;
	char buffer[BUF_LEN];
	if(argc != 2)
	{
		perror("error argc\n");
		return -1;
	}
	if((fd = my_open(argv[1], O_RDONLY)) < 0)
	{
		perror("open file fail\n");
		return -2;
	}
	else
	{
		while((n = read(fd, buffer, BUF_LEN))> 0)
		{
			write(1, buff, n);
		}
	}
	return 0;
}
