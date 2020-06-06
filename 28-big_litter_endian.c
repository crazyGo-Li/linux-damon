#include <stdlib.h>
#include <stdio.h>

#define BITS16 16
#define BITS32 32

/* 16 byte change */
typedef union{
	unsigned short int value;
	unsigned char byte[2];
}to;

/* 32 byte change */
typedef union{
	unsigned long int value;
	unsigned char byte[4];
}to32;

void showvalue(unsigned char *begin, int flag)
{
	int num = 0, i = 0;
	if(flag == BITS16)
	{
		num = 2;
	}
	else if(flag == BITS32)
	{
		num = 4;
	}
	else
	{
		printf("wrong flag\n");
		return;
	}
	for(i = 0; i<num; i++)
	{
		printf("%d : 0x%x\n", i, begin[i]);
	}
	printf("***************************\n");
	return;
}

int main(int argc, char *argv[])
{
	to typeorder;
	to32 typeorder32;
	typeorder32.value = 0x12345678;
	typeorder.value = 0xabcd;
	#if 0
	if(typeorder.byte[0] == 0xcd && typeorder.byte[1] == 0xab)
	{
		printf("low endian byte order\n");
		printf("byte0: 0x%x byte1: 0x%x\n", typeorder.byte[0], typeorder.byte[1]);
	}
	else
	{
		printf("big endian byte order\n");
		printf("byte0: 0x%x byte1: 0x%x\n", typeorder.byte[0], typeorder.byte[1]);		
	}
	#endif

	/* convert lower to big */
	showvalue(typeorder.byte, BITS16);
	typeorder.value = htons(typeorder.value);
	showvalue(typeorder.byte, BITS16);
	typeorder.value = htons(typeorder.value);
	showvalue(typeorder.byte, BITS16);

	showvalue(typeorder32.byte, BITS32);
	typeorder32.value = htons(typeorder32.value);
	showvalue(typeorder32.byte, BITS32);
	typeorder32.value = htons(typeorder32.value);
	showvalue(typeorder32.byte, BITS32);

	
	return 0;
}

