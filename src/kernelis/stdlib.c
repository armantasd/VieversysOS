#include<stdint.h>
#define HEAP_PRAD 0xFFFFFFFFFFD00000
#define HEAP_PAB 0xFFFFFFFFFFE00000
#define NULL 0x0

struct blokas{
	struct blokas *praitas;
	struct blokas *kitas;
	int dydis;
};
typedef struct blokas blokas;
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
uint8_t skaityti_porta(uint16_t portas)
{
	uint8_t rezultatas;
	asm volatile ("inb %1, %0" : "=a" (rezultatas) : "dN" (portas));
	return rezultatas;
}

void InicijuotiAlloc()
{
	blokas* Pirmas_blokas = (blokas*) HEAP_PAB;
	Pirmas_blokas->praitas = (blokas*) HEAP_PRAD;
	Pirmas_blokas->kitas = 0;
	Pirmas_blokas->dydis = 0;
}

void* malloc(int baitai)
{
	blokas* dabartinis_b = (blokas*) HEAP_PAB;
	while (true)
	{
		blokas* praitas_b = dabartinis_b->praitas;
		unsigned long int tarpas = praitas_b - dabartinis_b + sizeof(blokas) + dabartinis_b->dydis;
		if (tarpas < baitai + sizeof(blokas))
		{
			if (praitas_b == (blokas*) HEAP_PRAD)
			{
				return NULL;
			}
			dabartinis_b = praitas_b;
			continue;
		}
		else
		{
			blokas* n_dabartinis_b = praitas_b;
			n_dabartinis_b->kitas = sizeof(blokas) + n_dabartinis_b->dydis + n_dabartinis_b;
			blokas* naujas_b = n_dabartinis_b->kitas;
			naujas_b->dydis = baitai;
			naujas_b->praitas = n_dabartinis_b;
			naujas_b->kitas = dabartinis_b;
			praitas_b->kitas = naujas_b;
			return (void*) naujas_b + sizeof(blokas);
		}
	}
}
void free(void* ptr)
{
	blokas* blk = ptr - sizeof(blokas);
	blokas* praitas_b = blk->praitas;
	blokas* kitas_b = blk->kitas;
	praitas_b->kitas = kitas_b;
	kitas_b->praitas = praitas_b;
}
