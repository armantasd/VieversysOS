#include"vektoriai.h"
#include"procesai.h"
#include<stdint.h>
#include"stdlib.h"
#include"gdt.h"
#include"failai.h"

static vektorius* procesai;
static int veikiantis_procesas;
static int procesu_sk;
void mmap(p_lentele* pml4, uint64_t virt, void* fiz, unsigned int puslapiu_sk, uint8_t veliavos);

void Inicijuoti_procesus()
{
    procesai = Vektorius(sizeof(procesas));
};
void Paleisti_init_demona(uint16_t fptr)
{
    procesas initd;
    initd.PID = 1;
    initd.PPID = 0;
    initd.busena = 0;
    elf_headeris elfh;
    skaityti(&elfh, sizeof(elf_headeris), 0, fptr);
   	segmentas* segmentai = malloc(elfh.segm_dyd * elfh.segm_sk);
   	skaityti(segmentai, elfh.segm_dyd * elfh.segm_sk, elfh.segm, fptr);
   	vektorius* Load_segm = Vektorius(sizeof(segmentas));
   	for(int i = 0; i < elfh.segm_sk; i++)
   	{
   		if(segmentai[i].tipas == 1)
   		{
   			push_back(Load_segm, segmentai + i);
   		}
   	}
   	free(segmentai);
	int dydis = 0;
	uint64_t paskutinis_add = 0;
   	segmentas* load_seg = Load_segm->reiksmes;
   	for(int i = 0; i < Load_segm->elementu_sk; i++)
   	{
   		dydis += load_seg[i].dydis;
   		if(load_seg[i].vadresas + load_seg[i].dydis > paskutinis_add)
   		{
   			paskutinis_add = load_seg[i].vadresas + load_seg[i].dydis;
   		}
   	}
   	void* atmintis = valloc(2 + dydis + 5);
   	initd.kst = atmintis;
   	void* fiz = atmintis + 4096 * 2;
   	for(int i = 0; i < Load_segm->elementu_sk; i++)
   	{
   		uint8_t veliavos = 5;
   		if((load_seg[i].veliavos & 2) != 0)
   		{
   			veliavos += 2;
   		}
   		int puslapiu_sk = load_seg[i].dydis / 4096 + 1;
   		mmap(atmintis + 4096, load_seg[i].vadresas, fiz, puslapiu_sk, veliavos);
		skaityti(fiz, load_seg[i].dydis, load_seg[i].adresas, fptr);
   		fiz += puslapiu_sk * 4096;
   	}
   	uint8_t veliavos = 7;
   	int puslapiu_sk = 5;
   	mmap(atmintis + 4096, paskutinis_add, fiz, puslapiu_sk, veliavos);
   	push_back(procesai, &initd);
   	veikiantis_procesas = 0;
   	procesu_sk = 1;
   	// TO DO: sutvarkyti jei neveikia ir paleisti programa
}

void mmap(p_lentele* pml4, uint64_t virt, void* fiz, unsigned int puslapiu_sk, uint8_t veliavos)
{
	for(unsigned int i = 0; i < puslapiu_sk; i++)
	{
		int pml4_ind = (virt >> 39 & 0b111111111) + i / (512 * 512 * 512) % 512;
		int pdpt_ind = (virt >> 30 & 0b111111111) + i / (512 * 512) % 512;
		int pd_ind = (virt >> 21 & 0b111111111) + i / 512 % 512;
		int pt_ind = (virt >> 12 & 0b111111111) + i % 512;
		p_lentele* pdpt = (p_lentele*) (pml4->irasas[pml4_ind] - 3);
		if ((void*) pdpt + 3 == 0)
		{
			pdpt = palloc();
			pml4->irasas[pml4_ind] = (uint64_t) pdpt + 3;
		}
		pdpt = (p_lentele*)((void*)pdpt + PHEAP_PRAD - 0x400000); // vertimas i virtualia, kad galetume skaityt.
		p_lentele* pd = (p_lentele*) (pdpt->irasas[pdpt_ind] - 3);
		if ((void*) pd + 3 == 0)
		{
			pd = palloc();
			pdpt->irasas[pdpt_ind] = (uint64_t) pd + 3;
		}
		pd = (p_lentele*)((void*)pd + PHEAP_PRAD - 0x400000);
		p_lentele* pt = (p_lentele*) (pd->irasas[pd_ind] - 3);
		if ((void*) pt + 3 == 0)
		{
			pt = palloc();
			pd->irasas[pd_ind] = (uint64_t) pt + 3;
		}
		pt = (p_lentele*)((void*)pt + PHEAP_PRAD - 0x400000);
		pt->irasas[pt_ind] = (uint64_t)(fiz + i * 4096) + veliavos;
	}
}
