## LegacyOS
ROM BIOS-based MBR Operating System Project

The project is based on `mint64` project

### Build Instructions
This project is designed to be build for the i386 architecture, specifically targeting low-level systems programming.

To compile and build the project, you will need to have several tools and dependencies installed on your system.

#### Prerequisites
Before you begin, ensure you have the following tools installed:

* **GCC**: The GNU Compiler Collection, with support for compiling 32-bit(`-m32`) code. You can install GCC on most Linux distributions via your package manager. For Debian-based systems, use `sudo apt-get install gcc-multilib`.
* **NASM**: The Netwide Assembler, a popular assembler for x86 architecture. Install it using your package manager, for example, `sudo apt-get install nasm` on Debian-based systems.
* **GNU LD**: The GNU Linker, which should come with the binutils package. Ensure you have the 32-bit version capable of handling `elf_i386` format. For Debian-based systems, `sudo apt-get install binutils`.
* **GNU Objcopy**: Part of the GNU Binutils for converting and copying object files; used here to create a binary from the ELF file.

#### Build Process
To build the project, follow these steps:

1. **Clone the Repository**: To clone the repository, execute the following command on your local machine:
```bash
git clone https://github.com/DokaDev/LegacyOS.git
```

2. **Navigate to the Project Directory**: Change to the project directory using the command below:
```bash
cd LegacyOS
```

3. **Compile and Build Project**: After you clone the repository, just run `make` to build it. :)

The script contains `clean` label that automatically cleans the build temporary files before the build process begins.
```bash
make
```