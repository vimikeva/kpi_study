.386p
descriptor struc
    limit_1 dw 0 ; Limit (0 - 15).
    base_1 dw 0 ; Base (0 - 15).
    base_2 db 0 ; Base (16 - 23).
    attr db 0 ; Attributes.
    bt6 db 0 ; Limit_2 (0 - 3), Attributes (4 - 7).
    base_3 db 0 ; Base (24 - 31).
descriptor ends
;----------------------------------------------------------------------
dt_pointer struc
    limit dw 0
    address dd 0
dt_pointer ends
;----------------------------------------------------------------------
attr_code_seg equ 10011010b
attr_data_seg equ 10010010b
attr_ro_data_seg equ 10010000b
attr_data_dpl equ 10110010b
S_sw0   equ 10010110b ;   Writable stack segment
;----------------------------------------------------------------------
_ST segment use16
        db  1000 dup (0)
 Top_stp    equ $
_ST ends
;----------------------------------------------------------------------
gdt segment para public 'data' use16
assume ds: gdt
    selector0 descriptor <>
    descriptor_gdt descriptor <> ; global descriptors
    descriptor_ds descriptor <> ; _data
    descriptor_es descriptor <> ; 0b8000h
    descr_ss   descriptor   <>
    descriptor_undefined descriptor <> 
    descriptor_cs descriptor <> ;code
    descriptor_int_code descriptor <> ; int code
    descriptor_absent descriptor <>
    descriptor_fs descriptor <> ; try
    descriptor_gs descriptor <> ; try
    descr_t7 descriptor <>
    descr_t14 descriptor <>

    gdt_size equ $ - selector0 - 1
gdt ends
;----------------------------------------------------------------------
idt segment para public 'data' use16
    vector = 0

    rept 256
        dw vector * proc_int_size ; çì³ùåííÿ ïðîöåäóðè
        dw offset descriptor_int_code ; ñåëåêòîð äåñêðèïòîðà â GDT
        db 0 ; çàâæäè 0
        db 10001111b ; áàéò àòðèáóò³â - øëþç ïàñòêè; change dpl=1
        dw 0 ; âñ³ ïðîöåäóðè ðîçì³ùóþòüñÿ â ïåðøèõ 64Ê

        vector = vector + 1
    endm
idt ends
;----------------------------------------------------------------------
_readonly_data segment public 'data' use16
    assume fs: _readonly_data
    val dd 0
    readonly_data_size = 4
_readonly_data ends
;----------------------------------------------------------------------
_dpl1_data segment public 'data' use16
	assume gs:_dpl1_data
	just_something dd ?
	dpl_size = $ - _dpl1_data
_dpl1_data ends
;----------------------------------------------------------------------
_data segment public 'data' use16
assume ds: _data

data_start label dt_pointer
    gdt_pointer dt_pointer <>
    idt_pointer dt_pointer <>
    Top_real_mode dd 0

    message db "Hello from protected mode!"
    message_len = $ - message
    data_size = $ - data_start
_data ends
;----------------------------------------------------------------------
_interrupt_code segment para public 'code' use16
    assume cs: _interrupt_code
						
    vector = 0
int_begin:
    pusha
    push   bx
    push   2480
    call   show
    mov ax, vector
    jmp common_int
int_end:
    proc_int_size = int_end - int_begin

    vector = 1
    rept 255
        pusha
        push   bx
        push   2480
        call   show
        mov ax, vector
        jmp common_int
        vector = vector + 1
    endm

    int_msg db 'INTERRUPT N'
common_int:
    cld
    mov  cl, 10
    div cl ; ìîëîäøà öèôðà â ah
    or ah, 30h ; ôîðìóâàííÿ òåêñòîâîãî êîäó öèôðè
    mov bh, ah
    xor ah, ah
    div cl ; ñåðåäíÿ öèôðà â ah, ñòàðøà - â al
    or ax, 3030h ; ôîðìóâàííÿ òåêñòîâîãî êîäó öèôð
    mov dx, ax

    ; âèâåäåííÿ ïîâ³äîìëåííÿ íà åêðàí
    push offset descriptor_es ;  ñåëåêòîð äåñêðèïòîðà ³ç 1-ãî åòàïó
    pop es
    mov si, offset int_msg
    mov cx, 11
    mov di, 2620       ;ïî÷àòêîâà àäðåñà äëÿ âèâåäåííÿ
    mov ah, 07h        ;àòðèáóò ñèìâîë³â

