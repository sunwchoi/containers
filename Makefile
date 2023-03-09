CXX = c++ -Wall -Wextra -Werror -std=c++98 -g
RSC = main.cpp 
OBJS = $(RSC:%.cpp=%.o)
NAME1 = my_containers
NAME2 = origin_container
NAME = $(NAME1) $(NAME2)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CXX) -D TEST=0 $(OBJS) -o$(NAME1)
	$(CXX) -D TEST=1 $(OBJS) -o$(NAME2)

clean :
	rm -f $(OBJS)
fclean : clean
	rm -f $(NAME)

re : fclean all
