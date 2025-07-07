/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isr.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zerrino <zerrino@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 02:10:22 by zerrino           #+#    #+#             */
/*   Updated: 2025/06/27 01:49:21 by zerrino          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

//ISRHandler g_ISRHandlers[256];


static const char* const g_Exceptions[] = {
	"Divide by zero error",
	"Debug",
	"Non-maskable Interrupt",
	"Breakpoint",
	"Overflow",
	"Bound Range Exceeded",
	"Invalid Opcode",
	"Device Not Available",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Invalid TSS",
	"Segment Not Present",
	"Stack-Segment Fault",
	"General Protection Fault",
	"Page Fault",
	"",
	"x87 Floating-Point Exception",
	"Alignment Check",
	"Machine Check",
	"SIMD Floating-Point Exception",
	"Virtualization Exception",
	"Control Protection Exception ",
	"",
	"",
	"",
	"",
	"",
	"",
	"Hypervisor Injection Exception",
	"VMM Communication Exception",
	"Security Exception",
	""
};

void	__attribute__((cdecl)) kernelPanic();

void	__attribute__((cdecl)) ISR0();
void	__attribute__((cdecl)) ISR1();
void	__attribute__((cdecl)) ISR2();
void	__attribute__((cdecl)) ISR3();
void	__attribute__((cdecl)) ISR4();
void	__attribute__((cdecl)) ISR5();
void	__attribute__((cdecl)) ISR6();
void	__attribute__((cdecl)) ISR7();
void	__attribute__((cdecl)) ISR8();
void	__attribute__((cdecl)) ISR9();
void	__attribute__((cdecl)) ISR10();
void	__attribute__((cdecl)) ISR11();
void	__attribute__((cdecl)) ISR12();
void	__attribute__((cdecl)) ISR13();
void	__attribute__((cdecl)) ISR14();
void	__attribute__((cdecl)) ISR15();
void	__attribute__((cdecl)) ISR16();
void	__attribute__((cdecl)) ISR17();
void	__attribute__((cdecl)) ISR18();
void	__attribute__((cdecl)) ISR19();
void	__attribute__((cdecl)) ISR20();
void	__attribute__((cdecl)) ISR21();
void	__attribute__((cdecl)) ISR22();
void	__attribute__((cdecl)) ISR23();
void	__attribute__((cdecl)) ISR24();
void	__attribute__((cdecl)) ISR25();
void	__attribute__((cdecl)) ISR26();
void	__attribute__((cdecl)) ISR27();
void	__attribute__((cdecl)) ISR28();
void	__attribute__((cdecl)) ISR29();
void	__attribute__((cdecl)) ISR30();
void	__attribute__((cdecl)) ISR31();
void	__attribute__((cdecl)) ISR32();
void	__attribute__((cdecl)) ISR33();
void	__attribute__((cdecl)) ISR34();
void	__attribute__((cdecl)) ISR35();
void	__attribute__((cdecl)) ISR36();
void	__attribute__((cdecl)) ISR37();
void	__attribute__((cdecl)) ISR38();
void	__attribute__((cdecl)) ISR39();
void	__attribute__((cdecl)) ISR40();
void	__attribute__((cdecl)) ISR41();
void	__attribute__((cdecl)) ISR42();
void	__attribute__((cdecl)) ISR43();
void	__attribute__((cdecl)) ISR44();
void	__attribute__((cdecl)) ISR45();
void	__attribute__((cdecl)) ISR46();
void	__attribute__((cdecl)) ISR47();
void	__attribute__((cdecl)) ISR48();
void	__attribute__((cdecl)) ISR49();
void	__attribute__((cdecl)) ISR50();
void	__attribute__((cdecl)) ISR51();
void	__attribute__((cdecl)) ISR52();
void	__attribute__((cdecl)) ISR53();
void	__attribute__((cdecl)) ISR54();
void	__attribute__((cdecl)) ISR55();
void	__attribute__((cdecl)) ISR56();
void	__attribute__((cdecl)) ISR57();
void	__attribute__((cdecl)) ISR58();
void	__attribute__((cdecl)) ISR59();
void	__attribute__((cdecl)) ISR60();
void	__attribute__((cdecl)) ISR61();
void	__attribute__((cdecl)) ISR62();
void	__attribute__((cdecl)) ISR63();
void	__attribute__((cdecl)) ISR64();
void	__attribute__((cdecl)) ISR65();
void	__attribute__((cdecl)) ISR66();
void	__attribute__((cdecl)) ISR67();
void	__attribute__((cdecl)) ISR68();
void	__attribute__((cdecl)) ISR69();
void	__attribute__((cdecl)) ISR70();
void	__attribute__((cdecl)) ISR71();
void	__attribute__((cdecl)) ISR72();
void	__attribute__((cdecl)) ISR73();
void	__attribute__((cdecl)) ISR74();
void	__attribute__((cdecl)) ISR75();
void	__attribute__((cdecl)) ISR76();
void	__attribute__((cdecl)) ISR77();
void	__attribute__((cdecl)) ISR78();
void	__attribute__((cdecl)) ISR79();
void	__attribute__((cdecl)) ISR80();
void	__attribute__((cdecl)) ISR81();
void	__attribute__((cdecl)) ISR82();
void	__attribute__((cdecl)) ISR83();
void	__attribute__((cdecl)) ISR84();
void	__attribute__((cdecl)) ISR85();
void	__attribute__((cdecl)) ISR86();
void	__attribute__((cdecl)) ISR87();
void	__attribute__((cdecl)) ISR88();
void	__attribute__((cdecl)) ISR89();
void	__attribute__((cdecl)) ISR90();
void	__attribute__((cdecl)) ISR91();
void	__attribute__((cdecl)) ISR92();
void	__attribute__((cdecl)) ISR93();
void	__attribute__((cdecl)) ISR94();
void	__attribute__((cdecl)) ISR95();
void	__attribute__((cdecl)) ISR96();
void	__attribute__((cdecl)) ISR97();
void	__attribute__((cdecl)) ISR98();
void	__attribute__((cdecl)) ISR99();
void	__attribute__((cdecl)) ISR100();
void	__attribute__((cdecl)) ISR101();
void	__attribute__((cdecl)) ISR102();
void	__attribute__((cdecl)) ISR103();
void	__attribute__((cdecl)) ISR104();
void	__attribute__((cdecl)) ISR105();
void	__attribute__((cdecl)) ISR106();
void	__attribute__((cdecl)) ISR107();
void	__attribute__((cdecl)) ISR108();
void	__attribute__((cdecl)) ISR109();
void	__attribute__((cdecl)) ISR110();
void	__attribute__((cdecl)) ISR111();
void	__attribute__((cdecl)) ISR112();
void	__attribute__((cdecl)) ISR113();
void	__attribute__((cdecl)) ISR114();
void	__attribute__((cdecl)) ISR115();
void	__attribute__((cdecl)) ISR116();
void	__attribute__((cdecl)) ISR117();
void	__attribute__((cdecl)) ISR118();
void	__attribute__((cdecl)) ISR119();
void	__attribute__((cdecl)) ISR120();
void	__attribute__((cdecl)) ISR121();
void	__attribute__((cdecl)) ISR122();
void	__attribute__((cdecl)) ISR123();
void	__attribute__((cdecl)) ISR124();
void	__attribute__((cdecl)) ISR125();
void	__attribute__((cdecl)) ISR126();
void	__attribute__((cdecl)) ISR127();
void	__attribute__((cdecl)) ISR128();
void	__attribute__((cdecl)) ISR129();
void	__attribute__((cdecl)) ISR130();
void	__attribute__((cdecl)) ISR131();
void	__attribute__((cdecl)) ISR132();
void	__attribute__((cdecl)) ISR133();
void	__attribute__((cdecl)) ISR134();
void	__attribute__((cdecl)) ISR135();
void	__attribute__((cdecl)) ISR136();
void	__attribute__((cdecl)) ISR137();
void	__attribute__((cdecl)) ISR138();
void	__attribute__((cdecl)) ISR139();
void	__attribute__((cdecl)) ISR140();
void	__attribute__((cdecl)) ISR141();
void	__attribute__((cdecl)) ISR142();
void	__attribute__((cdecl)) ISR143();
void	__attribute__((cdecl)) ISR144();
void	__attribute__((cdecl)) ISR145();
void	__attribute__((cdecl)) ISR146();
void	__attribute__((cdecl)) ISR147();
void	__attribute__((cdecl)) ISR148();
void	__attribute__((cdecl)) ISR149();
void	__attribute__((cdecl)) ISR150();
void	__attribute__((cdecl)) ISR151();
void	__attribute__((cdecl)) ISR152();
void	__attribute__((cdecl)) ISR153();
void	__attribute__((cdecl)) ISR154();
void	__attribute__((cdecl)) ISR155();
void	__attribute__((cdecl)) ISR156();
void	__attribute__((cdecl)) ISR157();
void	__attribute__((cdecl)) ISR158();
void	__attribute__((cdecl)) ISR159();
void	__attribute__((cdecl)) ISR160();
void	__attribute__((cdecl)) ISR161();
void	__attribute__((cdecl)) ISR162();
void	__attribute__((cdecl)) ISR163();
void	__attribute__((cdecl)) ISR164();
void	__attribute__((cdecl)) ISR165();
void	__attribute__((cdecl)) ISR166();
void	__attribute__((cdecl)) ISR167();
void	__attribute__((cdecl)) ISR168();
void	__attribute__((cdecl)) ISR169();
void	__attribute__((cdecl)) ISR170();
void	__attribute__((cdecl)) ISR171();
void	__attribute__((cdecl)) ISR172();
void	__attribute__((cdecl)) ISR173();
void	__attribute__((cdecl)) ISR174();
void	__attribute__((cdecl)) ISR175();
void	__attribute__((cdecl)) ISR176();
void	__attribute__((cdecl)) ISR177();
void	__attribute__((cdecl)) ISR178();
void	__attribute__((cdecl)) ISR179();
void	__attribute__((cdecl)) ISR180();
void	__attribute__((cdecl)) ISR181();
void	__attribute__((cdecl)) ISR182();
void	__attribute__((cdecl)) ISR183();
void	__attribute__((cdecl)) ISR184();
void	__attribute__((cdecl)) ISR185();
void	__attribute__((cdecl)) ISR186();
void	__attribute__((cdecl)) ISR187();
void	__attribute__((cdecl)) ISR188();
void	__attribute__((cdecl)) ISR189();
void	__attribute__((cdecl)) ISR190();
void	__attribute__((cdecl)) ISR191();
void	__attribute__((cdecl)) ISR192();
void	__attribute__((cdecl)) ISR193();
void	__attribute__((cdecl)) ISR194();
void	__attribute__((cdecl)) ISR195();
void	__attribute__((cdecl)) ISR196();
void	__attribute__((cdecl)) ISR197();
void	__attribute__((cdecl)) ISR198();
void	__attribute__((cdecl)) ISR199();
void	__attribute__((cdecl)) ISR200();
void	__attribute__((cdecl)) ISR201();
void	__attribute__((cdecl)) ISR202();
void	__attribute__((cdecl)) ISR203();
void	__attribute__((cdecl)) ISR204();
void	__attribute__((cdecl)) ISR205();
void	__attribute__((cdecl)) ISR206();
void	__attribute__((cdecl)) ISR207();
void	__attribute__((cdecl)) ISR208();
void	__attribute__((cdecl)) ISR209();
void	__attribute__((cdecl)) ISR210();
void	__attribute__((cdecl)) ISR211();
void	__attribute__((cdecl)) ISR212();
void	__attribute__((cdecl)) ISR213();
void	__attribute__((cdecl)) ISR214();
void	__attribute__((cdecl)) ISR215();
void	__attribute__((cdecl)) ISR216();
void	__attribute__((cdecl)) ISR217();
void	__attribute__((cdecl)) ISR218();
void	__attribute__((cdecl)) ISR219();
void	__attribute__((cdecl)) ISR220();
void	__attribute__((cdecl)) ISR221();
void	__attribute__((cdecl)) ISR222();
void	__attribute__((cdecl)) ISR223();
void	__attribute__((cdecl)) ISR224();
void	__attribute__((cdecl)) ISR225();
void	__attribute__((cdecl)) ISR226();
void	__attribute__((cdecl)) ISR227();
void	__attribute__((cdecl)) ISR228();
void	__attribute__((cdecl)) ISR229();
void	__attribute__((cdecl)) ISR230();
void	__attribute__((cdecl)) ISR231();
void	__attribute__((cdecl)) ISR232();
void	__attribute__((cdecl)) ISR233();
void	__attribute__((cdecl)) ISR234();
void	__attribute__((cdecl)) ISR235();
void	__attribute__((cdecl)) ISR236();
void	__attribute__((cdecl)) ISR237();
void	__attribute__((cdecl)) ISR238();
void	__attribute__((cdecl)) ISR239();
void	__attribute__((cdecl)) ISR240();
void	__attribute__((cdecl)) ISR241();
void	__attribute__((cdecl)) ISR242();
void	__attribute__((cdecl)) ISR243();
void	__attribute__((cdecl)) ISR244();
void	__attribute__((cdecl)) ISR245();
void	__attribute__((cdecl)) ISR246();
void	__attribute__((cdecl)) ISR247();
void	__attribute__((cdecl)) ISR248();
void	__attribute__((cdecl)) ISR249();
void	__attribute__((cdecl)) ISR250();
void	__attribute__((cdecl)) ISR251();
void	__attribute__((cdecl)) ISR252();
void	__attribute__((cdecl)) ISR253();
void	__attribute__((cdecl)) ISR254();
void	__attribute__((cdecl)) ISR255();



void	ISR_InitializeGates()
{
	IDT_SetGate(0, ISR0, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(1, ISR1, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(2, ISR2, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(3, ISR3, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(4, ISR4, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(5, ISR5, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(6, ISR6, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(7, ISR7, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(8, ISR8, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(9, ISR9, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(10, ISR10, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(11, ISR11, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(12, ISR12, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(13, ISR13, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(14, ISR14, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(15, ISR15, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(16, ISR16, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(17, ISR17, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(18, ISR18, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(19, ISR19, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(20, ISR20, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(21, ISR21, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(22, ISR22, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(23, ISR23, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(24, ISR24, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(25, ISR25, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(26, ISR26, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(27, ISR27, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(28, ISR28, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(29, ISR29, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(30, ISR30, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(31, ISR31, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(32, ISR32, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(33, ISR33, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(34, ISR34, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(35, ISR35, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(36, ISR36, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(37, ISR37, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(38, ISR38, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(39, ISR39, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(40, ISR40, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(41, ISR41, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(42, ISR42, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(43, ISR43, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(44, ISR44, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(45, ISR45, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(46, ISR46, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(47, ISR47, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(48, ISR48, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(49, ISR49, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(50, ISR50, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(51, ISR51, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(52, ISR52, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(53, ISR53, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(54, ISR54, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(55, ISR55, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(56, ISR56, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(57, ISR57, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(58, ISR58, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(59, ISR59, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(60, ISR60, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(61, ISR61, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(62, ISR62, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(63, ISR63, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(64, ISR64, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(65, ISR65, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(66, ISR66, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(67, ISR67, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(68, ISR68, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(69, ISR69, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(70, ISR70, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(71, ISR71, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(72, ISR72, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(73, ISR73, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(74, ISR74, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(75, ISR75, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(76, ISR76, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(77, ISR77, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(78, ISR78, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(79, ISR79, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(80, ISR80, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(81, ISR81, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(82, ISR82, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(83, ISR83, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(84, ISR84, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(85, ISR85, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(86, ISR86, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(87, ISR87, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(88, ISR88, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(89, ISR89, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(90, ISR90, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(91, ISR91, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(92, ISR92, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(93, ISR93, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(94, ISR94, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(95, ISR95, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(96, ISR96, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(97, ISR97, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(98, ISR98, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(99, ISR99, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(100, ISR100, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(101, ISR101, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(102, ISR102, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(103, ISR103, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(104, ISR104, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(105, ISR105, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(106, ISR106, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(107, ISR107, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(108, ISR108, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(109, ISR109, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(110, ISR110, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(111, ISR111, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(112, ISR112, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(113, ISR113, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(114, ISR114, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(115, ISR115, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(116, ISR116, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(117, ISR117, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(118, ISR118, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(119, ISR119, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(120, ISR120, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(121, ISR121, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(122, ISR122, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(123, ISR123, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(124, ISR124, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(125, ISR125, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(126, ISR126, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(127, ISR127, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(128, ISR128, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(129, ISR129, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(130, ISR130, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(131, ISR131, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(132, ISR132, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(133, ISR133, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(134, ISR134, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(135, ISR135, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(136, ISR136, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(137, ISR137, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(138, ISR138, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(139, ISR139, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(140, ISR140, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(141, ISR141, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(142, ISR142, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(143, ISR143, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(144, ISR144, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(145, ISR145, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(146, ISR146, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(147, ISR147, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(148, ISR148, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(149, ISR149, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(150, ISR150, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(151, ISR151, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(152, ISR152, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(153, ISR153, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(154, ISR154, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(155, ISR155, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(156, ISR156, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(157, ISR157, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(158, ISR158, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(159, ISR159, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(160, ISR160, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(161, ISR161, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(162, ISR162, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(163, ISR163, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(164, ISR164, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(165, ISR165, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(166, ISR166, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(167, ISR167, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(168, ISR168, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(169, ISR169, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(170, ISR170, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(171, ISR171, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(172, ISR172, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(173, ISR173, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(174, ISR174, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(175, ISR175, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(176, ISR176, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(177, ISR177, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(178, ISR178, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(179, ISR179, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(180, ISR180, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(181, ISR181, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(182, ISR182, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(183, ISR183, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(184, ISR184, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(185, ISR185, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(186, ISR186, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(187, ISR187, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(188, ISR188, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(189, ISR189, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(190, ISR190, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(191, ISR191, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(192, ISR192, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(193, ISR193, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(194, ISR194, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(195, ISR195, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(196, ISR196, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(197, ISR197, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(198, ISR198, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(199, ISR199, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(200, ISR200, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(201, ISR201, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(202, ISR202, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(203, ISR203, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(204, ISR204, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(205, ISR205, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(206, ISR206, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(207, ISR207, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(208, ISR208, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(209, ISR209, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(210, ISR210, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(211, ISR211, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(212, ISR212, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(213, ISR213, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(214, ISR214, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(215, ISR215, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(216, ISR216, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(217, ISR217, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(218, ISR218, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(219, ISR219, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(220, ISR220, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(221, ISR221, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(222, ISR222, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(223, ISR223, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(224, ISR224, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(225, ISR225, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(226, ISR226, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(227, ISR227, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(228, ISR228, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(229, ISR229, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(230, ISR230, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(231, ISR231, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(232, ISR232, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(233, ISR233, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(234, ISR234, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(235, ISR235, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(236, ISR236, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(237, ISR237, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(238, ISR238, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(239, ISR239, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(240, ISR240, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(241, ISR241, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(242, ISR242, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(243, ISR243, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(244, ISR244, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(245, ISR245, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(246, ISR246, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(247, ISR247, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(248, ISR248, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(249, ISR249, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(250, ISR250, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(251, ISR251, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(252, ISR252, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(253, ISR253, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(254, ISR254, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(255, ISR255, GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
}

void	ISR_Initialize()
{
	int	i;
	//terminal_writestring("Setting up ISR gates...\n");
	ISR_InitializeGates();
	//terminal_writestring("ISR gates set up!\n");

	//terminal_writestring("Enabling ISR gates...\n");
	i = 0;
	while (i < 256)
	{
		IDT_EnableGate(i);
		i++;
	}
	//terminal_writestring("All ISR gates enabled!\n");
}


void	__attribute__((cdecl)) ISR_Handler(t_registers* regs)
{
	if (kernel.ISRhandlers[regs->interrupt] != NULL)
	{
		kernel.ISRhandlers[regs->interrupt](regs);
	}
	else if (regs->interrupt >= 32)
	{
		terminal_writestring("Unhandled interrupt : ");
		printnbr(regs->interrupt, 10);
		terminal_writestring("\n");
	}
	else
	{
		terminal_writestring("Unhandled exception : ");
		printnbr(regs->interrupt, 10);
		terminal_writestring("  ");
		terminal_writestring(g_Exceptions[regs->interrupt]);
		terminal_writestring("\nKERNEL PANIC!\n");
		kernelPanic();
	}
}

void ISR_RegisterHandler(int interrupt, ISRHandler handler)
{
	kernel.ISRhandlers[interrupt] = handler;
	IDT_EnableGate(interrupt);
}
