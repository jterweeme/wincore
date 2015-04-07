section .text
    global tiep1
    global tiep2
    global tiep3
    global tiep4
    global tiep5
    global tiep6

    tiep1:
        mov rax, rcx
        syscall
        ret       

    tiep2:
        mov rax, rcx
        syscall
        ret

    tiep3:
        mov rax, rcx
        syscall
        ret

    tiep4:
        mov rax, rcx
        syscall
        ret

    tiep5:
        mov rax, rcx
        syscall
        ret

    tiep6:
        mov rax, rcx
        syscall
        ret

    print2:
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

    open2:
        mov rax, 2
        ret

    read:
        mov rax, 0
        ret

