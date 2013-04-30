all:
	g++ -w -O2 p2.cc -o p2
debug:
	g++ -w -g -O2 -g p2.cc -o p2
clean:
	rm -f p2
test:
	chmod a+x difftest.pl
	perl difftest.pl -1 "rpal/rpal -ast FILE" -2 "./p2 -ast FILE" -t ./rpal/tests/