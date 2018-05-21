SRCS = src/parser/module.c src/parser/types.c src/parser/values.c
OBJS = $(foreach SRC, $(SRCS), $(addprefix build/, $(addsuffix .o, $(SRC))))
LIB = build/libwasm.a
TESTS = test/main.c

.PHONY: build
build: build/libwasm.a

build/src/parser/%.c.o: src/parser/%.c
	@mkdir -p $(dir $@)
	$(CC) -g -I include -c $^ -o $@

build/libwasm.a: $(OBJS)
	ar rcs $@ $^

.PHONY: test
test: build/test
	@build/test

build/test: $(TESTS) $(LIB)
	$(CC) -g -I include  $^ -o $@

.PHONY: clean
clean:
	@$(RM) -r build
