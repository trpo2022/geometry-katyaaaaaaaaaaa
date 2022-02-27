all: geometry
	@echo "компиляция успешно завершена"
geometry: geometry.c
	@gcc geometry.c -Wall -Werror -o geometry
	
test: all mistakes
	@./geometry < mistakes
