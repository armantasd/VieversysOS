#include"viev.h"

void _start()
{
	char komanda[40];
	for(;;)
	{
		rasyti(1, " vap / |>", 9);
		skaityti(0, komanda, 40);
		if(komanda[0] == '.')
		{
			int fd = atidaryti(komanda + 1);
			if(fd == -1)
			{
				rasyti(1, "vap: tokios programos nera.\n", 28);
				continue;
			}
			int pid = paleisti(fd);
			for(;;);
			// if(ar_gyvas(pid))
			// {
			// 	palaukti();
			// }
			// uzdaryti(fd);
		}
	}
}
