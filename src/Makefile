
CC=gcc
CFLAGS= -g -Wall

# Aruivos de bibliotecas
FONTES=$(wildcard *.c)
OBJS=$(patsubst %.c,build/%.o,$(FONTES))

# Arquivos de executáveis
MAINS=$(wildcard main/*.c)
EXES=$(patsubst main/%.c,bin/%,$(MAINS))

# Loader
SRC_ASM=loader.asm

OBJ_ASM=$(patsubst %.asm,build/%.o,$(SRC_ASM))

NASM=nasm

NFLAGS+=-f elf64 -g

# Marca os arquivos objeto para que não sejam excluídos
.PRECIOUS: %/ $(OBJS) $(OBJ_ASM)


# Target padrão
all: build/loader.o exe
	cp -f ./bin/* ./

clean:
	rm -f -r ./.dep
	rm -f -r ./bin
	rm -f -r ./build
	rm PUFEB
	rm packed.lhel

teste: exe
	(cd ./teste; make)


# Inclui as listas de dependências
include $(wildcard .dep/**/*)


# Cria pastas e compila todos executáveis
exe: $(EXES)

# Cria uma pasta
%/:
	mkdir -p $@

build/loader.o: loader.asm | build/
	$(NASM) $(NFLAGS) $< -o $@

# Compila bibliotecas
build/%.o: %.c | build/ # .dep/
	$(CC) -o $@ -c $< $(CFLAGS)
	@#$(CC) -MM -MT $@  $< $(CFLAGS)  > .dep/$<.d

# Compila de main/ para para bin/
bin/%: main/%.c $(OBJS) $(OBJ_ASM) | bin/ # .dep/main/
	$(CC) -o $@ $^  $(CFLAGS)
	@#$(CC) -MM -MT $@  $< $(CFLAGS)  > .dep/$<.d
