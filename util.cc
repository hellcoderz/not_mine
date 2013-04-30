#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"

#define sc(x,y,z) strncpy(x,y,z)
#define sc2(x,y) strcpy(x,y)
#define sh(x,y) strchr(x,y)
#define cmp(x,y) strcmp(x,y)
#define cmp2(x,y) strncmp(x,y,2)
#define cmp3(x,y) strncmp(x,y,3)
#define cmp4(x,y) strncmp(x,y,4)
#define cmp5(x,y) strncmp(x,y,5)
#define cmp6(x,y) strncmp(x,y,6)

extern "C" {
      
void t_new_node(struct exprAST *AST_node, int n);


struct lex_token *token_root; 
struct ExprAST_stack *root_node = EXIT; 


char *cs(char *tr_rt, int gth){ char *st_1; st_1=(char *) malloc(gth+1); sc(st_1,tr_rt,gth); st_1[gth]='\0'; return st_1;}
void tree_level(int n){  while(n>0) {  printf(".");  n--; }}
void dnn_dp(struct exprAST *AST_node){ if(AST_node->token_type==ID) {  printf("<ID:%s> \n",AST_node->node_name); }else if(AST_node->token_type==INTEGER) {  printf("<INT:%d> \n",AST_node->node_value); }else if(AST_node->token_type==STRING) {  printf("<STR:'%s'> \n",AST_node->node_name); }else {  printf("%s \n",keywords[AST_node->token_type]); }}
void new_node_t(struct exprAST *AST_node, int n) { tree_level(n); dnn_dp(AST_node); if (AST_node->more!=EXIT){  t_new_node(AST_node,n); } else {  if (AST_node->left!=EXIT) {    new_node_t(AST_node->left,n+1);  }  if (AST_node->right!=EXIT) {    new_node_t(AST_node->right,n+1);  } }}
void t_new_node(struct exprAST *AST_node, int n){new_node_t(AST_node->more,n+1);if (AST_node->left!=EXIT){t_new_node(AST_node->left,n);}}
char *remove_spaces_from_both_sides(char *str){int flag;char *str_1=(char *) malloc (sizeof(char)*(strlen(str)+1));char *str_t = sc2(str_1,str); while(*str_1!='\0') { if (*str_1==92) {flag=1;switch(str_1[1]) {case 92: case 39: break; case 't': str_1[1]=9; break;case 'n': str_1[1]=13; break;default : break; } if (flag==1) {str=str_1; while(*str_1!='\0') { str_1[0]=str_1[1]; str_1++;}str_1=str;}}str_1++;}return str_t;}
void add_to_token(struct lex_token *t, char *now,char *tt_me, int token_type){ t->token_len=now - tt_me + 1; t->token_name=(token_type==STRING) ? cs(tt_me+1,t->token_len-2) :         cs(tt_me,t->token_len); t->token_type=token_type; t->token_loc=LNUM; t->next_node=EXIT;}
void token_add1(struct lex_token *t, char *token_name, int token_type){ t->token_len=2; t->token_name=cs(token_name,2); t->token_type=token_type; t->token_loc=LNUM; t->next_node=EXIT;}
int check_for_op(char *tt_me){ char *ops="\"+-*<>&.@/:=~|$!#%^_[]{}`?"; return strchr(ops,*tt_me)!=EXIT;}
int check_for_p(char *tt_me){char *punct="();,";return sh(punct,*tt_me)!=EXIT ;}
int ms(char *tt_me, struct lex_token *t){ char *now=tt_me; if (isspace(*now)) {  while(isspace(*++now));  now--;  if (*now==10) --now;   add_to_token(t,now,tt_me,DELETE);  return 1; } return 0;}
void check_for_rsys(struct lex_token *t){
 if (t->token_len==1) {
  switch (*t->token_name) {case '.' : t->token_type=PERIOD; break;    case '&' : t->token_type=AND; break;    case '*' : t->token_type=TIMES; break;    case '+' : t->token_type=PLUS; break;    case '-' : t->token_type=MINUS; break;    case '/' : t->token_type=DIVIDE; break;    case '@' : t->token_type=INFIX; break;    case '|' : t->token_type=BAR; break;    case '=' : t->token_type=EQUALS; break;  }}
 else if (t->token_len==2) {
  if (cmp2("->",t->token_name)==0) t->token_type=CONDITIONAL;  else  if (cmp2("**",t->token_name)==0) t->token_type=EXPO;  else  if (cmp2(">=",t->token_name)==0) { t->token_type=GE; t->token_name=cs("ge",2); }  else  if (cmp2("<=",t->token_name)==0) { t->token_type=LE;t->token_name=cs("le",2); } }
}

void check_for_rw(struct lex_token *t){
 if (t->token_len==2) {
  if (cmp2("in",t->token_name)==0) t->token_type=IN;  else  if (cmp2("gr",t->token_name)==0) t->token_type=GR;  else  if (cmp2("ge",t->token_name)==0) t->token_type=GE;  else  if (cmp2("ls",t->token_name)==0) t->token_type=LS;  else  if (cmp2("le",t->token_name)==0) t->token_type=LE;  else  if (cmp2("eq",t->token_name)==0) t->token_type=EQ;  else  if (cmp2("ne",t->token_name)==0) t->token_type=NE;  else  if (cmp2("or",t->token_name)==0) t->token_type=OR;  else  if (cmp2("fn",t->token_name)==0) t->token_type=FN;
 }
 else  if (t->token_len==3) {
  if (cmp3("let",t->token_name)==0) t->token_type=LET;  else  if (cmp3("not",t->token_name)==0) t->token_type=NOT;  else  if (cmp3("neg",t->token_name)==0) t->token_type=NEG;  else  if (cmp3("nil",t->token_name)==0) t->token_type=XNIL;  else  if (cmp3("and",t->token_name)==0) t->token_type=AND_NEW;  else  if (cmp3("rec",t->token_name)==0) t->token_type=REC;  else  if (cmp3("aug",t->token_name)==0) t->token_type=AUG;
 }
 else  if ((t->token_len==4) && (cmp4("true",t->token_name)==0)) t->token_type=TRUE; else if (t->token_len==5) {  if (cmp5("where",t->token_name)==0) t->token_type=WHERE;  else  if (cmp5("false",t->token_name)==0) t->token_type=FALSE;  else  if (cmp5("dummy",t->token_name)==0) t->token_type=DUMMY; } if ((t->token_len==6) && (cmp6("within",t->token_name)==0)) t->token_type=WITHIN;
}
int mk_id(char *st_1, struct lex_token *t){ char *us="_"; char *nowow=st_1; if (isalpha(*nowow++)) {   while(isalnum(*nowow) || (strchr(us,*nowow)!=EXIT)) nowow++;   nowow--;   add_to_token(t,nowow,st_1,ID);   check_for_rw(t);   return 1; } return 0;}

int mk_c(char *st_1, struct lex_token *t){ char *nowow=st_1; if ( (*nowow=='/') && (nowow[1]=='/') ) {  nowow+=(strlen(nowow)-2);   add_to_token(t,nowow,st_1,DELETE);  return 1; } return 0;}


int mk_i(char *st_1, struct lex_token *t){ char *nowow=st_1; if (isdigit(*nowow++)) {   while(isdigit(*nowow)) nowow++;    nowow--;   add_to_token(t,nowow,st_1,INTEGER);   return 1; } return 0;}

int mk_d(char *st_1, struct lex_token *t){ char *nowow=st_1; if (check_for_p(nowow)) {   switch(*nowow) {  case '(' : add_to_token(t,nowow,st_1,L_PAREN); break;  case ')' : add_to_token(t,nowow,st_1,R_PAREN); break;  case ';' : add_to_token(t,nowow,st_1,SEMI); break;  case ',' : add_to_token(t,nowow,st_1,COMMA); break;  default: printf("Error in mk_d\n");   }   return 1; } return 0;}

int check2(char **nowow,  int first,  char *second){ char *tr_rt=*nowow; if (*tr_rt==first) {   if (strchr(second,tr_rt[1])!=EXIT) {    return 1;  } } return 0;}

int vw(char **nowow){ char *temp=*nowow; if (*temp==92) {  switch (*(temp+1)) {      case 92 :      case 39 :    case 't':     case 'n': break;     default : return 0;   } (*nowow)++; return 1; } return 0;}

int mk_s(char *st_1, struct lex_token *t){ char *nowow=st_1; if (*nowow==39) {  nowow++;  while(isalnum(*nowow) || isspace (*nowow) || check_for_op(nowow) ||   check_for_p(nowow) || vw(&nowow) ? 1 : 0) nowow++;  if (*nowow==39) {     add_to_token(t,nowow,st_1,STRING);    return 1;   } } return 0;   }

int mk_o(char *st_1, struct lex_token *t){ char *nowow=st_1; if (check_for_op(st_1)) {    switch(*nowow) {       case '>' : if (check2(&nowow,62,"=")) {           token_add1(t,"ge",GE);          return 1;           }            else {          token_add1(t,"gr",GR);           return 1;           }      case '<' : if (check2(&nowow,60,"=")) {           token_add1(t,"le",LE);          return 1;           }            else {          token_add1(t,"ls",LS);           return 1;           }      case '-' : if (check2(&nowow,45,">")) {               add_to_token(t,++nowow,st_1,CONDITIONAL);              return 1;           }           break;      case '*' : if (check2(&nowow,42,"*")) {               add_to_token(t,++nowow,st_1,EXPO);              return 1;           }           break;    }   add_to_token(t,nowow,st_1,OP);   check_for_rsys(t);    return 1; }  return 0;}

struct lex_token *mk_t(){ return  (struct lex_token *) malloc (sizeof(struct lex_token));}

char *run(char *str_2,  struct lex_token **token_root, struct lex_token **mc_cur, struct lex_token **mc_in)  {struct lex_token *temp=*mc_in;  if (*mc_cur==*token_root) {     *token_root=*mc_in;    *mc_cur=*mc_in;  }  if ((*mc_in=mk_t())==EXIT) {}  (*mc_in)->next_node=EXIT;    (*mc_cur)->next_node=*mc_in;  *mc_cur=*mc_in;  return &str_2[temp->token_len];}

struct lex_token *flatten(struct lex_token *token_root){ struct lex_token *temp=token_root, *prev=EXIT; while(temp!=EXIT) {  while (temp->token_type==DELETE) {    if (temp==token_root)  {      token_root=token_root->next_node;      free(temp);       temp=token_root; }    else {  prev->next_node=temp->next_node;      temp=temp->next_node;    }  }   prev=temp;  temp=temp->next_node; } return token_root;}

void G21(), G6(), G11(); 

struct exprAST *get() { struct exprAST *AST_node; AST_node=root_node->AST_node; root_node=root_node->next_node; return AST_node;}

void add(struct exprAST *AST_node){  struct ExprAST_stack *new_top = (struct ExprAST_stack *) malloc (sizeof(struct ExprAST_stack)); new_top->AST_node=AST_node; new_top->next_node=root_node; root_node=new_top;}

void bt(int token_name){ struct exprAST *new_node=(struct exprAST *) malloc (sizeof(struct exprAST)); if (((token_name!=REC)&&(token_name!=NOT)&&(token_name!=NEG))) new_node->right=get(); new_node->left=get(); new_node->token_type=token_name; new_node->more=EXIT; new_node->token_loc=0; add(new_node);}

void bt_multiple(int token_name,int number){ struct exprAST *new_node, *prev_node=EXIT; int left=number; while (left > 0) {  new_node = (struct exprAST*) malloc (sizeof(struct exprAST));  if (prev_node!=EXIT) {prev_node->right=new_node;}  new_node->more=get();  new_node->left=prev_node;  new_node->token_type=token_name;  new_node->token_loc=0;   prev_node=new_node;  left--; } new_node->right=EXIT; add(new_node);}

int get_token() {  return token_root->token_type;}

void read_next_token() {   token_root=token_root->next_node;}

void add_node() {  struct exprAST *new_node = (struct exprAST*) malloc (sizeof(struct exprAST)); new_node->node_name=cs(token_root->token_name,token_root->token_len); new_node->more=EXIT; new_node->left=EXIT; new_node->right=EXIT; new_node->token_type=get_token(); new_node->token_loc=token_root->token_loc; if (token_root->token_type==INTEGER) new_node->node_value=atoi(new_node->node_name); if (token_root->token_type==TRUE) new_node->node_value=1; if (token_root->token_type==FALSE) new_node->node_value=0; if (token_root->token_type==STRING) new_node->name_p=remove_spaces_from_both_sides(new_node->node_name); add(new_node);  read_next_token();}

void add_e_node() { struct exprAST *new_node = (struct exprAST*) malloc (sizeof(struct exprAST)); if (new_node==EXIT) {} new_node->node_name=cs("<()>",4); new_node->more=EXIT; new_node->left=EXIT; new_node->right=EXIT; new_node->token_type=EMPTY_PAREN; add(new_node);}

int G1() {  int n=0; if (get_token()==ID) {  add_node();   while(get_token()==COMMA) {     read_next_token();    if (get_token()!=ID) {}    add_node();    n++;  }  if (n>0) bt_multiple(COMMA,n+1);  return 1; } return 0;}

int G2() {  switch (get_token()) {  case ID : {    add_node();    return 1;  }  case L_PAREN : {    read_next_token();    if (G1()==1) {      if (get_token()!=R_PAREN) {}      read_next_token();       return 1;    }    read_next_token();    add_e_node();     return 1;  }  default : return 0; }}

void G3() { int n=0; if (G1()==1)   {if (get_token()==EQUALS) {    read_next_token();     G21();    bt(EQUALS);    return;  }  while (G2()==1) n++;  if (get_token()!=EQUALS) {}  read_next_token();   G21();  bt_multiple(FUNCT_FORM,n+2);  return;  } if (get_token()==L_PAREN) {  read_next_token();  G6();  if (get_token()!=R_PAREN) {}  read_next_token();  return;  } {}  }  
  
void G4() { if (get_token()==REC) {  read_next_token();  G3();  bt(REC); }  else G3();}

void G5() { int n=1; G4(); if (get_token()==AND_NEW) {  while (get_token()==AND_NEW) {    read_next_token();      G4();     n++;  }  bt_multiple(AND_NEW,n); }}

void G6() { G5(); if (get_token()==WITHIN) {  read_next_token();  G6();  bt(WITHIN); }}

int G7() {  switch (get_token()) {  case ID :  case INTEGER:  case STRING:  case TRUE:  case FALSE:  case XNIL: {add_node(); return 1;}  case L_PAREN: {    read_next_token();    G21();    if (get_token()!=R_PAREN) {}    read_next_token();    return 1;  } } if (get_token()==DUMMY) {  add_node();  return 1; } return 0;} 

void G8() { if (G7()==1) while(G7()==1) bt(GAMMA);}

void G9() { G8(); while (get_token()==INFIX) {  read_next_token();    if (get_token()!=ID) {}  add_node();  G8();  bt_multiple(INFIX,3); }}

void G10() {  int n=0; G9(); while (get_token()==EXPO) {  read_next_token();   G9();  n++; } while(--n >= 0) bt(EXPO);}

void G11() { int mulpti; G10(); mulpti=get_token(); while ( (mulpti==TIMES) || (mulpti==DIVIDE) ) {  read_next_token();  G10();  bt(mulpti);  mulpti=get_token(); }}

void G12() { int mulpti; if (get_token()==MINUS) {  read_next_token();   G11();  bt(NEG); }else  if (get_token()==PLUS) {read_next_token(); G11(); }else G11(); mulpti=get_token(); while ((mulpti==PLUS) || (mulpti==MINUS)) {  read_next_token();   G11();  bt(mulpti);  mulpti=get_token(); }}

void G13() {  int mulpti; G12(); mulpti=get_token(); if ( (mulpti>=GR) && (mulpti<=NE) ) {   read_next_token();  G12();  bt(mulpti); }}

void G14() { if (get_token()==NOT) {  read_next_token();  G13();  bt(NOT); } G13();}

void G15() { G14(); while (get_token()==AND) {   read_next_token();  G14();  bt(AND); }}

void G16() { G15(); while (get_token()==OR) {   read_next_token();  G15();  bt(OR); }}

void G17() { G16(); if (get_token()==CONDITIONAL) {  read_next_token();  G17();  if (get_token()!=BAR) {}  read_next_token();  G17();  bt_multiple(CONDITIONAL,3); }}

void G18() { G17(); while(get_token()==AUG) {  read_next_token();   G17();   bt(AUG); }}

void G19() { int n=1; G18(); if (get_token()==COMMA) {   while(get_token()==COMMA) {    read_next_token();     G18();     n++;  }  bt_multiple(TAU,n); }}

void G20() { G19(); if (get_token()==WHERE) {    read_next_token();    G4();   bt(WHERE);  }}

void G21() { int n=1; switch (get_token()) { case LET : {    read_next_token();     G6();    if (get_token()!=IN) {}    read_next_token();     G21();    bt(LET);    break;        } case FN : {    read_next_token();     G2();    while(get_token()!=PERIOD) {      n++;       G2();    }    read_next_token();     G21();    if (n==1) bt(LAMBDA);    else bt_multiple(LAMBDA,n+1);    break;      } default : G20(); }}
  
struct exprAST *E_2() { G21(); return root_node->AST_node ;}


int hint1(struct exprAST *num_now, int *num); 
void hint0(struct exprAST *num_now, int *num);

struct exprAST *standardize(struct exprAST *ast){
 struct exprAST *num_now=ast;
 int num= 1000; 
 while(num) {
  num=0;
  hint1(num_now,&num);
  }
 return num_now;
}

void ct(struct exprAST *first,struct exprAST *second){
 second->left=first->left;
 second->right=first->right;
 second->more=first->more;
 second->node_name=first->node_name;
 second->token_type=first->token_type;
}

struct exprAST *new_node(int token_type){
 struct exprAST *AST_node= (struct exprAST *) malloc(sizeof(struct exprAST));
 if (AST_node==EXIT) {}
 AST_node->more=EXIT;
 AST_node->left=EXIT;
 AST_node->right=EXIT;
 AST_node->node_name=EXIT;
 AST_node->token_type=token_type;
 return AST_node;
}

int make_tt(struct exprAST *num_now){
 struct exprAST *hit1, *miss1, *get1, *set1, *make1;
 switch (num_now->token_type) {
  case LET : if (num_now->left->token_type!=EQUALS) break; {    hit1=num_now->left->right;    num_now->left->right=num_now->right;    num_now->right=hit1;    num_now->left->token_type=LAMBDA;    num_now->token_type=GAMMA;    return 1;    }
  case WHERE : if (num_now->right->token_type!=EQUALS) break; {    hit1=num_now->left;    miss1=num_now->right->right;    num_now->left=num_now->right;    num_now->right=miss1;    num_now->left->right=hit1;    num_now->token_type=GAMMA;    num_now->left->token_type=LAMBDA;    return 1;    }
  case REC : if (num_now->left->token_type!=EQUALS) break; {     hit1=num_now->left->right;    ct(num_now->left,num_now);    num_now->right=new_node(GAMMA);    num_now->right->left=new_node(YSTAR);    num_now->right->right=new_node(LAMBDA);    num_now->right->right->right=hit1;     num_now->right->right->left=num_now->left; return 1;    }
  case INFIX : {     hit1=num_now->more;    miss1=num_now->left->more;    get1=num_now->left->left->more;    num_now->token_type=GAMMA;    num_now->left->right=num_now->left->left;    num_now->right=get1;    num_now->left->right=hit1;    num_now->left->left=miss1;    num_now->more=EXIT;    num_now->left->more=EXIT;    num_now->left->token_type=GAMMA;    return 1;    }
  case WITHIN : if (!((num_now->left->token_type==EQUALS) && (num_now->right->token_type==EQUALS))) break; {    hit1=num_now->right->right;    miss1=num_now->right->left;    num_now->right->left=new_node(0);    num_now->right->right=num_now->left->right;    num_now->right->left->left=num_now->left->left;    num_now->left=miss1;    num_now->right->left->right=hit1;    num_now->token_type=EQUALS;    num_now->right->token_type=GAMMA;    num_now->right->left->token_type=LAMBDA;    return 1;    }
  case AND_NEW: {hit1=num_now; while(hit1!=EXIT) {      if (hit1->more->token_type!=EQUALS) return 0;       hit1=hit1->left;}         hit1=num_now;        set1=miss1=new_node(COMMA);        make1=get1=new_node(TAU);        while(hit1->left!=EXIT) {      miss1->more=hit1->more->left;      get1->more=hit1->more->right;      miss1->left=new_node(COMMA);      get1->left=new_node(TAU);      miss1=miss1->left;      get1=get1->left;      hit1=hit1->left;        }        miss1->more=hit1->more->left;        get1->more=hit1->more->right;        num_now->left=set1;        num_now->right=make1;        num_now->more=EXIT;        num_now->token_type=EQUALS;        return 1;  }
  case FUNCT_FORM: case LAMBDA : {    if (num_now->more==EXIT) break;    get1=hit1=new_node(LAMBDA);    miss1=num_now->left;    while(miss1->left!=EXIT) {      hit1->right=new_node(LAMBDA);      hit1->left=miss1->more;      miss1=miss1->left;      if (miss1->left==EXIT) hit1->right=miss1->more;      hit1=hit1->right;    }    num_now->right=get1;    if (num_now->token_type==FUNCT_FORM) num_now->token_type=EQUALS;    num_now->left=num_now->more;    num_now->more=EXIT;        return 1;  }      
 }
 return 0;
}

int hint1(struct exprAST *num_now, int *num){  if (num_now!=EXIT) if (make_tt(num_now)) (*num)++;  if (num_now->more!=EXIT) hint0(num_now,num); else {  if (num_now->left!=EXIT) hint1(num_now->left,num);  if (num_now->right!=EXIT) hint1(num_now->right,num); } return *num;}

void hint0(struct exprAST *num_now, int *num){ if (num_now->more!= EXIT) hint1(num_now->more,num);  if (num_now->left!=EXIT) hint0(num_now->left,num);}




void Main(char *file_name){
 FILE *fp;
 char free_space[MAX_LINE_LENGTH+1];
 char *stp;
 struct lex_token *mkcur,*mkin;

 int i;


 if ((fp=fopen(file_name,"r"))==EXIT) {
	exit(1);
 }

 if ((mkin=mk_t())==EXIT) {}
 mkcur=token_root=mkin;
 stp=free_space; 
 while(fgets(stp,MAX_LINE_LENGTH,fp)!=EXIT) {  
	LNUM++;
 	while(*stp!=10) { 
		if ((ms(stp,mkin)) ||(mk_id(stp,mkin)) || (mk_s(stp,mkin)) || (mk_c(stp,mkin)) || (mk_o(stp,mkin)) || (mk_i(stp,mkin)) || (mk_d(stp,mkin)) ) { 
			stp=run(stp,&token_root,&mkcur,&mkin);}
		
	}
 stp=free_space;
 }
 //close(fp);
 
}



//function to print the program as it is
void print_prog(){
	char ch;
	ch = in.get();
	while(ch != EOF){
		cout << ch ;
		ch = in.get();
	}
}

}