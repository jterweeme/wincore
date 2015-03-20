section .text
    global create
    global print

    print:
        push rdx
        mov rdx, rsi
        mov rsi, rdi
        mov rax, 1
        pop rdi
        syscall
        ret

    create:
        mov rax, 85
        mov rsi, 0420
        syscall
        ret



