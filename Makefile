NAME		=	server

SRCS		=	main.cpp Server.cpp Client.cpp Message.cpp Commands.cpp Channel.cpp

CC			=	c++

RM			=	rm

CFLAGS		=	-Wall -Werror -Wextra -std=c++98 -g -fsanitize=address

all:		$(NAME)

$(NAME):	$(SRCS)
			$(CC) $(CFLAGS) -o $(NAME) $(SRCS)

run:		
			make re
			./server 6969 test

clean:		
			$(RM) -f $(NAME)

fclean:		clean

re:			fclean all

.PHONY:		all run clean fclean re