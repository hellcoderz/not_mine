#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <string.h>

#include "lexer.cc"


using namespace std;

int LNUM=0; 
struct env {   int number;  struct env *prev;   struct child_env *next_node;   struct cse_object *from, *to;   };

struct zta {   int k;   struct env *c;  struct cse_object *AST_node; };

struct mta {   struct delta *G19, *F;  };

struct tuple {   int number;  struct cse_object *cse_mem;  };

struct delta {   int number;  struct cse_object *cse_st;  struct delta *next_node;};

struct cse_object {   int token_type;  struct cse_object *next_node;  union {    struct zta *l;    struct env *epsilon;     struct mta *b;    struct exprAST *t;    struct tuple *tup;    int tau;  } val;};


//forward declaration of class
class ExprAST;

//forward declaration of functions
ExprAST *D(); ExprAST *Dr(); ExprAST *Da(); ExprAST *Db(); ExprAST *E(); ExprAST *Ew(); ExprAST *T(); ExprAST *Ta(); ExprAST *Tc(); ExprAST *B(); ExprAST *Bt(); ExprAST *Bs(); ExprAST *Bp(); ExprAST *A(); ExprAST *At(); ExprAST *Af(); ExprAST *Ap(); ExprAST *R(); ExprAST *Rn(); ExprAST *Vb(); ExprAST *Vl(); void print(ExprAST *AST, int DEPTH);

//reserved keywords
string reserverd[] = {"let","in", "fn", "aug", "ge", "gr", "ls", "le", "or", "and", "ne", "eq", "neg", "not", "tau", "where", "within", "rec"};
int no_of_keywords = 18;

//boolean flags for Rn(), Vl() and Vb() rules
bool RN_FLAG = 0, VL_FLAG = 0, VB_FLAG = 0;

//enumerations for expression types
enum ExprType { E_NIL = 100, E_DUMMY, E_INTEGER, E_IDENTIFIER, E_BOOLEAN, E_BINOP, E_UNARYOP, E_STRING, E_LET, E_LAMBDA, E_WHERE, E_TAU, E_AUG, E_COND, E_GAMMA, E_INFIX, E_WITHIN, E_AND, E_REC, E_ASSIGN, E_FUNC_FORM, E_PARENS, E_LIST, E_ERROR, E_VARLIST };

//enumerations for binary operator types
enum BINOP { B_PLUS, B_MINUS, B_MUL, B_DIV, B_NEG, B_OR, B_AND, B_NOT, B_GR, B_GE, B_LS, B_LE, B_EQ, B_NE, B_AUG, B_EXP};

//enumerations for unary operator types
enum UNARYOP { U_NOT, U_NEG };

//base class to hold all claases pointer
class ExprAST{
public:
	virtual ~ExprAST() {}
	virtual ExprType getType(){
		return E_ERROR;
	};
	//function to get the type of class object
	virtual vector<ExprAST*> *getVector(){
		return NULL;
	}
};

//class to store Integer types
class IntegerExprAST: public ExprAST{
public:
	ExprType type;
	int Val;
	IntegerExprAST(int val){
		type = E_INTEGER;
		Val = val;
	}

	ExprType getType(){
		return type;
	}
};

//class to hold Identifier types
class IdentifierExprAST: public ExprAST{
public:
	ExprType type;
	string Val;
	IdentifierExprAST(string val){
		type = E_IDENTIFIER;
		Val = val;
	}

	ExprType getType(){
		return type;
	}
};

//class to hold String data
class StringExprAST: public ExprAST{
public:
	ExprType type;
	string Val;
	StringExprAST(string val){
		type = E_STRING;
		Val = val;
	}

	ExprType getType(){
		return type;
	}
};

//class to hold Boolean data
class BooleanExprAST : public ExprAST{
public:
	ExprType type;
	bool Val;
	BooleanExprAST(bool val){
		type = E_BOOLEAN;
		Val = val;
	}

	ExprType getType(){
		return type;
	}
};

//class to hold nil node
class NilExprAST : public ExprAST{
public:
	ExprType type;
	NilExprAST(){
		type = E_NIL;
	}

	ExprType getType(){
		return type;
	}
};

//class to hold dummy node
class DummyExprAST : public ExprAST{
public:
	ExprType type;
	DummyExprAST(){
		type = E_DUMMY;
	}

	ExprType getType(){
		return type;
	}
};

