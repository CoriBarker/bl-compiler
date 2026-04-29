.data
.int_fmt: .string "%d"

.intel_syntax noprefix
.global prime
.global mod_pow
.global main
.global _start
.text

prime:
    push rbp
    mov rbp, rsp
    sub rsp, 64
    mov QWORD PTR [rbp + -24], rdi
    mov rax, [rbp + -24]
    push rax
    mov rax, 2
    mov rcx, rax
    pop rax
    cmp rax, rcx
    setl al
    movzx rax, al
    cmp rax, 0
    je .endif_2
    mov rax, 0
    jmp .return_0
    jmp .endif_2
.endif_2:
    mov rax, [rbp + -24]
    push rax
    mov rax, 2
    mov rcx, rax
    pop rax
    cmp rax, rcx
    sete al
    movzx rax, al
    cmp rax, 0
    je .endif_4
    mov rax, 1
    jmp .return_0
    jmp .endif_4
.endif_4:
    mov rax, [rbp + -24]
    push rax
    mov rax, 2
    mov rcx, rax
    pop rax
    cqo
    idiv rcx
    mov rax, rdx
    push rax
    mov rax, 0
    mov rcx, rax
    pop rax
    cmp rax, rcx
    sete al
    movzx rax, al
    cmp rax, 0
    je .endif_6
    mov rax, 0
    jmp .return_0
    jmp .endif_6
.endif_6:
    mov rax, [rbp + -24]
    push rax
    mov rax, 1
    mov rcx, rax
    pop rax
    sub rax, rcx
    mov QWORD PTR [rbp + -32], rax
    mov rax, 0
    mov QWORD PTR [rbp + -8], rax
.while_start_7:
    mov rax, [rbp + -32]
    push rax
    mov rax, 2
    mov rcx, rax
    pop rax
    cqo
    idiv rcx
    mov rax, rdx
    push rax
    mov rax, 0
    mov rcx, rax
    pop rax
    cmp rax, rcx
    sete al
    movzx rax, al
    cmp rax, 0
    je .while_end_8
    mov rax, [rbp + -32]
    push rax
    mov rax, 2
    mov rcx, rax
    pop rax
    cqo
    idiv rcx
    mov QWORD PTR [rbp + -32], rax
    mov rax, [rbp + -8]
    push rax
    mov rax, 1
    mov rcx, rax
    pop rax
    add rax, rcx
    mov QWORD PTR [rbp + -8], rax
    jmp .while_start_7
.while_end_8:
    mov rax, 2
    mov QWORD PTR [rbp + -40], rax
.while_start_9:
    mov rax, [rbp + -40]
    push rax
    mov rax, 11
    mov rcx, rax
    pop rax
    cmp rax, rcx
    setle al
    movzx rax, al
    cmp rax, 0
    je .while_end_10
    mov rax, 0
    mov QWORD PTR [rbp + -16], rax
    mov rax, [rbp + -40]
    push rax
    mov rax, [rbp + -24]
    mov rcx, rax
    pop rax
    cmp rax, rcx
    setl al
    movzx rax, al
    cmp rax, 0
    je .endif_12
    mov rax, [rbp + -40]
    mov rdi, rax
    mov rax, [rbp + -32]
    mov rsi, rax
    mov rax, [rbp + -24]
    mov rdx, rax
    sub rsp, 8
    call mod_pow
    add rsp, 8
    mov QWORD PTR [rbp + -48], rax
    mov rax, [rbp + -48]
    push rax
    mov rax, 1
    mov rcx, rax
    pop rax
    cmp rax, rcx
    sete al
    movzx rax, al
    push rax
    mov rax, [rbp + -48]
    push rax
    mov rax, [rbp + -24]
    push rax
    mov rax, 1
    mov rcx, rax
    pop rax
    sub rax, rcx
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
    je .endif_14
    mov rax, 1
    mov QWORD PTR [rbp + -16], rax
    jmp .endif_14
.endif_14:
    mov rax, [rbp + -16]
    push rax
    mov rax, 0
    mov rcx, rax
    pop rax
    cmp rax, rcx
    sete al
    movzx rax, al
    cmp rax, 0
    je .endif_16
    mov rax, 1
    mov QWORD PTR [rbp + -56], rax
    mov rax, 1
    mov QWORD PTR [rbp + -64], rax
