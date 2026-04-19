void _start()
{
	for(;;)
	{
		int a = 5;
		int b = 6;
		int c = a + b;
		asm volatile("mov $0, %rax\n" "syscall\n");
		asm volatile("mov $1, %rax\n" "mov $500000000, %rdi\n" "syscall\n");
	}
}