//class to hold tuple data structure
class TauExprAST: public ExprAST{
public:
	ExprType type;
	vector<ExprAST*> Ta;	//vector to hold arbitary no. of childs 
	TauExprAST(vector<ExprAST*> ta){
		type = E_TAU;
		Ta = ta;
	}

	ExprType getType(){
		return type;
	}
};

//class to hold 'aug' operator and its argument
class AugExprAST: public ExprAST{
public:
	ExprType type;
	ExprAST *Ta;
	ExprAST *Tc;
	AugExprAST(ExprAST *ta, ExprAST *tc){
		type = E_AUG;
		Ta = ta;
		Tc = tc;
	}

	ExprType getType(){
		return type;
	}
};

//class to hold IfThenElse node with its children
class CondExprAST: public ExprAST{
public:
	ExprType type;
	ExprAST *B;
	ExprAST *Tc_if;
	ExprAST *Tc_else;
	CondExprAST(ExprAST *b, ExprAST *tc_if, ExprAST *tc_else){
		type = E_COND;
		B = b;
		Tc_if = tc_if;
		Tc_else = tc_else;
	}

	ExprType getType(){
		return type;
	}
};

//class to hold all other binary operation nodes
class BinaryOpExprAST : public ExprAST{
public:
	ExprType type;
	ExprAST *Left;
	ExprAST *Right;
	BINOP OP;
	BinaryOpExprAST(ExprAST *left, ExprAST *right,BINOP op) {
		type = E_BINOP;
		Left = left;
		Right = right;
		OP = op;
	}

	ExprType getType(){
		return type;
	}
};

//class to hold all unary operation nodes
class UnaryOpExprAST : public ExprAST{
public:
	ExprType type;
	ExprAST *Node;
	UNARYOP OP;
	UnaryOpExprAST(ExprAST *node, UNARYOP op){
		type = E_UNARYOP;
		Node = node;
		OP = op;
	}

	ExprType getType(){
		return type;
	}
};

//class to hold Let node and its children
class LetExprAST: public ExprAST{
public:
	ExprType type;
	ExprAST *D;
	ExprAST *E;
	LetExprAST(ExprAST *d, ExprAST *e){
		type = E_LET;
		D = d;
		E = e;
	}

	ExprType getType(){
		return type;
	}
};

//class to hold Lambda node and its children
class LambdaExprAST: public ExprAST{
public:
	ExprType type;
	vector<ExprAST*> Vb;
	ExprAST *E;
	LambdaExprAST(vector<ExprAST*> vb, ExprAST *e){
		Vb = vb;
		E = e;
		type = E_LAMBDA;
	}

	ExprType getType(){
		return type;
	}
};

//class to hold Where node and its children
class WhereExprAST: public ExprAST{
public:
	ExprType type;
	ExprAST *T;
	ExprAST *Dr;
	WhereExprAST(ExprAST *t, ExprAST *dr){
		type = E_WHERE;
		T = t;
		Dr = dr;
	}

	ExprType getType(){
		return type;
	}
};

//class to store '@' operator and its operator in infix form
class InfixExprAST: public ExprAST{
public:
	ExprType type;
	ExprAST *A;
	ExprAST *Identifier;
	ExprAST *B;
	InfixExprAST(ExprAST *a,ExprAST *identifier, ExprAST *b){
		type = E_INFIX;
		A = a;
		Identifier = identifier;
		B = b;
	}

	ExprType getType(){
		return type;
	}
};

//Class to store 'within' node with its children
class WithinExprAST: public ExprAST{
public:
	ExprType type;
	ExprAST *Da;
	ExprAST *D;
	WithinExprAST(ExprAST *da, ExprAST *d){
		type = E_WITHIN;
		Da = da;
		D = d;
	}

	ExprType getType(){
		return type;
	}
};

//class to store 'And' node and its children
class AndExprAST: public ExprAST{
public:
	vector<ExprAST*> Dr;
	ExprType type;
	AndExprAST(vector<ExprAST*> dr){
		Dr = dr;
		type = E_AND;
	}

	ExprType getType(){
		return type;
	}
};

//class to store 'rec' node and its child
class RecExprAST: public ExprAST{
public:
	ExprType type;
	ExprAST *Db;
	RecExprAST(ExprAST *db){
		type = E_REC;
		Db = db;
	}

	ExprType getType(){
		return type;
	}
};


//class to store 'Assign' node and its left and right child
class AssignExprAST: public ExprAST{
public:
	ExprAST *Vl;
	ExprAST *E;
	ExprType type;
	AssignExprAST(ExprAST *vl, ExprAST *e){
		Vl = vl;
		E = e;
		type = E_ASSIGN;
	}

