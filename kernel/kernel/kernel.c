#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>

void kernel_main(void) {
	setup_gdt();
	setup_idt();
	terminal_initialize();
	printf("Hello, kernel World!\n");
}