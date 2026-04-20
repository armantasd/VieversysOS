#include"procesai.h"
#include"sisiskvietimai.h"
#include"stdio.h"
#include"failai.h"
#include<stdbool.h>

void cyp()
{
	asm volatile ("cli");
	print("CYP!\n");
	asm volatile ("sti");
}
void poll(uint64_t sk)
{
	for(uint64_t i = 0; i < sk; i++)
	{
		asm volatile("nop");
	}
}
void skip()
{
	asm volatile ("int $0x20");
}
void sisrasyti(uint64_t fd, char* buf, uint64_t dydis)
{
	if(fd == 1)
	{
		for(uint64_t i = 0; i < dydis; i++)
		{
			if (buf[i] == '\0')
			{
				break;
			}
			print("%r", buf[i]);
		}
	}
	if(fd == 0)
	{
		asm volatile ("mov $-1, %rax");
		return;
	}
}
void sisskaityti(uint64_t fd, char* buf, uint64_t dydis)
{
	if(fd == 0)
	{
		char raide = *(char*)(stdivestis->reiksmes + stdivestis->elementu_sk - 1);
		while(raide != '\n')
		{
			skip();
			raide = *(char*)(stdivestis->reiksmes + stdivestis->elementu_sk - 1);
		}
		bool terminuotas = false;
		for(uint64_t i = 0; i < dydis; i++)
		{
			if(terminuotas)
			{
				buf[i] = '\0';
				continue;
			}
			raide = *(char*)(stdivestis->reiksmes + i);
			buf[i] = raide;
			if(raide == ' ' || raide == '\n' || stdivestis->elementu_sk <= (int)i)
			{
				buf[i] = '\0';
				terminuotas = true;
			}
		}
		stdivestis->elementu_sk = 0;
	}
	else if(fd == 1)
	{
		return;
	}
	else
	{
		struct fd failas = fd_lent[fd - 2];
		skaityti(buf, dydis, failas.poslinkis, failas.klasteris);
	}
}
uint64_t sisatidaryti(char* failo_vard)
{
	uint16_t klasteris = atidaryti(failo_vard);
	if(klasteris == 0xffff)
	{
		return -1;
	}
	for(int i = 0; i < 256; i++)
	{
		if((fd_lent + i)->klasteris == 0)
		{
			(fd_lent + i)->klasteris = klasteris;
			return i + 2;
		}
	}
	return -1;
}
void sisuzdaryti(uint64_t fd)
{
	(fd_lent + fd - 2)->klasteris = 0;
	(fd_lent + fd - 2)->poslinkis = 0;
}
void* SisLentele[] = {
	sisskaityti,
	sisrasyti,
	sisatidaryti,
	sisuzdaryti,
	cyp,
	poll,
	skip
};

static uint64_t SisLentDyd = 3;

uint64_t vykdyti_si(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9)
{
	if(extra_reg + 1 > SisLentDyd)
	{
		return -1;
	}
	else if(SisLentele[extra_reg] == NENUST)
	{
		return -1;
	}
	return ((uint64_t(*)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t))SisLentele[extra_reg])(rdi, rsi, rdx, r10, r8, r9);
}
