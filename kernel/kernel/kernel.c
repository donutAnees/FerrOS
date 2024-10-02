#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>

void kernel_main(void) {
	setup_gdt();
	terminal_initialize();
	printf("Hello, kernel World!\n");
}