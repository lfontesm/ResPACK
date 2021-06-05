#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ELF.h"
#include "mapelfinput.h"

#if !defined(SECTION_H_)
#define SECTION_H_

void write_ehdr(int outfilefd);
void write_phdr(int outfilefd);
void write_encoded_tree(int inputfilefd, int outfilefd);


#endif // SECTION_H_

/* 
int32_t		insert_section(t_elf *elf)
{
  uint16_t	last_section = (uint16_t)-1;
  uint16_t	last_ptload = (uint16_t)-1;

  for (uint16_t id = 0; id < elf->elf_header->e_phnum; id += 1) {
      if (elf->prog_header[id].p_type == PT_LOAD) {
	last_ptload = id;
      }
  }
  if (last_ptload == (uint16_t)-1) {
    printf("can't find PT_LOAD section\n");
    return (-1);
  }

  for (uint16_t id = 0; id < elf->elf_header->e_shnum; id += 1) {

    Elf64_Phdr	*phdr = elf->prog_header + last_ptload;
    Elf64_Shdr	*shdr = elf->section_header + id;

    if (shdr->sh_addr + shdr->sh_size >= phdr->p_vaddr + phdr->p_memsz) {
      last_section = id;
    }
  }
  if (last_section == (uint16_t)-1) {
    printf("can't find last section\n");
    return (-1);
  }

  create_section(elf, last_section, last_ptload);

  last_section += 1;

  uint64_t	size = elf->prog_header[last_ptload].p_memsz + loader_size;
  elf->prog_header[last_ptload].p_memsz = size;
  elf->prog_header[last_ptload].p_filesz = size;

  for (uint16_t i = 0; i < elf->elf_header->e_phnum; i++) {
    if(elf->prog_header[i].p_type == PT_LOAD) {
      elf->prog_header[i].p_flags = PF_X | PF_W | PF_R;
    }
  }

  change_entry(elf, last_section);

  for (uint16_t i = last_section; i < elf->elf_header->e_shnum - 1; i += 1) {
    elf->section_header[i + 1].sh_offset = elf->section_header[i].sh_offset + elf->section_header[i].sh_size;
  }

  if (elf->elf_header->e_shstrndx > last_section) {
    elf->elf_header->e_shstrndx += 1;
  }

  uint16_t	shnum = elf->elf_header->e_shnum;
  elf->elf_header->e_shoff = elf->section_header[shnum - 1].sh_offset + elf->section_header[shnum - 1].sh_size;

  return (0);
}
 */