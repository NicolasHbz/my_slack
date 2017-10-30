CC	=	gcc

NAME	=	my_slack

SRC	=	main.c		\
		app.c		\
		command.c	\
		connection.c	\
		my_string.c	\

OBJ	=	$(SRC:.c=.o)

CFLAGS	=	-W -Wall -Wextra -Werror

RM	=	rm -f

$(NAME):	$(OBJ)
		$(CC) -g $(OBJ) -o $(NAME)

all:		$(NAME)

clean:
		$(RM) $(OBJ)

fclean:		clean
		$(RM) $(NAME)

re:		fclean all
