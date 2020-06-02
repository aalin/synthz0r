FILES = $(basename $(wildcard *.cpp))
OBJS = $(addsuffix .o, $(FILES))
TARGET = synthz0r

GREEN = "\\033[32m"
YELLOW = "\\033[33m"
RESET = "\\033[0m"

CC=g++
CFLAGS=-Wall -Wextra -ggdb -std=c++17
LFLAGS=-lpulse-simple -lpulse -lm

%.o: %.cpp %.hpp
	@echo -e "$(YELLOW)Compiling $< => $@$(RESET)"
	@$(CC) -c $(CFLAGS) $< -o $@


all: $(TARGET)

run: $(TARGET)
	$(TARGET)

test: $(TARGET)
	$(TARGET) test

info:
	@echo "CC: $(CC)"
	@echo "LFLAGS: $(LFLAGS)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo

$(TARGET): $(OBJS)
	@echo
	@echo -e "$(GREEN)Linking $(SUBDIR_OBJS) $(OBJS) => $(TARGET)$(RESET)"
	@$(CC) $(SUBDIR_OBJS) $(OBJS) $(LFLAGS) -o $(TARGET)

clean:
	rm -f *.o $(TARGET)

.PHONY: info all demo clean
