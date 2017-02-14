.386p

scale macro p1 
	fld		max_&p1
	fsub	min_&p1
	fild	max_crt_&p1
	fdivp	st(1), st(0)
	fstp	scale_&p1
endm


data segment para public 'data' use16
	min_x		dq	-30.0
	max_x		dq	30.0
	max_crt_x	dw	320

	crt_x		dw	?
	scale_x		dq	?
	
	min_y		dq	-3.0
	max_y		dq	3.0
	max_crt_y	dw	200 
	crt_y		dw	?
	scale_y		dq	?
	
	buff		dq	?
	control_reg	dw	?
data ends 


code segment para public 'code' use16
assume ds: data, cs: code
	
	_start:
	
	finit

	fldz
	fld1
	fsub
	fsqrt

	mov ax, 10000101b
	mov control_reg, ax
	fldcw control_reg

	mov ax, data
	mov ds, ax
	
	scale x
	scale y

	mov ax,13h
	int 10h
	
	mov ax,0a000h
	mov es,ax

	@x:
		mov cx,320
		mov di,32000
		mov al,2
		rep stosb
	@y:
		mov di,160
		mov cx,200
		mov al,2

	@vertical:
		add di,319
		stosb
	loop @vertical
	
	mov crt_x,0
	mov cx,320

@start:
	fld	scale_x
	fild	crt_x
	fmulp	st (1), st (0)
	fadd	min_x
	fld 	st

	fcos

	fld1
	fadd

	fsqrt
	fcom min_y
	fstsw ax
	sahf
	jc @minus
	
	fcom max_y
	fstsw ax
	sahf
	ja @plus
	
	fsub min_y
	fdiv scale_y
	frndint
	fistp crt_y
	mov ax,max_crt_y
	sub ax,crt_y
	mov crt_y,ax
	
	jmp @draw

	@minus:
		Mov ax,max_crt_y
		Mov crt_y,ax
		fstp buff
		jmp @draw
	@plus:
		mov crt_y,0
		fstp buff
		jmp @draw


	@draw:
		Mov si,crt_y
		mov ax,320
		mul si
		add ax,crt_x
		mov di,ax
		mov al,8
		dec di
		stosb
		inc crt_x
		dec cx
		cmp cx,0
		jne  @start
		
	mov ah,1h 
	int 21h

	@exit:
		mov ax,3 
		int 10h
		mov ax,4c00h
		int 21h
code ends
end _start
