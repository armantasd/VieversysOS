#include<stdint.h>
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
