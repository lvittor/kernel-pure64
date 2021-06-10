GLOBAL print
GLOBAL getChar
GLOBAL setReg
GLOBAL fillDate
GLOBAL inforeg
GLOBAL fillMem

EXTERN print_f

print:
    mov rax, 1
    int 80h
    ret

getChar:
    mov rax, 2
    int 80h
    ret

fillDate:
    mov rax, 3
    int 80h
    ret

fillMem:
    mov rax, 4
    int 80h
    ret

setReg:
    mov rax, 0xFFFF
    mov rbx, 0xFFF0
    mov rcx, 0xFF00
    mov rdx, 0xF000
    mov r8,  0xFF00
    mov r9,  0xFFF0
    mov r10, 0xFFFF
    mov r11, 0xFFF0
    mov r12, 0xFF00
    mov r13, 0xF000
    mov r14, 0xFF00
    mov r15, 0xFFF0
    div cl
    mov ax, cx
    ret

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro printOneReg 2
    mov rdi, 1          ; file descriptor = 1
    mov rsi, fmt        ; format: "%s:  %x/n"
    mov rdx, %1         ; nombre del registro
    mov rcx, [rbp + %2 * 8]         ; valor del registro
    pushState
    call print_f
    popState
%endmacro

; Después de armarte el stack frame, tenés el stack de la siguiente manera:
;   rbp
;   r15
;   r14
;   r13
;   r12
;   r11
;   r10
;   r9
;   r8
;   rsi
;   rdi
;   rbp
;   rdx
;   rcx
;   rbx
;   rax
;   rflags
;   rip

inforeg:
    pushf
    pushState
    push rbp
    mov rbp, rsp

    printOneReg srip, 17
    printOneReg srflags, 16
    printOneReg srax, 15
    printOneReg srbx, 14
    printOneReg srcx, 13
    printOneReg srdx, 12
    printOneReg srbp, 11
    printOneReg srdi, 10
    printOneReg srsi, 9
    printOneReg sr8, 8
    printOneReg sr9, 7
    printOneReg sr10, 6
    printOneReg sr11, 5
    printOneReg sr12, 4
    printOneReg sr13, 3
    printOneReg sr14, 2
    printOneReg sr15, 1

    mov rsp, rbp
    pop rbp
    popState
    popf
    ret

section .data
fmt db "%s: %xh", 10, 0
srip db "rip", 0
srflags db "rflags", 0
srax db "rax", 0
srbx db "rbx", 0
srcx db "rcx", 0
srdx db "rdx", 0
srbp db "rbp", 0
srdi db "rdi", 0
srsi db "rsi", 0
sr8 db "r8", 0
sr9 db "r9", 0
sr10 db "r10", 0
sr11 db "r11", 0
sr12 db "r12", 0
sr13 db "r13", 0
sr14 db "r14", 0
sr15 db "r15", 0
