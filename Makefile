INCLUDE_DIRS=-I SDL/include -I headers/
LIB_PATHS=-L SDL/lib
LIBS=-l SDL3


CC=gcc
CFLAGS=$(INCLUDE_DIRS) $(LIB_PATHS) $(LIBS)
SOURCE_FILES=source/main.c
OUTPUT_NAME=test.exe


build:
	$(CC) -o $(OUTPUT_NAME) $(SOURCE_FILES) $(CFLAGS)

clean:
	del $(OUTPUT_NAME)