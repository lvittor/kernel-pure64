GLOBAL init_process, _int20

init_process: ; uint64_t init_process(uint64_t stack_base, uint64_t rip, uint64_t argc, uint64_t argv);
    push rbp
    mov rbp, rsp 

    mov rsp, rdi
    push 0x00
    push rdi
    push 0x202
    push 0x8
    push rsi
	
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdx ; push argc into rdi
	push rcx ; push argv into rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15

    mov rax, rsp

    mov rsp, rbp
    pop rbp
    ret

_int20: ; void _int20(void)
	int 20h
	ret
