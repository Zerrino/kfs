/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_multiboot.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zerrino <zerrino@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 03:11:59 by zerrino           #+#    #+#             */
/*   Updated: 2025/07/13 04:13:09 by zerrino          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MULTIBOOT_H
#define FT_MULTIBOOT_H

#include <stdint.h>

/* ──────────────────────────────────────────────────────────
 *   1.  Multiboot 1
 * ────────────────────────────────────────────────────────── */

#define MULTIBOOT_HEADER_MAGIC        0x1BADB002u   /* dans le header ELF  */
#define MULTIBOOT_BOOTLOADER_MAGIC    0x2BADB002u   /* placé dans EAX       */

/* Align + meminfo -> flags « obligatoires » les plus courants */
#define MULTIBOOT_HEADER_FLAGS        0x00000003u

/* ── 1.1  Sous-structures ──────────────────────────────── */

typedef struct multiboot_module {
    uint32_t mod_start;
    uint32_t mod_end;
    uint32_t string;     /* ptr vers une C-string */
    uint32_t reserved;
} __attribute__((packed)) multiboot_module_t;

/* map BIOS type 1 = RAM libre, type 2 = réservé, etc.        */
typedef struct multiboot_mmap_entry {
    uint32_t size;       /* = sizeof(struct) - sizeof(size)   */
    uint64_t addr;
    uint64_t len;
    uint32_t type;
} __attribute__((packed)) multiboot_memory_map_t;

/* ── 1.2  Structure principale passée en EBX ────────────── */

typedef struct multiboot_info {
    uint32_t flags;            /* indique quels champs sont valides */

    /* 0 */ uint32_t mem_lower;   /* KB < 1 MiB                         */
          uint32_t mem_upper;   /* KB au-delà de 1 MiB                */

    /* 1 */ uint32_t boot_device;

    /* 2 */ uint32_t cmdline;

    /* 3 */ uint32_t mods_count;
          uint32_t mods_addr;   /* -> multiboot_module_t[mods_count]  */

    /* 4 ou 5 : symboles a.out ou table ELF (union) */
    union {
        struct {                /* a.out symbol table            */
            uint32_t tabsize;
            uint32_t strsize;
            uint32_t addr;
            uint32_t reserved;
        } aout;
        struct {                /* ELF section table             */
            uint32_t num;
            uint32_t size;
            uint32_t addr;
            uint32_t shndx;
        } elf;
    } syms;

    /* 6 */ uint32_t mmap_length;
          uint32_t mmap_addr;   /* -> multiboot_memory_map_t[...] */

    /* 7 */ uint32_t drives_length;
          uint32_t drives_addr;

    /* 8 */ uint32_t config_table;      /* PCI PnP                     */

    /* 9 */ uint32_t boot_loader_name;  /* C-string « GRUB 2.12 »       */

    /*10 */ uint32_t apm_table;         /* BIOS APM 1.2                */

    /*11 */ uint32_t vbe_control_info;  /* struct VBE info             */
          uint32_t vbe_mode_info;
          uint16_t vbe_mode;
          uint16_t vbe_interface_seg;
          uint16_t vbe_interface_off;
          uint16_t vbe_interface_len;

    /*12 */ uint64_t framebuffer_addr;  /* mode graphique moderne      */
          uint32_t framebuffer_pitch;
          uint32_t framebuffer_width;
          uint32_t framebuffer_height;
          uint8_t  framebuffer_bpp;
          uint8_t  framebuffer_type;
          union {
              struct {                   /* type 0 = indexed colour   */
                  uint32_t palette_addr;
                  uint16_t palette_num_colors;
              } __attribute__((packed)) indexed;
              struct {                   /* type 1 = direct RGB       */
                  uint8_t red_field_position;
                  uint8_t red_mask_size;
                  uint8_t green_field_position;
                  uint8_t green_mask_size;
                  uint8_t blue_field_position;
                  uint8_t blue_mask_size;
              } __attribute__((packed)) rgb;
          } __attribute__((packed)) framebuffer;
} __attribute__((packed)) multiboot_info_t;

/* ──────────────────────────────────────────────────────────
 *   2.  Multiboot 2 (tags en liste chaînée 8-octets alignée)
 * ────────────────────────────────────────────────────────── */

