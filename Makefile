SRCS		= ${wildcard ./srcs/*.cpp}
OBJS 		= ${SRCS:.cpp=.o}
CC			= c++
RM			= rm -f
CPPFLAGS	= -std=c++2a -I./includes
NAME		= SnakeGame

%.o: %.cpp ./includes/*.hpp
			@$(CC) ${CPPFLAGS} -c $< -o $@

$(NAME):	$(OBJS)
			@$(CC) $(OBJS) $(CPPFLAGS) -o $(NAME)
			@echo "$(GREEN) ✔ $(NAME) created!$(DEFAULT)"

all:		${NAME}

clean:
			@${RM} ${OBJS}
			@echo "$(YELLOW) ✔ object files deleted!$(DEFAULT)"

fclean:		clean
			@${RM} ${NAME}
			@echo "$(RED) ✔ executable deleted!$(DEFAULT)"

re:			fclean all

.PHONY:		all clean fclean re

#COLORS
RED			= \033[1;31m
GREEN		= \033[1;32m
YELLOW 		= \033[1;33m
DEFAULT		= \033[0m
