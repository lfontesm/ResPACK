	.file	"unpack.cpp"
	.intel_syntax noprefix
	.text
	.globl	_Z6getBitP4bitsPhP8_IO_FILEPiS4_j
	.type	_Z6getBitP4bitsPhP8_IO_FILEPiS4_j, @function
_Z6getBitP4bitsPhP8_IO_FILEPiS4_j:
.LFB15:
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
.LFE15:
	.size	_Z6getBitP4bitsPhP8_IO_FILEPiS4_j, .-_Z6getBitP4bitsPhP8_IO_FILEPiS4_j
	.section	.rodata
	.align 8
.LC0:
	.string	"Error while deserializing binary sequency\n"
	.text
	.globl	_Z11deserializeP4treeP8_IO_FILES2_j
	.type	_Z11deserializeP4treeP8_IO_FILES2_j, @function
_Z11deserializeP4treeP8_IO_FILES2_j:
.LFB16:
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
	setne	al
	test	al, al
	jne	.L17
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
	call	_Z12tree_is_leafP4tree@PLT
	test	eax, eax
	setne	al
	test	al, al
	je	.L8
	mov	rax, QWORD PTR -56[rbp]
	mov	rdi, rax
	call	_Z9tree_charP4tree@PLT
	mov	BYTE PTR -37[rbp], al
.L10:
	mov	eax, DWORD PTR -32[rbp]
	test	eax, eax
	je	.L18
	mov	r8d, DWORD PTR -76[rbp]
	lea	rdi, -32[rbp]
	lea	rcx, -36[rbp]
	mov	rdx, QWORD PTR -64[rbp]
	lea	rsi, -39[rbp]
	lea	rax, -16[rbp]
	mov	r9d, r8d
	mov	r8, rdi
	mov	rdi, rax
	call	_Z6getBitP4bitsPhP8_IO_FILEPiS4_j
	movzx	eax, BYTE PTR -37[rbp]
	mov	rdx, QWORD PTR -72[rbp]
	mov	rsi, rdx
	mov	edi, eax
	call	fputc@PLT
	jmp	.L10
.L8:
	mov	eax, DWORD PTR -32[rbp]
	test	eax, eax
	je	.L5
	mov	DWORD PTR -28[rbp], 1
	mov	rax, QWORD PTR -56[rbp]
	mov	QWORD PTR -24[rbp], rax
.L13:
	mov	eax, DWORD PTR -28[rbp]
	test	eax, eax
	je	.L12
	mov	eax, DWORD PTR -32[rbp]
	test	eax, eax
	je	.L12
	mov	r8d, DWORD PTR -76[rbp]
	lea	rdi, -32[rbp]
	lea	rcx, -36[rbp]
	mov	rdx, QWORD PTR -64[rbp]
	lea	rsi, -39[rbp]
	lea	rax, -16[rbp]
	mov	r9d, r8d
	mov	r8, rdi
	mov	rdi, rax
	call	_Z6getBitP4bitsPhP8_IO_FILEPiS4_j
	mov	BYTE PTR -38[rbp], al
	movzx	ecx, BYTE PTR -38[rbp]
	lea	rdx, -28[rbp]
	mov	rax, QWORD PTR -24[rbp]
	mov	esi, ecx
	mov	rdi, rax
	call	_Z4walkP4treehPi@PLT
	mov	QWORD PTR -24[rbp], rax
	jmp	.L13
.L12:
	mov	eax, DWORD PTR -28[rbp]
	test	eax, eax
	je	.L14
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
.L14:
	mov	rax, QWORD PTR -24[rbp]
	mov	rdi, rax
	call	_Z9tree_charP4tree@PLT
	movzx	eax, al
	mov	rdx, QWORD PTR -72[rbp]
	mov	rsi, rdx
	mov	edi, eax
	call	fputc@PLT
	jmp	.L8
.L17:
	nop
	jmp	.L5
.L18:
	nop
.L5:
	mov	rax, QWORD PTR -8[rbp]
	sub	rax, QWORD PTR fs:40
	je	.L16
	call	__stack_chk_fail@PLT
