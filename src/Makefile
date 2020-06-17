#FILES = $(basename $(wildcard *.cpp) $(wildcard **/*.cpp))
FILES = $(basename $(shell find . -name "*.cpp"))
OBJS = $(addsuffix .o, $(FILES))
TARGET = ../synthz0r

GREEN = "\\033[32m"
YELLOW = "\\033[33m"
RED = "\\033[31m"
RESET = "\\033[0m"

CC=g++
CFLAGS=-Wall -Wextra -pedantic -ggdb -std=c++2a
LFLAGS=-lpulse-simple -lpulse -lm -lpthread -lprotobuf

%.o: %.cpp %.hpp
	@echo -e "$(YELLOW)Compiling $< => $@$(RESET)"
	@$(CC) -c $(CFLAGS) $< -o $@

all: $(TARGET)

run: $(TARGET)
	$(TARGET)

test: $(TARGET)
	$(TARGET) test

protobuf: protobuf/messages.pb.cc

protobuf/messages.pb.cc: messages.proto
	@echo -e "$(YELLOW)Generating protocol buffers $< => $@$(RESET)"
	protoc --cpp_out=protobuf/ messages.proto

protobuf/messages.pb.o: protobuf/messages.pb.cc
	@echo -e "$(YELLOW)Compiling $< => $@$(RESET)"
	@$(CC) -c $(CFLAGS) $< -o $@

info:
	@echo "CC: $(CC)"
	@echo "LFLAGS: $(LFLAGS)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "FILES: $(FILES)"
	@echo "OBJS: $(OBJS)"
	@echo

waveform.cpp: $(shell find waveforms/)
ifeq (, $(shell which ruby 2>/dev/null))
	@echo -e "$(RED)ruby not found - can't regenerate $@$(RESET)"
else
	@echo -e "$(YELLOW)Regnenerating $@$(RESET)"
	@ruby make_waveform_source.rb
endif

$(TARGET): waveform.cpp protobuf/messages.pb.o $(OBJS)
	@echo
	@echo -e "$(GREEN)Linking $(OBJS) => $(TARGET)$(RESET)"
	@$(CC) $(OBJS) protobuf/messages.pb.o $(LFLAGS) -o $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
	rm -f protobuf/*.cc protobuf/*.h

.PHONY: info all demo clean

FORCE:
