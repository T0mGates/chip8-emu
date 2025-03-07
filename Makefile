# `make` or `make DEBUG=1`

INCLUDE_DIRS=-I SDL/include -I headers/
LIB_PATHS=-L SDL/lib
LIBS=-l SDL3


CC=gcc
CFLAGS=$(INCLUDE_DIRS) $(LIB_PATHS) $(LIBS)
SOURCE_FILES=source/*.c
OUTPUT_NAME=test.exe

ifdef DEBUG
CFLAGS += -DDEBUG -g
else
endif

build:
	$(CC) -o $(OUTPUT_NAME) $(SOURCE_FILES) $(CFLAGS)

clean:
	del $(OUTPUT_NAME)