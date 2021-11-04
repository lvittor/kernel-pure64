GLOBAL _buildProcessContext, _openProcessContext
GLOBAL _int20

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

; uint64_t _buildProcessContext(uint64_t base_rsp, uint64_t function_address, int argc, char *argv[]);
_buildProcessContext:
    push rbp
    mov rbp, rsp

	mov rsp, rdi
    push 0x0    ; SS
    push rdi    ; RSP
    push 0x202  ; RFLAGS
    push 0x8    ; CS
    push rsi    ; RIP

    push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdx
	push rcx
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15

    ; TODO: Check if necessary
    ; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

    mov rax, rsp

    mov rsp, rbp
    pop rbp
    ret

; void _openProcessContext(uint64_t baseRSP);
_openProcessContext:
    mov rsp, rdi
    popState
    iretq

; void _int20(void)
_int20:
	int 20h
	ret
