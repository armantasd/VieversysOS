#include"procesai.h"
#include"sisiskvietimai.h"
#include"stdio.h"

void cirp()
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

void* SisLentele[] = {
	cirp,
	poll
};

static uint64_t SisLentDyd = 2;

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