outstr:
    mov al, cs:[si]
    stosw
    inc si

    loop outstr

    mov al, ' '
    stosw

    mov al, dl ;ñòàðøà äåñÿòêîâà öèôðà íîìåðà âåêòîðà
    stosw

    mov al, dh ;ñåðåäíÿ äåñÿòêîâà öèôðà íîìåðà âåêòîðà
    stosw

    mov al, bh ;ìîëîäøà äåñÿòêîâà öèôðà íîìåðà âåêòîðà
    stosw

    ; ï³ñëÿ âèâåäåííÿ ïîâ³äîìëåííÿ îðãàí³çóºìî ïåðåäà÷ó óïðàâë³ííÿ
    ; â îñíîâíèé ñåãìåíò äëÿ ïåðåêëþ÷åííÿ â ðåæèì ðåàëüíèõ àäðåñ
    ; Äëÿ öüîãî âèêîðèñòàºìî êîìàíäó IRET
    ; Â äàí³é òî÷ö³ ïðîãðàìè â ñòåê çàïèñàíî âì³ñò âñ³õ ðåã³ñòð³â, àäðåñà
    ; êîìàíäè, ÿêà âèçâàëà âèêëþ÷åííÿ, òà âì³ñò ðåã³ñòðà îçíàê.
    ; Ìè âñå öå ïðî³ãíîðóºìî ³ çàïèøåìî â ñòåê íîâ³ äàí³

    pushf
    push offset descriptor_cs ; ñåëåêòîð äåñêðèïòîðà ñåãìåíòà
                              ; êîä³â ³ç 1-ãî åòàïó
    push offset dos_return
    iret ; ïåðåäà÷à óïðàâë³ííÿ ïî àäðåñ³ return_dos îñíîâíîãî ñåãìåíòà êîä³â

show    proc    near
    push    bp
    mov     bp,sp
    pusha
    push    es
    mov     ax,offset descriptor_es
    mov     es,ax

    std
ls20:
    mov     di,[bp+4]
    mov     bx,[bp+6]
    mov     cx,4
    mov     ah,0ah
ls40:
    mov     al,bl
    and     al,00001111b
    cmp     al,10
    jl  ls100
    add     al,7
ls100:
    add     al,30h
    stosw
    shr     bx,4
    loop    ls40

    pop     es
    popa
    pop     bp
    ret     4
show    endp


common_int_end:
    int_code_size = common_int_end - int_begin
_interrupt_code ends
;----------------------------------------------------------------------
_code segment byte public 'code' use16
assume cs: _code
main:
mov ax, _data
mov ds,ax
mov ax,3h
int 10h
    mov eax, cr0
    test al, 1
    jnz already_protected

    ; Create descriptors.
    mov ax, gdt
    mov ds, ax
    assume ds: gdt

    ; GDT descriptor.
    mov word ptr [descriptor_gdt.limit_1], gdt_size
    xor eax, eax
    mov ax, gdt
    shl eax, 4
    mov dword ptr [descriptor_gdt.base_1], eax
    mov byte ptr [descriptor_gdt.attr], attr_data_seg

    ; Code descriptor.
    mov word ptr [descriptor_cs.limit_1], code_size
    xor eax, eax
    mov ax, _code
    shl eax, 4
    mov dword ptr [descriptor_cs.base_1], eax
    mov byte ptr [descriptor_cs.attr], attr_code_seg

 ; DS descriptor.
    mov word ptr [descriptor_ds.limit_1], data_size
    xor eax, eax
    mov ax, _data
    shl eax, 4
    mov dword ptr [descriptor_ds.base_1], eax
    mov byte ptr [descriptor_ds.attr], attr_data_seg

    ; _ST segment descr
    mov descr_ss.limit_1, 10
    xor eax, eax
    mov ax, _ST
    shl eax, 4
    mov dword ptr descr_ss.base_1, eax
    mov descr_ss.attr, S_sw0

