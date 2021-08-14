##
##  Makefile
##

CFLAGS = -Wall -W -pipe -O2 -s

all: clean release

release: posix windows
	$(STRIP_POSIX)   --strip-unneeded -R .comment *.bin
	$(STRIP_WINDOWS) --strip-unneeded -R .comment *.exe
	chmod 600 Makefile *.c *.exe
	chmod 700 *.bin

clean:
	rm -f core *.bin *.exe

##
## POSIX
##

CC_POSIX       = gcc
STRIP_POSIX    = strip
CFLAGS_POSIX   = $(CFLAGS)

posix:
	${CC_POSIX} ${CFLAGS_POSIX} -o passgen01.bin passgen01.c
	${CC_POSIX} ${CFLAGS_POSIX} -o passgen02.bin passgen02.c -lcrypto
	${CC_POSIX} ${CFLAGS_POSIX} -o passgen03.bin passgen03.c
	${CC_POSIX} ${CFLAGS_POSIX} -o passgen04.bin passgen04.c
	${CC_POSIX} ${CFLAGS_POSIX} -o passgen05.bin passgen05.c -lcrypto
	${CC_POSIX} ${CFLAGS_POSIX} -o passgen06.bin passgen06.c -lcrypto
	${CC_POSIX} ${CFLAGS_POSIX} -o passgen07.bin passgen07.c -lcrypto
	${CC_POSIX} ${CFLAGS_POSIX} -o passgen08.bin passgen08.c -lcrypto
	${CC_POSIX} ${CFLAGS_POSIX} -o passgen09.bin passgen09.c
	${CC_POSIX} ${CFLAGS_POSIX} -o passgen10.bin passgen10.c
	${CC_POSIX} ${CFLAGS_POSIX} -o passgen11.bin passgen11.c
	${CC_POSIX} ${CFLAGS_POSIX} -o passgen12.bin passgen12.c
	${CC_POSIX} ${CFLAGS_POSIX} -o passgen13.bin passgen13.c
	${CC_POSIX} ${CFLAGS_POSIX} -o passgen14.bin passgen14.c
	${CC_POSIX} ${CFLAGS_POSIX} -o passgen15.bin passgen15.c -lcrypto
	${CC_POSIX} ${CFLAGS_POSIX} -o passgen16.bin passgen16.c -lcrypto

