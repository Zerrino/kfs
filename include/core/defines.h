/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexafer <alexafer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:30:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/10/09 11:01:42 by alexafer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_H
# define DEFINES_H


/* ──────────── VGA Display Constants ──────────── */
# define VGA_WIDTH   80
# define VGA_HEIGHT  25
# define VGA_MEMORY  0xB8000
# define VGA_CRSR_LOW   0x0F
# define VGA_CRSR_HIGH  0x0E
# define VGA_PORT_INDEX 0x3D4
# define VGA_PORT_DATA  0x3D5

/* ──────────── IDT Constants ──────────── */
# define IDT_ENTRIES 256

/* ──────────── Signal Handling Constants ──────────── */
# define SIGNAL_MAX 256
# define SIGNAL_QUEUE_SIZE 32
# define SIGNAL_TIMER_TICK 0
# define SIGNAL_KEYBOARD   1
# define SIGNAL_SYSCALL    2

/* ──────────── PIC 8259 Ports ──────────── */
#define PIC1_CMD    0x20    /* Command register (Master)  */
#define PIC1_DATA   0x21    /* Data register    (Master)  */
#define PIC2_CMD    0xA0    /* Command register (Slave)   */
#define PIC2_DATA   0xA1    /* Data register    (Slave)   */

/* ──────────── ICW1 (Initialization Cmd Word 1) ──────────── */
#define ICW1_ICW4   0x01    /* ICW4 will follow               */
#define ICW1_INIT   0x10    /* Launch initialization sequence */
#define ICW1        (ICW1_INIT | ICW1_ICW4)       /* 0x11 */

/* ──────────── Offsets after remap (ICW2) ──────────── */
#define PIC1_OFFSET 0x20    /* IRQ0-7  → vectors 0x20-27 */
#define PIC2_OFFSET 0x28    /* IRQ8-15 → vectors 0x28-2F */

/* ──────────── Cascade configuration (ICW3) ──────────── */
#define PIC1_CASCADE_IRQ2 0x04   /* Slave is connected to IRQ2 */
#define PIC2_CASCADE_ID   0x02   /* Slave ID in the chain      */

/* ──────────── ICW4 ──────────── */
#define ICW4_8086  0x01     /* 8086/88 mode */

/* ──────────── Masks (OCW1) ──────────── */
#define PIC_MASK_NONE 0x00  /* Unmask all lines */

/* ──────────── EOI Command (End-Of-Interrupt) ──────────── */
#define PIC_EOI    0x20

/* ──────────── Screen and Scroll Constants ──────────── */
# define NB_SCREEN 3
# define NB_SCROLL 3

/* ──────────── GDT Constants ──────────── */
#define GDT_BASE_ADDRESS        0x00000800  /* GDT base address */
#define GDT_MAX_ENTRIES         7           /* Number of GDT entries */

/* ──────────── GDT Segment Selectors ──────────── */
#define GDT_NULL_SEGMENT        0x00        /* Null segment selector */
#define GDT_KERNEL_CODE         0x08        /* Kernel code segment selector */
#define GDT_KERNEL_DATA         0x10        /* Kernel data segment selector */
#define GDT_KERNEL_STACK        0x18        /* Kernel stack segment selector */
#define GDT_USER_CODE           0x20        /* User code segment selector */
#define GDT_USER_DATA           0x28        /* User data segment selector */
#define GDT_USER_STACK          0x30        /* User stack segment selector */

/* ──────────── Legacy GDT Compatibility ──────────── */
#define GDT_CODE_SEGMENT        GDT_KERNEL_CODE  /* Legacy alias */
#define GDT_DATA_SEGMENT        GDT_KERNEL_DATA  /* Legacy alias */

/* ──────────── GDT Segment Information Array ──────────── */
#define GDT_SEGMENT_INFO_INIT { \
    {"Null Descriptor", GDT_NULL_SEGMENT}, \
    {"Kernel Code", GDT_KERNEL_CODE}, \
    {"Kernel Data", GDT_KERNEL_DATA}, \
    {"Kernel Stack", GDT_KERNEL_STACK}, \
    {"User Code", GDT_USER_CODE}, \
    {"User Data", GDT_USER_DATA}, \
    {"User Stack", GDT_USER_STACK} \
}

/* ──────────── Shell Constants ──────────── */
#define COMMAND_BUFFER_SIZE     256

/* ──────────── Stack Constants ──────────── */
#define KERNEL_STACK_SIZE           1024    /* Software stack size (entries) */
#define KERNEL_EXECUTION_STACK_SIZE 16384   /* Hardware execution stack size (bytes) - 16KB */
#define KERNEL_STACK_BASE           0x00090000  /* Base address for kernel execution stack */

/* ──────────── Keyboard Constants ──────────── */
#define KEYBOARD_DATA_PORT      0x60    /* Keyboard data port */
#define KEYBOARD_STATUS_PORT    0x64    /* Keyboard status/command port */
#define KEYBOARD_CMD_RESET      0xFF    /* Reset keyboard command */
#define KEYBOARD_CMD_ENABLE     0xAE    /* Enable keyboard command */
#define KEYBOARD_STATUS_READY   0x02    /* Keyboard ready status bit */
#define KEYBOARD_STATUS_DATA    0x01    /* Data available status bit */
#define KEYBOARD_ACK            0xFA    /* Keyboard acknowledgment */

