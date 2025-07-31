# LBYARCH-MCO
Bukuhan, Abram Aki R.
Ferrer, Angel June E.

How to Run:

 1. Run Command Prompt as administrator
 2. Run these commands:
        nasm -f win64 kernel.asm -o kernel.o
        gcc main.c kernel.o -o result.exe -lm (any .exe name will do)
        ./result.exe 

==========      COMPARATIVE EXECUTION TIME      ==========
_____________________________________________________________
VECTOR SIZE (n)     |   C KERNEL TIME   |   ASM KERNEL TIME |
                    |                   |                   |
2^20 = 1048576      |   8.548 ms        |   1.219 ms        |
                    |                   |                   |
2^24 = 16777216     |   136.249 ms	    |   19.902 ms       |
                    |                   |                   |
2^28 = 268435456    |   2223.328 ms 	|   356.164 ms      |
____________________|___________________|___________________|

Analysis:
The x86-64 assembly kernel performs significantly faster than the C kernel across all tested vector sizes. This is because assembly code is closer to the processor, allowing for more direct control and fewer unnecessary instructions. As a result, it avoids the overhead that C code usually has. In our tests, the assembly version ran around 6 to 7 times faster than the C version on average. Despite the difference in time, both the C and assembly versions produced the exact same output, verifying the correctness of the C version that has been served as the "sanity check answer key" for validating the accuracy of the x86-64 version.