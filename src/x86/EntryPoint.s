[org 0]
[bits 16]

SECTION .text

entry:
    mov ax, 0x1000
    mov ds, ax  ; set data segment to 0x1000
    mov es, ax  ; set extra segment to 0x1000

    cli
    lgdt [gdt_descriptor]  ; load the GDT

    mov eax, 0x4000003b
    mov cr0, eax  ; set PE, PG, and WP in CR0

    jmp dword 0x08:(protected_mode - $$ + 0x10000)  ; jump to protected mode

[bits 32]
protected_mode:
    mov ax, 0x10
    mov ds, ax  ; set data segment to 0x10
    mov es, ax  ; set extra segment to 0x10
    mov fs, ax  ; set fs to 0x10
    mov gs, ax  ; set gs to 0x10

    ; set up stack
    mov ss, ax
    mov esp, 0xFFFE ; set stack pointer to 0xFFFE
    mov ebp, 0xFFFE ; set base pointer to 0xFFFE

    jmp dword 0x08: 0x10200 ; enter c code

align 8, db 0

dw 0    ; 
gdt_descriptor:
    dw gdt_end - gdt_start - 1      ; limit (size of GDT)
    dd (gdt_start - $$ + 0x10000)   ; base of GDT

gdt_start:
    null_descriptor:    ; null descriptor
        dw 0
        dw 0
        db 0
        db 0
        db 0
        db 0

    code_descriptor:
        dw 0xffff   ; limit[0:15]
        dw 0        ; base[0:15]
        db 0        ; base[16:23]
        db 0x9a     ; access P=1, DPL=0, S=1, type=0xa
        db 0xcf     ; G=1, D/B=1, L=0, AVL=0, limit[16:19]=0xf
        db 0        ; base[31:24]

    data_descriptor:
        dw 0xffff   ; limit[0:15]
        dw 0        ; base[0:15]
        db 0        ; base[16:23]
        db 0x92     ; access P=1, DPL=0, S=1, type=0x2
        db 0xcf     ; G=1, D/B=1, L=0, AVL=0, limit[16:19]=0xf
        db 0        ; base[31:24]
gdt_end:

; 보호 모드로 전환되었다는 메시지
SWITCHSUCCESSMESSAGE: db 'Switch To Protected Mode Success~!!', 0

times 512-($-$$) db 0  ; pad the GDT to 512 bytes