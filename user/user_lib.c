/*
 * The supporting library for applications.
 * Actually, supporting routines for applications are catalogued as the user 
 * library. we don't do that in PKE to make the relationship between application 
 * and user library more straightforward.
 */

#include "user_lib.h"
#include "util/types.h"
#include "util/snprintf.h"
#include "kernel/syscall.h"

uint64 do_user_call(uint64 sysnum, uint64 a1, uint64 a2, uint64 a3, uint64 a4, uint64 a5, uint64 a6,
                 uint64 a7) {
  uint64 ret;

  // before invoking the syscall, arguments of do_user_call are already loaded into the argument
  // registers (a0-a7) of our (emulated) risc-v machine.
  asm volatile(
      "ecall\n"
      "sd a0, %0"
      : "=m"(ret)
      :
      : "memory");

  return ret;
}

//
// printu() supports user/lab1_1_helloworld.c
//
int printu(const char* s, ...) {
  va_list vl;
  va_start(vl, s);

  char out[256];  // fixed buffer size.
  int res = vsnprintf(out, sizeof(out), s, vl);
  va_end(vl);
  const char* buf = out;
  size_t n = res < sizeof(out) ? res : sizeof(out);

  // make a syscall to implement the required functionality.
  return do_user_call(SYS_user_print, (uint64)buf, n, 0, 0, 0, 0, 0);
}

//
// applications need to call exit to quit execution.
//
int exit(int code) {
  return do_user_call(SYS_user_exit, code, 0, 0, 0, 0, 0, 0); 
}

//
// lib call to naive_malloc
//
void* naive_malloc() {
  return (void*)do_user_call(SYS_user_allocate_page, 0, 0, 0, 0, 0, 0, 0);
}

//
// lib call to naive_free
//
void naive_free(void* va) {
  do_user_call(SYS_user_free_page, (uint64)va, 0, 0, 0, 0, 0, 0);
}

//
// lib call to naive_fork
int fork() {
  return do_user_call(SYS_user_fork, 0, 0, 0, 0, 0, 0, 0);
}

//
// lib call to yield
//
void yield() {
  do_user_call(SYS_user_yield, 0, 0, 0, 0, 0, 0, 0);
}

//
// add syscalls for uart IO @lab4_1
// applications need to call uart_putchar to output a char to device
//
int uartputchar(char ch) {
  return do_user_call(SYS_user_uart_putchar, ch, 0, 0, 0, 0, 0, 0);
}

//
// applications need to call uart_getchar to get a char from device
//
int uartgetchar() {
  return do_user_call(SYS_user_uart_getchar, 0, 0, 0, 0, 0, 0, 0);
}



// car
int gpio_reg_write(char val) {
  return do_user_call(SYS_user_gpio_reg_write, val, 0, 0, 0, 0, 0, 0);
}

// syscalls added @lab4_2
int do_fork(int code) {
  return do_user_call(SYS_user_fork, 0, 0, 0, 0, 0, 0, 0);
}

char *allocate_page() {
    return (char *)do_user_call(SYS_user_allocate_page, 0, 0, 0, 0, 0, 0, 0);
}

void free_page(char *addr) {
    do_user_call(SYS_user_free_page, (uint64)addr, 0, 0, 0, 0, 0, 0);
}

char *allocate_share_page() {
    return (char *)do_user_call(SYS_user_allocate_share_page, 0, 0, 0, 0, 0, 0, 0);
}

int do_open(const char *pathname, int flags) {
    return do_user_call(SYS_user_open, (uint64)pathname, flags, 0, 0, 0, 0, 0);
}

int do_write(int fd, void *buf, uint64 count) {
    return do_user_call(SYS_user_write, fd, (uint64)buf, count, 0, 0, 0, 0);
}

int do_close(int fd) {
    return do_user_call(SYS_user_close, fd, 0, 0, 0, 0, 0, 0);
}

int do_ioctl(int fd, uint64 request, void *data) {
    return do_user_call(SYS_user_ioctl, fd, request, (uint64)data, 0, 0, 0, 0);
}

void *do_mmap(void *addr, uint64 length, int prot, int flags, int fd, int64 offset) {
    return (void *)do_user_call(SYS_user_mmap, (uint64)addr, length, prot, flags, fd, offset, 0);
}

int do_munmap(void *addr, uint64 length) {
    return do_user_call(SYS_user_munmap, (uint64)addr, length, 0, 0, 0, 0, 0);
}

int read_mmap(char *dstva, char *src, uint64 count) {
    return do_user_call(SYS_user_readmmap, (uint64)dstva, (uint64)src, count, 0, 0, 0, 0);
}
