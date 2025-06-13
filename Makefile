# === Paramètres =============================================================
TARGET   := i686-elf
AS       := $(TARGET)-as
NASM     := nasm
CC       := $(TARGET)-gcc
CXX      := $(TARGET)-g++
GRUBFILE := $(TARGET)-grub-file
GRUBISO  := $(TARGET)-grub-mkrescue
QEMU     := qemu-system-i386

SRC_DIR := src
BUILD   := build

# Répertoires
BUILD    := build
ISO_DIR  := $(BUILD)/isodir
ISO_BOOT := $(ISO_DIR)/boot
ISO_GRUB := $(ISO_BOOT)/grub

# Fichiers
C_SRC := $(wildcard $(SRC_DIR)/*.c)
C_OBJ := $(patsubst $(SRC_DIR)/%.c,$(BUILD)/%.o,$(C_SRC))

BOOT_SRC := $(SRC_DIR)/boot.s
BOOT_OBJ := $(BUILD)/boot.o

ASM_SRC := $(filter-out $(BOOT_SRC), $(wildcard $(SRC_DIR)/*.s))
ASM_OBJ := $(patsubst $(SRC_DIR)/%.s,$(BUILD)/%.o,$(patsubst $(SRC_DIR)/%.S,$(BUILD)/%.o,$(ASM_SRC)))

BIN      := $(BUILD)/myos.bin
ISO      := $(BUILD)/myos.iso

# Flags
CFLAGS    := -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Werror
CXXFLAGS  := -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
LDFLAGS   := -T linker.ld -ffreestanding -O2 -nostdlib
NASMFLAGS := -f elf32
# ===========================================================================#
.PHONY: all iso run clean

all: $(BIN)                         ## Compile le noyau

# 1) Assembleur
$(BOOT_OBJ): $(BOOT_SRC) | $(BUILD)
	$(AS) $< -o $@

# 2) C
$(BUILD)/%.o: $(SRC_DIR)/%.c | $(BUILD)
	$(CC) -c $< -o $@ $(CFLAGS) -I./include

$(BUILD)/%.o: $(SRC_DIR)/%.s | $(BUILD)
	$(NASM) $< -o $@ $(NASMFLAGS) -I./include


# 4) Link
$(BIN): linker.ld $(BOOT_OBJ) $(C_OBJ) $(CPP_OBJ) $(ASM_OBJ)
	$(CC) $(LDFLAGS) $(BOOT_OBJ) $(C_OBJ) $(CPP_OBJ) $(ASM_OBJ) -o $@ -lgcc
	#@$(GRUBFILE) --is-x86-multiboot $@ \
	 && echo "multiboot confirmed" \
	 || (echo "the file is not multiboot" && false)

# 5) ISO bootable
iso: $(ISO)                        ## Crée l’image ISO

$(ISO): $(BIN) grub.cfg
	@mkdir -p $(ISO_GRUB)
	cp $(BIN)            $(ISO_BOOT)/
	cp grub.cfg          $(ISO_GRUB)/
	$(GRUBISO) -o $@ $(ISO_DIR)

# 6) Lancer QEMU directement
run: $(BIN)
	$(QEMU) -kernel $<

# 7) Lancer QEMU avec l’ISO GRUB
runiso: iso
	$(QEMU) -cdrom $(ISO)

# Répertoire build
$(BUILD):
	@mkdir -p $(BUILD)

# Nettoyage
clean:
	rm -rf $(BUILD)

re: clean all
