TARGET    := i686-elf
AS        := $(TARGET)-as
CC        := $(TARGET)-gcc
NASM      := nasm
QEMU      := qemu-system-i386
QEMU_RUN := env -i PATH="$(PATH)" HOME="$(HOME)" TERM="$(TERM)" SHELL="$(SHELL)" DISPLAY="$(DISPLAY)" XAUTHORITY="$(XAUTHORITY)" LD_LIBRARY_PATH="/usr/lib/x86_64-linux-gnu:/lib/x86_64-linux-gnu"
MKGRUB    := $(TARGET)-grub-mkrescue
ifeq ($(shell uname), Darwin)
	MKGRUB := $(TARGET)-grub-mkrescue
else
	MKGRUB := grub-mkrescue
endif

PREFIX    ?= $(HOME)/.kfs
CROSS_BIN := $(PREFIX)/bin
CROSS_BUILD := $(BUILD)/toolchain
BINUTILS_VER := 2.41
GCC_VER := 13.2.0
BINUTILS_ARCHIVE := binutils-$(BINUTILS_VER).tar.xz
GCC_ARCHIVE := gcc-$(GCC_VER).tar.xz
BINUTILS_URL := https://ftp.gnu.org/gnu/binutils/$(BINUTILS_ARCHIVE)
GCC_URL := https://ftp.gnu.org/gnu/gcc/gcc-$(GCC_VER)/$(GCC_ARCHIVE)
export PATH := $(CROSS_BIN):$(PATH)

DEPS      := $(AS) $(CC) $(NASM) $(QEMU) $(MKGRUB)

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
             -Iinclude -m32 -fno-pie
LDFLAGS   := -T utils/linker.ld -nostdlib -ffreestanding -O2 -m32 -no-pie
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
	$(QEMU_RUN) $(QEMU) -kernel $< -serial stdio

run-headless: $(KERNEL)
	$(QEMU_RUN) $(QEMU) -nographic -kernel $< -serial mon:stdio

runiso: $(ISO_IMG)
	$(QEMU_RUN) $(QEMU) -cdrom $< -serial stdio

install-deps:
	@echo 'Attempting to install required tooling...'
	@if command -v brew >/dev/null 2>&1; then \
		echo 'Detected Homebrew'; \
		brew tap nativeos/i686-elf-toolchain >/dev/null 2>&1 || true; \
		brew install nativeos/i686-elf-toolchain/i686-elf-binutils nativeos/i686-elf-toolchain/i686-elf-gcc nasm qemu gmp mpfr libmpc isl; \
		brew install xorriso || true; \
		brew install grub || true; \
	elif command -v apt-get >/dev/null 2>&1; then \
		echo 'Detected apt-get'; \
		echo 'You may be prompted for your password.'; \
		sudo apt-get update; \
		sudo apt-get install -y build-essential nasm qemu-system-x86 grub-pc-bin xorriso mtools \
			libgmp-dev libmpfr-dev libmpc-dev texinfo flex bison; \
		if ! command -v i686-elf-gcc >/dev/null 2>&1; then \
			echo 'apt-get does not provide i686-elf-gcc by default.'; \
			echo 'Please install a cross compiler manually:'; \
			echo 'https://wiki.osdev.org/GCC_Cross-Compiler'; \
		fi; \
	else \
		echo 'Unsupported package manager. Install dependencies manually.'; \
		echo 'Missing commands:'; \
		for dep in $(DEPS); do \
			if ! command -v $$dep >/dev/null 2>&1; then \
				echo "  - $$dep"; \
			fi; \
		done; \
		exit 1; \
	fi

install-cross:
	@set -e; \
	if [ -x $(CROSS_BIN)/$(TARGET)-gcc ] && [ -x $(CROSS_BIN)/$(TARGET)-as ]; then \
		echo 'Cross toolchain already present in $(CROSS_BIN)'; \
		exit 0; \
	fi; \
	echo 'Building cross toolchain for $(TARGET) into $(CROSS_BIN)'; \
	mkdir -p $(CROSS_BUILD)/src $(CROSS_BUILD)/build-binutils $(CROSS_BUILD)/build-gcc $(CROSS_BIN); \
	if [ ! -f $(CROSS_BUILD)/src/$(BINUTILS_ARCHIVE) ]; then \
		if command -v curl >/dev/null 2>&1; then \
			curl -L $(BINUTILS_URL) -o $(CROSS_BUILD)/src/$(BINUTILS_ARCHIVE); \
		elif command -v wget >/dev/null 2>&1; then \
			wget -O $(CROSS_BUILD)/src/$(BINUTILS_ARCHIVE) $(BINUTILS_URL); \
		else \
			echo 'Need curl or wget to download $(BINUTILS_ARCHIVE)'; \
			exit 1; \
		fi; \
	fi; \
	if [ ! -f $(CROSS_BUILD)/src/$(GCC_ARCHIVE) ]; then \
		if command -v curl >/dev/null 2>&1; then \
			curl -L $(GCC_URL) -o $(CROSS_BUILD)/src/$(GCC_ARCHIVE); \
		elif command -v wget >/dev/null 2>&1; then \
			wget -O $(CROSS_BUILD)/src/$(GCC_ARCHIVE) $(GCC_URL); \
		else \
			echo 'Need curl or wget to download $(GCC_ARCHIVE)'; \
			exit 1; \
		fi; \
	fi; \
	if [ ! -d $(CROSS_BUILD)/src/binutils-$(BINUTILS_VER) ]; then \
		( cd $(CROSS_BUILD)/src && tar -xf $(BINUTILS_ARCHIVE) ); \
	fi; \
	if [ ! -d $(CROSS_BUILD)/src/gcc-$(GCC_VER) ]; then \
		( cd $(CROSS_BUILD)/src && tar -xf $(GCC_ARCHIVE) ); \
	fi; \
	( cd $(CROSS_BUILD)/src/gcc-$(GCC_VER) && ./contrib/download_prerequisites ); \
	( cd $(CROSS_BUILD)/build-binutils && ../src/binutils-$(BINUTILS_VER)/configure --target=$(TARGET) --prefix=$(PREFIX) --with-sysroot --disable-nls --disable-werror ); \
	$(MAKE) -C $(CROSS_BUILD)/build-binutils; \
	$(MAKE) -C $(CROSS_BUILD)/build-binutils install; \
	( cd $(CROSS_BUILD)/build-gcc && ../src/gcc-$(GCC_VER)/configure --target=$(TARGET) --prefix=$(PREFIX) --disable-nls --enable-languages=c --without-headers --disable-libssp --disable-shared ); \
	$(MAKE) -C $(CROSS_BUILD)/build-gcc all-gcc; \
	$(MAKE) -C $(CROSS_BUILD)/build-gcc all-target-libgcc; \
	$(MAKE) -C $(CROSS_BUILD)/build-gcc install-gcc; \
	$(MAKE) -C $(CROSS_BUILD)/build-gcc install-target-libgcc; \
	echo 'Cross toolchain installed in $(CROSS_BIN)';

check-deps:
	@missing=''
	@for dep in $(DEPS); do \
		if ! command -v $$dep >/dev/null 2>&1; then \
			missing="$$missing $$dep"; \
		fi; \
	done; \
	if [ -n "$$missing" ]; then \
		echo "Missing dependencies:$$missing"; \
		exit 1; \
	else \
		echo 'All required tooling is present.'; \
	fi

clean:
	rm -rf $(BUILD)

fclean: clean

	rm -f $(KERNEL)

re: fclean all

.PHONY: run-headless all iso run runiso clean fclean re check-deps install-deps install-cross
