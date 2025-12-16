#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
MIPS ASCII Canvas
-----------------
Renders a textual "canvas" showing:
- Instruction Memory (32 entries) with the PC-selected instruction highlighted
- PC box (current and next PC)
- Register File (32 regs with MIPS names) as a nice table
Designed to be printed in a terminal with monospaced font.
"""

from typing import List, Dict

# ---------- ANSI COLORS ----------
RESET = "\x1b[0m"
BOLD = "\x1b[1m"
DIM = "\x1b[2m"

# Foreground
FG_BLACK  = "\x1b[30m"
FG_RED    = "\x1b[31m"
FG_GREEN  = "\x1b[32m"
FG_YELLOW = "\x1b[33m"
FG_BLUE   = "\x1b[34m"
FG_MAGENTA= "\x1b[35m"
FG_CYAN   = "\x1b[36m"
FG_WHITE  = "\x1b[37m"

# Background
BG_BLACK  = "\x1b[40m"
BG_RED    = "\x1b[41m"
BG_GREEN  = "\x1b[42m"
BG_YELLOW = "\x1b[43m"
BG_BLUE   = "\x1b[44m"
BG_MAGENTA= "\x1b[45m"
BG_CYAN   = "\x1b[46m"
BG_WHITE  = "\x1b[47m"

# Try to detect if ANSI is supported; allow fallback via env var override if needed
import os
ANSI_ENABLED = os.environ.get("ANSI_NO_COLOR", "0") != "1"

def c(text: str, *styles: str) -> str:
    if not ANSI_ENABLED:
        return text
    return "".join(styles) + text + RESET

# ---------- MIPS REGISTER NAMES ----------
REG_NAMES = [
    "$zero", "$at" , "$v0", "$v1",
    "$a0"  , "$a1" , "$a2", "$a3",
    "$t0"  , "$t1" , "$t2", "$t3",
    "$t4"  , "$t5" , "$t6", "$t7",
    "$s0"  , "$s1" , "$s2", "$s3",
    "$s4"  , "$s5" , "$s6", "$s7",
    "$t8"  , "$t9" , "$k0", "$k1",
    "$gp"  , "$sp" , "$fp", "$ra",
]

# ---------- RENDER HELPERS ----------
def box(title: str, lines: List[str], width: int) -> List[str]:
    """Draw a Unicode box with a title and a fixed width."""
    title = f" {title} "
    top = "┌" + "─" * (width - 2) + "┐"
    # insert title into top
    if len(title) <= width - 2:
        top = "┌" + title + "─" * (width - 2 - len(title)) + "┐"
    body = []
    for ln in lines:
        if len(ln) > width - 2:
            ln = ln[:width - 2]
        body.append("│" + ln + " " * (width - 2 - len(ln)) + "│")
    bottom = "└" + "─" * (width - 2) + "┘"
    return [top] + body + [bottom]

def pad(s: str, n: int) -> str:
    return s[:n].ljust(n)

def hex32(v: int) -> str:
    return f"0x{(v & 0xFFFFFFFF):08X}"

# ---------- COMPONENT RENDERERS ----------
def render_pc(pc: int, next_pc: int, width: int = 38) -> List[str]:
    lines = [
        f"PC atual : {hex32(pc)}",
        f"PC próximo: {hex32(next_pc)}",
    ]
    return box("PC", lines, width)

def render_instr_mem(instr_mem: List[str], pc: int, base_addr: int = 0x00000000, width: int = 66) -> List[str]:
    """
    Render 32 instruction lines. Highlight the one pointed by PC (word-aligned index = (pc-base)//4).
    Each line shows: [idx] address : text
    """
    n = 32
    assert len(instr_mem) >= n, "instr_mem precisa ter pelo menos 32 entradas"
    pc_index = max(0, min(31, (pc - base_addr) // 4))
    rows = []
    for i in range(n):
        addr = base_addr + i*4
        line = f"[{i:02}] {hex32(addr)} : {instr_mem[i]}"
        # highlight current
        if i == pc_index:
            row = c(line, BG_YELLOW, FG_BLACK, BOLD)
        else:
            row = line
        rows.append(pad(row, width-2))  # -2 because box will add borders
    return box("Instruction Memory (32)", rows, width)

def render_regfile(regs: List[int], width: int = 66) -> List[str]:
    """
    Render the 32 registers in 4 columns (8 rows), each cell as: name: 0xXXXXXXXX
    """
    assert len(regs) == 32, "regs precisa ter 32 inteiros"
    cols = 4
    per_col = 8
    col_w = (width - 2 - (cols - 1) * 2) // cols  # 2 spaces between columns
    lines = []
    for row in range(per_col):
        parts = []
        for col in range(cols):
            idx = col * per_col + row
            name = REG_NAMES[idx]
            val = hex32(regs[idx])
            cell = f"{name}: {val}"
            parts.append(pad(cell, col_w))
        lines.append(("  ").join(parts))
    return box("Register File (32)", lines, width)

# ---------- MASTER LAYOUT ----------
def hstack(blocks: List[List[str]], gap: int = 2) -> List[str]:
    """Horizontally stack boxed blocks (lists of lines) with equalized heights."""
    heights = [len(b) for b in blocks]
    H = max(heights)
    # normalize heights
    norm = []
    for b in blocks:
        if len(b) < H:
            b = b + [" " * len(b[0])] * (H - len(b))
        norm.append(b)
    # stitch columns
    out = []
    for i in range(H):
        out.append((" " * gap).join(col[i] for col in norm))
    return out

def vstack(blocks: List[List[str]], gap: int = 1) -> List[str]:
    out = []
    first = True
    for b in blocks:
        if not first:
            out.extend([""] * gap)
        out.extend(b)
        first = False
    return out

def render_frame(state: Dict) -> str:
    """
    state = {
      'pc': int,
      'next_pc': int,
      'instr_mem': List[str] (len>=32),
      'regs': List[int] (len=32),
      'base_addr': int (optional, default 0)
    }
    """
    pc_block  = render_pc(state["pc"], state["next_pc"], width=38)
    imem_block= render_instr_mem(state["instr_mem"], state["pc"], base_addr=state.get("base_addr", 0), width=66)
    regs_block= render_regfile(state["regs"], width=66)

    # Layout: left column = PC + (space) + small legend; right column = InstrMem on top, Regs below
    left = pc_block
    right_top = imem_block
    right_bot = regs_block

    right = vstack([right_top, right_bot], gap=1)
    canvas = hstack([left, right], gap=3)
    return "\n".join(canvas)

# ---------- DEMO ----------
DEMO_INSTR = [
    "addi $t0, $zero, 5",
    "addi $t1, $zero, 7",
    "add  $t2, $t0, $t1",
    "sub  $t3, $t1, $t0",
    "and  $t4, $t2, $t3",
    "or   $t5, $t2, $t3",
    "slt  $t6, $t0, $t1",
    "beq  $t0, $t1, label",
    "lw   $t7, 0($t0)",
    "sw   $t7, 4($t0)",
    "j    end",
    "label: addi $t0, $t0, 1",
    "bne  $t0, $t1, label",
    "nop",
    "xor  $t7, $t5, $t4",
    "sll  $t8, $t7, 2",
    "srl  $t9, $t8, 1",
    "jr   $ra",
    "end: nop",
] + ["nop"] * (32 - 19)

def demo_state(pc_word_index: int = 8) -> Dict:
    base_addr = 0x00400000
    pc = base_addr + 4 * pc_word_index
    next_pc = pc + 4
    regs = [0]*32
    # Fill some demo values
    regs[8]  = 5   # $t0
    regs[9]  = 7   # $t1
    regs[10] = regs[8] + regs[9]  # $t2
    regs[29] = 0x7FFFEFFC          # $sp
    regs[31] = 0x00400040          # $ra

    state = {
        "pc": pc,
        "next_pc": next_pc,
        "instr_mem": DEMO_INSTR,
        "regs": regs,
        "base_addr": base_addr,
    }
    return state

if __name__ == "__main__":
    s = demo_state(pc_word_index=8)
    print(render_frame(s))
    print("\nHint: import render_frame() and pass your own state dict to update PC, regs e instruções.\n")
