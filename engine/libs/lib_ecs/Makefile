##
## EPITECH PROJECT, 2024
## CPP-ECS
## File description:
## Makefile
##

# Variables

SRC						=	Chunks/ChunkPos.cpp	\
							Components/PureComponentPools.cpp	\
							Entities/AEntity.cpp	\
							Entities/AEntityPool.cpp	\
							Entities/PureEntities.cpp	\
							Entities/PureEntityPools.cpp	\
							Systems/ASystem.cpp	\
							Systems/SystemTree.cpp	\
							EntityManager.cpp	\


SRC_DIR					=	./src/

NAME					=	libecs-pure-base.a

CXXFLAGS				=	-Wall -Wextra -I./src

VALARGS					=	-g3

ASANARGS				=	-fsanitize=address -g3

TESTARGS				=	-fprofile-arcs -ftest-coverage

BUILD_DIR				=	./build/

PROD_DIR				=	$(BUILD_DIR)prod/

VAL_DIR					=	$(BUILD_DIR)valgrind/

ASAN_DIR				=	$(BUILD_DIR)asan/

TEST_DIR				=	$(BUILD_DIR)tests/

# Generated

OBJ						=	$(addprefix $(PROD_DIR), $(SRC:.cpp=.o))

VALOBJ					=	$(addprefix $(VAL_DIR), $(SRC:.cpp=.o))

ASANOBJ					=	$(addprefix $(ASAN_DIR), $(SRC:.cpp=.o))

TESTOBJ					=	$(addprefix $(TEST_DIR), $(SRC:.cpp=.o))

# Targets

$(NAME):				$(OBJ) | $(PROD_DIR)
	ar rc $(PROD_DIR)$(NAME) $(OBJ)

all:					$(NAME)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(PROD_DIR): | $(BUILD_DIR)
	mkdir -p $(PROD_DIR)
	rsync -a -f"+ */" -f"- *" $(SRC_DIR) $(PROD_DIR)

$(VAL_DIR): | $(BUILD_DIR)
	mkdir -p $(VAL_DIR)
	rsync -a -f"+ */" -f"- *" $(SRC_DIR) $(VAL_DIR)

$(ASAN_DIR): | $(BUILD_DIR)
	mkdir -p $(ASAN_DIR)
	rsync -a -f"+ */" -f"- *" $(SRC_DIR) $(ASAN_DIR)

$(TEST_DIR): | $(BUILD_DIR)
	mkdir -p $(TEST_DIR)
	rsync -a -f"+ */" -f"- *" $(SRC_DIR) $(TEST_DIR)

$(PROD_DIR)%.o:		$(SRC_DIR)%.cpp | $(PROD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(VAL_DIR)%.o:			$(SRC_DIR)%.cpp | $(VAL_DIR)
	$(CXX) $(CXXFLAGS) $(VALARGS) -c $< -o $@

$(ASAN_DIR)%.o:			$(SRC_DIR)%.cpp | $(ASAN_DIR)
	$(CXX) $(CXXFLAGS) $(ASANARGS) -c $< -o $@

$(TEST_DIR)%.o:			$(SRC_DIR)%.cpp | $(TEST_DIR)
	$(CXX) $(CXXFLAGS) $(TESTARGS) -c $< -o $@

$(VAL_DIR)$(NAME):		$(VALOBJ) | $(VAL_DIR)
	ar rc $(VAL_DIR)$(NAME) $(VALOBJ)

$(ASAN_DIR)$(NAME):		$(ASANOBJ) | $(ASAN_DIR)
	ar rc $(ASAN_DIR)$(NAME) $(ASANOBJ)

$(TEST_DIR)$(NAME):		$(TESTOBJ) | $(TEST_DIR)
	ar rc $(TEST_DIR)$(NAME) $(TESTOBJ)

grind:				$(VAL_DIR)$(NAME)

sanitize:			$(ASAN_DIR)$(NAME)

unit_tests:			$(TEST_DIR)$(NAME)

clean:
	rm -f $(TESTOBJ:.o=.gcda) $(TESTOBJ:.o=.gcno) $(TESTOBJ:.o=.gcov)
	rm -f $(OBJ)
	rm -f $(VALOBJ)
	rm -f $(ASANOBJ)
	rm -f $(TESTOBJ)

fclean: clean
	rm -rf $(BUILD_DIR)

demo: grind
	$(CXX) $(CXXFLAGS) -o demo main.cpp PointGoingUp.cpp -L$(VAL_DIR) -lecs-pure-base

val-demo: demo
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./demo

re: fclean all

.PHONY: all clean fclean re grind sanitize unit_tests
