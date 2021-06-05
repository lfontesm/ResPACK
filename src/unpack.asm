; 
; 
; 
; 
; 
; 
; 
; 
; 
; 
; FILE USED TO GET AN IDEA ON WHAT NEEDS TO BE DONE
; 
; 
; 
; 
; 
; 
; 
; 
; 
; 
	.file	"unpack.c"
	.intel_syntax noprefix
	.text
	.globl	getBit
	.type	getBit, @function
getBit:
.LFB6:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 64
	mov	QWORD PTR -24[rbp], rdi
	mov	QWORD PTR -32[rbp], rsi
	mov	QWORD PTR -40[rbp], rdx
	mov	QWORD PTR -48[rbp], rcx
	mov	QWORD PTR -56[rbp], r8
	mov	DWORD PTR -60[rbp], r9d
	mov	rax, QWORD PTR -24[rbp]
	movzx	eax, BYTE PTR 4[rax]
	movzx	esi, al
	mov	rax, QWORD PTR -24[rbp]
	mov	eax, DWORD PTR [rax]
	lea	ecx, 1[rax]
	mov	rdx, QWORD PTR -24[rbp]
	mov	DWORD PTR [rdx], ecx
	mov	edx, 7
	sub	edx, eax
	mov	eax, esi
	mov	ecx, edx
	sar	eax, cl
	and	eax, 1
	mov	BYTE PTR -1[rbp], al
	mov	rax, QWORD PTR -48[rbp]
	mov	eax, DWORD PTR [rax]
	test	eax, eax
	je	.L2
	mov	rax, QWORD PTR -24[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	DWORD PTR -60[rbp], eax
	jne	.L3
	mov	rax, QWORD PTR -56[rbp]
	mov	DWORD PTR [rax], 0
	jmp	.L3
.L2:
	mov	rax, QWORD PTR -24[rbp]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 8
	jne	.L3
	mov	rax, QWORD PTR -32[rbp]
	movzx	edx, BYTE PTR [rax]
	mov	rax, QWORD PTR -24[rbp]
	mov	BYTE PTR 4[rax], dl
	mov	rax, QWORD PTR -24[rbp]
	mov	DWORD PTR [rax], 0
	mov	rax, QWORD PTR -40[rbp]
	mov	rdi, rax
	call	fgetc@PLT
	mov	edx, eax
	mov	rax, QWORD PTR -32[rbp]
	mov	BYTE PTR [rax], dl
	mov	rax, QWORD PTR -40[rbp]
	mov	rdi, rax
	call	feof@PLT
	mov	rdx, QWORD PTR -48[rbp]
	mov	DWORD PTR [rdx], eax
.L3:
	movzx	eax, BYTE PTR -1[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	getBit, .-getBit
	.section	.rodata
	.align 8
.LC0:
	.string	"Error while deserializing binary sequency\n"
	.text
	.globl	deserialize
	.type	deserialize, @function
deserialize:
.LFB7:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 80
	mov	QWORD PTR -56[rbp], rdi
	mov	QWORD PTR -64[rbp], rsi
	mov	QWORD PTR -72[rbp], rdx
	mov	DWORD PTR -76[rbp], ecx
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR -8[rbp], rax
	xor	eax, eax
	mov	DWORD PTR -16[rbp], 0
	mov	BYTE PTR -12[rbp], 0
	mov	BYTE PTR -39[rbp], 0
	mov	DWORD PTR -36[rbp], 0
	mov	DWORD PTR -32[rbp], 1
	mov	rax, QWORD PTR -64[rbp]
	mov	rdi, rax
	call	fgetc@PLT
	mov	BYTE PTR -12[rbp], al
	mov	rax, QWORD PTR -64[rbp]
	mov	rdi, rax
	call	feof@PLT
	test	eax, eax
	jne	.L19
	mov	rax, QWORD PTR -64[rbp]
	mov	rdi, rax
	call	fgetc@PLT
	mov	BYTE PTR -39[rbp], al
	mov	rax, QWORD PTR -64[rbp]
	mov	rdi, rax
	call	feof@PLT
	mov	DWORD PTR -36[rbp], eax
	mov	rax, QWORD PTR -56[rbp]
	mov	rdi, rax
	call	tree_is_leaf@PLT
	test	eax, eax
	je	.L11
	mov	rax, QWORD PTR -56[rbp]
	mov	rdi, rax
	call	tree_char@PLT
	mov	BYTE PTR -37[rbp], al
	jmp	.L9
.L10:
	mov	r8d, DWORD PTR -76[rbp]
	lea	rdi, -32[rbp]
	lea	rcx, -36[rbp]
	mov	rdx, QWORD PTR -64[rbp]
	lea	rsi, -39[rbp]
	lea	rax, -16[rbp]
	mov	r9d, r8d
	mov	r8, rdi
	mov	rdi, rax
	call	getBit
	movzx	eax, BYTE PTR -37[rbp]
	mov	rdx, QWORD PTR -72[rbp]
	mov	rsi, rdx
	mov	edi, eax
	call	fputc@PLT
.L9:
	mov	eax, DWORD PTR -32[rbp]
	test	eax, eax
	jne	.L10
	jmp	.L5
.L16:
	mov	DWORD PTR -28[rbp], 1
	mov	rax, QWORD PTR -56[rbp]
	mov	QWORD PTR -24[rbp], rax
	jmp	.L12
.L14:
	mov	r8d, DWORD PTR -76[rbp]
	lea	rdi, -32[rbp]
	lea	rcx, -36[rbp]
	mov	rdx, QWORD PTR -64[rbp]
	lea	rsi, -39[rbp]
	lea	rax, -16[rbp]
	mov	r9d, r8d
	mov	r8, rdi
	mov	rdi, rax
	call	getBit
	mov	BYTE PTR -38[rbp], al
	movzx	ecx, BYTE PTR -38[rbp]
	lea	rdx, -28[rbp]
	mov	rax, QWORD PTR -24[rbp]
	mov	esi, ecx
	mov	rdi, rax
	call	walk@PLT
	mov	QWORD PTR -24[rbp], rax
.L12:
	mov	eax, DWORD PTR -28[rbp]
	test	eax, eax
	je	.L13
	mov	eax, DWORD PTR -32[rbp]
	test	eax, eax
	jne	.L14
.L13:
	mov	eax, DWORD PTR -28[rbp]
	test	eax, eax
	je	.L15
	mov	rax, QWORD PTR stderr[rip]
	mov	rcx, rax
	mov	edx, 42
	mov	esi, 1
	lea	rdi, .LC0[rip]
	call	fwrite@PLT
	mov	rax, QWORD PTR -72[rbp]
	mov	rdi, rax
	call	fclose@PLT
	mov	edi, 1
	call	exit@PLT
.L15:
	mov	rax, QWORD PTR -24[rbp]
	mov	rdi, rax
	call	tree_char@PLT
	movzx	eax, al
	mov	rdx, QWORD PTR -72[rbp]
	mov	rsi, rdx
	mov	edi, eax
	call	fputc@PLT
.L11:
	mov	eax, DWORD PTR -32[rbp]
	test	eax, eax
	jne	.L16
	jmp	.L5
.L19:
	nop
.L5:
	mov	rax, QWORD PTR -8[rbp]
	sub	rax, QWORD PTR fs:40
	je	.L18
	call	__stack_chk_fail@PLT
.L18:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	deserialize, .-deserialize
	.section	.rodata
.LC1:
	.string	"r"
.LC2:
	.string	"w"
	.text
	.globl	main
	.type	main, @function
main:
.LFB8:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 64
	mov	DWORD PTR -52[rbp], edi
	mov	QWORD PTR -64[rbp], rsi
	mov	rax, QWORD PTR -64[rbp]
	mov	rax, QWORD PTR 8[rax]
	mov	QWORD PTR -40[rbp], rax
	mov	rax, QWORD PTR -64[rbp]
	mov	rax, QWORD PTR 16[rax]
	mov	QWORD PTR -32[rbp], rax
	mov	rax, QWORD PTR -40[rbp]
	lea	rsi, .LC1[rip]
	mov	rdi, rax
	call	fopen@PLT
	mov	QWORD PTR -24[rbp], rax
	mov	rax, QWORD PTR -32[rbp]
	lea	rsi, .LC2[rip]
	mov	rdi, rax
	call	fopen@PLT
	mov	QWORD PTR -16[rbp], rax
	mov	rax, QWORD PTR -24[rbp]
	mov	rdi, rax
	call	fgetc@PLT
	mov	BYTE PTR -41[rbp], al
	mov	rax, QWORD PTR -24[rbp]
	mov	rdi, rax
	call	feof@PLT
	test	eax, eax
	je	.L21
	mov	eax, 1
	jmp	.L22
.L21:
	mov	rax, QWORD PTR -24[rbp]
	mov	rdi, rax
	call	tree_deserialize@PLT
	mov	QWORD PTR -8[rbp], rax
	movzx	ecx, BYTE PTR -41[rbp]
	mov	rdx, QWORD PTR -16[rbp]
	mov	rsi, QWORD PTR -24[rbp]
	mov	rax, QWORD PTR -8[rbp]
	mov	rdi, rax
	call	deserialize
	mov	rax, QWORD PTR -24[rbp]
	mov	rdi, rax
	call	fclose@PLT
	mov	rax, QWORD PTR -16[rbp]
	mov	rdi, rax
	call	fclose@PLT
	mov	rax, QWORD PTR -8[rbp]
	mov	rdi, rax
	call	free_tree@PLT
	mov	eax, 0
.L22:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	main, .-main
	.ident	"GCC: (GNU) 10.2.0"
	.section	.note.GNU-stack,"",@progbits
