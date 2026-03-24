ALEAE_HOME = "./"
CC         = g++
LIBS       = -lm
INCLUDEDIRS = -I$(ALEAE_HOME)
CFLAGS     = -O3 $(INCLUDEDIRS)
CPPFLAGS   = $(CFLAGS)
VPATH      = $(ALEAE_HOME)

ALEAE_HDRS = aleae.h 
ALEAE_OBJS = aleae_io.o aleae_main.o aleae_stoch.o

all: aleae

$(ALEAE_OBJS): $(ALEAE_HDRS) 

aleae: $(ALEAE_OBJS) 
	$(CC) $(CFLAGS) $(ALEAE_OBJS) -o $@

clean: 
	rm -f *.o aleae
