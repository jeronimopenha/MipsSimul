cmake_minimum_required(VERSION 3.29)
project(MIPSSimul)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_DEBUG "-g -fopenmp -lSDL2")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fopenmp -lSDL2")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()


include_directories(include)


file(GLOB_RECURSE G_SRC src/common.cpp)

# Lexer Test
file(GLOB_RECURSE LEXER_SRC ${G_SRC} src/lexer.cpp src/tokens.cpp)
add_executable(LexerTestMain src/tests/lexerTestMain.cpp ${LEXER_SRC})

# Parser Test
file(GLOB_RECURSE PARSER_SRC ${LEXER_SRC} src/parser.cpp)
add_executable(ParserTestMain src/tests/parserTestMain.cpp ${PARSER_SRC})

# Assembler Test
file(GLOB_RECURSE ASSEMBLER_SRC ${PARSER_SRC} src/registers.cpp src/isa.cpp src/assembler.cpp)
add_executable(AssemblerTestMain src/tests/assemblerTestMain.cpp ${ASSEMBLER_SRC}
        include/tools_cli.h
        src/tools_cli.cpp
        src/main/mips_tools.cpp)

# Disassembler Test
file(GLOB_RECURSE DISASSEMBLER_SRC ${ASSEMBLER_SRC} src/disassembler.cpp)
add_executable(DisassemblerTestMain src/tests/disassemblerTestMain.cpp ${DISASSEMBLER_SRC})

# mips toos
file(GLOB_RECURSE MIPS_TOOLS_SRC ${DISASSEMBLER_SRC} src/tools_cli.cpp)
add_executable(mips_tool src/main/mips_tools.cpp ${MIPS_TOOLS_SRC})