.L16:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE16:
	.size	_Z11deserializeP4treeP8_IO_FILES2_j, .-_Z11deserializeP4treeP8_IO_FILES2_j
	.section	.rodata
.LC1:
	.string	"Uso: %s <entrada> <outFile>\n"
.LC2:
	.string	"r"
	.align 8
.LC3:
	.string	"Falha ao abrir '%s' para leitura\n"
.LC4:
	.string	"w"
	.align 8
.LC5:
	.string	"Falha ao abrir '%s' para escrita\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB17:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 64
	mov	DWORD PTR -52[rbp], edi
	mov	QWORD PTR -64[rbp], rsi
	cmp	DWORD PTR -52[rbp], 2
	jg	.L20
	mov	rax, QWORD PTR -64[rbp]
	mov	rdx, QWORD PTR [rax]
	mov	rax, QWORD PTR stderr[rip]
	lea	rsi, .LC1[rip]
	mov	rdi, rax
	mov	eax, 0
	call	fprintf@PLT
	mov	eax, 0
	jmp	.L21
.L20:
	mov	rax, QWORD PTR -64[rbp]
	mov	rax, QWORD PTR 8[rax]
	mov	QWORD PTR -40[rbp], rax
	mov	rax, QWORD PTR -64[rbp]
	mov	rax, QWORD PTR 16[rax]
	mov	QWORD PTR -32[rbp], rax
	mov	rax, QWORD PTR -40[rbp]
	lea	rsi, .LC2[rip]
	mov	rdi, rax
	call	fopen@PLT
	mov	QWORD PTR -24[rbp], rax
	cmp	QWORD PTR -24[rbp], 0
	jne	.L22
	mov	rax, QWORD PTR stderr[rip]
	mov	rdx, QWORD PTR -40[rbp]
	lea	rsi, .LC3[rip]
	mov	rdi, rax
	mov	eax, 0
	call	fprintf@PLT
	mov	eax, 1
	jmp	.L21
.L22:
	mov	rax, QWORD PTR -32[rbp]
	lea	rsi, .LC4[rip]
	mov	rdi, rax
	call	fopen@PLT
	mov	QWORD PTR -16[rbp], rax
	cmp	QWORD PTR -16[rbp], 0
	jne	.L23
	mov	rax, QWORD PTR stderr[rip]
	mov	rdx, QWORD PTR -32[rbp]
	lea	rsi, .LC5[rip]
	mov	rdi, rax
	mov	eax, 0
	call	fprintf@PLT
	mov	eax, 1
	jmp	.L21
.L23:
	mov	rax, QWORD PTR -24[rbp]
	mov	rdi, rax
	call	fgetc@PLT
	mov	BYTE PTR -41[rbp], al
	mov	rax, QWORD PTR -24[rbp]
	mov	rdi, rax
	call	feof@PLT
	test	eax, eax
	setne	al
	test	al, al
	je	.L24
	mov	eax, 1
	jmp	.L21
.L24:
	mov	rax, QWORD PTR -24[rbp]
	mov	rdi, rax
	call	_Z16tree_deserializeP8_IO_FILE@PLT
	mov	QWORD PTR -8[rbp], rax
	movzx	ecx, BYTE PTR -41[rbp]
	mov	rdx, QWORD PTR -16[rbp]
	mov	rsi, QWORD PTR -24[rbp]
	mov	rax, QWORD PTR -8[rbp]
	mov	rdi, rax
	call	_Z11deserializeP4treeP8_IO_FILES2_j
	mov	rax, QWORD PTR -24[rbp]
	mov	rdi, rax
	call	fclose@PLT
	mov	rax, QWORD PTR -16[rbp]
	mov	rdi, rax
	call	fclose@PLT
	mov	rax, QWORD PTR -8[rbp]
	mov	rdi, rax
	call	_Z9free_treeP4tree@PLT
	mov	eax, 0
.L21:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE17:
	.size	main, .-main
	.ident	"GCC: (GNU) 10.2.0"
	.section	.note.GNU-stack,"",@progbits
