#include"vektoriai.h"
#include"procesai.h"
#include<stdint.h>
#include"stdlib.h"
#include"gdt.h"
#include"failai.h"
#include"stdio.h"
#include"laikrodis.h"

static vektorius* procesai;
static int veikiantis_procesas = 0;
static int procesu_sk = 0;
void mmap(p_lentele* pml4, uint64_t virt, void* fiz, unsigned int puslapiu_sk, uint8_t veliavos);
uint64_t Surasti_fiz(uint64_t virt);

procesas* dabartinis_p;
uint64_t extra_reg;

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
			push_back(Load_segm, &segmentai[i]);
   		}
   	}
   	free(segmentai);
	int dydis = 0;
	uint64_t pirmas_add = ~1;
	uint64_t paskutinis_add = 0;
	segmentas* load_seg = Load_segm->reiksmes;
   	for(int i = 0; i < Load_segm->elementu_sk; i++)
   	{
   		if(load_seg[i].vadresas + load_seg[i].atmdydis > paskutinis_add)
   		{
   			paskutinis_add = load_seg[i].vadresas + load_seg[i].atmdydis;
   		}
   		if(load_seg[i].vadresas < pirmas_add)
   		{
   			pirmas_add = load_seg[i].vadresas;
   		}
   	}
   	dydis = paskutinis_add - pirmas_add;
   	void* atmintis = valloc(2 + (dydis / 4096 + 1) + 5);
   	void* fiz = (void*)Surasti_fiz((uint64_t)atmintis + 4096 * 2);
   	void* virt_fiz = atmintis + 4096 * 2;
   	for(int i = 0; i < Load_segm->elementu_sk; i++)
   	{
   		uint8_t veliavos = 5;
   		if((load_seg[i].veliavos & 2) != 0)
   		{
   			veliavos += 2;
   		}
   		int puslapiu_sk = load_seg[i].atmdydis / 4096 + 1;
   		mmap(atmintis + 4096, load_seg[i].vadresas, fiz, puslapiu_sk, veliavos);
		skaityti(virt_fiz, load_seg[i].dydis, load_seg[i].adresas, fptr);
   		fiz += puslapiu_sk * 4096;
   		virt_fiz += puslapiu_sk * 4096;
   	}
   	paskutinis_add &= (~0xfff);
   	mmap(atmintis + 4096, paskutinis_add, fiz, 5, 7);
   	paskutinis_add += 4096 * 4;
   	initd.kst = (void*)((uint64_t)0xff << 39);
    fiz = (void*)Surasti_fiz((uint64_t)atmintis);
   	mmap(atmintis + 4096, (uint64_t)initd.kst, fiz, 2, 3);
   	p_lentele* puslapiu_lent = atmintis + 4096;
    puslapiu_lent->irasas[511] = *((uint64_t*)PUSL_LENT + 511);
    veikiantis_procesas = procesu_sk;
   	procesu_sk += 1;
   	initd.kst += 4096 + 8;
   	tss_ptr->rsp0 = (uint64_t)initd.kst;
   	initd.cr3 = fiz + 4096;
   	push_back(procesai, &initd);
   	procesas* proc = ((procesas*)procesai->reiksmes + veikiantis_procesas);
   	proc->kitas_proc = (procesas*)procesai->reiksmes + ((veikiantis_procesas + 1) % procesu_sk);
	atlaisvinti_vkt(Load_segm);
   	dabartinis_p = proc;
   	asm volatile ("cli");
	Inicijuoti_Laikrodi(100);
   	asm volatile ("mov %0, %%cr3" : : "r" (initd.cr3) : "rax");
  	asm volatile ("push $0x1b\n" "push %0" : : "r" (paskutinis_add) : "rax");
  	asm volatile ("push $0x202\n" "push $0x23\n" "push %0" : : "r" (elfh.pradz) : "rax");
	asm volatile ("mov $0, %rbp\n");
  	asm volatile ("iretq");
   	for(;;);
}

void mmap(p_lentele* pml4, uint64_t virt, void* fiz, unsigned int puslapiu_sk, uint8_t veliavos)
{
	for(unsigned int i = 0; i < puslapiu_sk; i++)
	{
		int pml4_ind = (virt >> 39 & 0b111111111) + i / (512 * 512 * 512) % 512;
		int pdpt_ind = (virt >> 30 & 0b111111111) + i / (512 * 512) % 512;
		int pd_ind = (virt >> 21 & 0b111111111) + i / 512 % 512;
		int pt_ind = (virt >> 12 & 0b111111111) + i % 512;
		p_lentele* pdpt = (p_lentele*) (pml4->irasas[pml4_ind] - 7);
		if ((void*) pdpt + 7 == 0)
		{
			pdpt = palloc();
			pml4->irasas[pml4_ind] = (uint64_t) pdpt + 7;
		}
		pdpt = (p_lentele*)((void*)pdpt + PHEAP_PRAD - 0x400000); // vertimas i virtualia, kad galetume skaityt.
		p_lentele* pd = (p_lentele*) (pdpt->irasas[pdpt_ind] - 7);
		if ((void*) pd + 7 == 0)
		{
			pd = palloc();
			pdpt->irasas[pdpt_ind] = (uint64_t) pd + 7;
		}
		pd = (p_lentele*)((void*)pd + PHEAP_PRAD - 0x400000);
		p_lentele* pt = (p_lentele*) (pd->irasas[pd_ind] - 7);
		if ((void*) pt + 7 == 0)
		{
			pt = palloc();
			pd->irasas[pd_ind] = (uint64_t) pt + 7;
		}
		pt = (p_lentele*)((void*)pt + PHEAP_PRAD - 0x400000);
		pt->irasas[pt_ind] = (uint64_t)(fiz + i * 4096) + veliavos;
	}
}
uint64_t Surasti_fiz(uint64_t virt)
{
	p_lentele* pml4 = (p_lentele*)PUSL_LENT;
	int pml4_ind = virt >> 39 & 0b111111111;
	int pdpt_ind = virt >> 30 & 0b111111111;
	int pd_ind = virt >> 21 & 0b111111111;
	int pt_ind = virt >> 12 & 0b111111111;
	p_lentele* pdpt = (p_lentele*) (pml4->irasas[pml4_ind] & (~0xfff));
	pdpt = (p_lentele*)((void*)pdpt + PHEAP_PRAD - 0x400000); // vertimas i virtualia, kad galetume skaityt.
	p_lentele* pd = (p_lentele*) (pdpt->irasas[pdpt_ind] & (~0xfff));
	pd = (p_lentele*)((void*)pd + PHEAP_PRAD - 0x400000);
	p_lentele* pt = (p_lentele*) (pd->irasas[pd_ind] & (~0xfff));
	pt = (p_lentele*)((void*)pt + PHEAP_PRAD - 0x400000);
	return (pt->irasas[pt_ind] & (~0xfff));
}
