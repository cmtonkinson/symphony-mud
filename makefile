
# General settings and variables
PROJECT				= symphony
CPPC					= ccache clang++
FLAGS_ALL			= --std=c++11 -fcolor-diagnostics
FLAGS_DEV			= $(FLAGS_ALL) -O0 -g -Wall -Werror -pedantic
FLAGS_PROD		= $(FLAGS_ALL) -O3
LIBS					= `pcre-config --libs` -lsodium
SRC_DIR				= src
OBJ_DIR				= obj
BIN_DIR				= bin
SRC_FILES			:= $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES			:= $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES)) $(PROTO_OBJ_FILES)
DEP_FILES			:= $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.d,$(SRC_FILES))
HASH_FILE			= $(SRC_DIR)/githash.h

# Main directives
dev: $(OBJ_FILES)
	$(CPPC) $(FLAGS_DEV) $(OBJ_FILES) $(LIBS) -o $(BIN_DIR)/$(PROJECT)

prod: $(SRC_FILES)
	$(CPPC) $(FLAGS_PROD) $(SRC_FILES) $(LIBS) -o $(BIN_DIR)/$(PROJECT)

clean:
	rm -f ./gmon.out ./core ./out.log
	rm -f $(OBJ_DIR)/*.d $(OBJ_DIR)/*.o
	rm -f $(BIN_DIR)/$(PROJECT)

# Lower level rules
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CPPC) -c $(FLAGS_DEV) $< -o $@

# Generate dependencies for application, dependency files, and unit test item files
$(OBJ_DIR)/%.d: $(SRC_DIR)/%.cpp
	@$(CPPC) -MM $< -o $@
	@sed -i -e 's|\(.*\)\.o:|$(OBJ_DIR)/\1.o $(OBJ_DIR)/\1.d $(TEST_OBJ_DIR)/\1_utest.o:|' $@

-include $(DEP_FILES)
