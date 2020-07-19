#include "unix_fd.h"

int main(int argc, char *argv[])
{
	int listen_fd = -1;
	int client_fd = -1;
	int ret = -1;
	char path[BUF_LEN] = "/tmp/unix_socket.domain";
	char buffer[BUF_LEN] = {0};
	ssize_t bytes = -1;
	socklen_t len = sizeof(struct sockaddr);
	struct sockaddr_un server_addr, client_addr;

	/* create server socket */
	listen_fd = socket(AF_LOCAL, SOCK_STREAM, 0);	
	if(listen_fd == -1)
	{
		DEBUG_MSG;
		return -1;
	}

	/* remove all link of socket file */
	unlink(path);

	/* file server address */
	server_addr.sun_family = AF_LOCAL;
	strcpy(server_addr.sun_path, UNIX_DOMAIN);

	/* bind socket fd with server address */
	ret = bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
	if(-1 == ret)
	{
		DEBUG_MSG;
		close(listen_fd);
		return -2;
	}

	ret = listen(listen_fd, BACKLOG);
	if(-1 == ret)
	{
		DEBUG_MSG;
		close(listen_fd);
		unlink(path);
		return -3;
	}

	client_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &len);
	while(1)
	{
		printf("server waiting:\n");
		
		bytes = read(client_fd, buffer, BUF_LEN);
		if(bytes < 0)
		{
			printf("get buffer fail\n");
			close(listen_fd);
			close(client_fd);
			unlink(path);
			return -4;
		}
		else
		{
			printf("get message: %s\n", buffer);
			if(strncmp(buffer, "exit", 3) == 0)
			{
				close(listen_fd);
				close(client_fd);
				unlink(path);
				return 0;
			}
		}
	}
	

	close(listen_fd);
	close(client_fd);
	unlink(path);	
	return 0;
}
