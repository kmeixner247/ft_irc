NAME		=	ircserv

SRCS		=	main.cpp Server.cpp Client.cpp Message.cpp Commands.cpp Channel.cpp ErrorMessages.cpp Replies.cpp BehaviourBot.cpp

CC			=	c++

RM			=	rm

CFLAGS		=	-Wall -Werror -Wextra -std=c++98 #-g -fsanitize=address
DFLAG		=	-g -D DEBUG=1
all:		$(NAME)

$(NAME):	$(SRCS)
			$(CC) $(CFLAGS) -o $(NAME) $(SRCS)

debug:		$(SRCS)
			$(CC) $(CFLAGS) $(DFLAG) -o $(NAME) $(SRCS)
run:		
			make re
			./ircserv 6969 12345

clean:		
			$(RM) -f $(NAME)

fclean:		clean
			$(RM) -rf $(NAME).dSYM

re:			fclean all

.PHONY:		all run clean fclean re debug