; absent descriptor.
    mov word ptr [descriptor_absent.limit_1], data_size
    xor eax, eax
    mov ax, _data
    shl eax, 4
    mov dword ptr [descriptor_absent.base_1], eax
    mov byte ptr [descriptor_absent.attr], attr_data_seg
	and byte ptr [descriptor_absent.attr], 01111111b

    ; GS descriptor.
    mov word ptr [descriptor_gs.limit_1], dpl_size
    xor eax, eax
    mov ax, _dpl1_data
    shl eax, 4
    mov dword ptr [descriptor_gs.base_1], eax
    mov byte ptr [descriptor_gs.attr], attr_data_dpl

    ; ES descriptor.
    mov word ptr [descriptor_es.limit_1], 0ffffh
    mov dword ptr [descriptor_es.base_1], 0b8000h
    mov byte ptr [descriptor_es.attr], attr_data_seg

    ; Interrupt descriptor
    mov word ptr [descriptor_int_code.limit_1], int_code_size
    xor eax, eax
    mov ax, _interrupt_code
    shl eax, 4
    mov dword ptr [descriptor_int_code.base_1], eax
    mov byte ptr [descriptor_int_code.attr], attr_code_seg

    ; FS descriptor
    mov word ptr [descriptor_fs.limit_1], readonly_data_size
    xor eax, eax
    mov ax, _readonly_data
    shl eax, 4
    mov dword ptr [descriptor_fs.base_1], eax
    mov byte ptr [descriptor_fs.attr], attr_ro_data_seg
    and	descriptor_fs.attr, 11111000b

    ;----------------------------------
    mov descr_t7.attr, 00011010b

    mov descr_t14.limit_1, data_size
    xor eax, eax
    mov ax, _data
    shr eax, 4
    mov dword ptr descr_t14.base_1, eax
    mov descr_t14. attr, attr_ro_data_seg

    ;------------------------------------
    ; Load GDTR.
    mov ax, _data
    mov ds, ax
    assume ds: _data

    ; Create gdt pointer.
    xor eax, eax
    mov ax, gdt
    shl eax, 4
    mov dword ptr ds:[gdt_pointer.address], eax
    mov ax, gdt_size
    mov word ptr ds:[gdt_pointer.limit], ax

    lgdt gdt_pointer

    ; Create idt pointer.
    xor eax, eax
    mov ax, idt
    shl eax, 4
    mov dword ptr [idt_pointer.address], eax
    mov word ptr [idt_pointer.limit], 8 * 256

    lidt idt_pointer

   cli
    ; Go to protected mode.
    mov word ptr Top_real_mode, sp
    mov word ptr Top_real_mode+2, ss
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Long jmp to protected.
    db 0eah
    dw offset protected
    dw offset descriptor_cs

protected:
    ; Load other segment registers.

    mov ax,offset descr_ss
    mov ss,ax
    mov sp,Top_stp

    ; ds.
    mov ax, offset descriptor_ds
    mov ds, ax

    ; es.
    mov ax, offset descriptor_es
    mov es, ax

    ; fs.
    mov ax, offset descriptor_fs
    mov fs, ax

    ; gs.
    mov ax, offset descriptor_gs
    mov gs, ax

    ;jmp dos_return

    ; Print message.
    mov cx, message_len
    mov si, offset message
    mov di, 0
    mov ah, 07h

message_printing:
    mov al, byte ptr ds:[si]
    mov word ptr es:[di], ax
    inc si
    add di, 2
    loop message_printing

;=================================================================================
	;test 1
	;mov bx, data_size+1
	;mov al, [bx]
        
    ;test 7
    ;mov ax, offset descr_t7
    ;mov ds, ax      

    ;test 8
    mov ax, offset descriptor_ds
    mov gs, ax

    ;test 14
    ;assume ds: gdt
    ;mov ax, offset decr_gdt
    ;mov ds, ax
    ;mov al, descr_t14.attr
    ;or al, 1
    ;mov descr.attr, al
    ;mov ax, offset_t14
    ;mov ds, ax

;===================================================================================
dos_return:
cli
    ; Return to dos.
    assume ds:gdt
    mov ax, offset descriptor_gdt
    mov ds, ax

    mov descriptor_cs.limit_1, 0ffffh
    mov descriptor_ds.limit_1, 0ffffh
    mov descriptor_es.limit_1, 0ffffh
    mov descriptor_fs.limit_1, 0ffffh
    mov descriptor_fs.attr, attr_data_seg
    mov descr_ss.limit_1, 0ffffh
    mov descr_ss.attr, attr_data_seg
    mov descriptor_gs.limit_1, 0ffffh

    ; Reload segment registers.
    mov ax, offset descriptor_ds
    mov ax, ds
    mov ax, offset descriptor_es
    mov es, ax
    mov ax, offset descriptor_fs
    mov fs, ax
    mov ax, offset descriptor_gs
    mov gs, ax
    mov ax, offset descr_ss
    mov ss, ax

    ; Restore old interrupt table.
    mov ax, offset descriptor_ds
    mov ds, ax
    assume ds: _data

    xor eax, eax
    mov dword ptr ds:[idt_pointer.address], eax
    mov word ptr ds:[idt_pointer.limit], 3ffh

    lidt ds:[idt_pointer]

    db 0eah
    dw offset descriptor_cs_reload
    dw offset descriptor_cs

descriptor_cs_reload:
    mov eax, cr0
    and al, 0feh
    mov cr0, eax

    db 0eah
    dw offset real_mode
    dw _code

real_mode:
    mov ax, _data
    mov ds, ax
    assume ds: _data
    
    lss sp, Top_real_mode

    mov ax, _data
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    sti

    mov ah, 0
    int 16h

    mov ax, 3
    int 10h

already_protected:
    mov ax, 4c00h
    int 21h

code_end:
    code_size = code_end - main
_code ends
end main
