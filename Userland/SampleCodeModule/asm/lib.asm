GLOBAL print
GLOBAL getChar
GLOBAL getPid
GLOBAL ps
GLOBAL createProcess
GLOBAL _kill
GLOBAL _block
GLOBAL _alloc
GLOBAL _free
GLOBAL _memdump
GLOBAL _nice
GLOBAL _open_pipe
GLOBAL _write_pipe
GLOBAL _read_pipe
GLOBAL _close_pipe
GLOBAL _secondsElapsed


EXTERN print_f

print:
    mov rax, 1
    int 80h
    ret

getChar:
    mov rax, 2
    int 80h
    ret

getPid:
    mov rax, 3
    int 80h
    ret

ps: 
    mov rax, 4
    int 80h
    ret

createProcess: 
    mov rax, 5
    int 80h
    ret

_kill:
    mov rax, 6
    int 80h
    ret

_block:
    mov rax, 7
    int 80h
    ret

_alloc:
    mov rax, 8
    int 80h
    ret

_free:
    mov rax, 9
    int 80h
    ret

_memdump:
    mov rax, 10
    int 80h

_nice:
    mov rax, 11
    int 80h
    ret

_open_sem:
    mov rax, 12
    int 80h
    ret

_wait_sem:
    mov rax, 13
    int 80h
    ret

_post_sem:
    mov rax, 14
    int 80h
    ret

_close_sem:
    mov rax, 15
    int 80h
    ret

_print_sem:
    mov rax, 16
    int 80h
    ret

_open_pipe:
    mov rax, 17
    int 80h
    ret

_write_pipe:
    mov rax, 18
    int 80h
    ret

_read_pipe:
    mov rax, 19
    int 80h
    ret

_close_pipe:
    mov rax, 20
    int 80h
    ret

_secondsElapsed:
    mov rax, 21
    int 80h
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
