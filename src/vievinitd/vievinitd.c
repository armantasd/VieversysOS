#include"viev.h"

void _start()
{
	char ivestis[10];
	skaityti(0, ivestis, 10);
	int fd = atidaryti("/PROG.ELF");
	paleisti(fd);
	rasyti(1, "done\n", 5);
	for(;;);
}