.while_start_17:
    mov rax, [rbp + -64]
    push rax
    mov rax, [rbp + -8]
    mov rcx, rax
    pop rax
    cmp rax, rcx
    setl al
    movzx rax, al
    cmp rax, 0
    je .while_end_18
    mov rax, [rbp + -48]
    push rax
    mov rax, [rbp + -48]
    mov rcx, rax
    pop rax
    imul rax, rcx
    push rax
    mov rax, [rbp + -24]
    mov rcx, rax
    pop rax
    cqo
    idiv rcx
    mov rax, rdx
    mov QWORD PTR [rbp + -48], rax
    mov rax, [rbp + -48]
    push rax
    mov rax, [rbp + -24]
    push rax
    mov rax, 1
    mov rcx, rax
    pop rax
    sub rax, rcx
    mov rcx, rax
    pop rax
    cmp rax, rcx
    sete al
    movzx rax, al
    cmp rax, 0
    je .else_19
    mov rax, 0
    mov QWORD PTR [rbp + -56], rax
    mov rax, [rbp + -8]
    mov QWORD PTR [rbp + -64], rax
    jmp .endif_20
.else_19:
    mov rax, [rbp + -64]
    push rax
    mov rax, 1
    mov rcx, rax
    pop rax
    add rax, rcx
    mov QWORD PTR [rbp + -64], rax
.endif_20:
    jmp .while_start_17
.while_end_18:
    mov rax, [rbp + -56]
    push rax
    mov rax, 1
    mov rcx, rax
    pop rax
    cmp rax, rcx
    sete al
    movzx rax, al
    cmp rax, 0
    je .endif_22
    mov rax, 0
    jmp .return_0
    jmp .endif_22
.endif_22:
    jmp .endif_16
.endif_16:
    jmp .endif_12
.endif_12:
    mov rax, [rbp + -40]
    push rax
    mov rax, 1
    mov rcx, rax
    pop rax
    add rax, rcx
    mov QWORD PTR [rbp + -40], rax
    jmp .while_start_9
.while_end_10:
    mov rax, 1
    jmp .return_0
.return_0:
    mov rsp, rbp
    pop rbp
    ret
mod_pow:
    push rbp
    mov rbp, rsp
    sub rsp, 48
    mov QWORD PTR [rbp + -32], rdi
    mov QWORD PTR [rbp + -40], rsi
    mov QWORD PTR [rbp + -24], rdx
    mov rax, 1
    mov QWORD PTR [rbp + -16], rax
    mov rax, [rbp + -32]
    push rax
    mov rax, [rbp + -24]
    mov rcx, rax
    pop rax
    cqo
    idiv rcx
    mov rax, rdx
    mov QWORD PTR [rbp + -8], rax
.while_start_24:
    mov rax, [rbp + -40]
    push rax
    mov rax, 0
    mov rcx, rax
    pop rax
    cmp rax, rcx
    setg al
    movzx rax, al
    cmp rax, 0
    je .while_end_25
    mov rax, [rbp + -40]
    push rax
    mov rax, 2
    mov rcx, rax
    pop rax
    cqo
    idiv rcx
    mov rax, rdx
    push rax
    mov rax, 1
    mov rcx, rax
    pop rax
    cmp rax, rcx
    sete al
    movzx rax, al
    cmp rax, 0
    je .endif_27
    mov rax, [rbp + -16]
    push rax
    mov rax, [rbp + -8]
    mov rcx, rax
    pop rax
    imul rax, rcx
    push rax
    mov rax, [rbp + -24]
    mov rcx, rax
    pop rax
    cqo
    idiv rcx
    mov rax, rdx
    mov QWORD PTR [rbp + -16], rax
    jmp .endif_27
.endif_27:
    mov rax, [rbp + -8]
    push rax
    mov rax, [rbp + -8]
    mov rcx, rax
    pop rax
    imul rax, rcx
    push rax
    mov rax, [rbp + -24]
    mov rcx, rax
    pop rax
    cqo
    idiv rcx
    mov rax, rdx
    mov QWORD PTR [rbp + -8], rax
    mov rax, [rbp + -40]
    push rax
    mov rax, 2
    mov rcx, rax
    pop rax
    cqo
    idiv rcx
    mov QWORD PTR [rbp + -40], rax
    jmp .while_start_24
.while_end_25:
    mov rax, [rbp + -16]
    jmp .return_23
.return_23:
    mov rsp, rbp
    pop rbp
    ret
main:
    push rbp
    mov rbp, rsp
    sub rsp, 16
    mov rax, 1
    mov QWORD PTR [rbp + -8], rax
.for_start_29:
    mov rax, [rbp + -8]
    mov rdi, rax
    sub rsp, 8
    call prime
    add rsp, 8
    push rax
    mov rax, 1
    mov rcx, rax
    pop rax
    cmp rax, rcx
    sete al
    movzx rax, al
    cmp rax, 0
    je .endif_32
    mov rax, [rbp + -8]
    lea rdi, [rip + .int_fmt]
    mov rsi, rax
    mov rax, 0
    call printf
    jmp .endif_32
.endif_32:
    mov rax, [rbp + -8]
    push rax
    mov rax, 1
    mov rcx, rax
    pop rax
    add rax, rcx
    mov QWORD PTR [rbp + -8], rax
    jmp .for_start_29
.for_end_30:
.return_28:
    mov rsp, rbp
    pop rbp
    ret
