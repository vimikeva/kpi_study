.386
data segment
data ends
assume ds: data, cs:code
code segment
	mov ax, data
	mov ds, ax
	nop
	mov ax, ds
	nop
	nop
code ends
end