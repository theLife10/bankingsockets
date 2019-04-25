bankClient: bankClient.o
	gcc bankClient.o -o bankClient

bankClient.o: bankClient.c
	gcc -c bankClient.c banking.h

clean:
	rm *.o; rm bankClient; rm *.h.gch
