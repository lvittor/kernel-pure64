GLOBAL init_process, run_process, get_rsp

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

init_process: ; uint64_t init_process(uint64_t stack_base, uint64_t rip);
    push rbp
    mov rbp, rsp 

    mov rsp, rdi
    push 0x00
    push rdi
    push 0x202
    push 0x8
    push rsi
	pushState

    mov rax, rsp

    mov rsp, rbp
    pop rbp
    ret

run_process ; void run_process(uint64_t rsp);
    mov rsp, rdi
    popState
    iretq
