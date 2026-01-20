/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kmem.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 16:40:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/12/24 16:40:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KMEM_H
# define KMEM_H

# include "libs.h"

# define PAGE_SIZE 4096
# define KHEAP_START 0x10000000
# define KHEAP_INITIAL_SIZE 0x00100000
# define KHEAP_MAX 0x40000000

# define BITMAP_INDEX(a) ((a) / (8 * sizeof(uint32_t)))
# define BITMAP_OFFSET(a) ((a) % (8 * sizeof(uint32_t)))

typedef struct s_mempage {
	uint32_t present  : 1;
	uint32_t rw       : 1;
	uint32_t user     : 1;
	uint32_t rsvd2    : 2;
	uint32_t accessed : 1;
	uint32_t dirty    : 1;
	uint32_t rsvd1    : 2;
	uint32_t unused   : 3;
	uint32_t frame    : 20;
} t_mempage;

typedef struct s_mempage_table {
	t_mempage pages[1024];
} t_mempage_table;

typedef struct s_mempage_directory {
	t_mempage_table *tables[1024];
	uint32_t tables_phys[1024];
	uint32_t phys_addr;
} t_mempage_directory;

extern t_mempage_directory *kernel_directory;
extern t_mempage_directory *current_directory;

void init_frames(void);
void alloc_frame(t_mempage *page, int is_kernel, int is_writeable);
void alloc_frame_at(t_mempage *page, uint32_t phys_addr, int is_kernel, int is_writeable);
void free_frame(t_mempage *page);

t_mempage *get_page(uint32_t address, t_mempage_directory *dir);
t_mempage *create_page(uint32_t address, t_mempage_directory *dir);
void switch_page_directory(t_mempage_directory *dir);
void init_paging(void);

void *kmem_early_alloc(uint32_t size, uint32_t align);

void *kmalloc(uint32_t size);
uint32_t ksize(void *ptr);
void kfree(void *ptr);
void *vmalloc(uint32_t size);
uint32_t vsize(void *ptr);
void vfree(void *ptr);

#endif