	ExprType getType(){
		return type;
	}
};

//class to store function name, its variables, and its definition
class FuncExprAST: public ExprAST{
public:
	ExprType type;
	ExprAST *Identifier;
	vector<ExprAST*> Vb;
	ExprAST *E;
	FuncExprAST(ExprAST *identifier, vector<ExprAST*> vb, ExprAST *e){
		type = E_FUNC_FORM;
		Identifier = identifier;
		Vb = vb;
		E = e;
	}

	ExprType getType(){
		return type;
	}

};

//claas to store '()' node
class ParensExprAST: public ExprAST{
public:
	ExprType type;
	ParensExprAST(){
		type = E_PARENS;
	}

	ExprType getType(){
		return type;
	}
};

//class to store 'gamma' node and its children
class GammaExprAST: public ExprAST{
public:
	ExprType type;
	ExprAST *Left;
	ExprAST *Right;
	GammaExprAST(ExprAST *left,ExprAST *right){
		type = E_GAMMA;
		Left = left;
		Right = right;
	}

	ExprType getType(){
		return type;
	}
};

//for variables node
class VariableExprAST: public ExprAST{
public:
	ExprType type;
	vector<ExprAST*> List;
	VariableExprAST(vector<ExprAST*> list){
		type = E_VARLIST;
		List = list;
	}

	ExprType getType(){
		return type;
	}

};

//for Error nodes
class ErrorExprAST: public ExprAST{
	ExprType type;
public:
	ErrorExprAST() {
		type = E_ERROR;
	}

	ExprType getType(){
		return type;
	}
};

//class 

////////////////////////
/////Parser Grammmer///////
////////////////////////

//helper function to throw error
void p_error(string st){
	cout << "Parse Error: " << st << endl;
	exit(0);
}

//function to read a string value and if not present generate error
void Read(string st){
	if(lexeme != st ){
		cout << "Error reading " << st << endl;
		exit(0);
	}
	if( Token_type != STRING){
		gettok();
	}else{
		p_error("token not found");
	}
}

/*
	E 	-> 	'let' D 'in' E 	=> 'let'
		->	'fn' Vb+ '.' E 	=> 	'lambda'
		-> Ew;
*/

ExprAST *E(){
	ExprAST *AST_D;
	vector<ExprAST*> AST_Vb;
	ExprAST *AST_E;

	//cout << "Inside E" << endl;
	//cout << lexeme << endl;
	if((lexeme == "let" || lexeme == "fn")  && Token_type != STRING){
		if(lexeme == "let"  && Token_type != STRING){
			gettok();  //changed
			AST_D = D();
			//cout << ">>>>>>>>>>>>>Type= " << AST_D->getType() << endl;
			Read("in");

			AST_E = E();
			return (new LetExprAST(AST_D,AST_E));
		}else{
			gettok();
			do{
				
				AST_Vb.push_back(Vb());
			}while(lexeme != "."  && Token_type != STRING);
			Read(".");
			//cout << "lexeme = " << lexeme << endl;
			AST_E = E();
			//print(AST_E,10);
			return (new LambdaExprAST(AST_Vb,AST_E));			
		}
	}
	return Ew();
}


/*
	Ew 	-> 	T 'where' Dr 	=> 	'where'
	 	-> 	T;
*/
ExprAST *Ew(){
	ExprAST *AST_T;
	ExprAST *AST_Dr;

	//cout << "Inside Ew" << endl;
	AST_T = T();
	if(lexeme == "where" && Token_type != STRING){
		gettok();   //changed
		AST_Dr = Dr();
		return (new WhereExprAST(AST_T,AST_Dr));
	}

	return AST_T;
}

/*
	T -> Ta ( ’,’ Ta )+ => ’tau’
	  -> Ta ;
*/
ExprAST *T(){
	vector<ExprAST*> AST_Ta;
	string temp;

	//cout << "Inside T" << endl;
	do{
		//cout << "================ INSIDE TAU LOOP" << AST_Ta.size() << endl;
		if(AST_Ta.size() >= 1)
			gettok();
		AST_Ta.push_back(Ta());
	}while(Token_type == COMMA);

	if(AST_Ta.size() > 1)
		return (new TauExprAST(AST_Ta));
	else
		return AST_Ta[0];
}

