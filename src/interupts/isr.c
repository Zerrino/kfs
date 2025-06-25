/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isr.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zerrino <zerrino@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 02:10:22 by zerrino           #+#    #+#             */
/*   Updated: 2025/06/26 00:22:23 by zerrino          ###   ########.fr       */
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
	IDT_SetGate(0, ISR0, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(1, ISR1, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(2, ISR2, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(3, ISR3, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(4, ISR4, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(5, ISR5, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(6, ISR6, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(7, ISR7, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(8, ISR8, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(9, ISR9, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(10, ISR10, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(11, ISR11, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(12, ISR12, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(13, ISR13, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(14, ISR14, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(15, ISR15, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(16, ISR16, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(17, ISR17, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(18, ISR18, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(19, ISR19, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(20, ISR20, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(21, ISR21, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(22, ISR22, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(23, ISR23, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(24, ISR24, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(25, ISR25, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(26, ISR26, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(27, ISR27, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(28, ISR28, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(29, ISR29, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(30, ISR30, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(31, ISR31, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(32, ISR32, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(33, ISR33, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(34, ISR34, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(35, ISR35, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(36, ISR36, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(37, ISR37, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(38, ISR38, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(39, ISR39, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(40, ISR40, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(41, ISR41, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(42, ISR42, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(43, ISR43, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(44, ISR44, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(45, ISR45, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(46, ISR46, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(47, ISR47, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(48, ISR48, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(49, ISR49, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(50, ISR50, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(51, ISR51, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(52, ISR52, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(53, ISR53, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(54, ISR54, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(55, ISR55, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(56, ISR56, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(57, ISR57, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(58, ISR58, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(59, ISR59, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(60, ISR60, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(61, ISR61, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(62, ISR62, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(63, ISR63, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(64, ISR64, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(65, ISR65, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(66, ISR66, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(67, ISR67, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(68, ISR68, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(69, ISR69, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(70, ISR70, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(71, ISR71, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(72, ISR72, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(73, ISR73, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(74, ISR74, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(75, ISR75, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(76, ISR76, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(77, ISR77, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(78, ISR78, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(79, ISR79, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(80, ISR80, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(81, ISR81, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(82, ISR82, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(83, ISR83, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(84, ISR84, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(85, ISR85, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(86, ISR86, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(87, ISR87, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(88, ISR88, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(89, ISR89, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(90, ISR90, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(91, ISR91, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(92, ISR92, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(93, ISR93, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(94, ISR94, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(95, ISR95, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(96, ISR96, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(97, ISR97, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(98, ISR98, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(99, ISR99, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(100, ISR100, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(101, ISR101, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(102, ISR102, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(103, ISR103, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(104, ISR104, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(105, ISR105, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(106, ISR106, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(107, ISR107, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(108, ISR108, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(109, ISR109, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(110, ISR110, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(111, ISR111, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(112, ISR112, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(113, ISR113, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(114, ISR114, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(115, ISR115, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(116, ISR116, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(117, ISR117, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(118, ISR118, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(119, ISR119, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(120, ISR120, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(121, ISR121, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(122, ISR122, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(123, ISR123, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(124, ISR124, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(125, ISR125, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(126, ISR126, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(127, ISR127, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(128, ISR128, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(129, ISR129, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(130, ISR130, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(131, ISR131, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(132, ISR132, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(133, ISR133, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(134, ISR134, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(135, ISR135, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(136, ISR136, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(137, ISR137, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(138, ISR138, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(139, ISR139, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(140, ISR140, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(141, ISR141, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(142, ISR142, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(143, ISR143, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(144, ISR144, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(145, ISR145, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(146, ISR146, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(147, ISR147, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(148, ISR148, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(149, ISR149, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(150, ISR150, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(151, ISR151, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(152, ISR152, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(153, ISR153, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(154, ISR154, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(155, ISR155, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(156, ISR156, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(157, ISR157, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(158, ISR158, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(159, ISR159, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(160, ISR160, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(161, ISR161, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(162, ISR162, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(163, ISR163, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(164, ISR164, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(165, ISR165, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(166, ISR166, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(167, ISR167, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(168, ISR168, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(169, ISR169, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(170, ISR170, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(171, ISR171, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(172, ISR172, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(173, ISR173, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(174, ISR174, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(175, ISR175, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(176, ISR176, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(177, ISR177, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(178, ISR178, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(179, ISR179, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(180, ISR180, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(181, ISR181, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(182, ISR182, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(183, ISR183, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(184, ISR184, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(185, ISR185, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(186, ISR186, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(187, ISR187, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(188, ISR188, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(189, ISR189, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(190, ISR190, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(191, ISR191, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(192, ISR192, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(193, ISR193, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(194, ISR194, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(195, ISR195, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(196, ISR196, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(197, ISR197, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(198, ISR198, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(199, ISR199, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(200, ISR200, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(201, ISR201, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(202, ISR202, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(203, ISR203, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(204, ISR204, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(205, ISR205, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(206, ISR206, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(207, ISR207, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(208, ISR208, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(209, ISR209, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(210, ISR210, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(211, ISR211, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(212, ISR212, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(213, ISR213, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(214, ISR214, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(215, ISR215, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(216, ISR216, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(217, ISR217, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(218, ISR218, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(219, ISR219, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(220, ISR220, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(221, ISR221, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(222, ISR222, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(223, ISR223, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(224, ISR224, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(225, ISR225, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(226, ISR226, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(227, ISR227, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(228, ISR228, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(229, ISR229, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(230, ISR230, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(231, ISR231, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(232, ISR232, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(233, ISR233, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(234, ISR234, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(235, ISR235, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(236, ISR236, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(237, ISR237, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(238, ISR238, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(239, ISR239, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(240, ISR240, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(241, ISR241, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(242, ISR242, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(243, ISR243, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(244, ISR244, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(245, ISR245, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(246, ISR246, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(247, ISR247, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(248, ISR248, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(249, ISR249, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(250, ISR250, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(251, ISR251, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(252, ISR252, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(253, ISR253, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(254, ISR254, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
	IDT_SetGate(255, ISR255, 0x08, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
}

void	ISR_Initialize()
{
	int	i;
	ISR_InitializeGates();

	i = 0;
	while (i < 256)
	{
		IDT_EnableGate(i);
		i++;
	}
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
