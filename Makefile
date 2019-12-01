METALKIT_LIB = metalkit/lib
TARGET = tsos.img
LIB_MODULES = console console_vga intr bios
APP_SOURCES = out/js.o $(wildcard src/*.c)

ifneq (,$(wildcard $(METALKIT_LIB)/Makefile.rules))
include $(METALKIT_LIB)/Makefile.rules
endif
JSCFLAGS := $(subst -Wall -Werror,,$(CFLAGS)) -Isrc
out/main.js: out/babeld.js
	@echo "ES2015 -> ES3"
	@tsc out/babeld.js --allowJs --outFile out/main.js
out/main.c: out/main.js
	@echo "ES3 -> C99"
	@ts2c out/main.js
	@echo "PATCH C99"
	@echo "#define main _2nd_main___" >out/main.head.c
	@cat out/main.head.c out/main.c >out/main2.c
	@sed "s/#include <stdlib.h>//g" >out/main3.c <out/main2.c
	@sed "s/#include <assert.h>//g" >out/main4.c <out/main3.c
	@sed "s/#include <stdio.h>/#include <libk.h>/g" >out/main.c <out/main4.c
out/babeld.js: $(wildcard src/js/*)
	@echo "ESnext -> ES2015"
	@node src/build
out/js.o: out/main.c
	@echo "C99 -> ELF"
	@$(CC) $(JSCFLAGS) -c out/main.c -o out/js.o

init:
	@git clone https://github.com/scanlime/metalkit metalkit
	@npm install
	@patch metalkit/lib/types.h < src/types.diff
	@patch metalkit/lib/Makefile.rules < src/Makefile.diff
	@mkdir -p out
.PHONY: target clean sizeprof listing init