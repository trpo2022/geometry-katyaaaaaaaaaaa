all: geometry.c
	@gcc geometry.c -Wall -Werror -o geometry
	@echo "компиляция успешно завершена"

test: all mistakes
	@./geometry < mistakes
