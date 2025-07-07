; GDT Segment Selector Definitions
; These must match the definitions in include/idt.h

%define GDT_NULL_SEGMENT    0x00
%define GDT_KERNEL_CODE     0x08
%define GDT_KERNEL_DATA     0x10
%define GDT_KERNEL_STACK    0x18
%define GDT_USER_CODE       0x20
%define GDT_USER_DATA       0x28
%define GDT_USER_STACK      0x30

; Legacy compatibility
%define GDT_CODE_SEGMENT GDT_KERNEL_CODE
%define GDT_DATA_SEGMENT GDT_KERNEL_DATA
