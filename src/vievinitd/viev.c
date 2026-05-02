#include "viev.h"
#include<stdbool.h>

uint64_t atidaryti(char* vieta)
{
	uint64_t rezultatas;
	asm volatile ("mov $2, %%rax\n" "mov %0, %%rdi\n" "syscall\n" "mov %%rax, %1" : "=r" (rezultatas) : "r" (vieta) : "rax", "rdi");
	return rezultatas;
}
void uzdaryti(uint64_t fd)
{
	asm volatile("mov $3, %%rax\n" "mov %0, %%rdi\n" "syscall" : : "r" (fd) : "rax", "rdi");
}
uint64_t paleisti(uint64_t prog_fd)
{
	uint64_t pid;
	asm volatile("mov $7, %%rax\n" "mov %0, %%rdi\n" "syscall\n" "mov %%rax, %1" : "=r" (pid) : "r" (prog_fd) : "rax", "rdi");
	return pid;
}
void skaityti(uint64_t fd, char* buf, uint64_t dydis)
{
	asm volatile("mov $0, %%rax\n" "mov %0, %%rdi\n" "mov %1, %%rsi\n" "mov %2, %%rdx\n" "syscall" : : "r" (fd), "r" (buf), "r" (dydis) : "rax", "rdi", "rsi", "rdx");
}
void rasyti(uint64_t fd, char* buf, uint64_t dydis)
{
	asm volatile("mov $1, %%rax\n" "mov %0, %%rdi\n" "mov %1, %%rsi\n" "mov %2, %%rdx\n" "syscall" : : "r" (fd), "r" (buf), "r" (dydis) : "rax", "rdi", "rsi", "rdx");
}
void palaukti()
{
	asm volatile("mov $6, %rax\n" "syscall");
}
void cyp()
{
	asm volatile("mov $4, %rax\n" "syscall");
}
void iseiti()
{
	asm volatile("mov $8, %rax\n" "syscall");
}
bool ar_gyvas(int pid)
{
	uint64_t rez;
	asm volatile("mov $9, %%rax\n" "mov %0, %%rdi\n" "syscall\n" "mov %%rax, %1" : "=r" (rez) : "r" ((uint64_t)pid) : "rax", "rdi");
	return (bool)rez;
}
