
global i686_outb
i686_outb:
    [bits 32]
    mov dx, [esp + 4]
    mov al, [esp + 8]
    out dx, al
    ret
global i686_outw
i686_outw:
    [bits 32]
    mov dx,[esp+4]
    mov ax,[esp+8]
    out dx,ax
    ret
global i686_inb
i686_inb:
    [bits 32]
    mov dx, [esp + 4]
    xor eax, eax
    in al, dx
    ret
global i686_inw
i686_inw:
    [bits 32]
    mov dx, [esp + 4]
    xor eax, eax
    in ax, dx
    ret
; ok but what if we want to do these operations a ton of times?
global i686_outsw
i686_outsw:
    [bits 32]
    mov dx,[esp+4]
    mov esi,[esp+12]
    mov ecx, [esp+16]
    rep outsw
    pop esi
    ret
global i686_insw
i686_insw:
    [bits 32]
    push edi
    mov dx, [esp + 8]
    mov edi, [esp + 12]
    mov ecx, [esp + 16]
    rep insw
    pop edi
    ret
global i686_io_wait
i686_io_wait:
    mov al,0
    out 0x80,al
    ret
global asm
asm:
    sti
