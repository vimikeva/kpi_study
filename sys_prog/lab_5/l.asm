.186
.model small
.data

.code

@empty:

	mov ax, @data
	mov ds, ax

	mov ax, 3
	int	10h
	mov ax, 0b800h
	mov es, ax

		;инициализация
	xor	ax, ax
	int	33h

	mov ax, 1
	int 33h

	mov ax, 0ch	; встановити режим обробки подій від мишки
	mov	cx, 1011b		; вибрати в якості події будь-яке переміщення мишки  (біт 0=1) або натискання правої кнопки (біт3=1)
	push es		;зберегти вміст сегментного регістра 
	push cs
	pop es		; вважаєм, що процедура користувача для обробки подій від мишки знаходиться в поточному сегменті кодів
	mov dx, offset prmaus; встановити зміщення процедури обробки подій від мишки в сегменті кодів
	int 33h		; регістрація адреси та умов виклику
	pop es


	mov ah, 01h	; ввести символ з клавіатури ПЕОМ
	int	21h		; виклик функції DOS

	xor	cx,cx		;cx=0
	mov	ax,0ch		

	;вважаємо що регістри es:dx містять логічну адресу процедури prmaus
	push es		 
	push cs
	pop es		
	mov dx, offset prmaus	; встановити зміщення процедури

	int	33h		;процедура prmaus
					;далі викликатись не буде
	mov ax, 03h
	int 10h

	mov ax, 4c00h
	int 21h


prmaus:
						; збереження вмісту регістрів ds, es та  РЗП
	push		ds    
	push 		es
	pusha
						;завантаження  сегментних регістрів ds та  es
	push		0b800h ; сегментна адреса відеобуфера
	pop 		es	
	push 		@data 
	pop ds

	shr dx, 3
	shr cx, 3
	mov ax, 80*2
	mul dx
	add ax, cx
	add ax, cx
	mov di, ax

	test bx, 0001b
	je @click_right

	test bx, 0010b
	je @click_left
	
	jmp @end
	
@click_right:
	mov	word ptr es:[di], 0f00h
	jmp @end

@click_left:
	mov word ptr es:[di], 0f2ah
@end:

	popa
	pop es
	pop ds
	mov ax, 1
	int 33h
retf

end @empty
