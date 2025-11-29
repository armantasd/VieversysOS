org 0x7c00
bits 16
jmp short start ; I had to do two jumps because jump short wasn't enough
nop

; FAT12 header (too lazy to translate that, it's a FAT12 header anyways lol)
bdb_oem: db 'MSWIN4.1'
bdb_sektoriaus_dydis: dw 512
bdb_sektoriai_per_clusteri: db 1
bdb_reservuoti_sektoriai: dw 1
bdb_fat_skaicius: db 2
bdb_dir_irasu_sk: dw 224
bdb_sektoriu_sk: dw 2880
bdb_media_desc_tipas: db 0xf0
bdb_sektoriai_per_fat: dw 9
bdb_sektoriai_per_trasa: dw 18
bdb_pusiu_sk: dw 2
bdb_paslepti_sektoriai: dd 0
bdb_dideliu_sektoriu_sk: dd 0
; boot irasas
ebr_drive_sk: db 0
db 0
ebr_zyme: db 0x29
ebr_turio_id: db 67, 67, 67, 67
ebr_turio_zyme: db 'VIEVERSYSOS'
ebr_sistemos_zyme: db 'FAT12   '

start:
	jmp main

; si - string pointer
print_msg:
	push si
	push ax
print_loop:
	lodsb
	cmp al, 0
	jz end_print_loop
	mov ah, 0x0e
	int 10h
	jmp print_loop
end_print_loop:
	pop ax
	pop si
	ret
;

; - ibrahim's disk read chs
read_sector_chs:
    pusha
    mov ah, 0x02
    int 0x13
    jc .error
    popa
    clc
    ret
.error:
	popa
    stc
    ret
read_fail:
	mov si, disk_error_msg
	call print_msg
	hlt
	jmp $
;

disk_reset:
	cmp si, 0
	jz read_fail
	mov ah, 0x0E
	mov al, '.'
	int 10h
	mov ah, 0
	mov dl, 0
	int 10h
	jc read_fail
	dec si
	jmp disk_retry

; on the stack: buffer ptr, lba, how many blocks to read
disk_read:
	pusha
	mov si, 3
disk_retry:
	mov bp, sp
	mov ax, word [bp+20] ; 8 16-bit registers + return address (holds lba)
	mov bx, word [bp+22] ; same thing + lba (holds buffer)
	xor dx, dx
	div word [bdb_sektoriai_per_trasa] ; divide by sectors per track
	mov cl, dl
	inc cl
	xor dx, dx
	div word [bdb_pusiu_sk] ; divide by the number of heads
	mov ch, al
	shl ax, 8
	or cl, al
	shl dx, 8
	mov ah, 2
	mov al, byte [bp+18]
	call read_sector_chs
	jc disk_reset
	popa
	ret
;
main:
	mov ax, 0
	mov bx, 0
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 0x7c00
	; Load FAT and root dir

	push FAT_tables
	push 1 			; position of FAT table in disk
	push 18
	call disk_read
	add sp, 6

	push root_dir
	push 19 		; position of root entries in disk
	push 14
	call disk_read
	add sp, 6

	; Look for kernel
	mov si, root_dir
	mov di, kernel_name
	mov dx, 12
strcmp_loop:
	cmp dx, 0
	jz match
	lodsb
	mov bl, byte [di]
	inc di
	dec dx
	cmp al, bl
	jz strcmp_loop
	jmp not_match
not_match:
	add di, dx
	sub di, 12
	add si, dx
	add si, 20
	jmp strcmp_loop
match:
	add si, 15
	mov ax, word [si]
	
	; Load clusters (ax - cluster)
	mov bp, kernel
clstrld_loop:
	; 1.LOAD DATA:
	mov di, ax
	; quick lba calculation
	sub ax, 2
	shl ax, 1
	add ax, 33 ; 33 base data for FAT, reserved and root dir
	;
	push bp
	push ax
	push 2
	call disk_read
	add sp, 6
	; 2.FIND CLUSTER ON FAT:
	mov si, FAT_tables
	mov ax, di
	mov bx, di
	mov dx, ax
	and dx, 1
	shr ax, 1
	add ax, bx
	cmp dx, 1
	jz odd
	add si, ax
	lodsw
	and ax, 0b0000111111111111
	jmp done_calc
odd:
	add si, ax
	lodsw
	shr ax, 4
done_calc:
	cmp ax, 0xfff
	jz end_boot
	add bp, 1024
	jc overflow
	jmp clstrld_loop

overflow:
	mov dx, es
	add dx, 512
	mov es, dx
	jmp clstrld_loop

end_boot:
	jmp kernel

welcome_message: db "Labas", 0x0D, 0xA, 0
disk_error_msg: db "Failed to read disk", 0x0D, 0xA, 0
kernel_name: db "KERNELISBIN"

times 510-($-$$) db 0
dw 0AA55h

FAT_tables equ 0x7e00 ; 18 sectors for FAT table
root_dir equ 0xa200 ; 14 sectors for root dir entries
kernel equ 0xb000 ; kernel
