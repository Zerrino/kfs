/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   idt.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zerrino <zerrino@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 22:23:46 by zerrino           #+#    #+#             */
/*   Updated: 2025/06/25 21:46:12 by zerrino          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IDT_H
# define IDT_H

# define GDT_CODE_SEGMENT 0x08
# define GDT_DATA_SEGMENT 0x10

# define FLAG_SET(x, flag) x |= (flag)
# define FLAG_UNSET(x, flag) x &= ~(flag)
 // https://wiki.osdev.org/Interrupt_Descriptor_Table
 typedef struct s_idt_descriptor
 {
	uint16_t	size;	// Size: One less than the size of the IDT in bytes.
	uint32_t	offset;	// Offset: The linear address of the Interrupt Descriptor Table (not the physical address, paging applies).
 }	__attribute__((packed)) t_idt_descryptor;

typedef struct s_idt_entry
{
	uint16_t offset_1;        // offset bits 0..15
	uint16_t selector;        // a code segment selector in GDT or LDT
	uint8_t  zero;            // unused, set to 0
	uint8_t  type_attributes; // gate type, dpl, and p fields
	uint16_t offset_2;        // offset bits 16..31
}	__attribute__((packed)) t_idt_entry;

typedef struct s_registers
{
	uint32_t	ds;
	uint32_t	edi, esi, ebp, kern_esp, ebx, edx, ecx, eax;
	uint32_t	interrupt, error;
	uint32_t	eip, cs, eflags, esp, ss;
}	__attribute__((packed)) t_registers;


typedef enum {
	IDT_FLAG_GATE_TASK			= 0x5,
	IDT_FLAG_GATE_16BIT_INT		= 0x6,
	IDT_FLAG_GATE_16BIT_TRAP	= 0x7,
	IDT_FLAG_GATE_32BIT_INT		= 0xE,
	IDT_FLAG_GATE_32BIT_TRAP	= 0xF,

	IDT_FLAG_RIN0				= (0 << 5),
	IDT_FLAG_RIN1				= (1 << 5),
	IDT_FLAG_RIN2				= (2 << 5),
	IDT_FLAG_RIN3				= (3 << 5),

	IDT_FLAG_PRESENT			= 0x80
}	IDT_FLAGS;


typedef enum {
	PIC_CMD_EOF		= 0x20,
	PIC_CMD_READ_IRR		= 0x0A,
	PIC_CMD_READ_ISR		= 0x0B,

}	PIC_CMD;

typedef enum {
	PIC_ICW1_ICW4		= 0x01,
	PIC_ICW1_SINGLE		= 0x02,
	PIC_ICW1_INTERVAL4	= 0x04,
	PIC_ICW1_LEVEL		= 0x08,
	PIC_ICW1_INIT		= 0x10,


}	PIC_ICW1;



typedef enum {
	PIC_ICW4_8086			= 0x01,
	PIC_ICW4_AUTO_EOI		= 0x02,
	PIC_ICW4_BUFFER_MASTER	= 0x04,
	PIC_ICW4_BUFFER_SLAVE	= 0x00,
	PIC_ICW4_BUFFERED		= 0x08,
	PIC_ICW4_SFNM			= 0x10,
}	PIC_ICW4;


typedef void (*ISRHandler)(t_registers *regs);
typedef void (*IRQHandler)(t_registers *regs);


void		__attribute__((cdecl)) IDT_Load(t_idt_descryptor* idtDescriptor);
void		__attribute__((cdecl)) EnableInterrupts();
void		__attribute__((cdecl)) DisableInterrupts();
void		__attribute__((cdecl)) i686_outb(uint16_t port, uint8_t value);
uint8_t		__attribute__((cdecl)) i686_inb(uint16_t port);

void		IDT_SetGate(int interrupt, void *base, uint16_t segmentDescriptor, uint8_t flags);
void		IDT_Initialize();
void		IDT_EnableGate(int interrupt);
void		IDT_DisableGate(int interrupt);

void		ISR_Initialize();
void		ISR_RegisterHandler(int interrupt, ISRHandler handler);
void		iowait();
void		PIC_Configure(uint8_t offsetPic1, uint8_t offsetPic2);
void		PIC_SendEOF(int irq);
void		PIC_Mask(int irq);
void		PIC_Unmask(int irq);
void		PIC_Disable();
uint16_t	PIC_ReadIRQRequestReg();
uint16_t	PIC_ReadInServiceReg();


void		IRQ_Initialize();
void		IRQ_RegisterHandler(int irq, IRQHandler handler);

/* handlers */
void	timer(t_registers* regs);


#endif
