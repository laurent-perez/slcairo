MODNAME = cairo
LFLAGS = `pkg-config --libs cairo`
IFLAGS = `pkg-config --cflags cairo`
MODULE = $(MODNAME)-module.so
SRC = cairo-module.c
SLANGINC = `pkg-config --libs slang`
SLANGLIB = `pkg-config --libs slang`
CC = gcc
# CFLAGS = -g -O2 -fno-strength-reduce -fno-strict-aliasing
CFLAGS = -O2 -fno-strength-reduce -fno-strict-aliasing
SO_CFLAGS = $(CFLAGS) -fPIC -I. $(SLANGINC) $(IFLAGS)
CC_SHARED = $(CC) $(CFLAGS) -shared -fPIC
C_COMPILE = $(CC) $(SO_CFLAGS) -c
DL_LIB = -ldl -lm -lc
LINK = $(CC) -shared -Wl,-soname,$(MODULE)
OBJ = $(SRC:.c=.o)

all: $(MODULE)

$(MODULE): $(OBJ)
	@echo
	$(LINK) -o $@ $(OBJ) $(LFLAGS) $(SLANGLIB) $(DL_LIB)

$(OBJ): $(SRC)
	@echo
	$(C_COMPILE) $(CFLAGS) $(SRC)

clean:
	rm $(OBJ)
