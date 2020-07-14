#include "unix_fd.h"

int main(int argc, char *argv[])
{
	socklen_t clt_addr_len;
	int listend_fd;
	int com_fd;
	int ret;
	int i;
	static char recv_buf[BUF_LEN] = {0};
	int len;
	struct sockaddr_un clt_addr;
	struct sockaddr_un srv_addr;

	/* create unix socket */
	listend_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
	if(-1 == listend_fd)
	{
		DEBUG_MSG;
		return -1;
	}

	/* fill server address and unlink socket path */
	srv_addr.sun_family = AF_LOCAL;
	strncpy(srv_addr.sun_path, UNIX_DOMAIN, sizeof(srv_addr.sun_path) - 1);
	unlink(UNIX_DOMAIN);

	/* bind server address with socket fd */
	ret = bind(listend_fd, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
	if(-1 == ret)
	{
		DEBUG_MSG;
		close(listend_fd);
		unlink(UNIX_DOMAIN);
		return -2;
	}

	/* connect request from client */
	len = sizeof(clt_addr);
	com_fd = accept(listen_fd, (struct sockaddr*)&clt_addr, &len);
	if(-1 == com_fd)
	{
		DEBUG_MSG;
		close(listend_fd);
		unlink(UNIX_DOMAIN);
	}
	

	/* receive messages from client and print */
	printf("***********************info*************************\n");
	for(i=0; i < 4; i++)
	{
		memset(recv_buf, 0, BUF_LEN);
		int num  = read(com_fd, recv_buf, sizeof(recv_buf));
		printf("message from client : %s(%d bytes)\n", recv_buf, num);
	}

	close(com_fd);
	close(listend_fd);
	unlink(UNIX_DOMAIN);
	
	return 0;
}
