#include "common.h"

void outb(u16int port, u8int value) {
  asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

u8int inb(u16int port) {
  u8int ret;
  asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
  return ret;
}

u16int inw(u16int port) {
  u16int ret;
  asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
  return ret;
}

void memcpy(u8int *dest, const u8int *src, u32int len) {
  // TODO: implementasi dari memcpy
}

void memset(u8int *dest, u8int val, u32int len) {
  // TODO:implementasi dari memset
}

int strcmp(char *str1, char *str2) {
  // TODO: implementasi dari strcmp
}

char *strcpy(char *dest, const char *src) {
  // TODO: implementasi dari strcpy
}

char *strcat(char *dest, const char *src) {
  // TODO: implementasi dari strcat
}
