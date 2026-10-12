labName=lab08
compileCmd=gcc -Wall -pedantic-errors $(labName).c -g -o $(labName).out

compile: $(labName).out

$(labName).out: $(labName).c
	$(compileCmd)

run: $(labName).out
	./$(labName).out -f -18.5

clean:
	rm $(labName).out
