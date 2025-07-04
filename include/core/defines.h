/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:11:14 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/04 15:11:16 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_H
# define DEFINES_H

/* ──────────── VGA Display Constants ──────────── */
# define VGA_WIDTH   80
# define VGA_HEIGHT  25
# define VGA_MEMORY  0xB8000
# define VGA_CRSR_LOW   0x0F
# define VGA_CRSR_HIGH  0x0E
# define VGA_PORT_INDEX 0x3D4
# define VGA_PORT_DATA  0x3D5

/* ──────────── IDT Constants ──────────── */
# define IDT_ENTRIES 256
# define GDT_CODE_SEGMENT 0x08
# define GDT_DATA_SEGMENT 0x10

/* ──────────── PIC 8259 Ports ──────────── */
#define PIC1_CMD    0x20    /* Command register (Master)  */
#define PIC1_DATA   0x21    /* Data register    (Master)  */
#define PIC2_CMD    0xA0    /* Command register (Slave)   */
#define PIC2_DATA   0xA1    /* Data register    (Slave)   */

/* ──────────── ICW1 (Initialization Cmd Word 1) ──────────── */
#define ICW1_ICW4   0x01    /* ICW4 suivra               */
#define ICW1_INIT   0x10    /* Lancer la séquence d'init */
#define ICW1        (ICW1_INIT | ICW1_ICW4)       /* 0x11 */

/* ──────────── Offsets après remap (ICW2) ──────────── */
#define PIC1_OFFSET 0x20    /* IRQ0-7  → vecteurs 0x20-27 */
#define PIC2_OFFSET 0x28    /* IRQ8-15 → vecteurs 0x28-2F */

/* ──────────── Cascade configuration (ICW3) ──────────── */
#define PIC1_CASCADE_IRQ2 0x04   /* Le Slave est branché sur IRQ2 */
#define PIC2_CASCADE_ID   0x02   /* ID du Slave dans la chaîne    */

/* ──────────── ICW4 ──────────── */
#define ICW4_8086  0x01     /* Mode 8086/88 */

/* ──────────── Masques (OCW1) ──────────── */
#define PIC_MASK_NONE 0x00  /* Dé-masquer toutes les lignes */

/* ──────────── Commande EOI (End-Of-Interrupt) ──────────── */
#define PIC_EOI    0x20

/* ──────────── Screen Management ──────────── */
# define NB_SCREEN 3
# define NB_SCROLL 3

/* ──────────── Utility Macros ──────────── */
# define N(x) (x >> ((sizeof(x) * 8) - 1))
# define ABS(x) ((x + (1 * N(x))) ^ N(x))
# define FLAG_SET(x, flag) x |= (flag)
# define FLAG_UNSET(x, flag) x &= ~(flag)

/* ──────────── Hardware I/O Constants ──────────── */
#define UNUSED_PORT		0x80

/* ──────────── Memory Management Constants ──────────── */
#define MIN_BLOCK_SIZE		(sizeof(t_mem_block) + 16)

/* ──────────── Stack Management Constants ──────────── */
#define STACK_SIZE		1024

/* ──────────── Shell Constants ──────────── */
#define COMMAND_BUFFER_SIZE	256

/* ──────────── Memory Constants ──────────── */
#define PAGE_SIZE           4096        /* 4KB pages */
#define PAGE_ALIGN_MASK     0xFFFFF000  /* Page alignment mask */
#define PAGE_OFFSET_MASK    0x00000FFF  /* Page offset mask */
#define PAGES_PER_TABLE     1024        /* Pages per page table */
#define TABLES_PER_DIR      1024        /* Page tables per directory */

/* Memory layout constants */
#define KERNEL_START        0x00100000  /* 1MB - Kernel start */
#define KERNEL_HEAP_START   0x00400000  /* 4MB - Kernel heap start */
#define KERNEL_HEAP_SIZE    0x00400000  /* 4MB - Kernel heap size */
#define USER_SPACE_START    0x40000000  /* 1GB - User space start */
#define USER_SPACE_END      0xC0000000  /* 3GB - User space end */

/* Physical memory constants */
#define PHYS_MEM_START      0x00100000  /* 1MB - Start of usable physical memory */
#define MAX_PHYS_MEMORY     0x10000000  /* 256MB - Maximum physical memory */
#define BITMAP_SIZE         (MAX_PHYS_MEMORY / PAGE_SIZE / 8) /* Bitmap size in bytes */

/* ──────────── Page Directory/Table Entry Flags ──────────── */
#define PAGE_PRESENT        0x001       /* Page is present in memory */
#define PAGE_WRITABLE       0x002       /* Page is writable */
#define PAGE_USER           0x004       /* Page is accessible by user */
#define PAGE_WRITE_THROUGH  0x008       /* Write-through caching */
#define PAGE_CACHE_DISABLE  0x010       /* Cache disabled */
#define PAGE_ACCESSED       0x020       /* Page was accessed */
#define PAGE_DIRTY          0x040       /* Page was written to (only for page table entries) */
#define PAGE_SIZE_4MB       0x080       /* 4MB page size (only for page directory entries) */
#define PAGE_GLOBAL         0x100       /* Global page */

#endif /* DEFINES_H */
