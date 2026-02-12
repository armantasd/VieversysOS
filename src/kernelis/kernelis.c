#include"stdio.h"
#include"stdlib.h"
#include"idt.h"

extern void dalybos_klaida();
extern void neteisinga_ins();
extern void tss_klaida();
extern void puslapiavimo_klaida();
extern void laikmacio_pab_isr();
extern void klaviaturos_pertr_isr();


void kernel_main()
{
	struct pertr_lent_ptr pertraukimai;
	struct pertr_irasas pertr_irasai[256];
	
	pertraukimai.limitas = sizeof(struct pertr_irasas) * 256 - 1;
	pertraukimai.adresas = (uint64_t) &pertr_irasai;
	
	irasyti_i_porta(0x20, 0x11);
	irasyti_i_porta(0xA0, 0x11);
	irasyti_i_porta(0x21, 0x20);
	irasyti_i_porta(0xA1, 0x28);
	irasyti_i_porta(0x21, 0x04);
	irasyti_i_porta(0xA1, 0x02);
	irasyti_i_porta(0x21, 0x01);
	irasyti_i_porta(0xA1, 0x01);
	irasyti_i_porta(0x21, 0x0);
	irasyti_i_porta(0xA1, 0x0);
	// irasytiIDTirasus(struct pertr_irasas* irasas, uint64_t ISRadresas, uint8_t veliavos)
	irasytiIDTirasus(&pertr_irasai[0], (uint64_t) dalybos_klaida, 0b11101110);
	irasytiIDTirasus(&pertr_irasai[6], (uint64_t) neteisinga_ins, 0b11101110);
	irasytiIDTirasus(&pertr_irasai[10], (uint64_t) tss_klaida, 0b10001110);
	irasytiIDTirasus(&pertr_irasai[14], (uint64_t) puslapiavimo_klaida, 0b10001110);
	irasytiIDTirasus(&pertr_irasai[32], (uint64_t) laikmacio_pab_isr, 0b10001110);
	irasytiIDTirasus(&pertr_irasai[33], (uint64_t) klaviaturos_pertr_isr, 0b10001110);

	asm volatile ("lidt %0\n" "sti\n" : : "m" (pertraukimai) : "memory");
	
	print("Veikia!", 0);
	return;
}
void Ant_laikmacio_pabaigos()
{
	print("Laikmatis pasibaige", 0);
	return;
}
void Klaviaturos_pertraukymas()
{
	print("Klaviaturos pertraukymas", 0);
	return;
}
