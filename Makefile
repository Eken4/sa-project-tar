# makefile ultra utile et vachement soin

# compiler options
CXX = gcc
CFLAGS = -Wall -fsanitize=address
# linker options
LD = gcc
LDFLAGS = -Wall -fsanitize=address

# project structure
SRC_FOLDER = src
OUT_FOLDER = out

# executable file
EXEC = $(OUT_FOLDER)/$(notdir $(shell pwd))

# sources detection
SRC := $(shell find $(SRC_FOLDER)/ -type f -regex ".*\.c")
OBJ := $(patsubst $(SRC_FOLDER)/%.c, $(OUT_FOLDER)/%.o, $(SRC))


.PHONY: all
all : $(EXEC)

.PHONY: build
build: $(EXEC) clear_objects

.PHONY: clean
clean : 
	rm -rf $(OUT_FOLDER)

.PHONY: clear_objects
clear_objects : 
	rm -f $(OBJ)

.PHONY: run
run : $(EXEC)
	./$(EXEC)

$(OUT_FOLDER)/%.o : $(SRC_FOLDER)/%.c
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -o $@ -c $<

$(EXEC) : $(OBJ)
	@mkdir -p $(dir $@)
	$(LD) $(LDFLAGS) -o $(EXEC) $(OBJ)