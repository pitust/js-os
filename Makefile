METALKIT_LIB = metalkit/lib
TARGET = js-os.img
LIB_MODULES = console console_vga intr bios timer keyboard
APP_SOURCES = out/js.o $(wildcard src/*.c)
CC := gcc
ifneq (,$(wildcard $(METALKIT_LIB)/Makefile.rules))
include $(METALKIT_LIB)/Makefile.rules
endif
CFLAGS += -DDEBUG=1
JSCFLAGS := $(subst -Wall -Werror,,$(CFLAGS)) -Isrc
out/main.c: out/main.js
	@echo "ES3 -> C99"
	@ts2c out/main.js
	@echo "PATCH C99"
	@echo "#define main _2nd_main___" >out/main.head.c
	@cat out/main.head.c out/main.c >out/main2.c
	@sed "s/#include <stdlib.h>//g" >out/main3.c <out/main2.c
	@sed "s/#include <assert.h>//g" >out/main4.c <out/main3.c
	@sed "s/#include <stdio.h>/#include <libk.h>/g" >out/main.c <out/main4.c
	@node src/cpatch
out/main.js: $(wildcard src/js/*)
	@echo "ESnext -> ES2015"
	@node src/build
out/js.o: out/main.c
	@echo "C99 -> ELF"
	@$(CC) $(JSCFLAGS) -c out/main.c -o out/js.o

init:
	@rm -rf metalkit
	@git clone https://github.com/scanlime/metalkit metalkit
	@npm install
	@patch metalkit/lib/types.h < src/types.diff
	@echo "GCCVER := $(shell gcc --version | egrep -o "[0-9.]{1,}" | head -n 1)" > tmp1
	@echo 'CFLAGS := -m32 -ffreestanding -nostdinc -fno-stack-protector -Isrc -I$(METALKIT_LIB) $(addprefix -I,$(wildcard /usr/lib/gcc/*-linux-gnu/7.4.0/include))' > tmp3
	@cat tmp1 tmp3 metalkit/lib/Makefile.rules | sed "s/\*.o/out\/*/g" >tmp2
	@cat tmp2 | sed 's/CFLAGS := -m32 -ffreestanding -nostdinc -fno-stack-protector -I$$(METALKIT_LIB)//g' >tmp1
	@cat tmp1 >metalkit/lib/Makefile.rules
	@rm tmp1 tmp2 tmp3
	@mkdir -p out
sizeof:
	@node src/sizeof
run: js-os.img
	qemu-system-i386 -fda js-os.img -hda fakeDrive.txt -curses
.PHONY: target clean sizeprof listing init sizeof run