#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
	int ret = 0;
	printf("��ǰ���̺�:%d\n", getpid());

	/*centos 7.6��������Ҫ����8λ��ȡ���յķ���ֵ����������û�в���*/
	/*system�е�ϵͳ�������ʹ������ľ���·��*/
	ret = system("ping 10.192.44.254 -c 2") >> 8;

	printf("ret: %d\n", ret);
	return ret;
}
