void _start()
{
	char failo_vard[] = "/VINITD.ELF";
	asm volatile ("mov $2, %%rax\n" "mov %0, %%rdi\n" "syscall" : : "r" (failo_vard) : "rax", "rdi");
	for(;;);
}
