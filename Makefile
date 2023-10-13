# Compiler flags
CC = clang++
CFLAGS = \
				-fcolor-diagnostics \
				-fansi-escape-codes \
				-std=c++20 \
        -Wall \
        -Wextra \
        -Wconversion \
        -Wsign-conversion
# can also use one or more "-I directory_name" flags to define additional include directories

# Project files
OBJECTS = helloworld.o
EXE = helloworld

# Debug build settings
DEBUG_DIR = debug
DEBUG_EXE = $(DEBUG_DIR)/$(EXE)
DEBUG_OBJS = $(addprefix $(DEBUG_DIR)/, $(OBJECTS))
DEBUG_CFLAGS = -g -ggdb -Werror -Weffc++

# Release build settings
REL_DIR = release
REL_EXE = $(REL_DIR)/$(EXE)
REL_OBJS = $(addprefix $(REL_DIR)/, $(OBJECTS))
REL_CFLAGS = -O2 -DNDEBUG

.PHONY: all clean debug prep release remake

# Default build
all: prep release

# Debug rules
debug: $(DEBUG_EXE)

$(DEBUG_EXE): $(DEBUG_OBJS)
	$(CC) $(CFLAGS) $(DEBUG_CFLAGS) -o $(DEBUG_EXE) $^

$(DEBUG_DIR)/%.o: src/%.cpp
	$(CC) -c $(CFLAGS) $(DEBUG_CFLAGS) -o $@ $<

# Release rules
release: $(REL_EXE)

$(REL_EXE): $(REL_OBJS)
	$(CC) $(CFLAGS) $(REL_CFLAGS) -o $(REL_EXE) $^

$(REL_DIR)/%.o: src/%.cpp
	$(CC) -c $(CFLAGS) $(REL_CFLAGS) -o $@ $<

# Other rules
prep:
	@mkdir -p $(DEBUG_DIR) $(REL_DIR)

remake: clean all

clean:
	rm -rf $(REL_EXE) $(REL_OBJS) $(DEBUG_EXE) $(DEBUG_OBJS)