all:
	gcc -o ini_file_test  ./src/ini_file_test.c ./src/ini_file.c 
	@./ini_file_test
clean:
	rm -f ini_file_test
