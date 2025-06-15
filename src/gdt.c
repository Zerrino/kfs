#include "../include/kernel.h"

// GDT entry structure
struct gdt_entry {
    uint16_t limit_low;           // The lower 16 bits of the limit
    uint16_t base_low;            // The lower 16 bits of the base
    uint8_t  base_middle;         // The next 8 bits of the base
    uint8_t  access;              // Access flags, determine ring level
    uint8_t  granularity;         // Granularity flags + limit bits 16-19
    uint8_t  base_high;           // The last 8 bits of the base
} __attribute__((packed));

// GDT pointer structure
struct gdt_ptr {
    uint16_t limit;               // Size of GDT minus one
    uint32_t base;                // Address of the first GDT entry
} __attribute__((packed));

// Define the GDT at the specific address 0x00000800 as required
struct gdt_entry *gdt = (struct gdt_entry *)0x00000800;
struct gdt_ptr   gdt_pointer;     // GDT pointer

// Function to set a GDT entry
void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;

    gdt[num].limit_low   = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);

    gdt[num].access      = access;
}

// Function to install the GDT
void gdt_install() {
    // Set up the GDT pointer
    gdt_pointer.limit = (sizeof(struct gdt_entry) * 6) - 1;
    
    // Set the GDT base to the required address 0x00000800
    gdt_pointer.base = 0x00000800;
    
    // Null descriptor (index 0)
    gdt_set_gate(0, 0, 0, 0, 0);
    
    // Kernel Code Segment (index 1)
    // Access: 0x9A = 10011010b (Present, Ring 0, Code, Executable, Direction 0, Readable)
    // Granularity: 0xC0 = 11000000b (4KiB blocks, 32-bit protected mode)
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xC0);
    
    // Kernel Data Segment (index 2)
    // Access: 0x92 = 10010010b (Present, Ring 0, Data, Direction 0, Writable)
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xC0);
    
    // Kernel Stack Segment (index 3)
    // Similar to data segment but for stack
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0x92, 0xC0);
    
    // User Code Segment (index 4)
    // Access: 0xFA = 11111010b (Present, Ring 3, Code, Executable, Direction 0, Readable)
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xFA, 0xC0);
    
    // User Data Segment (index 5)
    // Access: 0xF2 = 11110010b (Present, Ring 3, Data, Direction 0, Writable)
    gdt_set_gate(5, 0, 0xFFFFFFFF, 0xF2, 0xC0);
    
    // Load the GDT
    gdt_flush(&gdt_pointer);
}
