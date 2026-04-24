void _start()
{
	char failo_vard[] = "/VINITD.ELF";
	long fd;
	asm volatile ("mov $2, %%rax\n" "mov %0, %%rdi\n" "syscall\n" "mov %%rax, %1" : "=A" (fd) : "r" (failo_vard) : "rax", "rdi");
	asm volatile ("mov $7, %%rax\n" "mov %0, %%rdi\n" "syscall" : : "r" (fd) : "rax", "rdi");
	asm volatile ("mov $4, %rax\n" "syscall");
	for(;;);
}
