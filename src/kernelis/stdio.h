#pragma once
#include<stdint.h>
#include<stdarg.h>
#include"vektoriai.h"

extern char* stdivestis;
extern int stdini;

void prtchr(char c, int index);
vektorius* itoa(int a);
void print(char* tekstas, ...);
