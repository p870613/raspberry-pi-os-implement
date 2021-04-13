#include <string.h>

void* memcpy(void* dest, const void* src, size_t n) {
  const int8_t* src_ptr = src;
  int8_t* dest_ptr = dest;

  while(n--) {
    *dest_ptr ++ = *src_ptr ++;
  }
  return dest;
}

void* memset(void* dest, uint8_t val, size_t n) {
  int8_t* dest_ptr = dest;

  while(n--) {
    *dest_ptr ++ = val;
  }
  return dest;
}


size_t strlen(const char* s) {
  size_t len = 0;

  for (; *s; s++) {
    ++len;
  }
  return len;
}

int strcmp(const char* s1, const char* s2) {
  while (*s1 || *s2) {
    if (*s1 == *s2) {
      ++s1;
      ++s2;
    } else {
      return *s1 - *s2;
    }
  }
  return 0;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  while ((*s1 || *s2) && n--) {
    if (*s1 == *s2) {
      ++s1;
      ++s2;
    } else {
      return *s1 - *s2;
    }
  }
  return 0;
}

char* strcpy(char* dest, const char* src) {
  char* ret = dest;
  while (*src) {
    *dest++ = *src++;
  }
  *dest = '\0';
  return ret;
}

char* strncpy(char* dest, const char* src, size_t n) {
  char* ret = dest;
  while (*src && n--) {
    *dest++ = *src++;
  }
  *dest = '\0';
  return ret;
}

char* strcat(char* dest, const char* src) {
  char* cur = dest + strlen(dest);
  while (*src) {
    *cur++ = *src++;
  }
  *cur = '\0';
  return dest;
}

long strtol(const char * start_ptr, const char **__restrict endptr, int base) {
    int neg_flag = 0;
    unsigned long acc = 0;
    
    const char* ptr = start_ptr;

    int index = 0;
    while(1) {
        if(index >= strlen(start_ptr) || *ptr != ' ')
            break;
        index ++;
        ptr ++;
    }

    if(*ptr == '-') {
        neg_flag = 1;
        ptr ++;
    }else if(*ptr == '+') {
        ptr ++;
    }
    
    if((base == 0 || base == 16) &&
       (*ptr == '0') && 
       (*(ptr + 1) == 'x' || *(ptr + 1) == 'X') &&
       ((*(ptr + 2) >= '0' && *(ptr + 2) <= '9') || 
        (*(ptr + 2) >= 'a' && *(ptr + 2) <= 'f') || 
        (*(ptr + 2) >= 'A' && *(ptr + 2) <= 'F')))
       {
            base = 16;
            ptr = ptr + 2;
       }

    if(base == 0)
        base = 10;

    if(base < 2 || base > 36)
        return 0;

    int unsigned_long_size = sizeof(acc) * 8 - 1;
    unsigned long overflow = (neg_flag == 1) ? (1 << unsigned_long_size) - 1 : (1 << unsigned_long_size);

    int c = 0;
    while(1) {
        if(*ptr >= '0' && *ptr <= '9'){
            c = *ptr - '0';
        }else if(*ptr >= 'a' && *ptr <= 'z'){
            c = *ptr - 'a' + 10;
        }else if(*ptr >= 'A' && *ptr <= 'Z'){
            c = *ptr - 'A' + 10;
        }else 
            break;

        if(c >= base)
            break;

        if(acc >= overflow)
            return 0;
        
        acc = acc * base + c;
        uart_int(c);
        uart_put("\n");
        
        ptr ++;
    }

    if(endptr != 0)
        *endptr = ptr - 1;

    return (neg_flag == 1) ? -acc : acc;
}
