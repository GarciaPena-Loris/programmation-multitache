########################################
#~ définitions
########################################

# nom de l'executable
BIN=bin/acompleter bin/iteratif bin/concurrent

# liste des fichiers sources 
SRCS3=acompleter.c
SRCS4=iteratif.c
SRCS5=concurrent.c


default: $(BIN)

########################################
#~ regles pour l'executable
########################################

obj/%.o: %.c
	gcc -Wall -Iinclude -c $< -o $@


bin/acompleter: $(SRCS3:%.c=obj/%.o)
	gcc -o $@ $+
bin/iteratif: $(SRCS4:%.c=obj/%.o)
	gcc -o $@ $+
bin/concurrent: $(SRCS5:%.c=obj/%.o)
	gcc -o $@ $+

clean:
	rm -f $(BIN) obj/*.o *~
