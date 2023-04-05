########################################
#~ définitions
########################################

# Executables à générer
BIN=bin/exo1 bin/rdv bin/traitementImage bin/kohlanta

# liste des fichiers sources 
SRCS0=exo1.c
SRCS1=rdv.c
SRCS2=traitementImage.c
SRCS3=kohlanta.c

# List of object files, excluding calculC.o
OBJS := $(filter-out obj/calculC.o, $(wildcard obj/*.o))

default: $(BIN)

########################################
#~ regles pour l'executable
########################################

obj/%.o: %.c
	gcc -Wall -Iinclude -c $< -o $@

bin/exo1: $(SRCS0:%.c=obj/%.o)
	gcc -o $@ obj/calculC.o $+

bin/rdv: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ obj/calculC.o $+

bin/traitementImage: $(SRCS2:%.c=obj/%.o)
	gcc -o $@ obj/calculC.o $+

bin/kohlanta: $(SRCS3:%.c=obj/%.o)
	gcc -o $@ $+

# Rule to clean all object files except calculC.o
clean:
	rm -f $(BIN) $(OBJS) *~