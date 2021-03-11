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
