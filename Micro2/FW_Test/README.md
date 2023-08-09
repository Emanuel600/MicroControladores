# Firmware Test For ARM (STM32-F0)

Description:

> Testing the GNU-embedded toolchain for ARM and doxygen documentation
    generation

After disassembling, the symbol `<Reset_Handler>` could not be found,
  tested with:

 - Adding startup file directly to linker (no precompiling)
 - Making `<Reset_Handler>` global
 - Changing symbol to generate linker warning (no warning generated)

Question:

> Why can't I find the symbol, and is the code starting where it should?

TODO:

> Re-generate project makefile and disassemble again
