.POSIX:

TARGET = debug
OUTDIR = out
SRCDIR = src
BIN = main

PREFIX = /usr
DESTDIR =

ZIG = zig
AR = ${ZIG} ar
CC = ${ZIG} cc
CFLAGS_RELEASE = -O3 -g0 -Werror
# NOTE: -fsanitize=address doesn't work with zig cc yet, add later
CFLAGS_DEBUG = -O0 -ggdb -g3 -DDEBUG -fno-omit-frame-pointer
CFLAGS = -Wall -Wextra -std=gnu18 -Isrc
EXTRA_CFLAGS =
LDFLAGS =
LDLIBS =

ifeq (${TARGET}, debug)
	CFLAGS += ${CFLAGS_DEBUG}
else ifeq (${TARGET}, release)
	CFLAGS += ${CFLAGS_RELEASE}
else
	ERR = $(error invalid TARGET '${TARGET}')
endif

HEADERS := $(wildcard ${SRCDIR}/*.h)
SOURCES := $(wildcard ${SRCDIR}/*.c)
OBJECTS := $(patsubst ${SRCDIR}/%.c,${OUTDIR}/${TARGET}/obj/%.o,${SOURCES})

.SUFFIXES:
.PHONY: all clean err install
.PRECIOUS: ${OBJECTS}

.DEFAULT_GOAL = all

install: all
	mkdir -p $(foreach d,include bin,${DESTDIR}${PREFIX}/${d})
	install -m644 -p ${OUTDIR}/${TARGET}/bin/${BIN} ${DESTDIR}${PREFIX}/bin
	install -m644 -p source/*.h ${DESTDIR}${PREFIX}/include

err: ${ERR}

all: ${OUTDIR}/${TARGET}/bin/${BIN}

${OUTDIR}/${TARGET}/bin/${BIN}: ${OBJECTS}
	@mkdir -p $(dir $@)
	${CC} ${OBJECTS} ${LDLIBS} -o $@
	@# ${AR} rcs $@ $^
ifeq (${TARGET}, release)
	@# strip -g $@
	strip $@
endif

${OUTDIR}/${TARGET}/obj/%.o: ${SRCDIR}/%.c ${HEADERS}
	@mkdir -p $(dir $@)
	${CC} ${CFLAGS} ${EXTRA_CFLAGS} -c $< ${LDFLAGS} ${LDLIBS} -o $@

clean:
	-rm -rf ${OUTDIR}
