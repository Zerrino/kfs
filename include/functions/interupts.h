/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interupts.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:12:05 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/04 15:29:28 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERUPTS_FUNCTIONS_H
# define INTERUPTS_FUNCTIONS_H

#include "../core/structs.h"

/* ──────────── Assembly IDT Functions ──────────── */
void		__attribute__((cdecl)) IDT_Load(t_idt_descryptor* idtDescriptor);
void		__attribute__((cdecl)) EnableInterrupts();
void		__attribute__((cdecl)) DisableInterrupts();
void		__attribute__((cdecl)) i686_outb(uint16_t port, uint8_t value);
uint8_t		__attribute__((cdecl)) i686_inb(uint16_t port);

/* ──────────── IDT Management Functions ──────────── */
void		IDT_SetGate(int interrupt, void *base, uint16_t segmentDescriptor, uint8_t flags);
void		IDT_Initialize();
void		IDT_EnableGate(int interrupt);
void		IDT_DisableGate(int interrupt);

/* ──────────── ISR (Interrupt Service Routine) Functions ──────────── */
void		ISR_Initialize();
void		ISR_InitializeGates();
void		ISR_RegisterHandler(int interrupt, ISRHandler handler);

/* ──────────── PIC (Programmable Interrupt Controller) Functions ──────────── */
void		iowait();
void		PIC_Configure(uint8_t offsetPic1, uint8_t offsetPic2);
void		PIC_SendEOF(int irq);
void		PIC_Mask(int irq);
void		PIC_Unmask(int irq);
void		PIC_Disable();
uint16_t	PIC_ReadIRQRequestReg();
uint16_t	PIC_ReadInServiceReg();

/* ──────────── IRQ (Interrupt Request) Functions ──────────── */
void		IRQ_Initialize();
void		IRQ_Handler(t_registers* regs);
void		IRQ_RegisterHandler(int irq, IRQHandler handler);

/* ──────────── Interrupt Handlers ──────────── */
void		timer(t_registers* regs);

#endif /* INTERUPTS_FUNCTIONS_H */
