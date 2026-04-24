#include<stdint.h>
#include"stdlib.h"
#include"vektoriai.h"
#include"stdio.h"
#include"procesai.h"

static uint64_t p_bitmap[8];
static vektorius* vblokai;
static uint8_t* f_bitmap;
static int atminties_dydis;

void irasyti_baitus(uint8_t* ptr, uint8_t skaicius, int baitu_sk)
{
	for (int i = 0; i < baitu_sk; i++)
	{
		ptr[i] = skaicius;
	}
}

void irasyti_i_porta(uint16_t portas, uint8_t reiksme)
{
	asm volatile ("outb %1, %0" : : "dN" (portas), "a" (reiksme));
}
uint8_t skaityti_porta(uint16_t portas)
{
	uint8_t rezultatas;
	asm volatile ("inb %1, %0" : "=a" (rezultatas) : "dN" (portas));
	return rezultatas;
}
void InicijuotiAlloc()
{
	blokas* Pirmas_blokas = (blokas*) HEAP_PRAD;
	blokas* Paskutinis_blokas = (blokas*) HEAP_PAB;
	Pirmas_blokas->kitas = Paskutinis_blokas;
	Pirmas_blokas->praitas = 0;
	Pirmas_blokas->dydis = 0;
	Paskutinis_blokas->praitas = Pirmas_blokas;
	Paskutinis_blokas->kitas = 0;
	Paskutinis_blokas->dydis = 0;
}

