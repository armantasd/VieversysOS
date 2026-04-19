extern dabartinis_p
extern extra_reg
extern vykdyti_si
extern tss_ptr

section .text

%macro popall 0
pop r15
pop r14
pop r13
pop r12
pop r11
pop r10
pop r9
pop r8
pop rdi
pop rsi
pop rbp
pop rbx
pop rdx
pop rcx
pop rax
%endmacro
%macro pushall 0
push rax
push rcx
push rdx
push rbx
push rbp
push rsi
push rdi
push r8
push r9
push r10
push r11
push r12
push r13
push r14
push r15
%endmacro

global Inicijuoti_sistemos_iskvietimus
Inicijuoti_sistemos_iskvietimus:
	mov ecx, 0xC0000080
	rdmsr
	or rax, 1
	wrmsr
	mov rax, 0
	mov rdx, 0x130008
	mov rcx, 0xC0000081
	wrmsr
	mov rax, sisiskv
	mov rdx, sisiskv
	shr rdx, 32
	and rax, 0xffffffff
	mov rcx, 0xC0000082
	wrmsr
	mov rax, 0x200
	mov rdx, 0
	mov rcx, 0xC0000084
	wrmsr
	ret

global sisiskv
sisiskv:
	mov [extra_reg], rax
	mov rax, [dabartinis_p]
	mov [rax + 33], rsp
	mov rax, [tss_ptr]
	mov rsp, [rax + 4]
	pushall
	sti
	mov rcx, r10
	call vykdyti_si
	cli
	popall
	mov rax, [tss_ptr]
	mov [rax + 4], rsp
	mov rax, [dabartinis_p]
	mov rsp, [rax + 33]
	mov rax, [extra_reg]
	o64 sysret
