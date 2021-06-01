GLOBAL printReg 

extern ncPrint
extern ncNewline
extern ncPrintHex

section .text

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
    pushState
    mov rdi, %1
    call ncPrint
    popState
    pushState
    mov rdi, %2
    call ncPrintHex
    popState
    pushState
    call ncNewline
    popState 
%endmacro

; Should print:
; rax, rbx, rcx, rdx, rbp, rsp, rsi, rdi,
; r8, r9, r10, r11, r12, r13, r14, r15
; The called function preserves rsp, rbp, rbx, r12, r13, r14, r15
printReg:
    printOneReg srax, rax
    printOneReg srbx, rbx
    printOneReg srcx, rcx
    printOneReg srdx, rdx
    printOneReg srsi, rsi
    printOneReg srdi, rdi
    printOneReg srsp, rsp
    printOneReg srbp, rbp
    printOneReg sr8, r8
    printOneReg sr9, r9
    printOneReg sr10, r10
    printOneReg sr11, r11
    printOneReg sr12, r12
    printOneReg sr13, r13
    printOneReg sr14, r14
    printOneReg sr15, r15
    ret

section .data
srax db "rax: ", 0
srbx db "rbx: ", 0
srcx db "rcx: ", 0
srdx db "rdx: ", 0
srsi db "rsi: ", 0
srdi db "rdi: ", 0
srsp db "rsp: ", 0
srbp db "rbp: ", 0
sr8 db "r8: ", 0
sr9 db "r9: ", 0
sr10 db "r10: ", 0
sr11 db "r11: ", 0
sr12 db "r12: ", 0
sr13 db "r13: ", 0
sr14 db "r14: ", 0
sr15 db "r15: ", 0
; len equ $-srax


