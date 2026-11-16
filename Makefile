CC = cc
CPPFLAGS = -Iinclude
CFLAGS = -std=c11 -O2 -Wall -Wextra -Wpedantic -Wconversion -Wshadow
BUILD = build
.PHONY: all test sanitize clean
all: $(BUILD)/bsp-map
$(BUILD):
	mkdir -p $@
$(BUILD)/bsp-map: src/bsp.c examples/main.c include/bsp.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) src/bsp.c examples/main.c $(LDFLAGS) -o $@
$(BUILD)/test-bsp: src/bsp.c tests/test_bsp.c include/bsp.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) src/bsp.c tests/test_bsp.c $(LDFLAGS) -o $@
test: $(BUILD)/test-bsp $(BUILD)/bsp-map
	./$(BUILD)/test-bsp
	sh tests/cli.sh ./$(BUILD)/bsp-map
sanitize:
	$(MAKE) BUILD=build/sanitize CFLAGS='-std=c11 -O1 -g -Wall -Wextra -Wpedantic -Wconversion -Wshadow -fsanitize=address,undefined -fno-omit-frame-pointer' LDFLAGS='-fsanitize=address,undefined' test
clean:
	rm -rf build
