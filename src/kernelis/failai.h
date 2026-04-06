#pragma once
#include<stdint.h>

#define RAMDISK 0xffffffffffe35000

uint16_t atidaryti(char* failo_lokacija);
void skaityti(void* isvestis, int baitu_sk, int paslinkimas, uint16_t failas);
bool palyginti_teksta(char* tekstas1, char* tekstas2);

struct direktorija {
	char pav[11];
	uint8_t attr;
	uint8_t rez;
	uint8_t sukurimo_sekundes;
	uint16_t sukurimo_laikas;
	uint16_t sukurimo_data;
	uint16_t atidarimo_data;
	uint16_t nenaudojama;
	uint16_t keitimo_laikas;
	uint16_t keitimo_data;
	uint16_t klasteris;
	uint32_t dydis;
}__attribute__((packed));
typedef struct direktorija direktorija;
