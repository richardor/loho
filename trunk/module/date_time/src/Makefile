all:date_time_test
date_time_test:date_time_test.o date_time.o
	gcc -o date_time_test date_time_test.o date_time.o
date_time_test.o:date_time_test.c
	gcc -c date_time_test.c
date_time.o:date_time.c
	gcc -c date_time.c
clean:
	rm -rf date_time_test
	rm -rf *.o
