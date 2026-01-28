/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 17:10:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/12/24 17:10:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

#define HEAP_MIN_HOLE (sizeof(t_heap_header) + sizeof(t_heap_footer) + 4)

t_heap *kheap = NULL;

static void heap_insert_free(t_heap *heap, t_heap_header *header)
{
	header->prev_free = NULL;
	header->next_free = heap->free_list;
	if (heap->free_list != NULL)
		heap->free_list->prev_free = header;
	heap->free_list = header;
}

static void heap_remove_free(t_heap *heap, t_heap_header *header)
{
	if (header->prev_free != NULL)
		header->prev_free->next_free = header->next_free;
	else
		heap->free_list = header->next_free;
	if (header->next_free != NULL)
		header->next_free->prev_free = header->prev_free;
	header->prev_free = NULL;
	header->next_free = NULL;
}

static t_heap_footer *heap_footer_from_header(t_heap_header *header)
{
	return (t_heap_footer *)((uint8_t *)header + header->size - sizeof(t_heap_footer));
}

static t_heap_header *heap_next_header(t_heap_header *header, t_heap *heap)
{
	uint32_t next_addr = (uint32_t)header + header->size;
	if (next_addr >= heap->end)
		return NULL;
	return (t_heap_header *)next_addr;
}

static t_heap_footer *heap_prev_footer(t_heap_header *header, t_heap *heap)
{
	if ((uint32_t)header <= heap->start + sizeof(t_heap_footer))
		return NULL;
	return (t_heap_footer *)((uint8_t *)header - sizeof(t_heap_footer));
}

