#include <Zen.h>

const char *amd64_gcc = "../Toolchain/bin/x86_64-elf-gcc";
const char *nasm = "../Toolchain/bin/nasm";

const char *cflags = "-fno-pic -no-pie -mcmodel=large -ffreestanding -I. -ILibC -fomit-frame-pointer -mno-80387 -mno-mmx -mno-3dnow -mno-sse -mno-sse2 -fno-stack-protector -pipe -mno-red-zone";

void CompileFile(char *dir, char *f)
{
	Compile("%s -c %s%s.c -o ../bin/objects/%s.o %s", amd64_gcc, dir, f, f, cflags);
}

void CompileFile2(char *dir, char *f)
{
	Compile("%s -felf64 %s.asm -o ../bin/objects/%s.o", nasm, f, f);
}

void Clean()
{
	Execute("rm ../bin/objects/*.o", 0);
}

char *cflags2 = "-TLinker.ld -static -no-pie -fno-pic -nostdlib -z max-page-size=0x1000";

int main(int argc, char **argv)
{
	SetTarget("clean", Clean)

	ForEach("./", "c", CompileFile);
	ForEach("./", "asm", CompileFile2);
	ForEach("./Common/", "c", CompileFile);
	ForEach("./Core/", "c", CompileFile);
	ForEach("./LibC/", "c", CompileFile);
	ForEach("./Drivers/", "c", CompileFile);
	ForEach("./Scheduler/", "c", CompileFile);
	ForEach("./Graphics/", "c", CompileFile);

	Compile("%s %s ../bin/objects/*.o -o ./JOYOS.BIN", amd64_gcc, cflags2);
}