#define MULTIBOOT2_HEADER_MAGIC       0xE85250D6u    /* dans l’ELF           */
#define MULTIBOOT2_BOOTLOADER_MAGIC   0x36D76289u    /* dans EAX             */

/* ── 2.1  En-tête générique de tag ───────────────────────── */

typedef struct multiboot_tag {
    uint32_t type;
    uint32_t size;   /* inclut ce header                        */
} __attribute__((packed)) multiboot_tag_t;

/* Tag 0 = fin de liste                                          */
#define MULTIBOOT_TAG_TYPE_END                0
#define MULTIBOOT_TAG_ALIGN                   8

/* ── 2.2  Tags usuels ───────────────────────────────────────── */

#define MULTIBOOT_TAG_TYPE_CMDLINE            1
typedef struct multiboot_tag_cmdline {
    multiboot_tag_t tag;
    char            string[0];
} __attribute__((packed)) multiboot_tag_cmdline_t;

#define MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME   2
typedef struct multiboot_tag_string {
    multiboot_tag_t tag;
    char            string[0];
} __attribute__((packed)) multiboot_tag_string_t;

#define MULTIBOOT_TAG_TYPE_MODULE             3
typedef struct multiboot_tag_module {
    multiboot_tag_t tag;
    uint32_t        mod_start;
    uint32_t        mod_end;
    char            string[0];
} __attribute__((packed)) multiboot_tag_module_t;

#define MULTIBOOT_TAG_TYPE_BASIC_MEMINFO      4
typedef struct multiboot_tag_basic_meminfo {
    multiboot_tag_t tag;
    uint32_t        mem_lower;
    uint32_t        mem_upper;
} __attribute__((packed)) multiboot_tag_basic_meminfo_t;

#define MULTIBOOT_TAG_TYPE_MMAP               6
typedef struct multiboot_tag_mmap {
    multiboot_tag_t tag;
    uint32_t        entry_size;     /* = sizeof(mmap_entry2)         */
    uint32_t        entry_version;  /* = 0                           */
    /* array of mmap_entry2[] suit… */
} __attribute__((packed)) multiboot_tag_mmap_t;

typedef struct multiboot_mmap_entry2 {
    uint64_t addr;
    uint64_t len;
    uint32_t type;       /* 1 = RAM, 2 = reserved, … */
    uint32_t reserved;
} __attribute__((packed)) multiboot_mmap_entry2_t;

#define MULTIBOOT_TAG_TYPE_FRAMEBUFFER        8
typedef struct multiboot_tag_framebuffer {
    multiboot_tag_t tag;
    uint64_t        framebuffer_addr;
    uint32_t        framebuffer_pitch;
    uint32_t        framebuffer_width;
    uint32_t        framebuffer_height;
    uint8_t         framebuffer_bpp;
    uint8_t         framebuffer_type; /* 0=indexed,1=RGB,2=EGA       */
    uint16_t        reserved;
    /* palettes ou masks suivent selon framebuffer_type              */
} __attribute__((packed)) multiboot_tag_framebuffer_t;

/* ── 2.3  Macros utilitaires ─────────────────────────────────── */

#define MB2_TAG_NEXT(tag) \
    (multiboot_tag_t *)(((uint8_t *)(tag) + ((tag)->size + 7)) & ~7)


#define _1MB   (1024 * 1024)
#define _128MB (128 * _1MB)		// 134217728
#define _256MB (256 * _1MB)		// 268435456
#define _512MB (512 * _1MB)		// 536870912
#define _1GB   (1024 * _1MB)	// 1073741824
#define _2GB   (2048 * _1MB)	// 2147483648

#define PAGE_SIZE 4096

#define MAX_PAGES (_128MB / PAGE_SIZE)             /* 512 MiB / 4 KiB  → ajuste si besoin */


typedef struct
{
	uint32_t	stack_top;
	uint32_t	stack[MAX_PAGES];       /* addresses >> 12 (= numéros de pages) */
} pmm_t;

extern pmm_t pmm;


void	*phys_to_virt(uintptr_t p);
void	parse_multiboot1(const multiboot_info_t *mbi);


#endif
