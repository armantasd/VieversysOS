#include"procesai.h"
#include"sisiskvietimai.h"
#include"stdio.h"
#include"failai.h"
#include"stdlib.h"
#include<stdbool.h>

void cyp()
{
	asm volatile ("cli");
	print("CYP!\n");
	asm volatile ("sti");
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
		char raide = '\0';
		if(stdini != 0)
		{
			raide = stdivestis[stdini - 1];
		}
		while(raide != '\n')
		{
			asm volatile ("int $0x20");
			if(stdini != 0)
			{
				raide = stdivestis[stdini - 1];
			}
		}
		bool terminuotas = false;
		for(uint64_t i = 0; i < dydis; i++)
		{
			if(terminuotas)
			{
				buf[i] = '\0';
				continue;
			}
			raide = stdivestis[i];
			buf[i] = raide;
			if(raide == '\n' || stdini <= (int)i)
			{
				buf[i] = '\0';
				terminuotas = true;
			}
		}
		stdini = 0;
		stdivestis[stdini] = '\0';
	}
	// else if(fd == 1) // Kadangi kernelis.bin failas yra toks didelis, man prireike nukirpti keleta baitu, tsg neskaitykit stdout buferio plz
	// {
	// 	return;
	// }
	else
	{
		struct fd failas = fd_lent[fd - 2];
		skaityti(buf, dydis, failas.poslinkis, failas.klasteris);
	}
}
uint64_t sisatidaryti(char* failo_vard)
{
	uint16_t klasteris = atidaryti(failo_vard);
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
uint64_t paleisti(uint64_t fd)
{
	asm volatile ("cli");
	asm volatile("mov 0x7f8(%0), %%rbx\n" "mov $0x100000, %%rax\n" "mov %%rax, %%cr3\n" "mov %%rbx, 0x7f8(%1)" : : "r" (dabartinis_p->cr3_virt), "r" (PUSL_LENT) : "rax", "rbx");
	Paleisti_init_demona((fd_lent + fd - 2)->klasteris);
	asm volatile("mov %0, %%rax\n" "mov %%rax, %%cr3" : : "r" (dabartinis_p->cr3) : "rax");
	asm volatile ("sti");
	return procesu_sk;
}
void iseiti()
{
	if(dabartinis_p->PID == 1)
	{
		return;
	}
	asm volatile ("cli");
	asm volatile("mov 0x7f8(%0), %%rbx\n" "mov $0x100000, %%rax\n" "mov %%rax, %%cr3\n" "mov %%rbx, 0x7f8(%1)" : : "r" (dabartinis_p->cr3_virt), "r" (PUSL_LENT) : "rax", "rbx");
	nuzudyti_proc(dabartinis_p);
	asm volatile ("sti");
	asm volatile ("int $0x20");
}
uint64_t ar_gyvas(uint64_t pid)
{
	return ar_gyvas_proc(pid);
}
void* SisLentele[] = {
	sisskaityti,
	sisrasyti,
	sisatidaryti,
	sisuzdaryti,
	cyp,
	skip,
	paleisti,
	iseiti,
	ar_gyvas
};

const uint64_t SisLentDyd = 9;

uint64_t vykdyti_si(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9)
{
	if(extra_reg + 1 > SisLentDyd)
	{
		return -1;
	}
	return ((uint64_t(*)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t))SisLentele[extra_reg])(rdi, rsi, rdx, r10, r8, r9);
}
