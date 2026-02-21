#include<stdint.h>
#define NULL 0x0

void irasyti_baitus(uint8_t* ptr, uint8_t skaicius, int baitu_sk);
void irasyti_i_porta(uint16_t portas, uint8_t reiksme);
uint8_t skaityti_porta(uint16_t portas);
void* malloc(int baitai);
void free(void* ptr);
void InicijuotiAlloc();
