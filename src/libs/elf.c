#include <elf.h>
#include <string.h>

void *elf_header_parse(void *addr)
{
    Elf_hdr *elf_header;
    Elf64_Shdr *section_header_arr;
    void* shst;
    int name_index;

    // elf header address
    elf_header = (Elf_hdr*) addr;
    // section header address
    section_header_arr = (Elf64_Shdr *)(addr + elf_header->e_shoff);
    // section string table address
    shst = addr + ((Elf64_Shdr *)(section_header_arr + elf_header->e_shstrndx))->sh_offset;

    for (int i = 0; i < elf_header->e_shnum; i++) {
        name_index = (section_header_arr + i)->sh_name;
        if(strncmp((char*) (shst + name_index), ".text", 5) == 0) {
            return addr + (section_header_arr + i)->sh_offset;
        } 
    }
    return NULL;
}
