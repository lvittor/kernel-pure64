GLOBAL _acquire, _release

section .text

; void _acquire(char *lock)
_acquire:
    push rax
    mov al, 1
    xchg al, [rdi]
    cmp al, 0
    jne _acquire
    pop rax
    ret

; void _release(char *lock)
_release:
    mov byte [rdi], 0
    ret