/*
	Ta -> Ta ’aug’ Tc => ’aug’
 	   -> Tc ;
*/
ExprAST *Ta(){
	ExprAST *AST_Ta;
	ExprAST *AST_Tc;
	ExprAST *AST_Left;

	//cout << "Inside Ta" << endl;
	AST_Ta = Tc();
	AST_Left = AST_Ta;
	while(lexeme == "aug"  && Token_type != STRING){
		gettok();
		AST_Tc = Tc();
		AST_Left = new AugExprAST(AST_Left,AST_Tc);
	}
	return AST_Left;
}


/*
	Tc -> B ’->’ Tc ’|’ Tc => ’->’
 	   -> B ;
*/
ExprAST *Tc(){
	ExprAST *AST_B;
	ExprAST *AST_Tc_if;
	ExprAST *AST_Tc_else;

	//cout << "Inside Tc" << endl;
	AST_B = B();
	if(lexeme == "->" && Token_type != STRING){
		gettok();
		AST_Tc_if = Tc();
		Read("|");
		AST_Tc_else = Tc();
		return (new CondExprAST(AST_B,AST_Tc_if,AST_Tc_else));
	}

	return AST_B;
}

/*
	B -> B’or’ Bt => ’or’
	  -> Bt ;
*/
ExprAST *B(){
	ExprAST *AST_B;
	ExprAST *AST_Bt;

	//cout << "Inside B" << endl;
	AST_B = Bt();
	while(lexeme == "or" && Token_type != STRING){
		gettok();
		AST_Bt = Bt();
		AST_B = new BinaryOpExprAST(AST_B,AST_Bt,B_OR);
	}

	return AST_B;
}

/*
	Bt -> Bt ’&’ Bs => ’&’
	   -> Bs ;
*/
ExprAST *Bt(){
	ExprAST *AST_Bt;
	ExprAST *AST_Bs;

	//cout << "Inside Bt" << endl;
	AST_Bt = Bs();
	while(lexeme == "&" && Token_type != STRING){
		gettok();
		AST_Bs = Bs();
		AST_Bt = new BinaryOpExprAST(AST_Bt,AST_Bs,B_AND);
	}

	return AST_Bt;
}

/*
	Bs -> ’not’ Bp => ’not’
	   -> Bp ;
*/
ExprAST *Bs(){
	ExprAST *AST_Bp;

	//cout << "Inside Bs" << endl;
	if(lexeme == "not" && Token_type != STRING){
		gettok(); //new changed
		AST_Bp = Bp(); 
		return (new UnaryOpExprAST(AST_Bp,U_NOT));
	}else{
		return Bp();
	}
}

/*
	Bp -> A (’gr’ | ’>’ ) A => ’gr’
	   -> A (’ge’ | ’>=’) A => ’ge’
	   -> A (’ls’ | ’<’ ) A => ’ls’
	   -> A (’le’ | ’<=’) A => ’le’
	   -> A ’eq’ A => ’eq’
	   -> A ’ne’ A => ’ne’
	   -> A ;
*/
ExprAST *Bp(){
	ExprAST *AST_Left;
	ExprAST *AST_Right;

	//cout << "Inside Bp" << endl;
	AST_Left = A();
	if((lexeme == "gr" || lexeme == ">")  && Token_type != STRING){
		gettok();  //changed
		AST_Right = A();
		return (new BinaryOpExprAST(AST_Left,AST_Right,B_GR));
	}else if((lexeme == "ge" || lexeme == ">=") && Token_type != STRING){
		gettok();  //changed
		AST_Right = A();
		return (new BinaryOpExprAST(AST_Left,AST_Right,B_GE));
	}else if((lexeme == "ls" || lexeme == "<")  && Token_type != STRING){
		gettok();  //changed
		AST_Right = A();
		return (new BinaryOpExprAST(AST_Left,AST_Right,B_LS));
	}else if((lexeme == "le" || lexeme == "<=")  && Token_type != STRING){
		gettok();  //changed
		AST_Right = A();
		return (new BinaryOpExprAST(AST_Left,AST_Right,B_LE));
	}else if(lexeme == "eq" && Token_type != STRING){
		gettok();  //changed
		AST_Right = A();
		return (new BinaryOpExprAST(AST_Left,AST_Right,B_EQ));
	}else if(lexeme == "ne" && Token_type != STRING){
		gettok();  //changed
		AST_Right = A();
		return (new BinaryOpExprAST(AST_Left,AST_Right,B_NE));
	}

	return AST_Left;
}

