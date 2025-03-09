# Can use: DEBUG=1 | TRACE=1

INCLUDE_DIRS=-I SDL/include -I headers/
LIB_PATHS=-L SDL/lib
LIBS=-l SDL3


CC=gcc
CFLAGS=$(INCLUDE_DIRS) $(LIB_PATHS) $(LIBS)
SOURCE_FILES=source/*.c
OUTPUT_NAME=chip8emu.exe

ifdef DEBUG
CFLAGS += -DDEBUG -g
else
endif

ifdef TRACE
CFLAGS += -DTRACE
else
endif

build:
	$(CC) -o $(OUTPUT_NAME) $(SOURCE_FILES) $(CFLAGS)

clean:
	del $(OUTPUT_NAME)