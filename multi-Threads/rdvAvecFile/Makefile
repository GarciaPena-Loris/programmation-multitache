########################################
#~ définitions
########################################

BIN=bin/fileImage bin/supprimeFile bin/init

# liste des fichiers sources
SRCS0=fileImage.c
SRCS1=supprimeFile.c
SRCS2=init.c

default: $(BIN)

########################################
#~ regles pour l'executable
########################################

obj/%.o: %.c
	g++ -Wall -Iinclude -c $< -o $@

bin/fileImage: $(SRCS0:%.c=obj/%.o)
	g++ -o $@ $+

bin/supprimeFile: $(SRCS1:%.c=obj/%.o)
	g++ -o $@ $+

bin/init: $(SRCS2:%.c=obj/%.o)
	gcc -o $@ $+

clean:
	rm -f $(BIN) obj/*.o *~
