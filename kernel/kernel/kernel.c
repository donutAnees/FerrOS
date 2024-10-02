#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>

void kernel_main(void) {
	terminal_initialize();
	setup_gdt();
	printf("Hello, World!\n");
}