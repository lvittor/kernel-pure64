GLOBAL sem_dump, sem_open, sem_wait, sem_post, sem_close

sem_dump: ; void sem_dump(void);
    mov rax, 13
    int 80h
    ret

sem_open: ; int sem_open(uint16_t semId, uint64_t value);
    mov rax, 14
    int 80h
    ret

sem_post: ; int sem_post(uint16_t semId)
    mov rax, 15
    int 80h
    ret

sem_wait: ; int sem_wait(uint16_t semId);
    mov rax, 16
    int 80h
    ret

sem_close: ; int sem_close(uint16_t semId)
    mov rax, 17
    int 80h
    ret