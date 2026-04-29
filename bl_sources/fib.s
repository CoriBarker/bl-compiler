.data
.int_fmt: .string "%d"

.intel_syntax noprefix
.global fib
.global main
.global _start
.text

fib:
    push rbp
    mov rbp, rsp
    sub rsp, 16
    mov QWORD PTR [rbp + -16], rdi
    mov rax, [rbp + -16]
    push rax
    mov rax, 1
    mov rcx, rax
    pop rax
    cmp rax, rcx
    sete al
    movzx rax, al
    push rax
    mov rax, [rbp + -16]
    push rax
    mov rax, 2
    mov rcx, rax
    pop rax
    cmp rax, rcx
    sete al
    movzx rax, al
    mov rcx, rax
    pop rax
    or rax, rcx
    setne al
    movzx rax, al
    cmp rax, 0
    je .endif_2
    mov rax, 1
    jmp .return_0
    jmp .endif_2
.endif_2:
    mov rax, [rbp + -16]
    push rax
    mov rax, 1
    mov rcx, rax
    pop rax
    sub rax, rcx
    mov rdi, rax
    sub rsp, 8
    call fib
    add rsp, 8
    push rax
    mov rax, [rbp + -16]
    push rax
    mov rax, 2
    mov rcx, rax
    pop rax
    sub rax, rcx
    mov rdi, rax
    sub rsp, 8
    call fib
    add rsp, 8
    mov rcx, rax
    pop rax
    add rax, rcx
    mov QWORD PTR [rbp + -8], rax
    mov rax, [rbp + -8]
    jmp .return_0
.return_0:
    mov rsp, rbp
    pop rbp
    ret
main:
    push rbp
    mov rbp, rsp
    sub rsp, 16
    mov rax, 1
    mov QWORD PTR [rbp + -8], rax
.for_start_4:
    mov rax, [rbp + -8]
    push rax
    mov rax, 100
    mov rcx, rax
    pop rax
    cmp rax, rcx
    setle al
    movzx rax, al
    cmp rax, 0
    je .for_end_5
    mov rax, [rbp + -8]
    mov rdi, rax
    sub rsp, 8
    call fib
    add rsp, 8
    lea rdi, [rip + .int_fmt]
    mov rsi, rax
    mov rax, 0
    call printf
    mov rax, [rbp + -8]
    push rax
    mov rax, 1
    mov rcx, rax
    pop rax
    add rax, rcx
    mov QWORD PTR [rbp + -8], rax
    jmp .for_start_4
.for_end_5:
    mov rax, 0
    jmp .return_3
.return_3:
    mov rsp, rbp
    pop rbp
    ret
