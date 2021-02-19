
CC=gcc
CFLAGS= -g -Wall

# Aruivos de bibliotecas
FONTES=$(wildcard *.c)
OBJS=$(patsubst %.c,build/%.o,$(FONTES))

# Arquivos de executáveis
MAINS=$(wildcard main/*.c)
EXES=$(patsubst main/%.c,bin/%,$(MAINS))

# Marca os arquivos objeto para que não sejam excluídos
.PRECIOUS: %/ $(OBJS)


# Target padrão
all: exe
	cp -f ./bin/* ./

clean:
	rm -f -r ./.dep
	rm -f -r ./bin
	rm -f -r ./build

teste: exe
	(cd ./teste; make)


# Inclui as listas de dependências
include $(wildcard .dep/**/*)


# Cria pastas e compila todos executáveis
exe: $(EXES)

# Cria uma pasta
%/:
	mkdir -p $@

# Compila bibliotecas
build/%.o: %.c | build/ # .dep/
	$(CC) -o $@ -c $< $(CFLAGS)
	@#$(CC) -MM -MT $@  $< $(CFLAGS)  > .dep/$<.d

# Compila de main/ para para bin/
bin/%: main/%.c $(OBJS) | bin/ # .dep/main/
	$(CC) -o $@ $^  $(CFLAGS)
	@#$(CC) -MM -MT $@  $< $(CFLAGS)  > .dep/$<.d
