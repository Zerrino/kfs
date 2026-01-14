/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 17:10:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/12/24 17:10:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAP_H
# define HEAP_H

# include "libs.h"

# define HEAP_MAGIC 0xC0FFEE42

typedef struct s_heap_header {
	uint32_t			magic;
	uint32_t			size;
	uint8_t				is_hole;
	struct s_heap_header	*prev_free;
	struct s_heap_header	*next_free;
} t_heap_header;

typedef struct s_heap_footer {
	uint32_t		magic;
	t_heap_header	*header;
} t_heap_footer;

typedef struct s_heap {
	uint32_t		start;
	uint32_t		end;
	uint32_t		max;
	uint8_t			supervisor;
	uint8_t			readonly;
	t_heap_header	*free_list;
} t_heap;

extern t_heap *kheap;

t_heap	*create_heap(uint32_t start, uint32_t end, uint32_t max, uint8_t supervisor, uint8_t readonly);
void	*heap_alloc(uint32_t size, uint8_t align, t_heap *heap);
void	heap_free(void *ptr, t_heap *heap);
uint32_t heap_block_size(void *ptr);

#endif