void InicijuotiVAlloc(unsigned int puslapiu_sk)
{
	vblokai = Vektorius(sizeof(vblokas));
	vblokas kernelis;
	vblokas pradzia;
	kernelis.pradzia = (void*) PHEAP_PRAD;
	kernelis.galas = (void*) ~((uint64_t) 0x0);
	pradzia.pradzia = (void*) 0x0;
	pradzia.galas = (void*) 0x0;
	push_back(vblokai, &pradzia);
	push_back(vblokai, &kernelis);
	f_bitmap = malloc(puslapiu_sk / 8);
	irasyti_baitus(f_bitmap, 0, puslapiu_sk / 8);
	atminties_dydis = puslapiu_sk / 8;
}
void* malloc(int baitai)
{
	blokas* dabartinis_b = (blokas*) HEAP_PAB;
	while (true)
	{
		blokas* praitas_b = dabartinis_b->praitas;
		if((void*)dabartinis_b - ((void*)praitas_b + praitas_b->dydis + sizeof(blokas)) > (long int) (baitai + sizeof(blokas)))
		{
			blokas* naujas_b = (blokas*)((void*)praitas_b + praitas_b->dydis + sizeof(blokas));
			naujas_b->praitas = praitas_b;
			naujas_b->kitas = dabartinis_b;
			dabartinis_b->praitas = naujas_b;
			praitas_b->kitas = naujas_b;
			naujas_b->dydis = baitai;
			return naujas_b + 1;
		}
		if (dabartinis_b == (blokas*) HEAP_PAB)
		{
			return NULL;
		}
		dabartinis_b = dabartinis_b->praitas;
	}
}
void free(void* ptr)
{
	blokas* blk = ptr - sizeof(blokas);
	blokas* praitas_b = blk->praitas;
	blokas* kitas_b = blk->kitas;
	praitas_b->kitas = kitas_b;
	kitas_b->praitas = praitas_b;
}
void InicijuotiPAlloc()
{
	for (int i = 0; i < 8; i++)
	{
		p_bitmap[i] = 0;
	}
}
p_lentele* palloc()
{
	for (int i = 0; i < 512; i++)
	{
		long int kauke = 1 << (i % 64);
		if (!(p_bitmap[i >> 6] & kauke))
		{
			p_bitmap[i >> 6] |=kauke;
			p_lentele* rezultatas = (p_lentele*)(PHEAP_PRAD + i * 4096);
			for(int i = 0; i < 512; i++)
			{
				rezultatas->irasas[i] = 0;
			}
			return (p_lentele*)((void*)rezultatas - PHEAP_PRAD + 0x400000);
		}
	}
	return NULL;
}
void pfree(p_lentele* ptr)
{
	ptr = (p_lentele*)((void*)ptr + PHEAP_PRAD - 0x400000);
	int i = ((uint64_t) ptr - PHEAP_PRAD) / 4096;
	long int kauke = 1 << (i % 64);
	p_bitmap[i >> 6] &= ~kauke;
}
void* realloc(void* ptr, int naujas_dydis)
{
	blokas* Blokas = ptr - sizeof(blokas);
	if ((void*) Blokas->kitas > ptr + naujas_dydis)
	{
		Blokas->dydis = naujas_dydis;
		return ptr;
	}
	else
	{
		uint8_t* naujas_ptr = malloc(naujas_dydis);
		for(int i = 0; i < Blokas->dydis; i++)
		{
			naujas_ptr[i] = *((uint8_t*) ptr + i);
		}
		free(ptr);
		return naujas_ptr;
	}
}
void* valloc(unsigned int puslapiu_sk)
{
	// gauti fizines atminties
	p_lentele** puslapiai = malloc(sizeof(p_lentele*) * puslapiu_sk);
	int j = 0;
	for(unsigned int i = 0; i < puslapiu_sk; i++)
	{
		for(j = 0; j < atminties_dydis; j++)
		{
			uint8_t kauke = 1 << (j % 8);
			if(!(f_bitmap[j >> 3] & kauke))
			{
				f_bitmap[j >> 3] |= kauke;
				puslapiai[i] = (p_lentele*) (j * 4096 + 0x600000);
				break;
			}
		}
		if (j == atminties_dydis)
		{
			free(puslapiai);
			return NULL;
		}
	}
	//gauti virtualios atminties
	vblokas naujas_blokas;
	for(int i = 1; i < vblokai->elementu_sk; i++)
	{
		vblokas* reiksmes = vblokai->reiksmes;
		vblokas dabartinis_b = reiksmes[i];
		vblokas praitas_b = reiksmes[i - 1];
		unsigned long int tarpas = (dabartinis_b.pradzia - praitas_b.galas - 4096) / 4096;
		if (tarpas >= puslapiu_sk)
		{
			naujas_blokas.pradzia = praitas_b.galas + 4096;
			naujas_blokas.galas = praitas_b.galas + puslapiu_sk * 4096;
			ideti(vblokai, &naujas_blokas, i);
			break;
		}
	}
	// supuslapiuoti
	p_lentele* pml4;
	pml4  = (p_lentele*) PUSL_LENT;
	for(unsigned int i = 0; i < puslapiu_sk; i++)
	{
		int pml4_ind = ((uint64_t) naujas_blokas.pradzia >> 39 & 0b111111111) + i / (512 * 512 * 512) % 512;
		int pdpt_ind = ((uint64_t) naujas_blokas.pradzia >> 30 & 0b111111111) + i / (512 * 512) % 512;
		int pd_ind = ((uint64_t) naujas_blokas.pradzia >> 21 & 0b111111111) + i / 512 % 512;
		int pt_ind = ((uint64_t) naujas_blokas.pradzia >> 12 & 0b111111111) + i % 512;
		p_lentele* pdpt = (p_lentele*) (pml4->irasas[pml4_ind] & (~0xfff));
		if (pdpt == 0)
		{
			pdpt = palloc();
			pml4->irasas[pml4_ind] = (uint64_t) pdpt + 3;
		}
		pdpt = (p_lentele*)((void*)pdpt + PHEAP_PRAD - 0x400000); // vertimas i virtualia, kad galetume skaityt.
		p_lentele* pd = (p_lentele*) (pdpt->irasas[pdpt_ind] & (~0xfff));
		if (pd == 0)
		{
			pd = palloc();
			pdpt->irasas[pdpt_ind] = (uint64_t) pd + 3;
		}
		pd = (p_lentele*)((void*)pd + PHEAP_PRAD - 0x400000);
		p_lentele* pt = (p_lentele*) (pd->irasas[pd_ind] & (~0xfff));
		if (pt == 0)
		{
			pt = palloc();
			pd->irasas[pd_ind] = (uint64_t) pt + 3;
		}
		pt = (p_lentele*)((void*)pt + PHEAP_PRAD - 0x400000);
		pt->irasas[pt_ind] = (uint64_t) puslapiai[i] + 3;
	}
	// perkrauti puslapiu lenteles
	asm volatile ("movq %%cr3, %%rax\n" "movq %%rax, %%cr3" : : : "rax", "memory");
	free(puslapiai);
	return naujas_blokas.pradzia;
}
void vfree(void* ptr)
{
	//atlaisvinti virtualia atminti
	vblokas dabartinis_b;
	for (int i = 0; i < vblokai->elementu_sk + 1; i++)
	{
		if(i == vblokai->elementu_sk)
		{
			print("tokio ptr nera");
			return;
		}
		dabartinis_b = *((vblokas*)vblokai->reiksmes + i);
		if (dabartinis_b.pradzia == ptr)
		{
			istrinti(vblokai, i);
			break;
		}
	}
	int puslapiu_sk = (int)(((uint64_t)dabartinis_b.galas - (uint64_t)dabartinis_b.pradzia) / 4096);
	//atlaisvinti puslapius ir fizine atminti
	p_lentele* pml4;
	pml4 = (p_lentele*) PUSL_LENT;
	for(int i = 0; i < puslapiu_sk; i++)
	{
		int pml4_ind = ((uint64_t) dabartinis_b.pradzia >> 39 & 0b111111111) + i / (512 * 512 * 512) % 512;
		int pdpt_ind = ((uint64_t) dabartinis_b.pradzia >> 30 & 0b111111111) + i / (512 * 512) % 512;
		int pd_ind = ((uint64_t) dabartinis_b.pradzia >> 21 & 0b111111111) + i / 512 % 512;
		int pt_ind = ((uint64_t) dabartinis_b.pradzia >> 12 & 0b111111111) + i % 512;
		p_lentele* pdpt = (p_lentele*) (pml4->irasas[pml4_ind]  & ~((uint64_t)0xfff));
		pdpt = (p_lentele*)((void*)pdpt + PHEAP_PRAD - 0x400000);
		p_lentele* pd = (p_lentele*) (pdpt->irasas[pdpt_ind]  & ~((uint64_t)0xfff));
		pd = (p_lentele*)((void*)pd + PHEAP_PRAD - 0x400000);
		p_lentele* pt = (p_lentele*) (pd->irasas[pd_ind]  & ~((uint64_t)0xfff));
		pt = (p_lentele*)((void*)pt + PHEAP_PRAD - 0x400000);
		uint64_t puslapis = pt->irasas[pt_ind]  & ~((uint64_t)0xfff);
		pt->irasas[pt_ind] = 0;
		int bitmap_indeksas = (puslapis - 0x600000) / 4096;
		uint8_t kauke = ~(1 << (bitmap_indeksas % 8));
		f_bitmap[bitmap_indeksas >> 3] &= kauke;
	}
}
