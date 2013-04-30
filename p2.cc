#include "parser.cc"
#include "util.cc"
#include "cse.cc"

//MAIN
extern "C" {

int main(int argc, char *argv[])
 
{ 

	//checking argument list
	if(argc == 3){
		in.open(argv[2]);
		if(strcmp(argv[1],"-l")==0)
			print_prog();
		else if(strcmp(argv[1],"-ast")==0)
			Mainloop(argv[argc-1]);
	}

 		
 return 0;
}
}