/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   panic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/10 17:10:01 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kfs.h"

/* Static variables for panic handling */
static int panic_in_progress = 0;
static uint32_t panic_count = 0;

static void print_panic_header(t_panic_level level)
{
    switch (level) {
        case PANIC_LEVEL_WARNING:
            kernel.screens[kernel.screen_index].color = vga_entry_color(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLACK);
            terminal_writestring("\n*** KERNEL WARNING ***\n");
            break;
        case PANIC_LEVEL_ERROR:
            kernel.screens[kernel.screen_index].color = vga_entry_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
            terminal_writestring("\n*** KERNEL ERROR ***\n");
            break;
        case PANIC_LEVEL_FATAL:
            kernel.screens[kernel.screen_index].color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_RED);
            terminal_writestring("\n*** KERNEL PANIC ***\n");
            break;
    }
}

static void print_panic_footer(t_panic_level level)
{
    terminal_writestring("\nSystem Information:\n");
    terminal_writestring("Panic Count: ");
    printnbr(++panic_count, 10);
    terminal_writestring("\n");
    
    if (g_phys_mem_manager.total_pages > 0) {
        terminal_writestring("Memory: ");
        printnbr(g_phys_mem_manager.used_pages, 10);
        terminal_writestring("/");
        printnbr(g_phys_mem_manager.total_pages, 10);
        terminal_writestring(" pages used\n");
    }
    
    terminal_writestring("Current Directory: 0x");
    if (g_current_directory)
        printnbr((uint32_t)g_current_directory, 16);
    else
        terminal_writestring("NULL");

    terminal_writestring("\n");
    terminal_writestring("Stack Pointer: 0x");
    printnbr(get_esp(), 16);
    terminal_writestring("\n");
    terminal_writestring("Base Pointer: 0x");
    printnbr(get_ebp(), 16);
    terminal_writestring("\n");
    
    switch (level) {
        case PANIC_LEVEL_WARNING:
            terminal_writestring("*** System can continue ***\n");
            break;
        case PANIC_LEVEL_ERROR:
            terminal_writestring("*** System should halt ***\n");
            break;
        case PANIC_LEVEL_FATAL:
            terminal_writestring("*** System halted ***\n");
            break;
    }
    
    kernel.screens[kernel.screen_index].color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
}

static void dump_system_state(void)
{
    terminal_writestring("\nSystem State Dump:\n");
    if (is_paging_enabled()) {
        terminal_writestring("Paging: ENABLED\n");
        terminal_writestring("CR3: 0x");
        printnbr(get_cr3(), 16);
        terminal_writestring("\n");
    }
	else
        terminal_writestring("Paging: DISABLED\n");
    
    terminal_writestring("Interrupts: ");
    terminal_writestring("UNKNOWN\n");
    if (g_kernel_heap.start_addr != 0) {
        terminal_writestring("Kernel Heap: 0x");
        printnbr(g_kernel_heap.start_addr, 16);
        terminal_writestring(" - 0x");
        printnbr(g_kernel_heap.current_end, 16);
        terminal_writestring("\n");
    }
}

void kernel_panic(t_panic_level level, const char *message)
{
    if (panic_in_progress) {
        terminal_writestring("\n*** RECURSIVE PANIC DETECTED ***\n");
        terminal_writestring("Original panic in progress, halting immediately\n");
        DisableInterrupts();
        while (1)
            __asm__ volatile ("hlt");
    }
    
    panic_in_progress = 1;
    if (level >= PANIC_LEVEL_ERROR)
        DisableInterrupts();
    
    print_panic_header(level);
    terminal_writestring("Message: ");
    if (message)
        terminal_writestring(message);
    else
        terminal_writestring("(no message)");
    terminal_writestring("\n");
    
    if (level == PANIC_LEVEL_FATAL)
        dump_system_state();
    
    print_panic_footer(level);
    switch (level) {
        case PANIC_LEVEL_WARNING:
            panic_in_progress = 0;
            return; /* Continue execution */
            
        case PANIC_LEVEL_ERROR:
            terminal_writestring("\nPress any key to continue or wait 5 seconds...\n");
            /* In a real implementation, we might wait for input or timeout */
            /* For now, we'll just continue after a brief pause */
            for (volatile int i = 0; i < 10000000; i++); /* Simple delay */
            panic_in_progress = 0;
            EnableInterrupts();
            return;
            
        case PANIC_LEVEL_FATAL:
            terminal_writestring("\nSystem halted. Please reboot.\n"); /* Halt the system */
            while (1)
                __asm__ volatile ("hlt");
            break;
    }
}

void kernel_warning(const char *message)
{
    kernel_panic(PANIC_LEVEL_WARNING, message);
}

void kernel_error(const char *message)
{
    kernel_panic(PANIC_LEVEL_ERROR, message);
}

void kernel_fatal(const char *message)
{
    kernel_panic(PANIC_LEVEL_FATAL, message);
}

void kernelPanic(void)
{
    kernel_fatal("Legacy kernel panic called");
}

void page_fault_handler(t_registers *regs)
{
    uint32_t fault_addr;
    uint32_t error_code;
    char error_msg[256];
    int msg_pos = 0;
    
    fault_addr = get_page_fault_addr();
    error_code = regs->error;
    const char *base_msg = "Page fault at 0x";
    while (*base_msg && msg_pos < 200)
        error_msg[msg_pos++] = *base_msg++;
    
    for (int i = 7; i >= 0; i--) {
        uint8_t nibble = (fault_addr >> (i * 4)) & 0xF;
        if (nibble < 10)
            error_msg[msg_pos++] = '0' + nibble;
        else
            error_msg[msg_pos++] = 'A' + (nibble - 10);
    }
    
    const char *details = " - ";
    while (*details && msg_pos < 240)
        error_msg[msg_pos++] = *details++;
    
    if (error_code & 0x1) {
        const char *prot = "Protection violation";
        while (*prot && msg_pos < 250)
            error_msg[msg_pos++] = *prot++;
    }
	else {
        const char *not_present = "Page not present";
        while (*not_present && msg_pos < 250)
            error_msg[msg_pos++] = *not_present++;
    }
    
    if (error_code & 0x2) {
        const char *write = " (write)";
        while (*write && msg_pos < 255)
            error_msg[msg_pos++] = *write++;
    } 
	else {
        const char *read = " (read)";
        while (*read && msg_pos < 255) 
            error_msg[msg_pos++] = *read++;
    }
    
    error_msg[msg_pos] = '\0';
    if (fault_addr < KERNEL_START)
        kernel_fatal(error_msg);
	else if (fault_addr >= USER_SPACE_START && fault_addr < USER_SPACE_END) 
        kernel_error(error_msg);
    else
        kernel_fatal(error_msg);
}

void panic_init(void)
{
    ISR_RegisterHandler(14, page_fault_handler);
}