/* ──────────── Keyboard Scancodes ──────────── */
#define SCANCODE_ESC            27      /* ESC key scancode */
#define SCANCODE_CTRL_PRESS     29      /* Ctrl key press */
#define SCANCODE_CTRL_RELEASE   157     /* Ctrl key release */
#define SCANCODE_LSHIFT_PRESS   42      /* Left Shift key press */
#define SCANCODE_LSHIFT_RELEASE 170     /* Left Shift key release */
#define SCANCODE_RSHIFT_PRESS   54      /* Right Shift key press */
#define SCANCODE_RSHIFT_RELEASE 182     /* Right Shift key release */
#define SCANCODE_LEFT_ARROW     75      /* Left arrow key */
#define SCANCODE_RIGHT_ARROW    77      /* Right arrow key */
#define SCANCODE_UP_ARROW       72      /* Up arrow key */
#define SCANCODE_DOWN_ARROW     80      /* Down arrow key */

/* ──────────── System Control Constants ──────────── */
#define REBOOT_PORT             0x64    /* Keyboard controller port for reboot */
#define REBOOT_CMD              0xFE    /* Reboot command */
#define ACPI_SHUTDOWN_PORT      0xB004  /* ACPI shutdown port */
#define APM_SHUTDOWN_PORT       0x604   /* APM shutdown port */
#define SHUTDOWN_CMD            0x2000  /* Shutdown command */

/* ──────────── CPU Flags ──────────── */
#define EFLAGS_INTERRUPT_FLAG   0x200   /* Interrupt flag in EFLAGS register */

/* ──────────── Segment Privilege Levels ──────────── */
#define RPL_MASK                0x3     /* Requested Privilege Level mask */

/* ──────────── Test Values ──────────── */
#define TEST_VALUE_DEAD         0xDEAD  /* Test value for stack operations */
#define TEST_VALUE_BEEF         0xBEEF  /* Test value for stack operations */
#define TEST_VALUE_CAFE         0xCAFE  /* Test value for stack operations */

/* ──────────── Bit Manipulation ──────────── */
#define NIBBLE_MASK             0xF     /* Mask for extracting nibble (4 bits) */
#define BITS_PER_NIBBLE         4       /* Number of bits in a nibble */
#define BYTE_MASK               0xFF    /* Mask for extracting byte (8 bits) */
#define BITS_PER_BYTE           8       /* Number of bits in a byte */

/* ──────────── Exception Message Strings ──────────── */
#define EXCEPTION_MSG_DIVIDE_BY_ZERO        "Divide by zero error"
#define EXCEPTION_MSG_DEBUG                 "Debug"
#define EXCEPTION_MSG_NMI                   "Non-maskable Interrupt"
#define EXCEPTION_MSG_BREAKPOINT            "Breakpoint"
#define EXCEPTION_MSG_OVERFLOW              "Overflow"
#define EXCEPTION_MSG_BOUND_RANGE           "Bound Range Exceeded"
#define EXCEPTION_MSG_INVALID_OPCODE        "Invalid Opcode"
#define EXCEPTION_MSG_DEVICE_NOT_AVAILABLE  "Device Not Available"
#define EXCEPTION_MSG_DOUBLE_FAULT          "Double Fault"
#define EXCEPTION_MSG_COPROCESSOR_OVERRUN   "Coprocessor Segment Overrun"
#define EXCEPTION_MSG_INVALID_TSS           "Invalid TSS"
#define EXCEPTION_MSG_SEGMENT_NOT_PRESENT   "Segment Not Present"
#define EXCEPTION_MSG_STACK_SEGMENT_FAULT   "Stack-Segment Fault"
#define EXCEPTION_MSG_GENERAL_PROTECTION    "General Protection Fault"
#define EXCEPTION_MSG_PAGE_FAULT            "Page Fault"
#define EXCEPTION_MSG_RESERVED              ""
#define EXCEPTION_MSG_X87_FPU               "x87 Floating-Point Exception"
#define EXCEPTION_MSG_ALIGNMENT_CHECK       "Alignment Check"
#define EXCEPTION_MSG_MACHINE_CHECK         "Machine Check"
#define EXCEPTION_MSG_SIMD_FPU              "SIMD Floating-Point Exception"
#define EXCEPTION_MSG_VIRTUALIZATION        "Virtualization Exception"
#define EXCEPTION_MSG_CONTROL_PROTECTION    "Control Protection Exception "
#define EXCEPTION_MSG_HYPERVISOR_INJECTION  "Hypervisor Injection Exception"
#define EXCEPTION_MSG_VMM_COMMUNICATION     "VMM Communication Exception"
#define EXCEPTION_MSG_SECURITY              "Security Exception"

/* ──────────── Utility Macros ──────────── */
# define N(x) (x >> ((sizeof(x) * 8) - 1))
# define ABS(x) ((x + (1 * N(x))) ^ N(x))
# define FLAG_SET(x, flag) x |= (flag)
# define FLAG_UNSET(x, flag) x &= ~(flag)

#endif
