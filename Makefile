.PHONY: all
all: format test build

.PHONY: format
format:
	clang-format src/* include/* -i

.PHONY: build
build:
	mkdir -p build
	cd build && \
	cmake .. && \
	make

.PHONY: debug
debug:
	mkdir -p build
	cd build && \
	cmake -DCMAKE_BUILD_TYPE=debug .. && \
	make

.PHONY: test
test:
	mkdir -p build
	cd build && \
	cmake .. && \
	make monitor_test && \
	./monitor_test

.PHONY: clean
clean:
	rm -rf build