/*
	A -> A’+’ At => ’+’
	  -> A ’-’ At => ’-’
	  -> ’+’ At
	  -> ’-’ At => ’neg’
	  -> At ;
*/
ExprAST *A(){
	ExprAST *AST_A;
	ExprAST *AST_At;

	//cout << "Inside A" << endl;
	//cout << lexeme << " : " << Token_type << endl;
	if((lexeme == "+" || lexeme == "-")  && Token_type != STRING){
		if(lexeme == "+"  && Token_type != STRING){
			gettok();  //changed
			AST_A = At();
		}else{
			gettok();  //changed
			AST_A = At();
			AST_A = new UnaryOpExprAST(AST_A,U_NEG); 
		}
	}else{

		AST_A = At();
	}
	while((lexeme == "+" || lexeme == "-")  && Token_type != STRING){
		if(lexeme == "+" && Token_type != STRING){
			gettok(); //changed
			AST_At = At();
			AST_A = new BinaryOpExprAST(AST_A,AST_At,B_PLUS);
		}else{
			gettok(); //changed
			AST_At = At();
			AST_A = new BinaryOpExprAST(AST_A,AST_At,B_MINUS);
		}
	}

	return AST_A;
}

/*
	At -> At ’*’ Af => ’*’
	   -> At ’/’ Af => ’/’
       -> Af ;
*/
ExprAST *At(){
	ExprAST *AST_At;
	ExprAST *AST_Af;

	//cout << "Inside At" << endl;
	//cout << lexeme << " : " << Token_type << endl;
	AST_At = Af();
	//cout << "Outside At" << endl;
	while((lexeme == "*" || lexeme == "/")  && Token_type != STRING){
		if(lexeme == "*" && Token_type != STRING){
			gettok();	//changed
			AST_Af = Af();
			AST_At = new BinaryOpExprAST(AST_At,AST_Af,B_MUL);
		}else{
			gettok();	//changed
			AST_Af = Af();
			AST_At = new BinaryOpExprAST(AST_At,AST_Af,B_DIV);
		}
	}

	return AST_At;	
}

/*
	Af -> Ap ’**’ Af => ’**’
	   -> Ap ;
*/
ExprAST *Af(){
	ExprAST *AST_Ap;
	ExprAST *AST_Af;

	//cout << "Inside Af" << endl;
	//cout << "==============  " << lexeme << endl;
	AST_Ap = Ap();
	//cout << "Outside Af" << endl;
	//cout << "==============  " << lexeme << endl;
	if(lexeme == "**" && Token_type != STRING){
		gettok(); //changed
		//cout << "inside Af if" << endl;
		AST_Af = Af();
		AST_Ap = new BinaryOpExprAST(AST_Ap,AST_Af,B_EXP);
	}

	return AST_Ap;
}

/*
``Ap -> Ap ’@’ ’<IDENTIFIER>’ R => ’@’
	 -> R ;
*/
ExprAST *Ap(){
	ExprAST *AST_Ap;
	ExprAST *AST_R;
	ExprAST *AST_Id;
	
	//cout << "Inside Ap" << endl;
	AST_Ap = R();
	//cout << "out side Ap" << endl;
	//cout << lexeme << " -> " << Token_type << endl;
	while(lexeme == "@" && Token_type != STRING){
	//	cout << "got @" << endl;
		if(gettok() == IDENTIFIER){
	//		cout << "got identifier" << endl;
			AST_Id = new IdentifierExprAST(lexeme);
			gettok();
			AST_R = R();
			AST_Ap = new InfixExprAST(AST_Ap,AST_Id,AST_R);
		}
	}

	return AST_Ap;
}

/*
	R -> RRn => ’gamma’
	  -> Rn ;
*/
ExprAST *R(){
	ExprAST *AST_R;
	ExprAST *AST_Rn;

	//cout << "Inside R" << endl;
	//cout << lexeme << " : " << Token_type << endl;
	AST_R = Rn(); 
	if(RN_FLAG == 1){
		RN_FLAG = 0;
		//cout << "Inside first if" << endl;
		//cout << lexeme << " : " << Token_type << endl;
		AST_Rn = Rn();
		while(RN_FLAG == 1){
			//cout << "Inside second if" << endl;
			//cout << lexeme << " : " << Token_type << endl;
			RN_FLAG = 0;
			AST_R = new GammaExprAST(AST_R,AST_Rn);
			AST_Rn = Rn();
		}
	}
	RN_FLAG = 0;
	return AST_R;
}

