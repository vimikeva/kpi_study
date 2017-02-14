.386
scale	macro	p1
	fld		max_&p1
	fsub	min_&p1
	fild	max_crt_&p1
	fdivp	st(1), st(0)
	fstp	scale_&p1
	endm

_data	segment	use16
	min_x	dq	-3.2
	max_x	dq	3.2
	max_crt_x	dw	320
	crt_x	dw	?
	scale_x	dq	?

	min_y	dq	-0.4
	max_y	dq	5.0
	max_crt_y	dw	200
	crt_y	dw	?
	scale_y	dq	?

	step	dq	0.001
	tmp	dw	?
	color_axis	db 2bh
	color_plot	db 3bh
	value dd 1h
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
	call	draw_plot

	mov	ah, 8
	int	21h
	mov	ax, 3
	int	10h
	mov	ax, 4C00h
	int	21h

func	proc
	fld st(0)
	fmul st(0), st(0)
	faddp st(1), st(0)
	fld value
	faddp st(1), st(0)
	ret
func	endp

draw_plot	proc
	mov al, color_plot
	mov color_axis, al

	fld	min_x
@1:
	fld	st(0)
	fld	st(0)
	call	get_x
	call	func
	call	get_y
	call	draw_point
	fld	step
	faddp	st(1), st(0)
	fcom	max_x
	fstsw	ax
	sahf
	jna	@1
	ffree	st(0)
	ret
draw_plot	endp

get_x	proc
	fsub	min_x
	fdiv	scale_x
	frndint
	fistp	crt_x
	ret
get_x	endp

get_y	proc
	fsub	min_y
	fdiv	scale_y
	frndint
	fistp	crt_y
	mov	ax, max_crt_y
	sub	ax, crt_y
	mov	crt_y, ax
	ret
get_y	endp

draw_point	proc
	mov	ax, 0A000h
	mov	es, ax
	mov	si, crt_y
	mov	di, crt_x
	cmp	si, max_crt_y
	jae	@nd1
	cmp	di, max_crt_x
	jae	@nd1
	mov	ax, max_crt_x
	mul	si
	add	ax, di
	mov	bx, ax
	mov al , color_axis
	mov	byte ptr es:[bx], al
@nd1:
	ret
draw_point	endp

draw_axis	proc
	fldz
	call	get_y
	mov	crt_x, 0
	mov	cx, max_crt_x
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
	call	get_x
	call	draw_point

	fld1
	faddp	st(1), st(0)
	loop	@lx
	ffree	st(0)

	fldz
	call	get_x
	mov	crt_y, 0
	mov	cx, max_crt_y
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
