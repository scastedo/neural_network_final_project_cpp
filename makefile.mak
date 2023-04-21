CPPC = g++
CFLAGS = -W -Wall -Wshadow -pedantic -std=c++17
SRCS = value.cpp main.cpp
RM = del -rf

OBJS = $(SRCS:.cpp=.o) # fichiers objets


# Complèter la première règle => pour l'exécutable
a.out : $(OBJS)
	$(CPPC) $(OBJS)

value.o: value.cpp value.h neural_network.h optimiser.h
	$(CPPC) -c $(CFLAGS) $<
		
main.o: main.cpp value.h neural_network.h optimiser.h
	$(CPPC) -c $(CFLAGS) $<

# Exemples de règles pour le ménage
clean :
	$(RM) $(OBJS)

super_clean :
	$(RM) $(OBJS) a.out
