CC := gcc
AR := ar
CFLAGS := -std=c99 -Wall -Wextra -Werror -pedantic -Iinclude -I.

BUILD_DIR := build
LIB_DIR := lib
VM_LIB := $(LIB_DIR)/libmosvm.a

VM_SRCS := $(sort $(wildcard instructor/vm_runtime/*.c))
VM_OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(VM_SRCS))

MINIOS_SRCS := $(filter-out src/main.c,$(sort $(wildcard src/*.c)))
MINIOS_OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(MINIOS_SRCS))
MAIN_OBJ := $(BUILD_DIR)/src/main.o

LABS := 01 02 03 04 05 06 07 08 09 10

.PHONY: vm-lib verify-instructor minios test clean $(addprefix demo-lab,$(LABS)) $(addprefix test-lab,$(LABS))

vm-lib: $(VM_LIB)

$(VM_LIB): $(VM_OBJS) | $(LIB_DIR)
	$(AR) rcs $@ $^

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_DIR):
	mkdir -p $@

$(BUILD_DIR)/tests/test_vm_runtime: tests/test_vm_runtime.c $(VM_LIB)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< $(VM_LIB) -o $@

verify-instructor: vm-lib $(BUILD_DIR)/tests/test_vm_runtime
	$(BUILD_DIR)/tests/test_vm_runtime

minios: $(MAIN_OBJ) $(MINIOS_OBJS) $(VM_LIB)
	$(CC) $(CFLAGS) $(MAIN_OBJ) $(MINIOS_OBJS) $(VM_LIB) -o $(BUILD_DIR)/minios

define LAB_RULES
$(BUILD_DIR)/apps/lab$(1)_demo: apps/lab$(1)_*_demo.c $(MINIOS_OBJS) $(VM_LIB)
	@mkdir -p $$(dir $$@)
	$$(CC) $$(CFLAGS) $$< $$(MINIOS_OBJS) $$(VM_LIB) -o $$@

demo-lab$(1): $(BUILD_DIR)/apps/lab$(1)_demo
	$$<

$(BUILD_DIR)/tests/test_lab$(1): tests/test_lab$(1)_*.c $(MINIOS_OBJS) $(VM_LIB)
	@mkdir -p $$(dir $$@)
	$$(CC) $$(CFLAGS) $$< $$(MINIOS_OBJS) $$(VM_LIB) -o $$@

test-lab$(1): $(BUILD_DIR)/tests/test_lab$(1)
	$$<
endef

$(foreach lab,$(LABS),$(eval $(call LAB_RULES,$(lab))))

test: $(addprefix test-lab,$(LABS))

clean:
	rm -rf $(BUILD_DIR) $(VM_LIB)
