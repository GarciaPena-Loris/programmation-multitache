########################################
#~ définitions
########################################

BIN=bin/clientTCPfonctions bin/serveurTCPfonctions


# liste des fichiers sources 
SRCS0=clientTCPfonctions.c
SRCS1=serveurTCPfonctions.c

default: $(BIN)

########################################
#~ regles pour l'executable
########################################

obj/%.o: %.c
	gcc -Wall -Iinclude -c $< -o $@

bin/clientTCPfonctions: $(SRCS0:%.c=obj/%.o)
	gcc -o $@ $+

bin/serveurTCPfonctions: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+

clean:
	rm -f $(BIN) obj/*.o *~
