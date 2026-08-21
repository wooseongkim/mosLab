CC := gcc
AR := ar
CFLAGS := -std=c99 -Wall -Wextra -Werror -pedantic -I.. -I../include

BUILD_DIR := ../build/instructor
LIB_DIR := ../lib
VM_LIB := $(LIB_DIR)/libmosvm.a
VM_SRCS := $(sort $(wildcard vm_runtime/*.c))
VM_OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(VM_SRCS))

.PHONY: vm-lib clean

vm-lib: $(VM_LIB)

$(VM_LIB): $(VM_OBJS) | $(LIB_DIR)
	$(AR) rcs $@ $^

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)
