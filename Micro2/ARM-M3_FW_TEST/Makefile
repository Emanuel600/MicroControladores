# Load variables
include env_vars.mk
# Create build directory
$(BUILD_DIR)/:
	mkdir -p $(BUILD_DIR)
# Create bin directory
$(BIN_DIR)/:
	mkdir -p $(BIN_DIR)
# Created dep directory
$(DEP_DIR)/:
	mkdir -p $(DEP_DIR)
#====================#
.PHONY: clean compile CleanCompile run CleanAll CleanBin decompile debug
.PHONY: DecompileHeaders ShowHelp CleanDocs

# 'compile' tracks all relevant files]
compile: $(APP).hex

$(BUILD_DIR)/%.o: %.c $(CC_HEADS) | $(BUILD_DIR)/ $(DEP_DIR)/
	@echo ""
	@echo ""
	@echo "Building $(notdir $@):"
	$(CC) -c $(CC_FLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@
$(BUILD_DIR)/%.o: %.cpp $(CXX_HEADS) | $(BUILD_DIR)/ $(DEP_DIR)/
	@echo ""
	@echo ""
	@echo "Building $(notdir $@):"
	$(CXX) -c $(CXX_FLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@
$(BUILD_DIR)/%.o: %.s $(ASM_HEADS) | $(BUILD_DIR)/ $(DEP_DIR)/
	@echo ""
	@echo ""
	@echo "Building $(notdir $@):"
	$(AS) -c $(ASM_FLAGS) $< -o $@

$(APP).elf: $(OBJECTS) | $(BIN_DIR)/
	@echo ""
	@echo ""
	@echo "==| Linking files |=="
	$(CXX) $(OBJECTS) $(LDFLAGS) $(OPT_F) -o $(APP).elf
$(APP).bin: $(APP).elf
	$(OBJ_COPY) $(APP).elf -O binary $(APP).bin
$(APP).hex: $(APP).bin
	$(OBJ_COPY) $(APP).elf -O ihex $(APP).hex
	$(SZ) -x $(APP).elf

clean:
	rm -f $(BUILD_DIR)/*
	rm -f $(DEP_DIR)/*

CleanAll: clean CleanBin CleanDocs

CleanBin:
	rm -f $(BIN_DIR)/*

CleanDocs:
	rm -rf $(DOC_DIR)/*

CleanCompile: clean compile


decompile:
	$(OBJ_DUMP) -S -EL $(APP).elf
DecompileHeaders:
	$(OBJ_DUMP) -h -g -EL $(APP).elf

test:
	@echo "$(CC_SRCS)"

.DEFAULT_GOAL		:= ShowHelp

ShowHelp:
	@echo "Available rules:"
	@echo ""
	@echo "compile:"
	@echo "	Compiles target, tracks all objects, sources and headers"
	@echo "Compiles C, C++ and ASM as-is and links them into an ELF file"
	@echo "Also generates a .hex and .bin on top of size info"
	@echo ""
	@echo "clean:"
	@echo "	Cleans everything on /build and /dep directories"
	@echo ""
	@echo "CleanBin:"
	@echo "	Cleans /bin"
	@echo ""
	@echo "CleanDocs:"
	@echo "	Cleans /docs"
	@echo ""
	@echo "CleanAll:"
	@echo "	Cleans /build, /dep, /bin and /docs"
	@echo ""
	@echo "decompile:"
	@echo "	Decompiles the ELF file generated from the target"
	@echo ""
	@echo "DecompileHeaders:"
	@echo "	Shows asm headers and debug info"
	@echo ""
	@echo "ShowHelp (default goal):"
	@echo "	Shows this info, all available goals and extras configured"
	@echo "as tasks"
	@echo ""

# Load dependencies
-include $(wildcard $(DEP_DIR)/*.d)
