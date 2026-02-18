#include"stdlib.h"
#include<stdint.h>

void Inicijuoti_Laikrodi(int daznis)
{
	uint32_t par_daznis = 1193180/daznis;
	irasyti_i_porta(0x43, 0x36);
	irasyti_i_porta(0x40, (uint8_t)(par_daznis & 0xff));
	irasyti_i_porta(0x40, (uint8_t)(par_daznis >> 8) & 0xff);
	// atmaskuojam IRQ 1 ir IRQ 0
	irasyti_i_porta(0x21, 0b11111100);
	irasyti_i_porta(0xA1, 0b11111100);
}
