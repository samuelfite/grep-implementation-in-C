all: grehp.c
	gcc -Wall -Werror grehp.c token_dp.c token.c -o grehp.out
debug: grehp.c
	gcc -Wall -Werror -DDEBUG grehp.c token_dp.c token.c -o grehp.out
test: grehp.c
	gcc -Wall -g grehp.c token_dp.c token.c -o grehp.out
