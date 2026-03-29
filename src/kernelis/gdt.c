#include"stdlib.h"
#include"gdt.h"

static GDTdeskriptorius* gdt_ptr;

void InicijuotiGDT()
{
	gdt_ptr = malloc(sizeof(GDTdeskriptorius));
	gdt_ptr->ptr = malloc(sizeof(GDTirasas) * 4);
	gdt_ptr->dydis = sizeof(GDTirasas) * 4 - 1;
	uint64_t* gdt_0 = (uint64_t*) gdt_ptr->ptr;
	*gdt_0 = 0;
	GDTirasas* gdt_kodas = gdt_ptr->ptr + 1;
	gdt_kodas->dydis = 0xffff;
	gdt_kodas->adresas_z = 0;
	gdt_kodas->adresas_v = 0;
	gdt_kodas->prieeigos_baitas = 0b10011010;
	gdt_kodas->veliavos = 0b10101111;
	gdt_kodas->adresas_a = 0;
	GDTSistemosIrasas* tss_irasas = (GDTSistemosIrasas*) (gdt_ptr->ptr + 2);
	uint64_t tss_segmentas = (uint64_t) malloc(104);
	irasyti_baitus((uint8_t*) tss_segmentas, 0, 104);
	tss_irasas->dydis = 104;
	tss_irasas->adresas_z = tss_segmentas & 0xffff;
	tss_irasas->adresas_v = (tss_segmentas >> 16) & 0xff;
	tss_irasas->prieeigos_baitas = 0b10001001;
	tss_irasas->veliavos = 0;
	tss_irasas->adresas_a = (tss_segmentas >> 24) & 0xff;
	tss_irasas->adresas_p = tss_segmentas >> 32;
	tss_irasas->rezervuota = 0;
	tss* tss_ptr = (tss*) tss_segmentas;
	tss_ptr->iopb = 104;
	asm volatile (
	"lgdt %0\n" 
	"mov $0, %%ax\n" 
	"mov %%ax, %%ds\n" 
	"mov %%ax, %%ss\n" 
	"mov %%ax, %%es\n"
	"mov %%ax, %%fs\n"
	"mov %%ax, %%gs" 
	:  
	: "m"(*gdt_ptr) 
	: "memory", "ax");
	asm volatile ("ltr %0" : : "r"(0x10));
}
