#include<stdint.h>
#include"idt.h"

void irasytiIDTirasus(struct pertr_irasas* irasas, uint64_t ISRadresas, uint8_t veliavos)
{
	uint16_t zemas_ofsetas = ISRadresas & 0xffff;
	uint16_t vid_ofsetas = (ISRadresas >> 16) & 0xffff;
	uint32_t aukstas_ofsetas = ISRadresas >> 32;
	irasas->zemas_ofsetas = zemas_ofsetas;
	irasas->kodo_selektorius = 0x08;
	irasas->rezervuota = 0;
	irasas->veliavos = veliavos;
	irasas->vid_ofsetas = vid_ofsetas;
	irasas->aukstas_ofsetas = aukstas_ofsetas;
	irasas->rezervuota2 = 0;
};
