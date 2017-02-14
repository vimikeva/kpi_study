.386
scale	macro	p1
	fld	max_&p1
	fsub	min_&p1
	fild	p1&_res
	fdivp	st(1), st(0)
	fstp	scale_&p1
	endm

x_res_	equ	320
y_res_	equ	200

_data	segment	use16
min_x	dq	-10.4
max_x	dq	10.4
x_res	dw	x_res_
crt_x	dw	?
scale_x	dq	?

min_y	dq	-15.4
max_y	dq	15.4
y_res	dw	y_res_
crt_y	dw	?
scale_y	dq	?
tmp dw ? 
_data	ends

_code	segment	use16
	assume	cs:_code, ds:_data
@main:
	mov	ax, _data
	mov	ds, ax

	mov	ax, 13h
	int	10h

	finit
	scale	x
	scale	y

	call	draw_axis
	call	draw_gra

	mov	ah, 8
	int	21h
	mov	ax, 3
	int	10h
	mov	ax, 4C00h
	int	21h

func	proc
	fld	st
	fsin
	fxch st(1)
	fdivp	st(1), st
	ret
func	endp

draw_gra	proc
	fldz
@1:
	fld	st(0)
	call	get_x
	call	func
	call	get_y
	call	draw_point
	fld1
	faddp	st(1), st(0)
	ficom  x_res
	fstsw	ax
	sahf
	jna	@1
	ffree	st(0)
	ret
draw_gra	endp

get_x	proc
  fist crt_x
  fmul  scale_x
	fadd	min_x
	ret
get_x	endp

get_y	proc
	fsub	min_y
	fdiv	scale_y
	frndint
	fistp	crt_y
	mov	ax, y_res
	sub	ax, crt_y
	mov	crt_y, ax
	ret
get_y	endp

draw_point	proc
	mov	ax, 0A000h
	mov	es, ax
	mov	si, crt_y
	mov	di, crt_x
	cmp	si, y_res_
	jae	@nd
	cmp	di, x_res_
	jae	@nd
	mov	ax, x_res_
	mul	si
	add	ax, di
	mov	bx, ax
	mov	byte ptr es:[bx], 15
@nd:
	ret
draw_point	endp

draw_axis	proc
	fldz
	call	get_y
	mov	crt_x, 0
	mov	cx, x_res_
@x_c:
	call	draw_point
	inc	crt_x
	loop	@x_c

	fld	max_x
	fsub	min_x
	frndint
	fistp	tmp
	mov	cx, tmp

	fld	min_x
	frndint
	dec	crt_y
@lx:
	fld	st(0)
  fsub  min_x
  fdiv  scale_x
  frndint
  fistp crt_x	
	call	draw_point

	fld1
	faddp	st(1), st(0)
	loop	@lx
	ffree	st(0)

	fldz
  fsub  min_x
  fdiv  scale_x
  frndint
  fistp crt_x	
	mov	crt_y, 0
	mov	cx, y_res_
@y_c:
	call	draw_point
	inc	crt_y
	loop	@y_c

	fld	max_y
	fsub	min_y
	frndint
	fistp	tmp
	mov	cx, tmp

	fld	min_y
	frndint
	dec	crt_x
@ly:
	fst	st(1)
	call	get_y
	call	draw_point
	
	fld1
	faddp	st(1), st(0)
	fcom	max_y
	loop	@ly
	ffree	st(0)
	ret
draw_axis	endp

_code	ends
	end	@main
