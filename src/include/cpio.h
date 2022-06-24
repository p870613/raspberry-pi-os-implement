#ifndef _CPIO_H
#define _CPIO_H

#include <type.h>
#include <string.h>
#define HEADER_LEN 110
#define CPIO_ADDRESS 0X8000000
#define HEADER_FILESIZE_OFFSET 54
#define HEADER_NAMESIZE_OFFSET 94
#define CPIO_ARRAY_SIZE 100

struct cpio_data {
    size_t header_address;
    size_t name_address;
    size_t file_address;
    size_t name_size;
    size_t file_size;
};

struct cpio_data header[CPIO_ARRAY_SIZE];
int header_index;
void cpio_parse(size_t);
void get_file_content();
void get_path();
void get_content(size_t, size_t, char*);
void get_all_filename();
void* load_file(char*, size_t);

#endif
