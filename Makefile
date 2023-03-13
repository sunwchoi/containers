CXX = c++ -Wall -Wextra -Werror -std=c++98 -g
RSC = main.cpp 
OBJS1 = ft_main.o
OBJS2 = std_main.o
OBJS = $(OBJS1) $(OBJS2)
NAME1 = my_containers
NAME2 = origin_containers
NAME = $(NAME1) $(NAME2)

all : $(NAME)

$(OBJS) : $(RSC)
	$(CXX) -c -D TEST=0 $(RSC) -o $(OBJS1)
	$(CXX) -c -D TEST=1 $(RSC) -o $(OBJS2)

$(NAME) : $(OBJS)
	$(CXX)  $(OBJS1) -o$(NAME1)
	$(CXX)  $(OBJS2) -o$(NAME2)

clean :
	rm -f $(OBJS)
fclean : clean
	rm -f $(NAME)

re : fclean all
