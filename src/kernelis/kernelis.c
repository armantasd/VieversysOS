#include"stdio.h"
#include"stdlib.h"
#include"idt.h"
#include"laikrodis.h"

void kernel_main()
{
	print(" ", 0);
	struct pertr_lent_ptr pertraukimai;
	struct pertr_irasas pertr_irasai[256];
	InicijuotiIDTirasus(pertr_irasai);
	pertraukimai.limitas = sizeof(struct pertr_irasas) * 256 - 1;
	pertraukimai.adresas = (uint64_t) &pertr_irasai;
		
	asm volatile ("lidt %0\n" "sti\n" : : "m" (pertraukimai) : "memory");
	//Inicijuoti_Laikrodi(100);
	return;
}
void Ant_laikmacio_pabaigos()
{
	return;
}
void Klaviaturos_pertraukymas()
{
	return;
}
