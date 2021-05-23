Suppose you have built the toolchain and the executables are at $DIR (e.g. ~/riscv/bin).

How to test multiply* files:
 $DIR/riscv32-unknown-elf-gcc -mtune=hipaic-0 -mhipaic-x-arith -O2 -S multiply1.c
This generates multiply1.s with source symbol info. Look at it, the bar() function must call 6 instances of hp.savrnd, hp.ldopx, hp.mul with correct params.
Make sure none of them are optimized out, and the order is correct. multiply1.s.gold is a golden .s output with comments (starting with #) to explain control flow.
Script below compares them and prints OK if good. sed matches comment lines # with optional starting white space and delete them; diff - file compares stdin with file.
 sed '/^\s*#/d' multiply1.s.gold | diff - multiply1.s && echo "OK! gold matches generated .s"

 $DIR/riscv32-unknown-elf-gcc -mtune=hipaic-0 -mhipaic-x-arith -O2 -c multiply1.c

 $DIR/riscv32-unknown-elf-gcc -mtune=hipaic-0 -mhipaic-x-arith -O2 -o multiply2 multiply2.c && $DIR/riscv32-unknown-elf-run ./multiply2
Should prompt you to input two ints. "6000 70000" should output "123302769".
 $DIR/riscv32-unknown-elf-run --trace-insn --trace-reg ./multiply2 2> multiply2.trace.txt
This shows the detailed trace. You can check the hp.grnd, hp.lopx, hp.mul instructions.

 $DIR/riscv32-unknown-elf-gcc -mtune=hipaic-0 -g -mhipaic-x-arith -S multiply2.c && $DIR/riscv32-unknown-elf-as -o multiply2_b.o multiply2.s && $DIR/riscv32-unknown-elf-gcc -o multiply2_b multiply2_b.o && $DIR/riscv32-unknown-elf-run ./multiply2_b
This first generate multiply2.s, then generate multiply2_b.o, then link it to genereate multiply2_b. Test all tools are OK.
Compare the result of ./multiply2_b with ./multiply2 on several inputs to make sure they match:
 INPUT="6000 70000"; echo $INPUT | $DIR/riscv32-unknown-elf-run ./multiply2; echo $INPUT | $DIR/riscv32-unknown-elf-run ./multiply2_b  # Output: 123302769
 INPUT="0 0"; echo $INPUT | $DIR/riscv32-unknown-elf-run ./multiply2; echo $INPUT | $DIR/riscv32-unknown-elf-run ./multiply2_b  # Output: 3564
 INPUT="1 1"; echo $INPUT | $DIR/riscv32-unknown-elf-run ./multiply2; echo $INPUT | $DIR/riscv32-unknown-elf-run ./multiply2_b  # Output: 3564
 INPUT="-56 189"; echo $INPUT | $DIR/riscv32-unknown-elf-run ./multiply2; echo $INPUT | $DIR/riscv32-unknown-elf-run ./multiply2_b  # Output: 3378
 INPUT="77 654321"; echo $INPUT | $DIR/riscv32-unknown-elf-run ./multiply2; echo $INPUT | $DIR/riscv32-unknown-elf-run ./multiply2_b  # Output: 967944
 INPUT="888 -10000"; echo $INPUT | $DIR/riscv32-unknown-elf-run ./multiply2; echo $INPUT | $DIR/riscv32-unknown-elf-run ./multiply2_b  # Output: -39748908

 $DIR/riscv32-unknown-elf-gcc -mtune=hipaic-0 -mhipaic-x-arith -O2 -S multiply3.c
Verify that after "hp.mul	a4,a4,a7", two instructions that do not depend on a4 are inserted before a4 is used. This is to avoid stall.
 sed '/^\s*#/d' multiply3.s.gold | diff - multiply3.s && echo "OK! gold matches generated .s"
 If we do not tune for hipaic, but instead execute $DIR/riscv32-unknown-elf-gcc -mtune=rocket -mhipaic-x-arith -O2 -S multiply3.c
 We may observe that after hp.mul, not enough data-independent instructions were inserted, and there will be stall.

 $DIR/riscv32-unknown-elf-gcc -mtune=hipaic-0 -mhipaic-x-arith -O2 -o multiply4 multiply4.c && $DIR/riscv32-unknown-elf-run ./multiply4
The output should be:
u[0]=0 v[0]=0 result[0]=9
u[1]=567 v[1]=1234 result[1]=28
u[2]=1134 v[2]=2468 result[2]=68
u[3]=1701 v[3]=3702 result[3]=129
u[4]=2268 v[4]=4936 result[4]=211
u[5]=2835 v[5]=6170 result[5]=314
u[6]=3402 v[6]=7404 result[6]=437
u[7]=3969 v[7]=8638 result[7]=582

 $DIR/riscv32-unknown-elf-gcc -mtune=hipaic-0 -mhipaic-x-arith -O2 -o bitand2 bitand2.c && $DIR/riscv32-unknown-elf-run ./bitand2
Input two int32 u v:
6000 70000
bar(u, v)= 59911280

 $DIR/riscv32-unknown-elf-run --trace-insn --trace-reg ./bitand2 2> bitand2.trace.txt


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
Look at addmul2.s you can see that there's no call to f(), all inlined to be just mul, and the two mul for the first printf() are optimized to be just one. If you make "f()" to be "static" then it won't generate a function f at all. NOTE that "x*3" and "y*2" are implemented by addi and slli, because the ISA is rv32i, but we can use mul by __builtin.

 $DIR/riscv32-unknown-elf-gcc -o addmul2_b ./addmul2.s && $DIR/riscv32-unknown-elf-run  ./addmul2_b
This verifies that from the ASM output we can still build an executable that works.
 $DIR/riscv32-unknown-elf-gcc -mhipaic-x-arith -O2 -c addmul2.cc && $DIR/riscv32-unknown-elf-gcc -o addmul2_c ./addmul2.o && $DIR/riscv32-unknown-elf-run  ./addmul2_c
This verifies that from the Object file output we can still build an executable that works.

 $DIR/riscv32-unknown-elf-objdump -d ./addmul2.o
should disassemble the obj file and output readable ASM. part of the content should resemble addmul2.s

 $DIR/riscv32-unknown-elf-gdb ./addmul2
GDB should be able to load the ELF
 (gdb) disassemble _Z1fjj
 (gdb) disassemble main
should be able to disassemble the f(x,y) and main() functions, contents resembles addmul2.s and objdump result

