#include<stdint.h>
#define NULL 0x0

#define HEAP_PRAD 0xFFFFFFFFFFD00000
#define HEAP_PAB 0xFFFFFFFFFFE00000
#define PHEAP_PRAD 0xFFFFFFFFFFC00000
#define PHEAP_PAB 0xFFFFFFFFFFD00000

struct p_lentele {
	uint64_t irasas[512];
}__attribute__((packed));

struct blokas{
	struct blokas *praitas;
	struct blokas *kitas;
	int dydis;
}__attribute__((packed));


typedef struct blokas blokas;
typedef struct p_lentele p_lentele;

void irasyti_baitus(uint8_t* ptr, uint8_t skaicius, int baitu_sk);
void irasyti_i_porta(uint16_t portas, uint8_t reiksme);
uint8_t skaityti_porta(uint16_t portas);
void* malloc(int baitai);
void free(void* ptr);
void InicijuotiAlloc();
p_lentele* palloc();
void pfree(p_lentele* ptr);
