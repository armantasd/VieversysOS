#include<stdint.h>
#include"stdlib.h"
#include"vektoriai.h"

const float R = 1.66;

vektorius* Vektorius(int elemento_dydis)
{
	vektorius* naujas_vkt = malloc(sizeof(vektorius));
	naujas_vkt->elemento_dydis = elemento_dydis;
	naujas_vkt->rezervuota = elemento_dydis;
	naujas_vkt->elementu_sk = 0;
	naujas_vkt->reiksmes = malloc(naujas_vkt->rezervuota);
	return naujas_vkt;
}

void push_back(vektorius* vektorius, void* elementas)
{
	if(vektorius->elementu_sk + vektorius->elemento_dydis > vektorius->rezervuota)
	{
		int nauja_rez = (int) vektorius->rezervuota * R + vektorius->elemento_dydis;
		vektorius->reiksmes = realloc(vektorius->reiksmes, nauja_rez);
		vektorius->rezervuota = nauja_rez;
	}
	for(int i = 0; i < vektorius->elemento_dydis; i++)
	{
		uint8_t* reiksmes = vektorius->reiksmes;
		reiksmes[i + vektorius->elementu_sk * vektorius->elemento_dydis] = *((uint8_t*) elementas + i);
	}
	vektorius->elementu_sk += 1;
}
void ideti(vektorius* vektorius, void* elementas, int indeksas)
{
	if(vektorius->elementu_sk + vektorius->elemento_dydis > vektorius->rezervuota)
	{
		int nauja_rez = (int) vektorius->rezervuota * R + vektorius->elemento_dydis;
		vektorius->reiksmes = realloc(vektorius->reiksmes, nauja_rez);
		vektorius->rezervuota = nauja_rez;
	}
	for(int i = vektorius->elementu_sk - 1; i >= indeksas; i--)
	{
		for(int j = 0; j < vektorius->elemento_dydis; j++)
		{
			*((uint8_t*) vektorius->reiksmes + (i+1) * vektorius->elemento_dydis + j) = *((uint8_t*) vektorius->reiksmes + i * vektorius->elemento_dydis + j);
		}
	}
	for(int j = 0; j < vektorius->elemento_dydis; j++)
	{
		*((uint8_t*) vektorius->reiksmes + indeksas * vektorius->elemento_dydis + j) = *((uint8_t*) elementas + j);
	}
	vektorius->elementu_sk += 1;
}
void istrinti(vektorius* vektorius, int indeksas)
{
	for(int i = indeksas; i < vektorius->elementu_sk; i++)
	{
		for(int j = 0; j < vektorius->elemento_dydis; j++)
		{
			*((uint8_t*) vektorius->reiksmes + i * vektorius->elemento_dydis + j) = *((uint8_t*) vektorius->reiksmes + (i+1) * vektorius->elemento_dydis + j);
		}
	}
	vektorius->elementu_sk -= 1;
}
void rezervuoti(vektorius* vektorius, int dydis)
{
	vektorius->rezervuota = dydis;
	vektorius->reiksmes = realloc(vektorius->reiksmes, dydis);
}
void pop(vektorius* vektorius, void* kur_irasyti)
{
	vektorius->elemento_dydis -= 1;
	if (kur_irasyti != 0)
	{
		for(int i = 0; i < vektorius->elemento_dydis; i++)
		{
			*((uint8_t*) kur_irasyti + i) = *((uint8_t*) vektorius->reiksmes + i + vektorius->elementu_sk * vektorius->elemento_dydis);
		}
	}
}
void atlaisvinti_vkt(vektorius* vektorius)
{
	free(vektorius->reiksmes);
	free(vektorius);
}
