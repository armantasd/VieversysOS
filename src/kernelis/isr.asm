extern print
; print funkcija - void print(char* buffer, void* argv);
extern Ant_laikmacio_pabaigos
extern Klaviaturos_pertraukymas
extern irasyti_i_porta

section .data
	nulis: db 0
	dalklzin: db  "Dalybos klaida. ", 0x0
	netinszin: db "Neteisinga instrukcija. ", 0x0
	tsszin: db "Neteisingas tss. ", 0x0
	pslzin: db "Neteisingas puslapiavimas. ", 0x0
	rezzin: db "Rezervuotas pertraukymas. ", 0x0
	pabzin: db "Sistema nebegali veikti toliau.", 0x0

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

pertraukymo_aptvarkymas:
	
	mov ax, 0
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	ret
	
global dalybos_klaida
dalybos_klaida:
	cli
	pushall
	call pertraukymo_aptvarkymas
	mov rdi, dalklzin
	mov rsi, nulis
	call print
	popall
	jmp pabaiga
global neteisinga_ins
neteisinga_ins:
	cli
	pushall
	call pertraukymo_aptvarkymas
	mov rdi, netinszin
	mov rsi, nulis
	call print
	popall
	jmp pabaiga
global tss_klaida
tss_klaida:
	cli
	pushall
	call pertraukymo_aptvarkymas
	mov rdi, tsszin
	mov rsi, rsp
	add rsi, 8
	call print
	popall
	sti
	iretq
global puslapiavimo_klaida
puslapiavimo_klaida:
	cli
	pushall
	call pertraukymo_aptvarkymas
	mov rdi, pslzin
	mov rsi, rsp
	add rsi, 8
	call print
	popall
	jmp pabaiga

%macro rISR 1
	global rISR%1
	rISR%1:
		cli
		pushall
		call pertraukymo_aptvarkymas
		mov rdi, rezzin
		mov rsi, 0
		call print
		popall
		sti
		iretq

%endmacro

rISR 1
rISR 2
rISR 3
rISR 4
rISR 5
rISR 7
rISR 8
rISR 9
rISR 11
rISR 12
rISR 13
rISR 16
rISR 17
rISR 18
rISR 19
rISR 20
rISR 21


global laikmacio_pab_isr
laikmacio_pab_isr:
	cli
	pushall
	call pertraukymo_aptvarkymas
	call Ant_laikmacio_pabaigos
	mov rdi, 0x20
	mov rsi, 0x20
	call irasyti_i_porta
	popall
	sti
	iretq

global klaviaturos_pertr_isr
klaviaturos_pertr_isr:
	cli
	pushall
	call pertraukymo_aptvarkymas
	call Klaviaturos_pertraukymas
	mov rdi, 0x20
	mov rsi, 0x20
	call irasyti_i_porta
	popall
	sti
	iretq

pabaiga:
	mov rdi, pabzin + 1
	mov rsi, 0
	call print
	hlt
	jmp $
