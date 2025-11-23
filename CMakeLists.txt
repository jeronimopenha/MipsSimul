cmake_minimum_required(VERSION 3.29)
project(MIPSSimul)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_DEBUG "-g -fopenmp -lSDL2")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fopenmp -lSDL2")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()


include_directories(include)

# Fontes comuns
#file(GLOB_RECURSE COMMON_SRCS src/common/*.cpp)

# Parser Executável
file(GLOB_RECURSE G_SRC src/common.cpp)

file(GLOB_RECURSE LEXER_SRC ${G_SRC} src/lexer.cpp)
add_executable(LexerTestMain src/tests/lexerTestMain.cpp ${LEXER_SRC})

file(GLOB_RECURSE PARSER_SRC ${LEXER_SRC} src/parser.cpp)
add_executable(ParserTestMain src/tests/parserTestMain.cpp ${PARSER_SRC})

file(GLOB_RECURSE ASSEMBLER_SRC ${PARSER_SRC} src/registers.cpp src/isa.cpp src/assembler.cpp)
add_executable(AssemblerTestMain src/tests/assemblerTestMain.cpp ${ASSEMBLER_SRC})

