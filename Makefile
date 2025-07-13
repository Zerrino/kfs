TARGET    := i686-elf
AS        := $(TARGET)-as
CC        := $(TARGET)-gcc
NASM      := nasm
QEMU      := qemu-system-i386 -m 128
MKGRUB    := $(TARGET)-grub-mkrescue
ifeq ($(shell uname), Darwin)
	MKGRUB := $(TARGET)-grub-mkrescue
else
	MKGRUB := grub-mkrescue
endif

SRC_DIR   := src
BUILD     := build
ISO_DIR   := $(BUILD)/isodir
ISO_BOOT  := $(ISO_DIR)/boot
ISO_GRUB  := $(ISO_BOOT)/grub

KERNEL    := chickenKernel
ISO_IMG   := $(BUILD)/chicken.iso

C_SRC     := $(shell find $(SRC_DIR) -type f -name '*.c')
ASM_SRC   := $(shell find $(SRC_DIR) -type f -name '*.s')
BOOT_SRC  := $(SRC_DIR)/boot.s

C_OBJ     := $(patsubst $(SRC_DIR)/%.c,$(BUILD)/%.o,$(C_SRC))
ASM_OBJ   := $(patsubst $(SRC_DIR)/%.s,$(BUILD)/%.o,$(ASM_SRC))
BOOT_OBJ  := $(BUILD)/boot.o

CFLAGS    := -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Werror \
             -fno-builtin -fno-stack-protector -nostdlib -nodefaultlibs \
             -Iinclude -m32
LDFLAGS   := -T utils/linker.ld -nostdlib -ffreestanding -O2 -m32
NASMFLAGS := -f elf32 -I./

all: $(KERNEL)

$(BUILD)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: $(SRC_DIR)/%.s
	@mkdir -p $(dir $@)
	$(NASM) $(NASMFLAGS) $< -o $@

$(BOOT_OBJ): $(BOOT_SRC)
	@mkdir -p $(dir $@)
	$(AS) $< -o $@

$(KERNEL): $(BOOT_OBJ) $(C_OBJ) $(ASM_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

$(ISO_IMG): $(KERNEL) utils/grub.cfg
	@mkdir -p $(ISO_GRUB)
	cp $(KERNEL)       $(ISO_BOOT)/
	cp utils/grub.cfg        $(ISO_GRUB)/
	$(MKGRUB) -o $@ $(ISO_DIR)

iso: $(ISO_IMG)

run: $(KERNEL)
	$(QEMU) -kernel $< -serial stdio

runiso: $(ISO_IMG)
	$(QEMU) -cdrom $< -serial stdio

clean:
	rm -rf $(BUILD)

fclean: clean

	rm -f $(KERNEL)

re: fclean all

.PHONY: all iso run runiso clean fclean re
