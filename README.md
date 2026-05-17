# KFS Status Notes

This repository currently contains the base work from KFS1, KFS2, KFS3, and the
current KFS4 interrupt work.

## KFS4 Checklist

### Already present

- IDT entry and descriptor structures are defined in `include/core/structs.h`.
- The IDT is initialized and loaded with `lidt` through `IDT_Initialize()` and
  `IDT_Load()`.
- ISR stubs exist for all 256 interrupt vectors in `src/asm/isr.s`.
- IDT gates are filled for vectors 0 through 255 in `ISR_InitializeGates()`.
- CPU exceptions have basic messages, with a dedicated page fault handler.
- The PIC is remapped to `0x20` / `0x28`.
- IRQ0 and IRQ1 are unmasked.
- IRQ0 is connected to the timer handler.
- IRQ1 is connected to the keyboard handler.
- A signal callback queue exists through `t_signal`, `kernel.signalQueue`,
  `init_signals()`, and the timer-driven dispatch path.
- A first syscall base exists on interrupt `0x80`, with `SYS_write()`.
- Keyboard IRQ handling now decodes scancodes in the kernel, queues characters in a kernel input buffer, and lets the shell consume that buffer.

### Mandatory items still missing or needing cleanup

- Add a clear public API for scheduling signals.
  - The internal queue exists, but there is no obvious function such as
    `signal_schedule(handler, regs)` or equivalent for kernel code to call.
- Finish the panic / halt cleanup interface.
  - `kernelPanic()` currently disables interrupts and halts.
  - The subject asks for an interface to clean registers before panic / halt.
    That behavior should be explicit and easy to defend.
- Finish the stack-save interface for panic.
  - ISR entry saves registers with `pusha`, but there is no dedicated panic
    stack snapshot/dump path.
  - Add a function that copies or prints the relevant stack/register state before
    stopping the kernel.
- Verify keyboard IRQ acknowledgement and dispatch behavior.
  - IRQ1 is queued as a signal and EOI is sent, then the actual keyboard handler
    runs later from the timer path.
  - This may be acceptable, but it should be tested carefully because delayed
    PS/2 reads can lose or confuse keyboard data.
- Fix remaining keyboard scancode handling edge cases.
  - Extended `0xE0` arrow prefixes are handled, but more extended keys can be added.
  - Shift and caps lock are applied to printable characters.
  - Alt and richer release handling are still incomplete.
- Build and boot-test the full KFS4 path in QEMU/Bochs before defense.

### Bonus status

- Multi-layout keyboard support is started.
  - QWERTY and AZERTY keymaps exist.
  - The active layout can be switched with `layout qwerty` or `layout azerty`.
- `get_line`-style input is started.
  - `keyboard_readline()` returns a completed kernel input line without blocking.
  - `keyboard_getline()` blocks with `hlt` until Enter completes a line.
- Improve keyboard editing behavior.
  - Backspace exists in the shell path.
  - Useful bonus polish would include left/right movement inside the current
    input line, delete, home/end, and safe buffer bounds.
- Syscall bonus is started but not complete.
  - `int 0x80` and syscall number `1` / write exist.
  - A fuller base should define syscall numbers, argument conventions, return
    values, and error behavior in headers.

## Suggested Next Order

1. Add an explicit signal scheduling function and use it from ISR/IRQ code.
2. Implement panic register cleanup plus stack snapshot/dump.
3. Expand keyboard editing beyond backspace, especially in-line left/right edits.
4. Decide whether the shell should keep consuming characters or move fully to
   `keyboard_getline()`.
5. Boot-test timer, keyboard, shell commands, page fault panic, and `int 0x80`.
