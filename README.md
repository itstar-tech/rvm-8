# RVM-8 (Retro Virtual Modular Console)

The **RVM-8 (Retro Virtual Modular Console)** project is an advanced architectural proposal to build a high-performance emulator using a hybrid design that combines **C** and **Rust**. The goal is to leverage the speed and low-level control of C for hardware simulation, while using the safety and modern ecosystem of Rust to manage the interface with the operating system and the user.

## Project Pillars

### 1. Architectural Philosophy: Role Specialization
The project is based on a strict separation of responsibilities to optimize both performance and safety:

*   **The Core in C (Simulated Hardware):** C is chosen to simulate the CPU and memory due to its ability to handle pointers and memory layouts directly and predictably. This core is "unsafe" by nature but isolated; it contains the machine state (registers, RAM, VRAM) and executes the *Fetch-Decode-Execute* instruction cycle.
*   **The Host in Rust (Control and Safety):** Rust acts as the emulator's "operating system". Its function is to encapsulate the insecurity of the C core through a safe interface (RAII), managing the window, user input, and time synchronization.

### 2. Virtual Hardware Technical Specifications
The RVM-8 defines an 8-bit architecture designed for pedagogical and efficiency purposes:
*   **ISA (Instruction Set Architecture):** Uses fixed-length 8-bit instructions to facilitate decoding. Although data is 8-bit, the Program Counter (PC) is 16-bit, allowing addressing of up to 64 KB of memory.
*   **Opcode Execution:** The C core uses a massive `switch` statement to dispatch opcodes. Although theoretically this could affect *branch prediction*, modern compilers optimize this by generating **jump tables**, resulting in extremely efficient instruction dispatch.

### 3. The FFI Bridge (Foreign Function Interface)
Communication between Rust and C is the critical component of the design:
*   **Binary Compatibility (`#[repr(C)]`):** To share CPU state between both languages without copying data, identical structures are defined on both sides. In Rust, it is mandatory to use the `#[repr(C)]` attribute to ensure memory aligns exactly the same as in C.
*   **Automation with `cbindgen`:** To avoid manual errors when writing header files (.h), the project uses a `build.rs` script that invokes the `cbindgen` tool. This automatically generates C definitions based on Rust code during compilation.
*   **Cross-Compilation with Zig:** Since mixing C and Rust complicates compilation for other platforms (such as WebAssembly or Linux from Windows), the project suggests using the **Zig** compiler (`cargo-zigbuild`) as a universal *toolchain* to simplify this process.

### 4. Critical Subsystems: Graphics and Timing
The project solves two of the most common problems in emulation:
*   **Rendering (Logical PPU):** The C core writes color indices to a memory buffer (VRAM). Rust reads this buffer through a raw pointer and uses the **`pixels`** library (based on `wgpu`) to perform palette conversion and render the image on the GPU. This allows scaling low-resolution graphics (e.g., 64x64) to 4K screens while maintaining sharpness and performance.
*   **Synchronization (Fixed Timestep):** To prevent the emulator from running too fast or too slow, Rust implements a "fixed timestep" loop. It accumulates the actual elapsed time and executes the virtual CPU (the C core) in exact increments (e.g., 1/60th of a second) until it catches up to real time, guaranteeing a deterministic clock speed.