/*
	Rn -> ’<IDENTIFIER>’
	   -> ’<INTEGER>’
	   -> ’<STRING>’
	   -> ’true’ => ’true’
	   -> ’false’ => ’false’
	   -> ’nil’ => ’nil’
	   -> ’(’ E ’)’
	   -> ’dummy’ => ’dummy’ ;
*/
ExprAST *Rn(){
	ExprAST *AST_E;
	int res;
	string temp;
	
	//cout << "Inside Rn" << endl;
	//cout << lexeme << " : " << Token_type << endl;
	switch(Token_type){
		case IDENTIFIER: for(int i = 0; i< no_of_keywords ;i++){  //changed
							if(lexeme == reserverd[i]){
								RN_FLAG = 0; 
								return (new ErrorExprAST());
							}
						 }
						 RN_FLAG = 1; 
						 //cout << "entered" << endl;
						 if((lexeme == "true" || lexeme == "false") && Token_type != STRING){
						 	if(lexeme == "true" && Token_type != STRING){
						 		gettok();
						 		return (new BooleanExprAST(true)); break;
						 	}else{
						 		gettok();
						 		return (new BooleanExprAST(false)); break;
						 	}
						 }else if(lexeme == "nil" && Token_type != STRING){
						 	gettok();
						 	return (new NilExprAST()); break;
						 }else if(lexeme == "dummy" && Token_type != STRING){
						 	gettok();
						 	return (new DummyExprAST()); break;
						 }else{
						 	temp = lexeme;
						 	gettok();
						 	//cout << lexeme << endl;
						 	return (new IdentifierExprAST(temp)); break;
						 }
						 break;
		case INTEGER: RN_FLAG = 1; stringstream(lexeme) >> res; gettok(); return (new IntegerExprAST(res)); break;
		case STRING: RN_FLAG = 1; temp = lexeme; gettok(); return (new StringExprAST(temp)); break;
		case BRACKET_OPEN: 
						   gettok();
						   //cout << "(E)" << endl;
						   AST_E = E();
						   if(Token_type != BRACKET_CLOSE){
						   		p_error(")");
						   }
						   gettok();
						   RN_FLAG = 1;   //changed
						   return AST_E;
						   break;

		default : RN_FLAG = 0; return (new ErrorExprAST());
	}
	
}

/*
	D -> Da ’within’ D => ’within’
	  -> Da ;
*/
ExprAST *D(){
	ExprAST *AST_Da;
	ExprAST *AST_D;

	//cout << "Inside D" << endl;
	AST_Da = Da();
	if(lexeme == "within" && Token_type != STRING){
		gettok();  //new changed
		AST_D = D();
		AST_Da = new WithinExprAST(AST_Da, AST_D);
	}

	return AST_Da;
}


/*
	Da -> Dr ( ’and’ Dr )+ => ’and’
	   -> Dr ;
*/
ExprAST *Da(){
	vector<ExprAST*> AST_Dr;

	//cout << "Inside Da" << endl;
	do{
		if(AST_Dr.size() >= 1)
			gettok();
		AST_Dr.push_back(Dr());
	}while(lexeme == "and" && Token_type != STRING);

	//changed
	if(AST_Dr.size() > 1)
		return (new AndExprAST(AST_Dr));
	else
		return AST_Dr[0];
}

/*
	Dr -> ’rec’ Db => ’rec’
	   -> Db ;
*/
ExprAST *Dr(){
	ExprAST *AST_Db;

	//cout << "Inside Dr" << endl;
	if(lexeme == "rec" && Token_type != STRING){
		gettok();  //changed
		AST_Db = Db();
		return (new RecExprAST(AST_Db));
	}
	AST_Db = Db();

	return AST_Db;
}

/*
	Db -> Vl ’=’ E => ’=’
	   -> ’<IDENTIFIER>’ Vb+ ’=’ E => ’fcn_form’
       -> ’(’ D ’)’ ;
*/
ExprAST *Db(){
	ExprAST *AST_Vl;
	ExprAST *AST_E;
	vector<ExprAST*> AST_Vb;
	ExprAST *AST_D;

	//cout << "Inside Db" << endl;
	AST_Vl = Vl();
	if(VL_FLAG == 1){	
		if(lexeme == "=" && Token_type != STRING){
			//cout << "REed =" << endl;
			gettok();  //changed
			AST_E = E();
			//cout << "Assignment ExprAST" << endl;
			//cout << lexeme << endl;
			return (new AssignExprAST(AST_Vl,AST_E));
		}
		do{
			
			AST_Vb.push_back(Vb());
		}while(lexeme != "=" && Token_type != STRING);
		//cout << "+++++++++++++++++++SIze of VB" << AST_Vb.size() << endl;
		gettok();   //changed
		AST_E = E();
		//cout << "Function ExprAST" << endl;
			//cout << lexeme << endl;
		return (new FuncExprAST(AST_Vl,AST_Vb,AST_E));
	}
	if(Token_type == BRACKET_OPEN){
		gettok();
		AST_D = D();
		if(Token_type != BRACKET_CLOSE) p_error(")");
		return AST_D;
	}
	p_error("Nothing to parse");
	VL_FLAG = 0;
	VB_FLAG = 0;
	return (new ErrorExprAST());
}

