NAME		=	scop
OBJDIR		=	obj/
MAIN		=	main
UTIL		=	object \
				utils

GLEW_PATH	=	includes/glew
GLEW_LIB    = $(GLEW_PATH)/lib/libGLEW.a

SOURCE	= $(MAIN) $(UTIL)

SRC	= $(addsuffix .cpp, $(SOURCE))
OBJ	= $(addprefix $(OBJDIR), $(addsuffix .o, $(SOURCE)))

CC		= c++
RMF		= rm -f
RMRF	= rm -rf
RMDIR	= rmdir
MAKE	= make
CFLAGS	= -Wall -Wextra -Werror -std=c++17
LFLAGS = \
	-L$(GLEW_PATH)/lib \
	-Wl,-rpath,$(abspath $(GLEW_PATH)/lib) \
	-lglfw -lGL -lGLEW

all: $(NAME)

$(GLEW_LIB):
	$(MAKE) PYTHON=python3 -C $(GLEW_PATH)/auto
	$(MAKE) -C $(GLEW_PATH)

$(OBJDIR)%.o: %.cpp
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ) | $(GLEW_LIB)
	$(CC) $(OBJ) $(CFLAGS) -o $(NAME) $(LFLAGS)

glewclean:
	$(MAKE) clean -C $(GLEW_PATH)
	$(MAKE) clean -C $(GLEW_PATH)/auto

glewfclean: glewclean
	$(MAKE) distclean -C $(GLEW_PATH)
	$(MAKE) destroy -C $(GLEW_PATH)/auto

clean: glewclean
	$(RMRF) $(OBJ)
	$(RMRF) $(OBJDIR)

fclean: clean glewfclean
	$(RMF) $(NAME)

re: fclean all


.PHONY: all clean fclean re