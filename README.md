# interprocess-communication-with-multiple-processes.

Operating Systems Windows.

Reads a special market.txt.

Every Child has 4 threads which holds different number of products of market.txt.

Child processes send their informations to parent process via unnamed pipe.

Calculations done by parent process.

REQUIREMENTS:

1) Add market.txt's path to read function.
2) Add _CRT_SECURE_NO_WARNINGS to proprocessors of parent.c and child.c
3) Set Parent as startup project
