/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isr.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 02:10:22 by zerrino           #+#    #+#             */
/*   Updated: 2025/07/10 17:05:37 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kfs.h"

const char *get_exception_message(uint32_t exception_num)
{
    switch (exception_num) {
        case EXCEPTION_DIVIDE_BY_ZERO:
            return EXCEPTION_MSG_DIVIDE_BY_ZERO;
        case EXCEPTION_DEBUG:
            return EXCEPTION_MSG_DEBUG;
        case EXCEPTION_NMI:
            return EXCEPTION_MSG_NMI;
        case EXCEPTION_BREAKPOINT:
            return EXCEPTION_MSG_BREAKPOINT;
        case EXCEPTION_OVERFLOW:
            return EXCEPTION_MSG_OVERFLOW;
        case EXCEPTION_BOUND_RANGE:
            return EXCEPTION_MSG_BOUND_RANGE;
        case EXCEPTION_INVALID_OPCODE:
            return EXCEPTION_MSG_INVALID_OPCODE;
        case EXCEPTION_DEVICE_NOT_AVAILABLE:
            return EXCEPTION_MSG_DEVICE_NOT_AVAILABLE;
        case EXCEPTION_DOUBLE_FAULT:
            return EXCEPTION_MSG_DOUBLE_FAULT;
        case EXCEPTION_COPROCESSOR_OVERRUN:
            return EXCEPTION_MSG_COPROCESSOR_OVERRUN;
        case EXCEPTION_INVALID_TSS:
            return EXCEPTION_MSG_INVALID_TSS;
        case EXCEPTION_SEGMENT_NOT_PRESENT:
            return EXCEPTION_MSG_SEGMENT_NOT_PRESENT;
        case EXCEPTION_STACK_SEGMENT_FAULT:
            return EXCEPTION_MSG_STACK_SEGMENT_FAULT;
        case EXCEPTION_GENERAL_PROTECTION:
            return EXCEPTION_MSG_GENERAL_PROTECTION;
        case EXCEPTION_PAGE_FAULT:
            return EXCEPTION_MSG_PAGE_FAULT;
        case EXCEPTION_X87_FPU:
            return EXCEPTION_MSG_X87_FPU;
        case EXCEPTION_ALIGNMENT_CHECK:
            return EXCEPTION_MSG_ALIGNMENT_CHECK;
        case EXCEPTION_MACHINE_CHECK:
            return EXCEPTION_MSG_MACHINE_CHECK;
        case EXCEPTION_SIMD_FPU:
            return EXCEPTION_MSG_SIMD_FPU;
        case EXCEPTION_VIRTUALIZATION:
            return EXCEPTION_MSG_VIRTUALIZATION;
        case EXCEPTION_CONTROL_PROTECTION:
            return EXCEPTION_MSG_CONTROL_PROTECTION;
        case EXCEPTION_HYPERVISOR_INJECTION:
            return EXCEPTION_MSG_HYPERVISOR_INJECTION;
        case EXCEPTION_VMM_COMMUNICATION:
            return EXCEPTION_MSG_VMM_COMMUNICATION;
        case EXCEPTION_SECURITY:
            return EXCEPTION_MSG_SECURITY;
        case EXCEPTION_RESERVED_15:
        case EXCEPTION_RESERVED_22:
        case EXCEPTION_RESERVED_23:
        case EXCEPTION_RESERVED_24:
        case EXCEPTION_RESERVED_25:
        case EXCEPTION_RESERVED_26:
        case EXCEPTION_RESERVED_27:
        case EXCEPTION_RESERVED_31:
        default:
            return EXCEPTION_MSG_RESERVED;
    }
}

static void* get_isr_function(int index) {
    switch (index) {
        case 0: return ISR0; case 1: return ISR1; case 2: return ISR2; case 3: return ISR3;
        case 4: return ISR4; case 5: return ISR5; case 6: return ISR6; case 7: return ISR7;
        case 8: return ISR8; case 9: return ISR9; case 10: return ISR10; case 11: return ISR11;
        case 12: return ISR12; case 13: return ISR13; case 14: return ISR14; case 15: return ISR15;
        case 16: return ISR16; case 17: return ISR17; case 18: return ISR18; case 19: return ISR19;
        case 20: return ISR20; case 21: return ISR21; case 22: return ISR22; case 23: return ISR23;
        case 24: return ISR24; case 25: return ISR25; case 26: return ISR26; case 27: return ISR27;
        case 28: return ISR28; case 29: return ISR29; case 30: return ISR30; case 31: return ISR31;
        case 32: return ISR32; case 33: return ISR33; case 34: return ISR34; case 35: return ISR35;
        case 36: return ISR36; case 37: return ISR37; case 38: return ISR38; case 39: return ISR39;
        case 40: return ISR40; case 41: return ISR41; case 42: return ISR42; case 43: return ISR43;
        case 44: return ISR44; case 45: return ISR45; case 46: return ISR46; case 47: return ISR47;
        case 48: return ISR48; case 49: return ISR49; case 50: return ISR50; case 51: return ISR51;
        case 52: return ISR52; case 53: return ISR53; case 54: return ISR54; case 55: return ISR55;
        case 56: return ISR56; case 57: return ISR57; case 58: return ISR58; case 59: return ISR59;
        case 60: return ISR60; case 61: return ISR61; case 62: return ISR62; case 63: return ISR63;
        case 64: return ISR64; case 65: return ISR65; case 66: return ISR66; case 67: return ISR67;
        case 68: return ISR68; case 69: return ISR69; case 70: return ISR70; case 71: return ISR71;
        case 72: return ISR72; case 73: return ISR73; case 74: return ISR74; case 75: return ISR75;
        case 76: return ISR76; case 77: return ISR77; case 78: return ISR78; case 79: return ISR79;
        case 80: return ISR80; case 81: return ISR81; case 82: return ISR82; case 83: return ISR83;
        case 84: return ISR84; case 85: return ISR85; case 86: return ISR86; case 87: return ISR87;
        case 88: return ISR88; case 89: return ISR89; case 90: return ISR90; case 91: return ISR91;
        case 92: return ISR92; case 93: return ISR93; case 94: return ISR94; case 95: return ISR95;
        case 96: return ISR96; case 97: return ISR97; case 98: return ISR98; case 99: return ISR99;
        case 100: return ISR100; case 101: return ISR101; case 102: return ISR102; case 103: return ISR103;
        case 104: return ISR104; case 105: return ISR105; case 106: return ISR106; case 107: return ISR107;
        case 108: return ISR108; case 109: return ISR109; case 110: return ISR110; case 111: return ISR111;
        case 112: return ISR112; case 113: return ISR113; case 114: return ISR114; case 115: return ISR115;
        case 116: return ISR116; case 117: return ISR117; case 118: return ISR118; case 119: return ISR119;
        case 120: return ISR120; case 121: return ISR121; case 122: return ISR122; case 123: return ISR123;
        case 124: return ISR124; case 125: return ISR125; case 126: return ISR126; case 127: return ISR127;
        case 128: return ISR128; case 129: return ISR129; case 130: return ISR130; case 131: return ISR131;
        case 132: return ISR132; case 133: return ISR133; case 134: return ISR134; case 135: return ISR135;
        case 136: return ISR136; case 137: return ISR137; case 138: return ISR138; case 139: return ISR139;
        case 140: return ISR140; case 141: return ISR141; case 142: return ISR142; case 143: return ISR143;
        case 144: return ISR144; case 145: return ISR145; case 146: return ISR146; case 147: return ISR147;
        case 148: return ISR148; case 149: return ISR149; case 150: return ISR150; case 151: return ISR151;
        case 152: return ISR152; case 153: return ISR153; case 154: return ISR154; case 155: return ISR155;
        case 156: return ISR156; case 157: return ISR157; case 158: return ISR158; case 159: return ISR159;
        case 160: return ISR160; case 161: return ISR161; case 162: return ISR162; case 163: return ISR163;
        case 164: return ISR164; case 165: return ISR165; case 166: return ISR166; case 167: return ISR167;
        case 168: return ISR168; case 169: return ISR169; case 170: return ISR170; case 171: return ISR171;
        case 172: return ISR172; case 173: return ISR173; case 174: return ISR174; case 175: return ISR175;
        case 176: return ISR176; case 177: return ISR177; case 178: return ISR178; case 179: return ISR179;
        case 180: return ISR180; case 181: return ISR181; case 182: return ISR182; case 183: return ISR183;
        case 184: return ISR184; case 185: return ISR185; case 186: return ISR186; case 187: return ISR187;
        case 188: return ISR188; case 189: return ISR189; case 190: return ISR190; case 191: return ISR191;
        case 192: return ISR192; case 193: return ISR193; case 194: return ISR194; case 195: return ISR195;
        case 196: return ISR196; case 197: return ISR197; case 198: return ISR198; case 199: return ISR199;
        case 200: return ISR200; case 201: return ISR201; case 202: return ISR202; case 203: return ISR203;
        case 204: return ISR204; case 205: return ISR205; case 206: return ISR206; case 207: return ISR207;
        case 208: return ISR208; case 209: return ISR209; case 210: return ISR210; case 211: return ISR211;
        case 212: return ISR212; case 213: return ISR213; case 214: return ISR214; case 215: return ISR215;
        case 216: return ISR216; case 217: return ISR217; case 218: return ISR218; case 219: return ISR219;
        case 220: return ISR220; case 221: return ISR221; case 222: return ISR222; case 223: return ISR223;
        case 224: return ISR224; case 225: return ISR225; case 226: return ISR226; case 227: return ISR227;
        case 228: return ISR228; case 229: return ISR229; case 230: return ISR230; case 231: return ISR231;
        case 232: return ISR232; case 233: return ISR233; case 234: return ISR234; case 235: return ISR235;
        case 236: return ISR236; case 237: return ISR237; case 238: return ISR238; case 239: return ISR239;
        case 240: return ISR240; case 241: return ISR241; case 242: return ISR242; case 243: return ISR243;
        case 244: return ISR244; case 245: return ISR245; case 246: return ISR246; case 247: return ISR247;
        case 248: return ISR248; case 249: return ISR249; case 250: return ISR250; case 251: return ISR251;
        case 252: return ISR252; case 253: return ISR253; case 254: return ISR254; case 255: return ISR255;
        default: return ISR0; /* Fallback to ISR0 for safety */
    }
}

void	ISR_InitializeGates()
{
    for (int i = 0; i < 256; i++) {
        IDT_SetGate(i, get_isr_function(i), GDT_KERNEL_CODE, IDT_FLAG_RIN0 | IDT_FLAG_GATE_32BIT_INT);
    }
}

void	ISR_Initialize()
{
	ISR_InitializeGates();
	for (int i = 0; i < 256; i++)
		IDT_EnableGate(i);
}


void	__attribute__((cdecl)) ISR_Handler(t_registers* regs)
{
	if (kernel.ISRhandlers[regs->interrupt] != NULL)
		kernel.ISRhandlers[regs->interrupt](regs);
	else if (32 <= regs->interrupt)
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
		terminal_writestring(get_exception_message(regs->interrupt));
		terminal_writestring("\nKERNEL PANIC!\n");
		kernelPanic();
	}
}

void ISR_RegisterHandler(int interrupt, ISRHandler handler)
{
	kernel.ISRhandlers[interrupt] = handler;
	IDT_EnableGate(interrupt);
}
