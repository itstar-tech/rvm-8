fn main() {
    cc::Build::new()
        .file("../kernel/cpu.c")
        .file("../kernel/bus.c")
        .file("../kernel/opcodes.c")
        .compile("rvm8_kernel");

    println!("cargo:rerun-if-changed=../kernel/cpu.h");
}