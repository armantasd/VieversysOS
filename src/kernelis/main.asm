[bits 32]
global _start
extern kernel_main
section .text

_start:
	call init_page_tables

	lgdt [sgdt_descriptor]
	
	mov edx, cr4
	or  edx, (1 << 5)
	mov cr4, edx
	mov eax, 0
	mov ecx, 0xC0000080
	rdmsr
	or  eax, (1 << 8)
	wrmsr

	mov eax, pml4
	mov cr3, eax
	mov eax, cr0
	or eax, (1 << 31)
	mov cr0, eax
	
	jmp 0x08:long_mode

[bits 64]
long_mode:
	mov ax, 0
	mov ss, ax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	; TO DO: fix it

	; page higher half at address  0xFFFFFFFFFFE00000 (last page dir)
	; PDPT
	mov qword [hhpdpt + 4088], 0x105003 ; last pdpt
	; PD
	mov qword [hhpdpt + 8184], 0x106003 ; last pd
	mov rax, 0xb003
	mov rbx, 0
page_loop:
	cmp rbx, 0x95
	je end_page_loop
	mov qword [hhpt + rbx * 8], rax
	add rax, 4096
	inc rbx
	jmp page_loop
end_page_loop:

	; map stack space
	mov rax, 0x0003
	mov rbx, 0
spage_loop:
	cmp rbx, 7 ; six seven pages for the stack
	je send_page_loop
	mov qword [hhpt + rbx * 8 + 4040], rax
	add rax, 4096
	inc rbx
	jmp spage_loop
send_page_loop:
	
	mov rsp, 0xFFFFFFFFFFFFFFFF

	call vir_kernel
    hlt
	jmp $

init_page_tables:
	; pml4
	mov ebx, pml4
	add ebx, 4096
	mov ecx, ebx
	or ebx, 3
	mov dword [pml4], ebx
	mov dword [pml4 + 4088], 0x104003 ; higher half kernel
	; pdpt
	mov ebx, ecx
	add ebx, 4096
	mov ecx, ebx
	or ebx, 3
	mov dword [pml4 + 4096], ebx
	; pd
	mov ebx, ecx
	add ebx, 4096
	or ebx, 3
	mov dword [pml4 + 8192], ebx
	; identity map kernel
	mov dword [pt + 0xb * 8], 0xb003
	; map page tables
	mov dword [pt + 0x100 * 8], 0x100003
	mov dword [pt + 0x101 * 8], 0x101003
	mov dword [pt + 0x102 * 8], 0x102003
	mov dword [pt + 0x103 * 8], 0x103003
	; map extra space (for higher half pages)
	mov dword [pt + 0x104 * 8], 0x104003
	mov dword [pt + 0x105 * 8], 0x105003
	mov dword [pt + 0x106 * 8], 0x106003
	ret
	
section .data
sgdt_start:
	; null descriptor
	dq 0
	; kernel code segment
	dw 0xffff
	dw 0
	db 0
	db 0x9c
	db 0xaf
	db 0
	; tss segment
	dw 0x93
	dw tss
	db 0
	db 0x89
	db 0
	db 0
	dq 0
sgdt_end:

sgdt_descriptor:
	dw sgdt_end - sgdt_start - 1
	dd sgdt_start

tss:
	times 148 db 0
tss_base equ tss

;section .pgtables
;global PML4
;align 4096
;PML4:
;    dq PDPT + 0x03
;    times 511 dq 0
;align 4096
;PDPT:
;    dq PD + 0x03
;    times 511 dq 0
;
;align 4096
;PD:
;    dq PT + 0x03
;    times 511 dq 0
;
;align 4096
;PT:
;	times 7 dq 0
;	dq 0x7003
;	dq 0x8003
;	dq 0x9003
;	dq 0xa003
;	dq 0xb003
;	dq 0xc003
;	dq 0xd003
;	dq 0xe003
; 	dq 0xf003

pml4 equ 0x100000
pt equ 0x103000
hhpdpt equ 0x104000
hhpt equ 0x106000
vir_kernel equ kernel_main + 0xFFFFFFFFFFE00000 - 0xb000
