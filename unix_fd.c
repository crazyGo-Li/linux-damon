#include "unix_fd.c"

ssize_t send_fd(int fd, void *data, size_t bytes, int sendfd)
{
	struct msghdr msghdr_send;
	struct iovec iov[1];

	union{
		struct cmsghdr cm;
		char control[CMSG_SPACE(sizeof(int))];
	}control_un;
}
