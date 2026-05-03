NAME		=	scop
OBJDIR		=	obj/
MAIN		=	main
UTIL		=	object \
				Renderer \
				VertexBuffer \
				VertexBufferLayout \
				VertexArray \
				IndexBuffer \
				Shader \
				Texture \
				Camera \
				GUI \
				stb_image \
				utils

GLEW_PATH	=	includes/glew
GLEW_LIB	=	$(GLEW_PATH)/lib/libGLEW.a

IMGUI_PATH	=	includes/imgui/
IMGUI_FILES	=	imgui \
				imgui_widgets \
				imgui_tables \
				imgui_draw \
				imgui_demo \
				backends/imgui_impl_glfw \
				backends/imgui_impl_opengl3

STB_IMAGE_PATH =includes/stb_image

SOURCE	= $(MAIN) $(UTIL) $(addprefix $(IMGUI_PATH), $(IMGUI_FILES))

SRC	= $(addsuffix .cpp, $(SOURCE))
OBJ	= $(addprefix $(OBJDIR), $(addsuffix .o, $(SOURCE)))

CC		= c++
RMF		= rm -f
RMRF	= rm -rf
RMDIR	= rmdir
MAKE	= make
CFLAGS	= -Wall -Wextra -Werror -std=c++17
IFLAGS = -I$(GLEW_PATH)/include -I$(IMGUI_PATH)
LFLAGS = \
	-L$(GLEW_PATH)/lib \
	-Wl,-rpath,$(abspath $(GLEW_PATH)/lib) \
	-lglfw -lGL -lGLEW

all: $(NAME)

$(GLEW_LIB):
	$(MAKE) PYTHON=python3 -C $(GLEW_PATH)/auto
	$(MAKE) -C $(GLEW_PATH)

$(OBJDIR)%.o: %.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(NAME): $(GLEW_LIB) $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $(NAME) $(LFLAGS)

glewclean:
	$(MAKE) clean -C $(GLEW_PATH)
	$(MAKE) clean -C $(GLEW_PATH)/auto

glewfclean: glewclean
	$(MAKE) distclean -C $(GLEW_PATH)
	$(MAKE) destroy -C $(GLEW_PATH)/auto

clean:
	$(RMRF) $(OBJ)
	$(RMRF) $(OBJDIR)

fclean: clean
	$(RMF) $(NAME)

re: fclean all

.PHONY: all clean fclean re