#include<stdint.h>

typedef struct {
    int PID;
    int PPID;
    uint8_t busena;
    void* kst;
} procesas;
void Inicijuoti_procesus();
void Paleisti_init_demona(uint16_t fptr);
void Ant_laiko_pabaigos();
