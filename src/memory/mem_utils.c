/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/10 16:01:54 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kfs.h"

uint32_t align_to_page(uint32_t addr)
{
    if (addr & PAGE_OFFSET_MASK)
        return (addr & PAGE_ALIGN_MASK) + PAGE_SIZE;
    return addr;
}

uint32_t page_align_down(uint32_t addr)
{
    return addr & PAGE_ALIGN_MASK;
}

uint32_t get_page_index(uint32_t addr)
{
    return (addr >> 22) & 0x3FF;
}

uint32_t get_table_index(uint32_t addr)
{
    return (addr >> 12) & 0x3FF;
}

uint32_t get_page_offset(uint32_t addr)
{
    return addr & PAGE_OFFSET_MASK;
}

void mem_print_stats(void)
{
    terminal_writestring("\n=== Memory Statistics ===\n");
    terminal_writestring("Physical Memory:\n");
    terminal_writestring("  Total pages: ");
    printnbr(g_phys_mem_manager.total_pages, 10);
    terminal_writestring("\n  Used pages: ");
    printnbr(g_phys_mem_manager.used_pages, 10);
    terminal_writestring("\n  Free pages: ");
    printnbr(g_phys_mem_manager.free_pages, 10);
    terminal_writestring("\n  Memory usage: ");
	
    if (g_phys_mem_manager.total_pages > 0) {
        uint32_t usage_percent = (g_phys_mem_manager.used_pages * 100) / g_phys_mem_manager.total_pages;
        printnbr(usage_percent, 10);
        terminal_writestring("%\n");
    }
	else 
        terminal_writestring("N/A\n");
    
    terminal_writestring("Kernel Heap:\n");
    terminal_writestring("  Start: 0x");
    printnbr(g_kernel_heap.start_addr, 16);
    terminal_writestring("\n  End: 0x");
    printnbr(g_kernel_heap.end_addr, 16);
    terminal_writestring("\n  Current: 0x");
    printnbr(g_kernel_heap.current_end, 16);
    terminal_writestring("\n  Size: ");
    printnbr((g_kernel_heap.current_end - g_kernel_heap.start_addr) / 1024, 10);
    terminal_writestring(" KB\n");
    terminal_writestring("========================\n\n");
}

void mem_dump_page_directory(t_page_directory *dir)
{
    uint32_t i;
    t_page_dir_entry *entry;
    
    if (!dir) {
        terminal_writestring("Page directory is NULL\n");
        return;
    }
    
    terminal_writestring("=== Page Directory Dump ===\n");
    for (i = 0; i < TABLES_PER_DIR; i++) {
        entry = &dir->tables[i];
        if (entry->present) {
            terminal_writestring("Entry ");
            printnbr(i, 10);
            terminal_writestring(": Frame=0x");
            printnbr(entry->frame << 12, 16);
            terminal_writestring(" Flags=");
            if (entry->writable)
				terminal_writestring("W");
            if (entry->user)
				terminal_writestring("U");
            if (entry->page_size)
				terminal_writestring("4M");
            else
			terminal_writestring("4K");
            terminal_writestring("\n");
        }
    }
    terminal_writestring("===========================\n");
}

void mem_dump_page_table(t_page_table *table)
{
    uint32_t i;
    t_page_table_entry *entry;
    uint32_t present_count = 0;
    
    if (!table) {
        terminal_writestring("Page table is NULL\n");
        return;
    }
    
    terminal_writestring("=== Page Table Dump ===\n");
    for (i = 0; i < PAGES_PER_TABLE; i++) {
        entry = &table->pages[i];
        if (entry->present) {
            present_count++;
            if (present_count <= 10) { /* Limit output to first 10 entries */
                terminal_writestring("Page ");
                printnbr(i, 10);
                terminal_writestring(": Frame=0x");
                printnbr(entry->frame << 12, 16);
                terminal_writestring(" Flags=");
                if (entry->writable) 
					terminal_writestring("W");
                if (entry->user)
					terminal_writestring("U");
                if (entry->dirty)
					terminal_writestring("D");
                if (entry->accessed)
				terminal_writestring("A");
                terminal_writestring("\n");
            }
        }
    }
    
    if (present_count > 10) {
        terminal_writestring("... and ");
        printnbr(present_count - 10, 10);
        terminal_writestring(" more pages\n");
    }
    
    terminal_writestring("Total present pages: ");
    printnbr(present_count, 10);
    terminal_writestring("\n========================\n");
}

void mem_check_integrity(void)
{
    uint32_t calculated_used = 0;
    uint32_t calculated_free = 0;
    uint32_t i;
    
    terminal_writestring("Checking memory integrity...\n");
    for (i = 0; i < g_phys_mem_manager.total_pages; i++) {
        if (phys_is_page_free(i * PAGE_SIZE))
            calculated_free++;
        else
            calculated_used++;
    }
    
    if (calculated_used != g_phys_mem_manager.used_pages) {
        terminal_writestring("ERROR: Used page count mismatch! ");
        terminal_writestring("Expected: ");
        printnbr(g_phys_mem_manager.used_pages, 10);
        terminal_writestring(", Calculated: ");
        printnbr(calculated_used, 10);
        terminal_writestring("\n");
        kernel_error("Memory integrity check failed");
        return;
    }
    
    if (calculated_free != g_phys_mem_manager.free_pages) {
        terminal_writestring("ERROR: Free page count mismatch! ");
        terminal_writestring("Expected: ");
        printnbr(g_phys_mem_manager.free_pages, 10);
        terminal_writestring(", Calculated: ");
        printnbr(calculated_free, 10);
        terminal_writestring("\n");
        kernel_error("Memory integrity check failed");
        return;
    }
    
    if (calculated_used + calculated_free != g_phys_mem_manager.total_pages) {
        terminal_writestring("ERROR: Total page count mismatch!\n");
        kernel_error("Memory integrity check failed");
        return;
    }
    
    terminal_writestring("Memory integrity check passed\n");
}

void format_size(uint32_t bytes, char *buffer, size_t buffer_size)
{
    const char *units[] = {"B", "KB", "MB", "GB"};
    uint32_t unit_index = 0;
    uint32_t size = bytes;
    
    while (size >= 1024 && unit_index < 3) {
        size /= 1024;
        unit_index++;
    }
    
    uint32_t i = 0;
    uint32_t temp = size;
    if (temp == 0)
        buffer[i++] = '0';
    else {
        char temp_buf[16];
        uint32_t j = 0;
        while (temp > 0 && j < 15) {
            temp_buf[j++] = '0' + (temp % 10);
            temp /= 10;
        }
        
        while (j > 0 && i < buffer_size - 4)
            buffer[i++] = temp_buf[--j];
    }
    
    if (i < buffer_size - 3) {
        buffer[i++] = ' ';
        const char *unit = units[unit_index];
        while (*unit && i < buffer_size - 1)
            buffer[i++] = *unit++;
    }
    
    buffer[i] = '\0';
}
