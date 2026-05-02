#include<stdint.h>
#ifndef VIEV_H
#define VIEV_H

uint64_t atidaryti(char* vieta);
void uzdaryti(uint64_t fd);
uint64_t paleisti(uint64_t prog_fd);
void skaityti(uint64_t fd, char* buf, uint64_t dydis);
void rasyti(uint64_t fd, char* buf, uint64_t dydis);
void palaukti();
void cyp();
void iseiti();
bool ar_gyvas(int pid);

#endif
