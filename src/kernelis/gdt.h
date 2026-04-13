#pragma once
#include<stdint.h>

typedef struct GDTdeskriptorius GDTdeskriptorius;
typedef struct GDTirasas GDTirasas;
typedef struct GDTSistemosIrasas GDTSistemosIrasas;
typedef struct tss tss;

extern GDTdeskriptorius* gdt_ptr;
extern tss* tss_ptr;

struct GDTdeskriptorius
{
	uint16_t dydis;
	GDTirasas* ptr;
}__attribute__((packed));
struct GDTirasas
{
	uint16_t dydis;
	uint16_t adresas_z;
	uint8_t adresas_v;
	uint8_t prieeigos_baitas;
	uint8_t veliavos;
	uint8_t adresas_a;
}__attribute__((packed));
struct GDTSistemosIrasas
{
	uint16_t dydis;
	uint16_t adresas_z;
	uint8_t adresas_v;
	uint8_t prieeigos_baitas;
	uint8_t veliavos;
	uint8_t adresas_a;
	uint32_t adresas_p;
	uint32_t rezervuota;
}__attribute__((packed));
struct tss
{
	uint32_t rezervuota;
	uint64_t rsp0;
	uint64_t rsp1;
	uint64_t rsp2;
	uint64_t rezervuota2;
	uint64_t ist1;
	uint64_t ist2;
	uint64_t ist3;
	uint64_t ist4;
	uint64_t ist5;
	uint64_t ist6;
	uint64_t ist7;
	uint64_t ist8;
	uint16_t rezervuota3;
	uint16_t iopb;
}__attribute__((packed));

void InicijuotiGDT();
