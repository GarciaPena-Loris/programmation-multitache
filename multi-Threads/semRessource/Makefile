########################################
#~ définitions
########################################

BIN=bin/init bin/supprimeSem bin/semRessource

# liste des fichiers sources
SRCS0=init.c
SRCS1=supprimeSem.c
SRCS2=semRessource.c

default: $(BIN)

########################################
#~ regles pour l'executable
########################################

obj/%.o: %.c
	gcc -Wall -Iinclude -c $< -o $@

bin/init: $(SRCS0:%.c=obj/%.o)
	gcc -o $@ $+

bin/supprimeSem: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+

bin/semRessource: $(SRCS2:%.c=obj/%.o)
	gcc -o $@ $+


clean:
	rm -f $(BIN) obj/*.o *~
