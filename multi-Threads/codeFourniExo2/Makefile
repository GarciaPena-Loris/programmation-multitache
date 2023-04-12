########################################
#~ définitions
########################################

BIN=bin/semInit bin/suppr bin/semRdv

# liste des fichiers sources
SRCS0=semInit.c
SRCS1=supprimeSem.c
SRCS2=semRdv.c

default: $(BIN)

########################################
#~ regles pour l'executable
########################################

obj/%.o: %.c
	gcc -Wall -Iinclude -c $< -o $@

bin/semInit: $(SRCS0:%.c=obj/%.o)
	gcc -o $@ $+

bin/suppr: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+

bin/semRdv: $(SRCS2:%.c=obj/%.o)
	gcc -o $@ $+


clean:
	rm -f $(BIN) obj/*.o *~
