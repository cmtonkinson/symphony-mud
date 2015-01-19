
# General settings and variables
PROJECT				= symphony
CPPC					= ccache clang++
FLAGS_ALL			= --std=c++11 -fcolor-diagnostics
FLAGS_DEV			= $(FLAGS_ALL) -O0 -ggdb3 -Wall -Werror -pedantic
FLAGS_PROD		= $(FLAGS_ALL) -O3
LIBS					= `pcre-config --libs` `mysql_config --libs`
SRC_DIR				= src
OBJ_DIR				= obj
BIN_DIR				= bin
SRC_FILES			:= $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES			:= $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES)) $(PROTO_OBJ_FILES)
DEP_FILES			:= $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.d,$(SRC_FILES))

# Main directives
dev: $(OBJ_FILES)
	$(CPPC) $(FLAGS_DEV) $(OBJ_FILES) $(LIBS) -o $(BIN_DIR)/$(PROJECT)

prod: $(SRC_FILES)
	$(CPPC) $(FLAGS_PROD) $(SRC_FILES) $(LIBS) -o $(BIN_DIR)/$(PROJECT)

clean: muscl-clean test-clean
	rm -f ./gmon.out ./core ./out.log
	rm -f $(OBJ_DIR)/*.d $(OBJ_DIR)/*.o
	rm -f $(BIN_DIR)/$(PROJECT)

# Lower level rules
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CPPC) -c $(FLAGS_DEV) $< -o $@

# Generate dependencies for application, dependency files, and unit test object files
$(OBJ_DIR)/%.d: $(SRC_DIR)/%.cpp
	@$(CPPC) -MM $< -o $@
	@sed -i -e 's|\(.*\)\.o:|$(OBJ_DIR)/\1.o $(OBJ_DIR)/\1.d $(TEST_OBJ_DIR)/\1_utest.o:|' $@

-include $(DEP_FILES)

######################## MUSCL ########################
MUSCL_SRC_DIR     = src/muscl
MUSCL_OBJ_DIR     = $(OBJ_DIR)
MUSCL_BIN_DIR     = $(BIN_DIR)
FLEX_OPS          = --align --full --case-insensitive --yylineno
BISON_OPS         = --defines --graph --report=all --debug --verbose
MUSCL_SRC_FILES  := $(wildcard $(MUSCL_SRC_DIR)/*.cpp)
MUSCL_OBJ_FILES   = $(patsubst $(MUSCL_SRC_DIR)/%.cpp,$(MUSCL_OBJ_DIR)/%.o,$(MUSCL_SRC_FILES))
MUSCL_DEP_FILES   = $(patsubst $(MUSCL_SRC_DIR)/%.cpp,$(MUSCL_OBJ_DIR)/%.d,$(MUSCL_SRC_FILES))

muscl: muscl-lexer muscl-parser $(MUSCL_OBJ_FILES)
	$(CPPC) -x c++ $(MUSCL_SRC_DIR)/lex.yy.c $(MUSCL_SRC_DIR)/muscl.tab.c $(MUSCL_OBJ_FILES) -o $(MUSCL_BIN_DIR)/muscl

muscl-lexer: $(MUSCL_SRC_DIR)/muscl.l
	rm -f $(MUSCL_SRC_DIR)/lex.yy.cc
	flex $(FLEX_OPS) $(MUSCL_SRC_DIR)/muscl.l && mv lex.yy.c $(MUSCL_SRC_DIR)

muscl-parser: $(MUSCL_SRC_DIR)/muscl.y
	rm -f $(MUSCL_SRC_DIR)/muscl.tab.{h,c}
	bison $(BISON_OPS) $(MUSCL_SRC_DIR)/muscl.y && mv muscl.tab.* $(MUSCL_SRC_DIR)

muscl-clean:
	rm -f $(MUSCL_OBJ_DIR)/*.{d,o}
	rm -f $(MUSCL_SRC_DIR)/lex.yy.c $(MUSCL_SRC_DIR)/muscl.tab.{h,c}
	rm -f $(MUSCL_ROOT)/{muscl.output,muscl.dot}
	rm -f $(MUSCL_BIN_DIR)/muscl

$(MUSCL_OBJ_DIR)/%.o: $(MUSCL_SRC_DIR)/%.cpp
	$(CPPC) -c $(FLAGS_DEV) $< -o $@

$(MUSCL_OBJ_DIR)/%.d: $(MUSCL_SRC_DIR)/%.cpp
	@$(CPPC) -MM -MD $< -o $@
	@sed -i -e 's|\(.*\)\.o:|$(MUSCL_OBJ_DIR)/\1.o $(MUSCL_OBJ_DIR)/\1.d:|' $@

-include $(DEP_FILES)

######################## GOOGLE TEST (1.5.0) FRAMEWORK ########################
GTEST_DIR       = gtest-1.5.0
TEST_ROOT       = test
TEST_SRC_DIR   := $(TEST_ROOT)/src
TEST_OBJ_DIR   := $(TEST_ROOT)/obj
TEST_BIN_DIR   := $(TEST_ROOT)/bin
TEST_FLAGS     += -I$(GTEST_DIR) -I$(GTEST_DIR)/include -ggdb3
TEST_SRC_FILES := $(wildcard $(TEST_SRC_DIR)/*.cpp)
TEST_OBJ_FILES := $(patsubst $(TEST_SRC_DIR)/%.cpp,$(TEST_OBJ_DIR)/%_utest.o,$(TEST_SRC_FILES))
TEST_DEP_FILES := $(patsubst $(TEST_SRC_DIR)/%.cpp,$(TEST_OBJ_DIR)/%_utest.d,$(TEST_SRC_FILES))
TEST_BIN_FILES := $(patsubst $(TEST_SRC_DIR)/%.cpp,$(TEST_BIN_DIR)/%_utest,$(TEST_SRC_FILES))
TEST_LINK_FILES:= $(subst $(OBJ_DIR)/main.o,,$(OBJ_FILES))
GTEST_HEADERS   = $(GTEST_DIR)/include/gtest/*.h $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS_     = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

test: $(OBJ_FILES) $(TEST_BIN_FILES)

test-clean:
	rm -f $(TEST_BIN_FILES)
	rm -f $(TEST_OBJ_DIR)/*.{a,d,o}

$(TEST_BIN_DIR)/%_utest: $(TEST_SRC_DIR)/%.cpp $(GTEST_HEADERS) $(TEST_OBJ_DIR)/gtest_main.a
	$(CPPC) $(TEST_FLAGS) $< $(TEST_OBJ_DIR)/gtest_main.a $(TEST_LINK_FILES) $(LIBS) -o $@

$(TEST_OBJ_DIR)/gtest-all.o : $(GTEST_SRCS_)
	$(CPPC) -c $(TEST_FLAGS) $(GTEST_DIR)/src/gtest-all.cc -o $@

$(TEST_OBJ_DIR)/gtest_main.o : $(GTEST_SRCS_)
	$(CPPC) -c $(TEST_FLAGS) $(GTEST_DIR)/src/gtest_main.cc -o $@

$(TEST_OBJ_DIR)/gtest.a : $(TEST_OBJ_DIR)/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

$(TEST_OBJ_DIR)/gtest_main.a : $(TEST_OBJ_DIR)/gtest-all.o $(TEST_OBJ_DIR)/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^
