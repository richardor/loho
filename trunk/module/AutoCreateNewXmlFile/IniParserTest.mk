all:
	gcc -o ini_parser_test ./src/ini_parser_test.c ./src/ini_parser.c
	@./ini_parser_test
clean:
	rm -f ./ini_parser_test
