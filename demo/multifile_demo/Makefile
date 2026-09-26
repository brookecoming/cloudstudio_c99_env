CC = gcc
CFLAGS = -std=gnu17 -Wall -g

# 一步到位:make 会按依赖顺序自动完成"分步编译 + 链接"
app: main.o calc.o
	$(CC) main.o calc.o -o app

# 分步编译(核心!):-c 表示只把 .c 编译成 .o 目标文件,先不链接
main.o: main.c calc.h
	$(CC) $(CFLAGS) -c main.c -o main.o

calc.o: calc.c calc.h
	$(CC) $(CFLAGS) -c calc.c -o calc.o

# 编译并运行
run: app
	./app

# 清理:删掉可执行文件和中间产物 .o
clean:
	rm -f app main.o calc.o

.PHONY: run clean
