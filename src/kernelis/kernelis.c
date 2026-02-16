#include"stdio.h"
#include"stdlib.h"
#include"idt.h"

extern void rISR1();
extern void rISR2();
extern void rISR3();
extern void rISR4();
extern void rISR5();
extern void rISR7();
extern void rISR8();
extern void rISR9();
extern void rISR11();
extern void rISR12();
extern void rISR13();
extern void rISR16();
extern void rISR17();
extern void rISR18();
extern void rISR19();
extern void rISR20();
extern void rISR21();
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
	// irasytiIDTirasus(&pertr_irasai[1], (uint64_t) rISR1, 0b11101110);
	// irasytiIDTirasus(&pertr_irasai[2], (uint64_t) rISR2, 0b11101110);
	// irasytiIDTirasus(&pertr_irasai[3], (uint64_t) rISR3, 0b11101110);
	// irasytiIDTirasus(&pertr_irasai[4], (uint64_t) rISR4, 0b11101110);
	// irasytiIDTirasus(&pertr_irasai[5], (uint64_t) rISR5, 0b11101110);
	irasytiIDTirasus(&pertr_irasai[6], (uint64_t) neteisinga_ins, 0b11101110);
	// irasytiIDTirasus(&pertr_irasai[7], (uint64_t) rISR7, 0b11101110);
	// irasytiIDTirasus(&pertr_irasai[8], (uint64_t) rISR8, 0b11101110);
	// irasytiIDTirasus(&pertr_irasai[9], (uint64_t) rISR9, 0b11101110);
	irasytiIDTirasus(&pertr_irasai[10], (uint64_t) tss_klaida, 0b10001110);
	// irasytiIDTirasus(&pertr_irasai[11], (uint64_t) rISR11, 0b11101110);
	// irasytiIDTirasus(&pertr_irasai[12], (uint64_t) rISR12, 0b11101110);
	// irasytiIDTirasus(&pertr_irasai[12], (uint64_t) rISR13, 0b11101110);
	irasytiIDTirasus(&pertr_irasai[14], (uint64_t) puslapiavimo_klaida, 0b10001110);
	// irasytiIDTirasus(&pertr_irasai[16], (uint64_t) rISR16, 0b11101110);
	// irasytiIDTirasus(&pertr_irasai[17], (uint64_t) rISR17, 0b11101110);
	// irasytiIDTirasus(&pertr_irasai[18], (uint64_t) rISR18, 0b11101110);
	// irasytiIDTirasus(&pertr_irasai[19], (uint64_t) rISR19, 0b11101110);
	// irasytiIDTirasus(&pertr_irasai[20], (uint64_t) rISR20, 0b11101110);
	// irasytiIDTirasus(&pertr_irasai[21], (uint64_t) rISR21, 0b11101110);
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