/*
	Vb -> ’<IDENTIFIER>’
	   -> ’(’ Vl ’)’
       -> ’(’ ’)’ => ’()’;
*/
ExprAST *Vl(){
	ExprAST *AST_Id;
	vector<ExprAST*> AST_List;

	//cout << lexeme << " : " << Token_type << endl;
	//cout << "Inside Vl" << endl;
	if(Token_type == IDENTIFIER){
		AST_List.push_back(new IdentifierExprAST(lexeme));
		while(gettok() == COMMA){
			if(gettok() != IDENTIFIER) p_error("no identifier found");
			AST_List.push_back(new IdentifierExprAST(lexeme));
		}
		VL_FLAG = 1;
		//cout << "+++++++++++++++++++SIze of VL" << AST_List.size() << endl;
		if(AST_List.size() < 2)
			return (AST_List[0]);
		else
			return (new VariableExprAST(AST_List));
	}
	VL_FLAG = 0;
	return (new ErrorExprAST());
}

/*
	Vl -> ’<IDENTIFIER>’ list ’,’ => ’,’?;
*/
ExprAST *Vb(){
	ExprAST *AST_Vl;
	string temp;

	//cout << "Inside Vb" << endl;
	switch(Token_type){
		case IDENTIFIER: for(int i = 0; i< no_of_keywords ;i++){  //changed
							if(lexeme == reserverd[i]){
								VB_FLAG = 0; 
								return (new ErrorExprAST());
							}
						 } 
						 //changed
						 VB_FLAG = 1; temp = lexeme;
						 gettok(); return (new IdentifierExprAST(temp)); break;

		case BRACKET_OPEN: gettok(); AST_Vl = Vl(); if(VL_FLAG == 1){
								if(Token_type != BRACKET_CLOSE) p_error(")");
								gettok();
								return AST_Vl;
		          		   }
		          		   if(Token_type != BRACKET_CLOSE) p_error(")");
		          		   gettok();
		          		   VB_FLAG = 1;
		          		   return (new ParensExprAST()); break;
		default: VB_FLAG = 0; return (new ErrorExprAST());
	}

}

