CC = cc
CPPFLAGS = -Iinclude
CFLAGS = -std=c11 -O2 -Wall -Wextra -Wpedantic -Wconversion -Wshadow
BUILD = build
.PHONY: all test sanitize clean
all: $(BUILD)/bsp-map
$(BUILD):
	mkdir -p $@
$(BUILD)/bsp-map: src/bsp.c src/random.c cli/main.c include/bsp.h src/random.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) src/bsp.c src/random.c cli/main.c $(LDFLAGS) -o $@
$(BUILD)/test-bsp: src/bsp.c src/random.c tests/test_bsp.c include/bsp.h src/random.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) src/bsp.c src/random.c tests/test_bsp.c $(LDFLAGS) -o $@
test: $(BUILD)/test-bsp $(BUILD)/bsp-map $(BUILD)/library-example
	./$(BUILD)/test-bsp
	sh tests/cli.sh ./$(BUILD)/bsp-map
	./$(BUILD)/library-example > $(BUILD)/example.txt
	cmp examples/dungeon.txt $(BUILD)/example.txt
sanitize:
	$(MAKE) BUILD=build/sanitize CFLAGS='-std=c11 -O1 -g -Wall -Wextra -Wpedantic -Wconversion -Wshadow -fsanitize=address,undefined -fno-omit-frame-pointer' LDFLAGS='-fsanitize=address,undefined' test
clean:
	rm -rf build

.PHONY: examples
examples: $(BUILD)/library-example
$(BUILD)/library-example: src/bsp.c src/random.c examples/library.c include/bsp.h src/random.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) src/bsp.c src/random.c examples/library.c $(LDFLAGS) -o $@
