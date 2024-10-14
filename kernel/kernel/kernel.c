#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/pic.h>
#include <kernel/ps2_kbd.h>

void kernel_main(void) {
	terminal_initialize();
	setup_gdt();
	setup_idt();
	PIC_remap(0x20, 0x28);
	ps2_kbd_init();
	printf("Welcome to FerrOS\n");
	printf("You can type here for now, more features to be added!\n");
	while(true){
	}
}