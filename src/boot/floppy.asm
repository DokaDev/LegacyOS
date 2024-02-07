[org 0x00]
[bits 16]

SECTION .text
jmp 0x07c0:entry

entry:
    mov ax, 0x07c0
    mov ds, ax

    mov ax, 0xb800
    mov es, ax

    ; setup stack pointer(ss)
    mov ax, 0x0000
    mov ss, ax
    mov sp, 0xfffe
    mov bp, 0xfffe

clearScreen:
    mov si, 0

    loop_clearScreen:
        mov byte[es:si], 0
        mov byte[es:si+1], 0x0f
        add si, 2

        cmp si, 4000 ; 80*25*2
        jl loop_clearScreen

diskLoad:
    ; floppy

    init_disk:
        mov ah, 0   ; reset disk
        mov dl, 0   ; drive 0
        int 0x13    ; call bios
        jc handle_diskError

    ; read sector from disk
    ; set the address to copy disk data => 0x10000
    ; [es:bx] = [0x1000:0000]
    mov si, 0x1000  ; segment
    mov es, si
    mov bx, 0x0000  ; offset

    mov di, word[totalSectorCount]  ; set the number of sectors to read

    read:
        ; read sector from disk
        cmp di, 0   ; if di == 0, then end_read
        je end_read

        mov ah, 0x02 ; read disk
        mov al, 0x01 ; read 1 sector
        mov ch, byte[trackNumber]   ; track number
        mov cl, byte[sectorNumber]  ; sector number
        mov dh, byte[headNumber]    ; head number
        mov dl, 0   ; drive 0
        int 0x13    ; call bios
        jc handle_diskError

        ; move to next sector
        add si, 512 ; 512 bytes per sector
        mov es, si

        mov al, byte[sectorNumber]
        add al, 1
        mov byte[sectorNumber], al
        cmp al, 37 ; 0x25
        jl read

        xor byte[headNumber], 1     ; toggle head number
        mov byte[sectorNumber], 1   ; reset sector number

        cmp byte[headNumber], 0     ; if head number == 0, then read next track
        jne read

        add byte[trackNumber], 1    ; read next track
        jmp read

    end_read:
        jmp 0x1000:0x0000 ; jump to the protected mode kernel

    handle_diskError:
        jmp $

totalSectorCount: dw 0x02
protectedModeKernelSectorCount: dw 0x02
bootStrapProcessor: db 0x01
startGraphicMode: db 0x01

sectorNumber: db 0x02
headNumber: db 0x00
trackNumber: db 0x00

times 510-($-$$) db 0
dw 0xaa55