# MipsSimul
Mips Simulator

Little MIPS Tools for convert MIPS asm to lexemes, Parse structure, hexadecimal words and hexa disassembly.

To compile: Clone the repository on newer tag and:

``` bash
cd MipsSimul
mkdir build
cd build
cmake -DCMAKE_BUILD=Release ..
cmake --build . --target mips_tool
```

To run:
``` bash
./mips_tool -t {lex|parse|asm|disasm} [-i file] [-o file]
```

The files are .txt format and you can see on screen the response if you don't use -o.



