#include <cpio.h>

void get_name_size(size_t* name_size, size_t address) {
    char buf[9];
    strncpy(buf, (char*)(address + HEADER_NAMESIZE_OFFSET), 8);
    buf[8] = '\0';
    *name_size = strtol(buf, 0, 16); 
}

void get_file_size(size_t* file_size, size_t address) {
    char buf[9];
    strncpy(buf, (char*)(address + HEADER_FILESIZE_OFFSET), 8);
    buf[8] = '\0';
    *file_size = strtol(buf, 0, 16); 
}

void get_name_address(size_t* name_address, size_t address) {
    *name_address = address;
}

void get_file_address(size_t* file_address, size_t address) {
    *file_address = address;
}

int padding(size_t address) {
    char* t = (char*) address;
    int ret = 0;
    while(*t == '\0') {
        ret ++;
        t = t + 1;
    }
    return ret;
}

int check_cpio_end(size_t name_size, size_t address) {
    if(name_size == 11 && strncmp((char *)address, "TRAILER!!!", 10))
        return 0;
    return 1;
}

void assing_to_struct(int index, size_t address, size_t file_size, size_t name_size, size_t file_address, size_t name_address){
    header[index].header_address = address;
    header[index].file_size = file_size;
    header[index].name_size = name_size;
    header[index].name_address = name_address;
    header[index].file_address = file_address;    
}

void cpio_parse(size_t address){
    int index = 0;
    size_t cur_address = address;
    size_t file_size, name_size, file_address, name_address;
    
    while(1) {
       get_name_size(&name_size, cur_address);
       get_file_size(&file_size, cur_address);

       if(check_cpio_end(name_size, file_address) == 0)
           break;

       cur_address = cur_address + HEADER_LEN;
       
       get_name_address(&name_address, cur_address);
       cur_address = cur_address + name_size;

       cur_address = cur_address + padding(cur_address);

       get_file_address(&file_address, cur_address);
       cur_address = cur_address + file_size;
       cur_address = cur_address + padding(cur_address);
       
       address = cur_address;
       assing_to_struct(index, address, file_size, name_size, file_address, name_address);
       index ++;
    }
    header_index = index;
}

void get_content(size_t address, size_t offset, char* buf){
    strncpy(buf, (char*)address, offset);
    buf[offset] = '\0';
}

void get_all_filename(){
    for(int i = 0; i < header_index; i++){
        char buf[500];
        get_content(header[i].name_address, header[i].name_size, buf);
        uart_put(buf);
        uart_put("\n");
        /*get_content(header[i].file_address, header[i].file_size, buf);*/
        /*uart_put("file_content: \n");*/
        /*uart_put(buf);*/
        /*uart_put("\n");*/
    }
}

void get_file_content(char* filename) {
    for(int i = 0; i < header_index; i++) {
        char buf[500];
        get_content(header[i].name_address, header[i].name_size, buf);

        if(!strcmp(filename, buf)) {
            uart_put("123");
        }
    }
}

void* load_file(char* filename, size_t filename_size) {
    for(int i = 0; i < header_index; i++) {
        if (!strncmp((char*)header[i].name_address, filename, filename_size)) {
            return (void*)header[i].file_address;
        }
    }
    return NULL;
}
