toolchain:
	@which $(TARGET)-gcc > /dev/null || (echo "ERROR: $(TARGET)-gcc not found. Please install i686-elf-gcc from the AUR." && exit 1)
	@which $(TARGET)-ld > /dev/null || (echo "ERROR: $(TARGET)-ld not found. Please install i686-elf-binutils from the AUR." && exit 1)
	@echo "Toolchain verified: $(TARGET)-gcc and $(TARGET)-ld are ready."

clean-toolchain:
	@echo "Toolchain is managed globally by the system package manager. Nothing to clean."

clean-toolchain-all:
	@echo "Toolchain is managed globally by the system package manager. Nothing to clean."

.PHONY: toolchain clean-toolchain clean-toolchain-all
