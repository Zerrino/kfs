/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 11:54:32 by alexafer          #+#    #+#             */
/*   Updated: 2025/07/10 19:44:19 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kfs.h"

void kfs2_init()
{
	DisableInterrupts();
	gdt_install();
	IDT_Initialize();
	ISR_Initialize();
	IRQ_Initialize();
	stack_push(0xCAFEBABE);
	stack_push(0x12345678);
	keyboard_init();
	EnableInterrupts();
}

void kfs3_init()
{
	DisableInterrupts(); /* Disable interrupts during memory initialization */
	phys_mem_init(16 * 1024 * 1024);  /* Initialize with 16MB of memory */
	paging_init(); /* Initialize paging system */
	kmem_init(); /* Initialize kernel memory allocator */
	panic_init(); /* Initialize panic system */
	paging_enable(); /* Enable paging */
	EnableInterrupts(); /* Re-enable interrupts */
}

void kernel_initialize(void)
{
	kernel_globals_init();  /* Initialize kernel structure first */
	kfs2_init();
	kfs3_init();
}
