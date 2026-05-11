#include"viev.h"

void _start()
{
	rasyti(1, "vievinitd ir vieversio apvalkalas (vap) sekmingai paleistas\n\n", 61);
	char komanda[40];
	for(;;)
	{
		rasyti(1, " | vap / |>", 11);
		skaityti(0, komanda, 40);
		if(komanda[0] == '.')
		{
			int fd = atidaryti(komanda + 1);
			if(fd == -1)
			{
				rasyti(1, "Tokios programos nera\n", 22);
				uzdaryti(fd);
				continue;
			}
			int pid = paleisti(fd);
			uzdaryti(fd);
			while(ar_gyvas(pid))
			{
				palaukti();
			}
		}
		else if(komanda[0] == 'i' && komanda[1] == 's' && komanda[2] == 'v')
		{
			rasyti(1, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n", 27);
			char raide = 1;
			rasyti(1, &raide, 1);
		}
		else
		{
			rasyti(1, "Nezinoma komanda, iveskite teisinga komanda\n", 44);
		}
	}
}
