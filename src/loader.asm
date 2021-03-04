%macro pushx 1-*
 %rep %0
   push %1
   %rotate 1
 %endrep
%endmacro

%macro popx 1-*
  %rep %0
    %rotate -1
    pop %1
  %endrep
%endmacro

[BITS 64]

; section .bss
; original_file_sz: resq 1

section .text

global	entry_loader:function
global	loader_size:data
global	infos_size:data

loader_size	dq	end - entry_loader
infos_size	dq	end - info_start

entry_loader:

	pushfq
	pushx	rax, rdi, rsi, rsp, rdx, rcx

	; write syscall
	mov	rdi, 1
	lea	rsi, [rel msg]
	mov	rdx, msg_len
	mov	rax, rdi
	syscall
	
	; open the executable itself in memory
	lea rdi, [rel filename]
	mov rsi, 0
	mov rax, 2
	syscall

	; test if file is 0
	test rax, rax
	jz end_of_func

	; rbx = open return
	mov rbx, rax

	; mmap syscall (Allocate space to read to process)
	xor r9, r9
	mov r8, -1
	mov r10, 0x22
	mov rsi, [rel original_file_sz]
	mov rdx, 3
	xor edi, edi
	mov rax, 9
	syscall

	; test if addr is NULL
	cmp rax, -1
	jz end_of_func

	; r13 = mmap return
	mov r13, rax

	; Copy the content of the file into the new allocated memory
	xor r9, r9
	mov r8, rbx
	mov r10, 0x12
	mov rsi, [rel original_file_sz]
	mov rdx, 3
	mov rdi, r13
	mov rax, 9
	syscall

	; test if addr is NULL
	cmp rax, -1
	jz end_of_func

	; r13 = mmap return
	mov r13, rax

	; looks for stub
	mov rdi, r13
	lea rsi, [rel stub]
	call strcmp

	; save the return address
	push rax

	; close the opened file
	mov rdi, rbx
	mov rax, 3
	syscall

	;munmap
	mov rsi, [rel original_file_sz]
	mov rdi, r13
	mov rax, 0xb
	syscall

end_of_func:
	; exit syscall
	mov rax, 0x3c
	mov rdi, 0
	syscall

	jmp	start_unpacking

msg	db	"[Unpacking...]", 10, 0
msg_len	equ	$ - msg
stub	 db  "LHEL!", 0
filename db "/proc/self/exe", 0

strcmp:
	push rcx				; save value of rcx and rbx
	push rbx

.reset_rcx:
	xor rcx, rcx
.loop:
	mov bl, byte [rsi+rcx]	; mov al, byte [edi+ecx]	; Subtracts both character to see if they are the same
	mov dl, byte [rsi+rcx]
	sub bl, byte [rdi]		; ----
	jnz .noteq
	inc cl
	inc rdi
	xor dl, 0x21			; Check if the end of the stub was found
	jz .eq					; If end of stub was found, then the stub was found
	jmp .loop				; Continue iterating the string otherwise

.noteq:						; Routine to continue looking for the stub
	inc rdi
	jmp .reset_rcx

.eq:
	pop rbx
	pop rcx					; restore rcx and rbx
	lea rax, [rsi+1]		; Return the address of the remainder, positioned after the end of stub
	ret

start_unpacking:
	mov	rax, [rel info_addr]
	mov	rcx, [rel info_size]
	mov	rdx, [rel info_key]

	add	rcx, rax

.boop:
	xor	byte [rax], dl
	ror	rdx, 8
	inc	rax
	cmp	rax, rcx
	jnz	.boop

	popx	rax, rdi, rsi, rsp, rdx, rcx
	popfq
	jmp	0xFFFFFFFF

info_start:
info_key:		  dq 0x9999999999999999
info_addr:		  dq 0xAAAAAAAAAAAAAAAA
info_size:		  dq 0xBBBBBBBBBBBBBBBB
original_file_sz: dq 0x0000000000000000


end: