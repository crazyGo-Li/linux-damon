#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/un.h>

#define DEBUG_MSG printf("%s(%d): %s\n", __func__, __LINE__, strerror(errno))

#define BUF_LEN 128

static void display_err(const char *on_what)
{
	perror(on_what);
	exit(1);
}

int main(int argc, char *argv[])
{
	int error;
	int sock_UNIX;
	struct sockaddr_un addr_UNIX;
	int len_UNIX;
	const char path[BUF_LEN] = "/tmp/sock_UNIX";

	sock_UNIX = socket(AF_LOCAL, SOCK_STREAM, 0);
	if(-1 == sock_UNIX)
	{
		display_err(strerror(errno));
	}

	unlink(path);

	memset(&addr_UNIX, 0, sizeof(addr_UNIX));

	addr_UNIX.sun_family = AF_UNIX;
	strcpy(addr_UNIX.sun_path, path);
	len_UNIX = sizeof(struct sockaddr_un);

	error = bind(sock_UNIX, (struct sockaddr*)&addr_UNIX, sizeof(addr_UNIX));
	if(-1 == error)
	{
		DEBUG_MSG;
		close(sock_UNIX);
		return -1;
	}

	close(sock_UNIX);
	unlink(path);
	
	
	return 0;
}