// function to print the AST
void print(ExprAST *AST, int DEPTH){
	if(DEPTH == -1)
		return;

	DEPTH++; //Variable to keep track of the depth of the tree
	ExprType type = AST->getType();
	for(int i=0; i<DEPTH;i++)
			cout << "." ;
	//cout << AST->getType() << endl;
	if( type == E_VARLIST){
		//cout << "Variable List" << endl; 
				
		VariableExprAST *list = (VariableExprAST*)AST; 
		cout << "," << endl;
		for(int i = 0;i<list->List.size();i++){
			print(list->List[i], DEPTH);
		}
	}else if(type == E_IDENTIFIER){
		IdentifierExprAST *id = (IdentifierExprAST*)AST;
		cout << "<ID:" << id->Val << ">" << endl;
	}else if(type == E_PARENS){
		cout << "()" << endl;
	}else if(type == E_INTEGER){
		IntegerExprAST *i = (IntegerExprAST*) AST;
		cout << "<INT:" << i->Val << ">" << endl;
	}else if(type == E_STRING){
		StringExprAST *s = (StringExprAST*) AST;
		cout << "<STR:'" << s->Val << "'>" << endl;
	}else if(type == E_BOOLEAN){
		BooleanExprAST *b = (BooleanExprAST*) AST;
		string val;
		if(b->Val) val = "true"; else val = "false";
		cout << "<" << val << ">" << endl;
	}else if(type == E_DUMMY){
		cout << "<dummy>" << endl;
	}else if(type == E_NIL){
		cout << "<nil>" << endl;
	}else if(type == E_INFIX){
		InfixExprAST *infix = (InfixExprAST*) AST;
		cout << "@" << endl;;
		print(infix->A,DEPTH);
		print(infix->Identifier,DEPTH);
		print(infix->B,DEPTH);
	}else if(type == E_GAMMA){
		cout << "gamma" << endl;
		GammaExprAST *g = (GammaExprAST *) AST; 
		print(g->Left,DEPTH);
		print(g->Right,DEPTH);
		
	}else if(type == E_BINOP){
		BinaryOpExprAST *bin = (BinaryOpExprAST*) AST;
		if(bin->OP == B_EXP){
			cout << "**" << endl;
		}else if(bin->OP == B_PLUS){
			cout << "+" << endl;

		}else if(bin->OP == B_MINUS){
			cout << "-" << endl;

		}else if(bin->OP == B_MUL){
			cout << "*" << endl;

		}else if(bin->OP == B_DIV){
			cout << "/" << endl;

		}else if(bin->OP == B_GE){
			cout << "ge" << endl;

		}else if(bin->OP == B_GR){
			cout << "gr" << endl;

		}else if(bin->OP == B_LS){
			cout << "ls" << endl;

		}else if(bin->OP == B_LE){
			cout << "le" << endl;

		}else if(bin->OP == B_OR){
			cout << "or" << endl;

		}else if(bin->OP == B_AND){
			cout << "&" << endl;

		}else if(bin->OP == B_NE){
			cout << "ne" << endl;

		}else if(bin->OP == B_EQ){
			cout << "eq" << endl;

		}

		print(bin->Left,DEPTH);
		print(bin->Right,DEPTH);
	}else if(type == E_UNARYOP){
		UnaryOpExprAST *un = (UnaryOpExprAST*) AST;
		if(un->OP == U_NEG){
			cout << "neg" << endl;
			print(un->Node,DEPTH);
		}else if(un->OP == U_NOT){
			cout << "not" << endl;
			print(un->Node,DEPTH);
		}
	}else if(type == E_LET){
		LetExprAST *let = (LetExprAST*) AST;
		cout << "let" << endl;
		print(let->D,DEPTH);
		print(let->E,DEPTH);
	}else if(type == E_WHERE){
		WhereExprAST *w = (WhereExprAST*) AST;
		cout << "where" << endl;
		print(w->T,DEPTH);
		print(w->Dr,DEPTH);
	}else if(type == E_AUG){
		AugExprAST *aug = (AugExprAST*) AST;
		cout << "aug" << endl;
		print(aug->Ta,DEPTH);
		print(aug->Tc,DEPTH);
	}else if(type == E_COND){
		CondExprAST *c = (CondExprAST*) AST;
		cout << "->" << endl;
		print(c->B,DEPTH);
		print(c->Tc_if,DEPTH);
		print(c->Tc_else,DEPTH);
	}else if( type == E_LAMBDA){				
		LambdaExprAST *lam = (LambdaExprAST*)AST; 
		cout << "lambda" << endl;
		for(int i = 0;i<lam->Vb.size();i++){
			print(lam->Vb[i], DEPTH);
		}
		print(lam->E,DEPTH);
	}else if( type == E_TAU){				
		TauExprAST *tau = (TauExprAST*)AST; 
		cout << "tau" << endl;
		for(int i = 0;i<tau->Ta.size();i++){
			print(tau->Ta[i], DEPTH);
		}
	}else if(type == E_ASSIGN){
		AssignExprAST *as = (AssignExprAST*) AST;
		cout << "=" << endl;
		print(as->Vl,DEPTH);
		print(as->E,DEPTH);
	}else if (type == E_FUNC_FORM){
		FuncExprAST *fun = (FuncExprAST*) AST;
		cout << "function_form" << endl;
		print(fun->Identifier,DEPTH);
		for(int i = 0;i<fun->Vb.size();i++){
			print(fun->Vb[i], DEPTH);
		}
		print(fun->E,DEPTH);
	}else if(type == E_REC){
		RecExprAST *re = (RecExprAST*) AST;
		cout << "rec" << endl;
		print(re->Db,DEPTH);
	}else if(type == E_WITHIN){
		WithinExprAST *wit = (WithinExprAST*) AST;
		cout << "within" << endl;
		print(wit->Da, DEPTH);
		print(wit->D, DEPTH);
	}else if(type == E_AND){
		AndExprAST *an = (AndExprAST*) AST;
		cout << "and" << endl;
		for(int i = 0;i<an->Dr.size();i++){
			print(an->Dr[i], DEPTH);
		}
	}

}

ExprAST* transformer(ExprAST* AST){
	

	return AST;
}


