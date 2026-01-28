/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frames.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 16:40:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/12/24 16:40:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

static uint32_t *frames_bitmap = NULL;
static uint32_t nframes = 0;

static void set_frame(uint32_t frame_idx)
{
	frames_bitmap[BITMAP_INDEX(frame_idx)] |= (0x1 << BITMAP_OFFSET(frame_idx));
}

static void clear_frame(uint32_t frame_idx)
{
	frames_bitmap[BITMAP_INDEX(frame_idx)] &= ~(0x1 << BITMAP_OFFSET(frame_idx));
}

static int test_frame(uint32_t frame_idx)
{
	return (frames_bitmap[BITMAP_INDEX(frame_idx)] & (0x1 << BITMAP_OFFSET(frame_idx))) != 0;
}

static uint32_t first_free_frame(void)
{
	for (uint32_t i = 0; i < BITMAP_INDEX(nframes) + 1; i++)
	{
		if (frames_bitmap[i] != 0xFFFFFFFF)
		{
			for (uint32_t j = 0; j < 32; j++)
			{
				uint32_t idx = (i * 32) + j;
				if (idx >= nframes)
					return (uint32_t)-1;
				if (!test_frame(idx))
					return idx;
			}
		}
	}
	return (uint32_t)-1;
}

void init_frames(void)
{
	uint32_t bitmap_size;

	if (g_memory_limit == 0)
		g_memory_limit = 0x1000000;
	nframes = g_memory_limit / PAGE_SIZE;
	bitmap_size = (BITMAP_INDEX(nframes) + 1) * sizeof(uint32_t);
	frames_bitmap = (uint32_t *)kmem_early_alloc(bitmap_size, 1);
	ft_memset(frames_bitmap, 0, bitmap_size);
}

void alloc_frame(t_mempage *page, int is_kernel, int is_writeable)
{
	uint32_t idx;

	if (page->frame != 0)
		return;
	idx = first_free_frame();
	if (idx == (uint32_t)-1)
	{
		KPANIC("No free frames");
		return;
	}
	set_frame(idx);
	page->present = 1;
	page->rw = is_writeable ? 1 : 0;
	page->user = is_kernel ? 0 : 1;
	page->frame = idx;
}

void alloc_frame_at(t_mempage *page, uint32_t phys_addr, int is_kernel, int is_writeable)
{
	uint32_t idx = phys_addr / PAGE_SIZE;

	if (page->frame != 0)
		return;
	if (idx >= nframes)
	{
		KPANIC("Frame out of range");
		return;
	}
	set_frame(idx);
	page->present = 1;
	page->rw = is_writeable ? 1 : 0;
	page->user = is_kernel ? 0 : 1;
	page->frame = idx;
}

void free_frame(t_mempage *page)
{
	if (page->frame == 0)
		return;
	clear_frame(page->frame);
	page->frame = 0;
	page->present = 0;
}
