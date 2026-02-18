#include<stdint.h>

struct pertr_irasas {
	uint16_t zemas_ofsetas;
	uint16_t kodo_selektorius;
	uint8_t rezervuota;
	uint8_t veliavos;
	uint16_t vid_ofsetas;
	uint32_t aukstas_ofsetas;
	uint32_t rezervuota2;
}__attribute__((packed));

struct pertr_lent_ptr {
	uint16_t limitas;
	uint64_t adresas;
}__attribute__((packed));

void irasytiIDTirasus(struct pertr_irasas* irasas, uint64_t ISRadresas, uint8_t veliavos);
void InicijuotiIDTirasus(struct pertr_irasas pertr_irasai[]);
