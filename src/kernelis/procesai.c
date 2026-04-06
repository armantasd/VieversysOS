#include"vektoriai.h"
#include"procesai.h"
#include<stdint.h>
#include"stdlib.h"
#include"gdt.h"

static vektorius* procesai;
static int veikiantis_procesas;
static int procesu_sk;

void Inicijuoti_procesus()
{
    procesai = Vektorius(sizeof(procesas));
};
void Paleisti_init_demona(uint16_t fptr)
{
    procesas initd;
    initd.PID = 1;
    initd.PPID = 0;
    initd.busena = 0;
    void* atmintis = valloc(2);
    initd.kst = atmintis + 4096;    
}
