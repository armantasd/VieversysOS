typedef struct vektorius vektorius;
#include<stdint.h>

struct vektorius
{
	int elemento_dydis;
	int rezervuota;
	int elementu_sk;
	void* reiksmes;
};
vektorius* Vektorius(int elemento_dydis);
void push_back(vektorius* vektorius, void* elementas);
void ideti(vektorius* vektorius, void* elementas, int indeksas);
void istrinti(vektorius* vektorius, int indeksas);
void rezervuoti(vektorius* vektorius, int dydis);
void pop(vektorius* vektorius, void* kur_irasyti);
void atlaisvinti_vkt(vektorius* vektorius);
