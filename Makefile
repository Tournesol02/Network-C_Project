#Ceci est un fichier Makefile
#Pour pouvoir créer une executable il faut ecrire la commande: make
#Pour enlever l'environnement après avoir compilé: make clean

#Compilateur
CC = gcc

#Fanions de compilateur
CFLAGS = -Wall -Wextra -g

#Fichier cible
TARGET = tsock.exe

#Fichiers sources
SRCS = tsock.c  tsockSource.c

#On crée un fichier objet
OBJS = $(SRCS:.c=.o)

# Cible
all: $(TARGET)

# On crée le fichier .exe à l'aide des fichiers objet
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compiler .c à .o
%.o: %.c tsock.h
	$(CC) $(CFLAGS) -c $< -o $@

# make clean: on supprime le fichier cible
clean:
	rm -f *.o $(TARGET)
