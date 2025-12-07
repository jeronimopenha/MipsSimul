cmake_minimum_required(VERSION 3.29)
project(MIPSSimul)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_DEBUG "-g -fopenmp")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fopenmp")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()


include_directories(
        ${CMAKE_SOURCE_DIR}/minic/include
        ${CMAKE_SOURCE_DIR}/assembler/include
        ${CMAKE_SOURCE_DIR}/common/include
        ${CMAKE_SOURCE_DIR}/tools/include
)


#target_include_directories(LexerTestMain PRIVATE
#        ${CMAKE_SOURCE_DIR}/assembler/include
#        ${CMAKE_SOURCE_DIR}/common/include
#        ${CMAKE_SOURCE_DIR}/tools/include
#)

file(GLOB_RECURSE COMMON_SRC
        common/src/common.cpp
)

# Asm Lexer Test
file(GLOB_RECURSE ASM_LEXER_SRC
        ${COMMON_SRC}
        common/src/lexer.cpp
        assembler/src/asm_lexer.cpp
        assembler/src/asm_t_kind.cpp
)
add_executable(AsmLexerTestMain
        tools/tests/asmLexerTestMain.cpp
        ${ASM_LEXER_SRC}
)

# Asm Lexer Test
file(GLOB_RECURSE ASM_PARSER_SRC
        ${ASM_LEXER_SRC}
        common/src/parser.cpp
        assembler/src/asm_parser.cpp
)
add_executable(AsmParserTestMain
        tools/tests/asmParserTestMain.cpp
        ${ASM_PARSER_SRC}
)

# Asm Test
file(GLOB_RECURSE ASSEMBLER_SRC
        ${ASM_PARSER_SRC}
        assembler/src/asm_registers.cpp
        assembler/src/asm_isa.cpp
        assembler/src/asm.cpp
)
add_executable(AssemblerTestMain
        tools/tests/asmTestMain.cpp
        ${ASSEMBLER_SRC}
)

# Disasm Test
file(GLOB_RECURSE DISASSEMBLER_SRC
        ${ASSEMBLER_SRC}
        assembler/src/disasm.cpp
)
add_executable(DisassemblerTestMain
        tools/tests/disasmTestMain.cpp
        ${DISASSEMBLER_SRC}
)

# MiniC Lexer Test
file(GLOB_RECURSE MINIC_LEXER_SRC
        ${COMMON_SRC}
        common/src/lexer.cpp
        minic/src/minic_lexer.cpp
        minic/src/minic_t_kind.cpp
)
add_executable(MinicLexerTestMain
        tools/tests/asmLexerTestMain.cpp
        ${MINIC_LEXER_SRC}
)

# assembler tool
file(GLOB_RECURSE ASSEMBLER_TOOLS_SRC
        ${DISASSEMBLER_SRC}
        tools/src/assembler_cli.cpp
)
add_executable(assembler
        tools/src/assembler_tools.cpp
        ${ASSEMBLER_TOOLS_SRC}
)