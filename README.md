# TEUOS
This is a self-written 32-bit protected mode kernel for the x86 architecture, written as a pet project with the desire to explore and implement operating system principles in great detail.

### The project is abandoned because I got bored of writing in C and decided to switch to Rust.<br>There is an actual Rust OS project among my repositories.

## How to run it in a virtual machine
All you need to do is download *bootablekernel.iso* and run it in any virtual machine.  
For example in qemu:
`qemu-system-i386 bootablekernel.iso`

## How to build
To build the kernel you need to have:
* **GCC Cross-Compiler** for the **i686-elf** platform. It is better to [**build**](https://wiki.osdev.org/GCC_Cross-Compiler) it yourself, but you can download a prebuilt cross-compiler [**here**](https://wiki.osdev.org/GCC_Cross-Compiler#Prebuilt_Toolchains). Finally, you should have **i686-elf-gcc** in your system.
* **GRUB.** You should have the **grub-mkrescue** in your system because it is used to create a bootable *.iso* file. 
* *Optional.* **qemu.** To easily run the kernel in a virtual machine.
#### If you have all the utilities you need, there are only a few steps to do: 
```
git clone https://github.com/mrjbom/TEUOS.git  
make
```
Now you have *.iso* file that you can run in a virtual machine.  
`qemu-system-i386 bootablekernel.iso`