static void heap_expand(t_heap *heap, uint32_t size)
{
	uint32_t new_end = heap->end + size;
	uint32_t addr;

	if (new_end > heap->max)
		return;
	new_end = (new_end + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
	for (addr = heap->end; addr < new_end; addr += PAGE_SIZE)
	{
		t_mempage *page = create_page(addr, kernel_directory);
		alloc_frame(page, heap->supervisor ? 1 : 0, heap->readonly ? 0 : 1);
	}

	t_heap_header *header = (t_heap_header *)heap->end;
	header->magic = HEAP_MAGIC;
	header->is_hole = 1;
	header->size = new_end - heap->end;
	t_heap_footer *footer = heap_footer_from_header(header);
	footer->magic = HEAP_MAGIC;
	footer->header = header;
	heap->end = new_end;
	heap_insert_free(heap, header);
}

t_heap *create_heap(uint32_t start, uint32_t end, uint32_t max, uint8_t supervisor, uint8_t readonly)
{
	t_heap *heap;
	t_heap_header *header;
	t_heap_footer *footer;

	start = (start + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
	end = (end + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
	heap = (t_heap *)kmem_early_alloc(sizeof(t_heap), 1);
	if (heap == NULL)
		return NULL;
	heap->start = start;
	heap->end = end;
	heap->max = max;
	heap->supervisor = supervisor;
	heap->readonly = readonly;
	heap->free_list = NULL;

	header = (t_heap_header *)start;
	header->magic = HEAP_MAGIC;
	header->is_hole = 1;
	header->size = end - start;
	header->prev_free = NULL;
	header->next_free = NULL;
	footer = heap_footer_from_header(header);
	footer->magic = HEAP_MAGIC;
	footer->header = header;

	heap_insert_free(heap, header);
	return heap;
}

static t_heap_header *heap_find_hole(t_heap *heap, uint32_t size, uint8_t align)
{
	t_heap_header *header = heap->free_list;

	while (header != NULL)
	{
		uint32_t hole_start = (uint32_t)header;
		uint32_t data_start = hole_start + sizeof(t_heap_header);
		uint32_t aligned = data_start;
		uint32_t aligned_size = header->size;

		if (align)
		{
			aligned = (data_start + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
			uint32_t offset = aligned - data_start;
			if (offset)
			{
				if (offset < HEAP_MIN_HOLE)
				{
					header = header->next_free;
					continue;
				}
				aligned_size = header->size - offset;
			}
		}

		if (aligned_size >= size)
			return header;
		header = header->next_free;
	}
	return NULL;
}

void *heap_alloc(uint32_t size, uint8_t align, t_heap *heap)
{
	t_heap_header *header;
	t_heap_footer *footer;
	uint32_t total_size;

	if (heap == NULL)
		return NULL;
	total_size = size + sizeof(t_heap_header) + sizeof(t_heap_footer);
	header = heap_find_hole(heap, total_size, align);
	if (header == NULL)
	{
		heap_expand(heap, total_size);
		header = heap_find_hole(heap, total_size, align);
		if (header == NULL)
			return NULL;
	}

	heap_remove_free(heap, header);

	uint32_t hole_start = (uint32_t)header;
	uint32_t original_size = header->size;
	uint32_t data_start = hole_start + sizeof(t_heap_header);
	uint32_t aligned = data_start;

	if (align)
	{
		aligned = (data_start + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
		uint32_t offset = aligned - data_start;
		if (offset)
		{
			t_heap_header *lead = header;
			lead->size = offset;
			lead->is_hole = 1;
			footer = heap_footer_from_header(lead);
			footer->magic = HEAP_MAGIC;
			footer->header = lead;
			heap_insert_free(heap, lead);

			header = (t_heap_header *)(hole_start + offset);
			header->magic = HEAP_MAGIC;
			header->is_hole = 1;
			header->size = original_size - offset;
			header->prev_free = NULL;
			header->next_free = NULL;
			hole_start = (uint32_t)header;
		}
	}

	uint32_t remaining = header->size - total_size;
	if (remaining >= HEAP_MIN_HOLE)
	{
		t_heap_header *split = (t_heap_header *)(hole_start + total_size);
		split->magic = HEAP_MAGIC;
		split->is_hole = 1;
		split->size = remaining;
		footer = heap_footer_from_header(split);
		footer->magic = HEAP_MAGIC;
		footer->header = split;
		heap_insert_free(heap, split);
		header->size = total_size;
	}

	header->is_hole = 0;
	footer = heap_footer_from_header(header);
	footer->magic = HEAP_MAGIC;
	footer->header = header;
	return (void *)((uint8_t *)header + sizeof(t_heap_header));
}

void heap_free(void *ptr, t_heap *heap)
{
	if (ptr == NULL || heap == NULL)
		return;
	t_heap_header *header = (t_heap_header *)((uint8_t *)ptr - sizeof(t_heap_header));
	if (header->magic != HEAP_MAGIC)
		return;

	header->is_hole = 1;
	t_heap_footer *footer = heap_footer_from_header(header);
	footer->magic = HEAP_MAGIC;
	footer->header = header;

	t_heap_footer *prev_footer = heap_prev_footer(header, heap);
	if (prev_footer && prev_footer->magic == HEAP_MAGIC && prev_footer->header->is_hole)
	{
		t_heap_header *prev = prev_footer->header;
		heap_remove_free(heap, prev);
		prev->size += header->size;
		footer->header = prev;
		header = prev;
	}

	t_heap_header *next = heap_next_header(header, heap);
	if (next && next->magic == HEAP_MAGIC && next->is_hole)
	{
		heap_remove_free(heap, next);
		header->size += next->size;
		t_heap_footer *new_footer = heap_footer_from_header(header);
		new_footer->magic = HEAP_MAGIC;
		new_footer->header = header;
	}

	heap_insert_free(heap, header);
}

uint32_t heap_block_size(void *ptr)
{
	t_heap_header *header = (t_heap_header *)((uint8_t *)ptr - sizeof(t_heap_header));
	if (header->magic != HEAP_MAGIC)
		return 0;
	return header->size - sizeof(t_heap_header) - sizeof(t_heap_footer);
}
