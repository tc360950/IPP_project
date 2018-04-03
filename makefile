main:   main.o tree.o sorted_list.o user_list.o
	gcc -o main main.o tree.o sorted_list.o user_list.o
main.o: main.c
	gcc -Wall -Wextra -std=c11 -O2 -c main.c
user_list.o: user_list.c user_list.h
	gcc -Wall -Wextra -std=c11 -O2 -c user_list.c
sorted_list.o: sorted_list.c sorted_list.h
	gcc -Wall -Wextra -std=c11 -O2 -c sorted_list.c
tree.o: tree.c tree.h 
	gcc -Wall -Wextra -std=c11 -O2 -c tree.c


clean:        
	rm tree.o
	rm sorted_list.o
	rm main.o
	rm user_list.o
	rm main
