NAME	=	scop

OBJDIR	=	obj/

MAIN	=	main
UTIL	=	object \
			utils


SOURCE	= $(MAIN) $(UTIL)

SRC	= $(addsuffix .cpp, $(SOURCE))
OBJ	= $(addprefix $(OBJDIR), $(addsuffix .o, $(SOURCE)))

CC		= c++
RMF		= rm -f
RMRF	= rm -rf
RMDIR	= rmdir
CFLAGS	= -Wall -Wextra -Werror -std=c++17
LFLAGS	= -lglfw -lGL -lglew

libs:
	make PYTHON=python3 -C ./includes/glew/auto/
	make -C ./includes/glew/

$(OBJDIR)%.o: %.cpp
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $(NAME) $(LFLAGS)

clean:
	$(RMRF) $(OBJ)
	$(RMRF) $(OBJDIR)

fclean: clean
	$(RMF) $(NAME)

re: fclean all

all: $(NAME)

.PHONY: all clean fclean re libmake