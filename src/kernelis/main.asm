[bits 32]
global _start
extern kernel_main
section .text
_start:
	lgdt [sgdt_descriptor]

	mov edx, cr4
	or  edx, (1 << 5)
	mov cr4, edx

	mov ecx, 0xC0000080
	rdmsr
	or  eax, (1 << 8)
	wrmsr

	mov eax, PML4
	mov cr3, eax
	mov eax, cr0
	or eax, (1 << 31)
	mov cr0, eax
	jmp 0x08:long_mode
	
long_mode:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	call kernel_main
    hlt
	jmp $
	
section .data
sgdt_start:
	; null descriptor
	dq 0
	dq 0
	; kernel code segment
	dw 0xffff
	dw 0
	db 0
	db 0x9a
	db 0xaf
	db 0
	dq 0
	; kernel data segment
	dw 0xffff
	dw 0
	db 0
	db 0x92
	db 0xaf
	db 0
	dq 0
	; user mode data segment
	dw 0xffff
	dw 0
	db 0
	db 0xf2
	db 0xaf
	db 0
	dq 0
	; user mode code segment
	dw 0xffff
	dw 0
	db 0
	db 0xfa
	db 0xaf
	db 0
	dq 0
	; tss segment
	dw 0x93
	dw tss
	db 0
	db 0x89
	db 0x0
	db 0
	dq 0
sgdt_end:

sgdt_descriptor:
	dw sgdt_end - sgdt_start - 1
	dd sgdt_start

tss:
	times 148 db 0

section .pgtables
global PML4
align 4096
PML4:
    dq PDPT + 0x03
    times 511 dq 0
align 4096
PDPT:
    dq PD + 0x03
    times 511 dq 0

align 4096
PD:
    dq PT + 0x03
    times 511 dq 0

align 4096
PT:
	times 7 dq 0
	dq 0x7003
	dq 0x8003
	dq 0x9003
	dq 0xa003
	dq 0xb003
	dq 0xc003
	dq 0xd003
	dq 0xe003
	dq 0xf003
