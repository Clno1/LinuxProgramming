src = $(wildcard *.c)
target = $(patsubst %.c, %, $(src))

ALL : $(target)

%:%.c		#单文件编译，直接gcc一步到位
	gcc $< -o $@

clean:
	-rm -rf $(target)

.PHONY: clean ALL
