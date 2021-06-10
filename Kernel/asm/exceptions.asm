GLOBAL printRegs

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
    mov rdi, %1
    call ncPrint
    mov rdi, [rbp + %2 * 8 + 8]
    call ncPrintHex
    call ncNewline
%endmacro

; This function expects the following stack:
;	RBP
;	RIP <-- From the calling function
;	R15
;	R14
;	R13
;	R12
;	R11
;	R10
;	R9
;	R8
;	RSI
;	RDI
;	RBP
;	RDX
;	RCX
;	RBX
;	RAX
;	RIP <-- From the instruction that caused the exception
;	CS
;	RFLAGS
;	RSP
;	SS
printRegs:
	push rbp
	mov rbp, rsp
	pushState

	printOneReg srsp, 19
	printOneReg srflags, 18
    printOneReg srip, 16
    printOneReg srax, 15
    printOneReg srbx, 14
    printOneReg srcx, 13
    printOneReg srdx, 12
    printOneReg srbp, 11
    printOneReg srdi, 10
    printOneReg srsi, 9
    printOneReg sr8,  8
    printOneReg sr9,  7
    printOneReg sr10, 6
    printOneReg sr11, 5
    printOneReg sr12, 4
    printOneReg sr13, 3
    printOneReg sr14, 2
    printOneReg sr15, 1

	popState
	mov rsp, rbp
	pop rbp
	ret

section .data
srsp db "rsp: ", 0
srflags db "rflags: ", 0
srip db "rip: ", 0
srax db "rax: ", 0
srbx db "rbx: ", 0
srcx db "rcx: ", 0
srdx db "rdx: ", 0
srsi db "rsi: ", 0
srdi db "rdi: ", 0
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
