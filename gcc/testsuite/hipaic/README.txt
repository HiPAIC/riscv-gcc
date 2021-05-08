Suppose you have built the toolchain and the executables are at $DIR (e.g. ~/riscv/bin).

How to test addmul* files:

 $DIR/riscv32-unknown-elf-gcc -mhipaic-x-arith -O2 -S addmul1.c
Look at the output addmul1.s: the "f, @function" body should have only two instructions "mul a0, a0, a1" and "ret".
You can also replace -S with "-c" (only compile) and it should output a .o file.
Now remove the -mhipaic-x-arith flag, gcc won't be compiled with HiPAIC extended arithmetic, then the __builtin_riscv_hipaic_mulsi function won't be considered a builtin at all. The ASM will become a call to an implicit function.


 $DIR/riscv32-unknown-elf-g++ -mhipaic-x-arith -O2 -o addmul2 addmul2.cc
This should produce an ELF executable ./addmul2 which cannot be run directly. We have to use simulator.
 $DIR/riscv32-unknown-elf-run  ./addmul2
You can input different numbers and test. Some useful flags to get more debug info:  --trace-linenum --trace-insn --trace-debug --debug-insn --debug (find more by running /riscv32-unknown-elf-run --help)


 $DIR/riscv32-unknown-elf-g++ -mhipaic-x-arith -O2 -S addmul2.cc
Look at addmul2.s you can see that there's no call to f(), all inlined to be just mul, and the two mul for the first printf() are optimized to be just one. If you make "f()" to be "static" then it won't generate a function f at all.

 $DIR/riscv32-unknown-elf-gcc -o addmul2_b ./addmul2.s && $DIR/riscv32-unknown-elf-run  ./addmul2_b
This verifies that from the ASM output we can still build an executable that works.
 $DIR/riscv32-unknown-elf-gcc -mhipaic-x-arith -c addmul2.cc && $DIR/riscv32-unknown-elf-gcc -o addmul2_c ./addmul2.o && $DIR/riscv32-unknown-elf-run  ./addmul2_c
This verifies that from the Object file output we can still build an executable that works.
