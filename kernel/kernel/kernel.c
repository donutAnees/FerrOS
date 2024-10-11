#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/pic.h>

void kernel_main(void) {
	setup_gdt();
	setup_idt();
	PIC_remap(0x20, 0x28);
	terminal_initialize();
	printf("Hello, kernel World!\n");
}