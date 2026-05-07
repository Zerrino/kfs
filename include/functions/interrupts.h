/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interrupts.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:30:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/09 21:37:58 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERRUPTS_H
# define INTERRUPTS_H

#include "../core/enums.h"
#include "../core/libs.h"
#include "../core/structs.h"

/* ──────────── Functions from src/interupts/handlers.c ──────────── */

void timer(t_registers* regs);

/* ──────────── Functions from src/interupts/isr.c ──────────── */

void ISR_Initialize(void);
void ISR_InitializeGates(void);
void ISR_RegisterHandler(int interrupt, ISRHandler handler);

/* ──────────── Functions from src/interupts/irq.c ──────────── */

void IRQ_Initialize(void);
void IRQ_RegisterHandler(int irq, IRQHandler handler);

/* ──────────── Functions from src/interupts/idt.c ──────────── */

void IDT_SetGate(int interrupt, void *base, uint16_t segmentDescriptor, uint8_t flags);
void IDT_Initialize(void);
void IDT_EnableGate(int interrupt);
void IDT_DisableGate(int interrupt);

/* ──────────── Functions from src/interupts/pic.c ──────────── */

void PIC_Configure(uint8_t offsetPic1, uint8_t offsetPic2);
void PIC_SendEOF(int irq);
void PIC_Mask(int irq);
void PIC_Unmask(int irq);
void PIC_Disable(void);
uint16_t PIC_ReadIRQRequestReg(void);
uint16_t PIC_ReadInServiceReg(void);

/* ──────────── Functions from src/interupts/io.c ──────────── */

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t value);
void outw(uint16_t port, uint16_t value);
void iowait(void);

#endif