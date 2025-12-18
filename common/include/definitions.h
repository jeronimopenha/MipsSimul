#ifndef DEFINITIONS_H
#define DEFINITIONS_H

//definitions for all project

//TODO
//In future I will put here, or in other place the specs for each CPU and forms to handle
// the mul and div and FP

#include <bits/stdc++.h>

//#define DEBUG

//Asm and Disasm Definitions.
inline constexpr uint32_t baseAsmPc = 0x00400000;
inline constexpr auto BenchAsmPath = "benchmarks/asm/";
inline constexpr auto benchAsmExt = ".asm";
inline constexpr auto benchDisasmExt = ".hex";

//MiniC definitions
inline constexpr auto BenchMiniCPath = "benchmarks/miniC";
inline constexpr auto benchMiniCExt = ".c";



#endif //DEFINITIONS_H
