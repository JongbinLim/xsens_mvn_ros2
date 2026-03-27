#!/usr/bin/env python3
"""Generate a linkable stub library from an ELF shared library with stripped sections.

Reads the dynamic symbol table via program headers (DYNAMIC segment) and
generates a C source file with empty stub definitions for every exported
symbol.  Compiling that source into a shared library produces a thin .so
the static linker can resolve symbols against, while the real (stripped)
library is loaded at runtime.

Usage:
    generate_link_stubs.py <input.so> <output.c>
"""

import struct
import sys
from pathlib import Path

# ELF constants
PT_LOAD = 1
PT_DYNAMIC = 2
DT_NULL = 0
DT_HASH = 4
DT_STRTAB = 5
DT_SYMTAB = 6
DT_STRSZ = 10
DT_SYMENT = 11

# Symbol types (ELF64_ST_TYPE)
STT_NOTYPE = 0
STT_OBJECT = 1
STT_FUNC = 2

# Symbol binding
STB_GLOBAL = 1
STB_WEAK = 2


def vaddr_to_offset(vaddr, loads):
    for seg in loads:
        if seg["vaddr"] <= vaddr < seg["vaddr"] + seg["filesz"]:
            return seg["offset"] + (vaddr - seg["vaddr"])
    raise ValueError(f"0x{vaddr:x} not in any LOAD segment")


def main():
    if len(sys.argv) != 3:
        print(__doc__)
        sys.exit(1)

    in_path, out_path = sys.argv[1], sys.argv[2]

    with open(in_path, "rb") as f:
        data = f.read()

    assert data[:4] == b"\x7fELF" and data[4] == 2 and data[5] == 1

    e_phoff = struct.unpack_from("<Q", data, 32)[0]
    e_phentsize = struct.unpack_from("<H", data, 54)[0]
    e_phnum = struct.unpack_from("<H", data, 56)[0]

    loads = []
    dyn_off = dyn_sz = 0
    for i in range(e_phnum):
        base = e_phoff + i * e_phentsize
        p_type = struct.unpack_from("<I", data, base)[0]
        p_offset = struct.unpack_from("<Q", data, base + 8)[0]
        p_vaddr = struct.unpack_from("<Q", data, base + 16)[0]
        p_filesz = struct.unpack_from("<Q", data, base + 32)[0]
        if p_type == PT_LOAD:
            loads.append({"offset": p_offset, "vaddr": p_vaddr, "filesz": p_filesz})
        elif p_type == PT_DYNAMIC:
            dyn_off, dyn_sz = p_offset, p_filesz

    # Parse DYNAMIC entries
    symtab_va = strtab_va = strsz = hash_va = syment = 0
    j = 0
    while j < dyn_sz:
        tag = struct.unpack_from("<q", data, dyn_off + j)[0]
        val = struct.unpack_from("<Q", data, dyn_off + j + 8)[0]
        if tag == DT_SYMTAB: symtab_va = val
        elif tag == DT_STRTAB: strtab_va = val
        elif tag == DT_STRSZ: strsz = val
        elif tag == DT_HASH: hash_va = val
        elif tag == DT_SYMENT: syment = val
        elif tag == DT_NULL: break
        j += 16

    syment = syment or 24
    symtab_off = vaddr_to_offset(symtab_va, loads)
    strtab_off = vaddr_to_offset(strtab_va, loads)
    hash_off = vaddr_to_offset(hash_va, loads)

    nchain = struct.unpack_from("<I", data, hash_off + 4)[0]

    # Extract symbols
    funcs = []
    objects = []
    for i in range(nchain):
        ent = symtab_off + i * syment
        st_name = struct.unpack_from("<I", data, ent)[0]
        st_info = data[ent + 4]
        st_shndx = struct.unpack_from("<H", data, ent + 6)[0]
        st_size = struct.unpack_from("<Q", data, ent + 16)[0]

        bind = st_info >> 4
        stype = st_info & 0xF

        if st_shndx == 0:  # SHN_UNDEF
            continue
        if bind not in (STB_GLOBAL, STB_WEAK):
            continue

        # Read name from strtab
        name_off = strtab_off + st_name
        end = data.index(b"\x00", name_off)
        name = data[name_off:end].decode("ascii", errors="replace")

        if not name or name.startswith("_"):
            continue  # skip internal/reserved symbols
        if not name.replace("_", "").isalnum():
            continue  # skip names with dots or other non-C-identifier chars

        if stype == STT_FUNC:
            funcs.append(name)
        elif stype == STT_OBJECT:
            objects.append((name, st_size))

    # Generate C source
    lines = [
        "/* Auto-generated link stubs — do not edit. */",
        "/* Provides empty symbol definitions so the static linker can resolve",
        "   references to the vendor library whose ELF section headers are stripped. */",
        "",
    ]

    for name in sorted(set(funcs)):
        lines.append(f"void {name}(void) {{}}")

    for name, size in sorted(set(objects)):
        size = max(size, 8)  # minimum 8 bytes
        lines.append(f"char {name}[{size}];")

    lines.append("")

    Path(out_path).write_text("\n".join(lines))
    print(f"Generated {len(funcs)} function stubs and "
          f"{len(objects)} object stubs -> {out_path}")


if __name__ == "__main__":
    main()
