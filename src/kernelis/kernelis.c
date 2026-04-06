#include"stdio.h"
#include<stdbool.h>
#include"stdlib.h"
#include"idt.h"
#include"laikrodis.h"
#include"raides.h"
#include"vektoriai.h"
#include"gdt.h"
#include"procesai.h"
#include"failai.h"

extern void Tr_sk_init();
extern void dalybos_klaida();

static bool Didz_raides;
static bool Capslock;

void kernel_main()
{
	// istrinti pirma irasa
	uint64_t* pusl_prad = (uint64_t*) 0x100000;
	*pusl_prad = 0;
	asm volatile ("movq %%cr3, %%rax\n" "movq %%rax, %%cr3" : : : "rax", "memory");
	InicijuotiAlloc();
	InicijuotiPAlloc();
	print("Atminties priskirejai veikia\n");
	InicijuotiGDT();
	print("GDT veikia\n");
	struct pertr_lent_ptr pertraukimai;
	struct pertr_irasas *pertr_irasai = malloc(sizeof(struct pertr_irasas) * 256);
	InicijuotiIDTirasus(pertr_irasai);
	pertraukimai.limitas = sizeof(struct pertr_irasas) * 256 - 1;
	pertraukimai.adresas = (uint64_t) pertr_irasai;
	asm volatile ("lidt %0\n" "sti\n" : : "m" (pertraukimai) : "memory");
	print("Pertraukymai paruosti\n");
	Tr_sk_init();
	print("FPU pajungtas\n");
	InicijuotiVAlloc(800);
	print("Virtuolios atminties priskirejas paruostas\n");
	uint16_t fptr = atidaryti("/VINITD.ELF");
	Inicijuoti_procesus();
	Paleisti_init_demona(fptr);
	print("Paleistas vievinitd");
	//Inicijuoti_Laikrodi(100);
	for(;;);
	return;
}
void Ant_laikmacio_pabaigos()
{
	return;
}
void Klaviaturos_pertraukymas()
{
	uint8_t kodas = skaityti_porta(0x60) & 0x7f;
	uint8_t tipas = skaityti_porta(0x60) & 0x80;
	char m_raides[] = maz_raides;
	char d_raides[] = did_raides;
	switch(kodas)
	{
		case 1:
		case 29:
		case 56:
		case 59:
		case 60:
		case 61:
		case 62:
		case 63:
		case 64:
		case 65:
		case 66:
		case 67:
		case 68:
		case 87:
		case 88:
			break;
		case 42:
			if (tipas == 0)
			{
				Didz_raides = true;
			}
			else
			{
				Didz_raides = false;
			}
			break;
		case 58:
			if (Capslock && tipas == 0)
			{
				Capslock = false;
			}
			else if(!Capslock && tipas == 0)
			{
				Capslock = true;
			}
			break;
		default:
			if (tipas == 0)
			{
				if (Capslock || Didz_raides)
				{
					char raide = d_raides[kodas];
					if (raide == '\0')
					{
						break;
					}
					print("%r", raide);
				}
				else
				{
					char raide = m_raides[kodas];
					if (raide == '\0')
					{
						break;
					}
					print("%r", raide);	
				}
			}
	}
	return;
}
