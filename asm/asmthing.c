#include <unistd.h>
#include <stdlib.h>

#define _ASM(N,NE, C) asm(".global "#N"\n"#N":\n\t"C".global "#NE"\n"#NE":\n\t"); extern char N[], NE[];
#define ASM(N, C) _ASM(N,end_##N, C)

ASM(test,
"nop\n\t"
"int3\n\t"
);

int main() {
	write(1, test, end_test-test);
}
