#include<stdint.h>

typedef struct procesas procesas;

extern procesas* dabartinis_p;
extern uint64_t extra_reg;

struct procesas {
    int PID;
    int PPID;
    uint8_t busena;
    void* kst;
    void* cr3;
   	procesas* kitas_proc;
}__attribute__((packed));
void Inicijuoti_procesus();
void Paleisti_init_demona(uint16_t fptr);
void Ant_laiko_pabaigos();

typedef struct {
	char indent[16];
	uint16_t tipas;
	uint16_t arch;
	uint32_t versija;
	void* pradz;
	uint64_t segm;
	uint64_t sekc;
	uint32_t veliavos;
	uint16_t eh_dydis;
	uint16_t segm_dyd;
	uint16_t segm_sk;
	uint16_t sekc_dyd;
	uint16_t sekc_sk;
	uint16_t shsti;
} elf_headeris;
typedef struct {
	uint32_t tipas;
	uint32_t veliavos;
	uint64_t adresas;
	uint64_t vadresas;
	uint64_t padresas;
	uint64_t dydis;
	uint64_t atmdydis;
	uint64_t lygiavimas;
} segmentas;
