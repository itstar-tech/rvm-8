# rvm-8/kernel

A small partial implementation of the rvm-8 emulator kernel.

Author: foxomax
License: MIT (SPDX: MIT)

This repository contains the kernel components for the rvm-8 emulator. The project is fully open-source under the MIT license.

Relevant files
- `cpu.h` - definition of the `CPU` structure and the public interface.
- `cpu.c` - partial implementation of the CPU (flags, state). Additional logic and opcodes are pending.
- `bus.c` - memory bus implementation (currently a stub).
- `opcodes.c` - opcode tables/handlers (currently a stub).
- `Makefile` - rules to build the `libkernel.a` static library.
- `tests/` - unit tests (to be implemented).

Building

From this directory (`kernel`) run:

```bash
make
```

This will compile the sources and produce `libkernel.a`.

Notes

- All source files include SPDX headers and copyright by `foxomax`.
- The project is in an early stage; many files are skeletons and will be expanded.

Contributing

Contributions are welcome. Please open an issue or a pull request with changes or improvements.

License

This project is released under the MIT License. See the `LICENSE` file for the full text.
