########################################
#~ définitions
########################################

# Executables à générer
BIN=bin/clientTCP  bin/serveurTCP bin/clientTCPfull bin/serveurTCPfull bin/clientTCPfonctions bin/serveurTCPfonctions

# liste des fichiers sources 
SRCS0=clientTCP.c
SRCS1=serveurTCP.c
SRCS2=clientTCPfull.c
SRCS3=serveurTCPfull.c
SRCS4=clientTCPfonctions.c
SRCS5=serveurTCPfonctions.c

default: $(BIN)

########################################
#~ regles pour l'executable
########################################

obj/%.o: %.c
	gcc -Wall -Iinclude -c $< -o $@

bin/clientTCP: $(SRCS0:%.c=obj/%.o)
	gcc -o $@ $+

bin/serveurTCP: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+

bin/clientTCPfull: $(SRCS2:%.c=obj/%.o)
	gcc -o $@ $+

bin/serveurTCPfull: $(SRCS3:%.c=obj/%.o)
	gcc -o $@ $+

bin/clientTCPfonctions: $(SRCS4:%.c=obj/%.o)
	gcc -o $@ $+

bin/serveurTCPfonctions: $(SRCS5:%.c=obj/%.o)
	gcc -o $@ $+

clean:
	rm -f $(BIN) obj/*.o *~
