.data
.int_fmt: .string "%lld\n"

.intel_syntax noprefix
.global arctan
.global main
.global _start
.text

arctan:
    push rbp
    mov rbp, rsp
    sub rsp, 64
    mov QWORD PTR [rbp + -32], rdi
    mov QWORD PTR [rbp + -56], rsi
    mov rax, [rbp + -56]
    push rax
    mov rax, [rbp + -32]
    mov rcx, rax
    pop rax
    cqo
    idiv rcx
    mov QWORD PTR [rbp + -24], rax
    mov rax, [rbp + -56]
    push rax
    mov rax, [rbp + -32]
    mov rcx, rax
    pop rax
    cqo
    idiv rcx
    mov QWORD PTR [rbp + -16], rax
    mov rax, 1
    mov QWORD PTR [rbp + -8], rax
    mov rax, 0
    push rax
    mov rax, 1
    mov rcx, rax
    pop rax
    sub rax, rcx
    mov QWORD PTR [rbp + -48], rax
.for_start_1:
    mov rax, [rbp + -16]
    push rax
    mov rax, [rbp + -32]
    push rax
    mov rax, [rbp + -32]
    mov rcx, rax
    pop rax
    imul rax, rcx
    mov rcx, rax
    pop rax
    cqo
    idiv rcx
    mov QWORD PTR [rbp + -16], rax
    mov rax, [rbp + -16]
    push rax
    mov rax, 0
    mov rcx, rax
    pop rax
    cmp rax, rcx
    sete al
    movzx rax, al
    cmp rax, 0
    je .endif_4
    mov rax, [rbp + -24]
    jmp .return_0
    jmp .endif_4
.endif_4:
    mov rax, [rbp + -16]
    push rax
    mov rax, 2
    push rax
    mov rax, [rbp + -8]
    mov rcx, rax
    pop rax
    imul rax, rcx
    push rax
    mov rax, 1
    mov rcx, rax
    pop rax
    add rax, rcx
    mov rcx, rax
    pop rax
    cqo
    idiv rcx
    mov QWORD PTR [rbp + -40], rax
    mov rax, [rbp + -40]
    push rax
    mov rax, 0
    mov rcx, rax
    pop rax
    cmp rax, rcx
    sete al
    movzx rax, al
    cmp rax, 0
    je .endif_6
    mov rax, [rbp + -24]
    jmp .return_0
    jmp .endif_6
.endif_6:
    mov rax, [rbp + -48]
    push rax
    mov rax, 0
    mov rcx, rax
    pop rax
    cmp rax, rcx
    setl al
    movzx rax, al
    cmp rax, 0
    je .else_7
    mov rax, [rbp + -24]
    push rax
    mov rax, [rbp + -40]
    mov rcx, rax
    pop rax
    sub rax, rcx
    mov QWORD PTR [rbp + -24], rax
    jmp .endif_8
.else_7:
    mov rax, [rbp + -24]
    push rax
    mov rax, [rbp + -40]
    mov rcx, rax
    pop rax
    add rax, rcx
    mov QWORD PTR [rbp + -24], rax
.endif_8:
    mov rax, 0
    push rax
    mov rax, [rbp + -48]
    mov rcx, rax
    pop rax
    sub rax, rcx
    mov QWORD PTR [rbp + -48], rax
    mov rax, [rbp + -8]
    push rax
    mov rax, 1
    mov rcx, rax
    pop rax
    add rax, rcx
    mov QWORD PTR [rbp + -8], rax
    jmp .for_start_1
.for_end_2:
.return_0:
    mov rsp, rbp
    pop rbp
    ret
main:
    push rbp
    mov rbp, rsp
    sub rsp, 32
    mov rax, 1000000000000000000
    mov QWORD PTR [rbp + -8], rax
    mov rax, 4
    push rax
    mov rax, 5
    mov rdi, rax
    mov rax, [rbp + -8]
    mov rsi, rax
    sub rsp, 8
    call arctan
    add rsp, 8
    mov rcx, rax
    pop rax
    imul rax, rcx
    push rax
    mov rax, 239
    mov rdi, rax
    mov rax, [rbp + -8]
    mov rsi, rax
    sub rsp, 8
    call arctan
    add rsp, 8
    mov rcx, rax
    pop rax
    sub rax, rcx
    mov QWORD PTR [rbp + -16], rax
    mov rax, 4
    push rax
    mov rax, [rbp + -16]
    mov rcx, rax
    pop rax
    imul rax, rcx
    mov QWORD PTR [rbp + -24], rax
    mov rax, [rbp + -24]
    lea rdi, [rip + .int_fmt]
    mov rsi, rax
    mov rax, 0
    call printf
    mov rax, 0
    jmp .return_9
.return_9:
    mov rsp, rbp
    pop rbp
    ret
