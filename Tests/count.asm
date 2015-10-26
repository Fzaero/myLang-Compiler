code segment 
 	 call myread 	 ; read a
 	 mov  va,cx 	 ; 
Lab0: 			 ; while begin 
 	 push 100d 
 	 push va w	 	 ; 
 	 pop cx 	 	 ; 
 	 pop ax 	 	 ; 
 	 sub ax,cx  	 ; 
 	 push ax 	 	 ; 
 	 pop ax 	 	 ; 
 	 cmp ax,0 	 	 ; 
 	 IF Z jmp Lab1 	 	 ; 
 	 push va w	 	 ; 
 	 pop ax 	 	 ; 
 	 call myprint 	 ; 
 	 push va w	 	 ; 
 	 push 1d 
 	 pop cx 	 	 ; 
 	 pop ax 	 	 ; 
 	 add ax,cx  	 ; 
 	 push ax 	 	 ; 
 	 pop ax 	 	 ; 
 	 push offset va 	 ; 
 	 pop bp 	 	 ; 
 	 mov [bp],ax  	 ; 
	 jmp Lab0 	 	 ; jump to while beginning 
Lab1: 			 ; 
	 int 20h 		 ; exit to dos 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
;reads a nonnegative integer and puts 
;it in cx register  
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
myread: 
	 MOV  CX,0 
morechar: 
	 mov  ah,01h 
	 int  21H 
	 mov  dx,0 
	 mov  dl,al 
	 mov  ax,cx 
	 cmp  dl,0D 
	 je   myret 
	 sub  dx,48d 
	 mov  bp,dx  
	 mov  ax,cx 
	 mov  cx,10d 
	 mul  cx  
	 add  ax,bp 
	 mov  cx,ax 
	 jmp  morechar 
myret: 
	 ret 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; number in AX register is printed
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
myprint: 
	 mov    si,10d 
	 xor    dx,dx 
	 push   ' ' 		 ; push newline 
	 mov    cx,1d 
nonzero: 
	 div    si 
	 add    dx,48d 
	 push   dx 
	 inc    cx 
	 xor    dx,dx 
	 cmp    ax,0h 
	 jne    nonzero 
writeloop: 
	 pop    dx 
	 mov    ah,02h 
	 int    21h 
	 dec    cx 
	 jnz    writeloop 
	 ret 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Variables are put in this area 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
va 	 dw ?  ;  variable a
code ends