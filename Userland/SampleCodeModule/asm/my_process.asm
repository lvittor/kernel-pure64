GLOBAL ps, createPs, blockPs, unblockPs, killPs, getpid, nice, exit, yield

ps: ; void ps void(void);
    mov rax, 4
    int 80h
    ret

createPs: ; pid_t createPs(uint64_t rip, char *name, int argc, char *argv[], uint8_t mode);
    mov rax, 5
    int 80h
    ret

blockPs: ;int blockPs(pid_t pid);
    mov rax, 6
    int 80h
    ret

unblockPs: ; int unblockPs(pid_t pid);
    mov rax, 7
    int 80h
    ret

killPs: ; int unblockPs(pid_t pid);
    mov rax, 8
    int 80h
    ret

getpid: ; pid_t getpid(void);
    mov rax, 9
    int 80h
    ret

nice: ; int nice(pid_t pid, int adjustment);
    mov rax, 10
    int 80h
    ret

exit: ; void exit(void);
    mov rax, 11
    int 80h
    ret

yield: ; void yield(void);
    mov rax, 12
    int 80h
    ret

; TODO: check if syscalls could be handled in a better way