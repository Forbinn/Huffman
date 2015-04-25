##
## Makefile
##
## Made by vincent leroy
## Login   <leroy_v@epitech.eu>
##
## Started on  Sat Apr 25 21:04:18 2015 vincent leroy
## Last update Sat Apr 25 21:13:50 2015 vincent leroy
##

SRC			= src/arg_callback.c \
			  src/compression/compression.c \
			  src/compression/compression_threaded.c \
			  src/crc/crc32.c \
			  src/decompression/decompression.c \
			  src/decompression/decompression_threaded.c \
			  src/header/header.c \
			  src/huffman_tree/huffman.c \
			  src/huffman_tree/huffmannode.c \
			  src/huffman_tree/huffmantree.c \
			  src/io_operation.c \
			  src/main.c \
			  src/parse_arg/parse_arg.c \
			  src/parse_arg/parse_error.c \
			  src/parse_arg/parse_long_opt.c \
			  src/parse_arg/parse_short_opt.c \
			  src/thread_pool/future.c \
			  src/thread_pool/pool.c \
			  src/thread_pool/queue.c \
			  src/thread_pool/syncqueue.c \
			  src/thread_pool/thread.c \

CFLAGS		= -Wall -Wextra
CFLAGS		+= -pthread
CFLAGS		+= -I include/
CFLAGS		+= -I include/compression/
CFLAGS		+= -I include/crc/
CFLAGS		+= -I include/decompression/
CFLAGS		+= -I include/header/
CFLAGS		+= -I include/huffman_tree/
CFLAGS		+= -I include/parse_arg/
CFLAGS		+= -I include/thread_pool

LDFLAGS		= -pthread

NAME		= ha
NAME_DEBUG	= $(NAME).debug

OBJ			= $(SRC:.c=.o)
OBJ_DEBUG	= $(SRC:.c=.debug.o)

RM			= rm -f

CC			= gcc

MAKE		= make -C

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(NAME)

clean:
	$(RM) $(OBJ) $(OBJ_DEBUG) *.swp *~ *#

fclean: clean
	$(RM) $(NAME) $(NAME_DEBUG)

re: fclean all

debug: CFLAGS += -ggdb3
debug: $(OBJ_DEBUG)
	$(CC) $(OBJ_DEBUG) $(LDFLAGS) -o $(NAME_DEBUG)

%.debug.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean fclean re debug
