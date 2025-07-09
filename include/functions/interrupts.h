/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interrupts.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:30:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/09 16:39:43 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERRUPTS_H
# define INTERRUPTS_H

#include "../core/enums.h"
#include "../core/libs.h"
#include "../core/structs.h"

/* ──────────── IDT Function Prototypes ──────────── */

void IDT_SetGate(int interrupt, void *base, uint16_t segmentDescriptor, uint8_t flags);
void IDT_Initialize(void);
void IDT_EnableGate(int interrupt);
void IDT_DisableGate(int interrupt);

/* ──────────── ISR Function Prototypes ──────────── */

void ISR_Initialize(void);
void ISR_InitializeGates(void);

void ISR_RegisterHandler(int interrupt, ISRHandler handler);

/* ──────────── IRQ Function Prototypes ──────────── */

void IRQ_Initialize(void);
void IRQ_RegisterHandler(int irq, IRQHandler handler);

/* ──────────── PIC Function Prototypes ──────────── */

void PIC_Configure(uint8_t offsetPic1, uint8_t offsetPic2);
void PIC_SendEOF(int irq);
void PIC_Mask(int irq);
void PIC_Unmask(int irq);
void PIC_Disable(void);
uint16_t PIC_ReadIRQRequestReg(void);
uint16_t PIC_ReadInServiceReg(void);

/* ──────────── I/O Function Prototypes ──────────── */

void iowait(void);

/* ──────────── Handler Functions ──────────── */

void timer(t_registers* regs);

#endif