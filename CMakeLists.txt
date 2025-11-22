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
file(GLOB_RECURSE PARSER_SRCS src/*.cpp)
add_executable(ParserTestMain parserTestMain.cpp ${PARSER_SRCS}) #${COMMON_SRCS} ${FPGA_SRCS})
add_executable(LexerTestMain lexerTestMain.cpp ${PARSER_SRCS}) #${COMMON_SRCS} ${FPGA_SRCS})
add_executable(AssemblerTestMain assemblerTestMain.cpp ${PARSER_SRCS}) #${COMMON_SRCS} ${FPGA_SRCS})


# Ferramentas / Outros
#file(GLOB_RECURSE TOOL_SRCS impl/tools/*.cpp)
#add_executable(chDotStrToN chDotStrToNMain.cpp ${COMMON_SRCS})

#add_executable(rearrange rearrangeInOut.cpp ${COMMON_SRCS})

#add_executable(test test.cpp ${COMMON_SRCS})