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
; output_file_sz: resq 1

section .text

global	entry_loader:function
global	loader_size:data
global	infos_size:data

loader_size	dq	end - entry_loader
infos_size	dq	end - info_start

%include "tree.h"

entry_loader:

	; int3

	;; How to debug this code:  Uncomment the 'int3' breakpoint instruction above.  --> (This was literally copy pasted from UPX doc)
	;; Invoke gdb, and give a 'run' command.  Define a single-step macro such as		you have to stepthrough the code with this otherwise
	;;      define g																	it doesn't work (the program simply executes disregarding your stepping).
	;;      stepi
	;;      x/i $pc
	;;      end
	;; and a step-over macro such as
	;;      define h
	;;      x/2i $pc
	;;      tbreak *$_
	;;      continue
	;;      x/i $pc
	;;      end
	;; Step through the code; remember that <Enter> repeats the previous command.

	pushfq
	pushx	rax, rdi, rsi, rsp, rdx, rcx

	; preamble
	push rbp
	mov rbp, rsp

	; allocate space on the stack
	sub rsp, 64
	
	mov rbp, [rel test]

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

	; test if file fd is 0
	test rax, rax
	jz end_of_func

	; rbp-64 = open return
	mov [rbp-64], rax

	; mmap syscall (Allocate space to read to process)
	xor r9, r9
	mov r8, -1						; fd is -1 (recquired for MAP_ANONYMOUS)
	mov r10, 0x22					; MAP_PRIVATE | MAP_ANONYMOUS
	mov rsi, [rel output_file_sz]
	mov rdx, 3						; PROT_READ | PROT_WRITE
	xor edi, edi					; addr is NULL
	mov rax, 9
	syscall

	; test if addr is NULL
	cmp rax, -1
	jz end_of_func

	; [rbp-56] = first mmap return
	mov [rbp-56], rax

	; Copy the content of the file into the new allocated memory
	xor r9, r9
	mov r8, [rbp-64]				; fd is the value of open fetched above
	mov r10, 0x12					; MAP_PRIVATE | AP_FIXED
	mov rsi, [rel output_file_sz]
	mov rdx, 3
	mov rdi, [rbp-56]				; addr is result of mmap above
	mov rax, 9
	syscall

	; test if addr is NULL
	cmp rax, -1
	jz end_of_func

	; [rbp - 48] = second mmap return (the region of mem where the contents were copied)
	mov [rbp - 48], rax

	; gets the value of "remainder" in code
	mov rdi, [rbp - 48]
	lea rsi, [rel stub]
	call get_remainder

	; save the return value (the ramainder)
	mov [rbp-40], rax

	; fetch the size of the original file and save it
	; [rbp-32] = original file size
	mov eax, dword [rdi]
	mov [rbp-32], rax

	; map in memory a space to unpack the file
	xor r9, r9
	mov r8, -1			; fd is -1
	mov r10, 0x21		; MAP_SHARED | MAP_ANONYMOUS
	mov rsi, [rbp-32]	; length is equal to the size of the original unpacked binary
	mov rdx, 7			; PROT_READ|EXEC|WRITE
	xor rdi, rdi		; addr is NULL
	mov rax, 9			; syscall number
	syscall

	; check if mmap was successful
	cmp rax, -1
	jz end_of_func

	; save the base addr allocated
	; [rbp-24] = rax
	mov [rbp-24], rax

	; close the opened file
	mov rdi, [rbp-64]
	mov rax, 3
	syscall

	; save the size of original file again
	; TODO: the unpacking routine

	;munmap
	mov rsi, [rel output_file_sz]
	mov rdi, [rbp-56]
	mov rax, 0xb
	syscall
	
	;munmap
	mov rsi, [rel output_file_sz]
	mov rdi, [rbp-48]
	mov rax, 0xb
	syscall

	;munmap
	mov rsi, [rbp-32]
	mov rdi, [rbp-24]
	mov rax, 0xb
	syscall

end_of_func:
	; finish context stack
	popx rax, rdi, rsi, rsp, rdx, rcx
	popfq

	; exit syscall
	mov rax, 0x3c
	mov rdi, 0
	syscall

	jmp	start_unpacking

msg	db	"[Unpacking...]", 10, 0
msg_len	equ	$ - msg
stub	 db  "LEL", 0
filename db "/proc/self/exe", 0

get_remainder:
	push rcx				; save value of rcx
.reset_rcx:
	xor rcx, rcx
.fetch_remainder:
	add rdi, 0x7b			; The remainder will always be on offset 0x7b

.eq:
	pop rcx					; restore rcx
	xor rax, rax
	mov al, byte [rdi]		; Return the address of the remainder, positioned after the end of stub
	inc rdi					; Increment rdi after fetching the remainder
	ret

malloc:						; malloc function
	push rbp
	mov rbp, rsp

	xor rdi, rdi
	mov rax, 0xc
	syscall					; brk syscall to get the top of the program break

	mov rdi, rax
	add rdi, 0x5000		    ; mallocs 5kb

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
output_file_sz:   dq 0x0000000000000000


end: