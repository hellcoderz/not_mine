#include <iostream>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <string.h>

void print_cse(ExprAST *AST, int DEPTH){
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


extern "C" {
struct env *senv(struct cse_object *s) {    while (s->token_type!=ENV) s=s->next_node;  return  s->val.epsilon;}

struct cse_object *new_root(int tt) {
  struct cse_object *c = (struct cse_object *) malloc(sizeof(struct cse_object));
  if (c==EXIT) exit(1);
  c->token_type=tt;
  return c;
}


struct zta *acnt(int k,struct exprAST *AST_node){ 
  struct cse_object *it=new_root(EXIT);
  struct zta *l = (struct zta *) malloc(sizeof(struct zta));
  it->val.t=AST_node; 
  l->k=k;
  l->AST_node=it;
  l->c=EXIT;
  return l;
}


struct mta *scnt()  
{
  struct mta *mlb = (struct mta *) malloc(sizeof(struct mta));
  mlb->G19 = EXIT;
  mlb->F = EXIT;
  return mlb;
}


struct delta *dcnt() 
{
  struct delta *d = (struct delta *) malloc(sizeof(struct delta));
  d->cse_st=EXIT;
  d->number=0;
  d->next_node=EXIT;
  return d;
}


struct cse_object *lngt(struct cse_object *clst){ 
 if (clst==EXIT) return EXIT;
 while(clst->next_node!=NULL) clst=clst->next_node;
 return clst;
}


struct cse_object *iter(struct cse_object *clst){ 
 if (clst->next_node==EXIT) return EXIT;
 while(clst->next_node->next_node!=EXIT) clst=clst->next_node;
 return clst;
}


struct delta *dmem(struct delta *clst){ 
 if (clst==EXIT) return EXIT;
 while(clst->next_node!=EXIT) clst=clst->next_node;
 return clst;
}



struct cse_object *create_environment(struct env *AST_node){
 struct cse_object *temp = new_root(ENV);
 temp->val.epsilon=AST_node;
 return temp;
}


struct cse_object *dcpy(struct cse_object *AST_node)  
{
 struct cse_object *temp=new_root(AST_node->token_type);
 if ( (AST_node->token_type==LAMBDA)||(AST_node->token_type==ETA)) {
  temp->val.l=(struct zta *) malloc(sizeof (struct zta));
  temp->val.l->k=AST_node->val.l->k;
  temp->val.l->c=AST_node->val.l->c;
  temp->val.l->AST_node=dcpy(AST_node->val.l->AST_node);
 }

 else temp->val=AST_node->val;
 return temp;
}


struct cse_object *dcpy_all(struct cse_object *AST_node) 

{
 struct cse_object *temp=dcpy(AST_node);
 struct cse_object *token_root=temp;
 while(AST_node->next_node!=EXIT) { 
  temp->next_node=dcpy(AST_node->next_node);
  temp=temp->next_node;
  AST_node=AST_node->next_node;
 }
 temp->next_node=EXIT;
 return token_root;
}



struct cse_object *d_scan(struct delta *d,int number)
{struct delta *d_temp=d;
 while(d_temp!=EXIT) {
  if (d_temp->number==number) return dcpy_all(d_temp->cse_st);
  d_temp=d_temp->next_node;
 }
 printf("Unable to locate delta #%d!\n",number);
 return EXIT;
}


struct cse_object *mke_it(int token_type,int v) 
{
 struct cse_object *temp_s=new_root(ANY);
 struct exprAST *temp_t=(struct exprAST *) malloc (sizeof(struct exprAST));
 temp_t->token_type=token_type;
 temp_t->token_loc=0;
 temp_t->node_value=v;
 temp_s->val.t=temp_t;
 return temp_s;
}


struct cse_object *generate_true_false(int v) 
{
 struct cse_object *temp_s=new_root(ANY);
 struct exprAST *exprAST=(struct exprAST *) malloc (sizeof(struct exprAST));
 exprAST->token_loc=0;
 exprAST->node_value=v;
 exprAST->token_type = (v ? TRUE : FALSE );
 temp_s->val.t=exprAST;
 return temp_s;
}


void make_root(struct cse_object *nh,struct cse_object **oh) 
 
{
 nh->next_node=*oh;
 (*oh)=nh;
}


int math_power(int AST_node,int y) 
{int t,total=1;
 if (y<0) return 0; 
 if (y==0) return 1;
 for(t=0;t<y;t++) total *= AST_node;
 return total;
}


int boolean(int AST_node) 
{return (AST_node==TRUE)||(AST_node==FALSE);}


int true_type(struct cse_object *AST_node) 
{
 switch (AST_node->token_type) {
  case ANY:
  case UNARYOP: 
  case ARITHMATIC_BINOP:
  case LOGICAL_BINOP: return AST_node->val.t->token_type;
  default: return AST_node->token_type;
 }
}


void pins(struct cse_object *c) 

{struct cse_object *temp;
 temp=iter(c);
 if (temp!=EXIT) temp->next_node=EXIT;
 else c->token_type=EMPTY;
}



void add_env(struct env *tr_rt,struct env *num_now)

{
 struct child_env *token_root;
 struct child_env *temp = (struct child_env *) malloc(sizeof(struct child_env));
 temp->num_now=num_now;
 temp->next_node=EXIT;
 token_root=tr_rt->next_node;
 while(token_root->next_node!=EXIT) token_root=token_root->next_node; 
 token_root->next_node=temp;
};


struct env *gene_es(int e_num,struct cse_object *e_from,struct cse_object *e_to,struct env *where)
{
 struct env *temp = (struct env *) 
          malloc(sizeof(struct env));
 struct child_env *miss1 = (struct child_env *) malloc(sizeof(struct child_env));

 miss1->next_node=EXIT; 
 miss1->num_now=EXIT;
 temp->next_node=miss1; 
 temp->prev=where; 
 temp->number=e_num; 
 temp->from=e_from; 
 temp->to=e_to;

 if (where==EXIT) return temp; 

 if (where->next_node->num_now==EXIT) 
  where->next_node->num_now=temp;
 else 
  add_env(where,temp); 
 return (temp);
}


void pp_10(struct cse_object *s); 

int chk_fn(struct cse_object *it)
{
 char *node_name;
 switch (it->token_type) {
  case LAMBDA:  return LAMBDA; 
  case ETA   :  return ETA;    
  case TUPLE :  return TUPLE;  
 }
 if (it->token_type==ANY) {
  if (it->val.t->token_type==ID) {
    node_name=it->val.t->node_name;
    if (!cmp("Print",node_name)) return PRINT;
    if (!cmp("print",node_name)) return PRINT; 
    if (!cmp("Istuple",node_name)) return ISTUPLE;
    if (!cmp("Isfunction",node_name)) return ISFUNCTION;
    if (!cmp("Isdummy",node_name)) return ISDUMMY;
    if (!cmp("Isstring",node_name)) return ISSTRING;
    if (!cmp("Isinteger",node_name)) return ISINTEGER;
    if (!cmp("Istruthvalue",node_name)) return ISTRUTHVALUE;
    if (!cmp("Conc",node_name)) return CONC;
    if (!cmp("func_conc",node_name)) return CONC; 
    if (!cmp("ItoS",node_name)) return ITOS;
    if (!cmp("Order",node_name)) return ORDER;
    if (!cmp("Stern",node_name)) return STERN;
    if (!cmp("Stem",node_name)) return STEM;
    if (!cmp("Null",node_name)) return XNULL;
  }
  if (it->val.t->token_type==YSTAR) return YSTAR; 
 }
 return UNKNOWN;
}


void R3(struct cse_object *c,struct cse_object **s,struct delta *d,int *env)
{
 struct cse_object *temp_c;
 struct cse_object *s_top=*s;
 struct cse_object *s_temp;
 struct exprAST *temp_t; 
 struct cse_object *temp_x;
 int close_hold=s_top->val.l->k;
 struct env *close_env=s_top->val.l->c;

 
 if (true_type(s_top->val.l->AST_node)==COMMA) {
  if (true_type(s_top->next_node)!=TUPLE) { 
    exit(1);
  }

  temp_t=s_top->val.l->AST_node->val.t; 
  s_top=s_top->next_node->val.tup->cse_mem; 
  *s=(*s)->next_node->next_node;  

  while(temp_t!=EXIT) {
    if (s_top==EXIT) {
       exit(1);
    }
    (*env)++; 
    temp_x=mke_it(ID,0);
    temp_x->val.t->node_name=temp_t->more->node_name; 
    close_env=gene_es(*env,temp_x,s_top,close_env);
    s_temp=create_environment(close_env); 
    make_root(s_temp,s);
    temp_c=lngt(c);
    temp_c->next_node=create_environment(close_env);
    temp_t=temp_t->left;
    s_top=s_top->next_node;
  }
 }
 else { 
    if (s_top->next_node!=EXIT) {
    *s=(*s)->next_node->next_node;  
      (*env)++;
      close_env=gene_es(*env,
      dcpy(s_top->val.l->AST_node),dcpy(s_top->next_node),close_env);
    s_temp=create_environment(close_env); 
    make_root(s_temp,s);
    temp_c=lngt(c);
    temp_c->next_node=create_environment(close_env);
    }
  }
 temp_c=lngt(c);
 temp_c->next_node=d_scan(d,close_hold); 
}


void pp_12(struct cse_object *piece)
{
 char *temp,*miss1;
 switch (true_type(piece)) {
  case INTEGER :  printf("%d",piece->val.t->node_value);break;
  case STRING : temp=piece->val.t->name_p;      miss1=temp;      while((miss1=strchr(temp,13))!=EXIT) {        *miss1='\0';        printf("%s\n",temp);         *miss1=13;        temp=miss1+1;      }      printf("%s",temp);      break;
  case DUMMY :  printf("dummy");break;
  case TRUE : printf("true");break;
  case FALSE :  printf("false");break;
  case LAMBDA:  printf("[lambda closure: %s: %d]",        piece->val.l->AST_node->val.t->node_name,        piece->val.l->k); break;
  case ETA: printf("[eta closure: %s: %d]",       piece->val.l->AST_node->val.t->node_name,      piece->val.l->k); break; 
  case XNIL :   printf("nil"); break;
  case TUPLE: pp_10(piece);break;
  default : break;
    dnn_dp(piece->val.t);
 }
}


void pp_10(struct cse_object *s) { 
  struct cse_object *temp;
 printf("(");
 temp=s->val.tup->cse_mem;
 while(temp!=EXIT) {
  pp_12(temp);
  temp=temp->next_node;
  if (temp!=EXIT) printf(", ");
 }
 printf(")");
}


void Print(struct cse_object **s)
{ 
 *s=(*s)->next_node;
 if ((*s)->token_type!=TUPLE) pp_12(*s);
 else pp_10(*s);
 (*s)=(*s)->next_node; 
 make_root(mke_it(DUMMY,0),s); 
}


void IsSomething(int funct,struct cse_object **s) 
{
 int token_type;
 *s=(*s)->next_node; 
 token_type = true_type(*s);
 *s=(*s)->next_node; 
 switch (funct) {
  case ISINTEGER : make_root(generate_true_false(token_type==INTEGER),s); break;
  case ISTUPLE : make_root(generate_true_false(token_type==TUPLE||token_type==XNIL),s);
               break;
  case ISSTRING : make_root(generate_true_false(token_type==STRING),s); break;
  case ISDUMMY : make_root(generate_true_false(token_type==DUMMY),s); break;
  case ISTRUTHVALUE : make_root(generate_true_false(boolean(token_type)),s); break;
  case ISFUNCTION : make_root(generate_true_false(token_type==LAMBDA||token_type==ETA),s);
                  break;
  case XNULL : make_root(generate_true_false(token_type==XNIL),s); break;
  default : printf("This point should never be reached (IsSomething)\n");
 }
}


void func_conc(struct cse_object *c,struct cse_object **s) {
 struct cse_object *AST_node=mke_it(STRING,0);
 char *s1,*s2;
 pins(c); 
 *s=(*s)->next_node; 
 if ( (true_type(*s)!=STRING) || (true_type((*s)->next_node)!=STRING)) {
  printf("Non-strings used in func_conc call\n");
  free(AST_node);
  exit(1);
 }
 s1=(*s)->val.t->name_p;
 s2=(*s)->next_node->val.t->name_p;

 AST_node->val.t->name_p=cs(s1,strlen(s1)+strlen(s2));
 AST_node->val.t->name_p=strcat(AST_node->val.t->name_p,s2);
 (*s)=(*s)->next_node->next_node; 
 make_root(AST_node,s);
}


void func_order(struct cse_object **s)
{
 struct cse_object *AST_node;
 int i;
 i=true_type((*s)->next_node);
 if (! ( (i==TUPLE) ||  (i==XNIL) ) ) {
  printf("Attempt to find the func_order of a non-tuple in line %d\n",
  (*s)->val.t->token_loc);
  exit(1);
 }
 *s=(*s)->next_node; 
 i = ( (i==XNIL) ? 0 : (*s)->val.tup->number);
 AST_node=mke_it(INTEGER,i);
 *s=(*s)->next_node; 
 make_root(AST_node,s); 
}


void func_ts(struct cse_object **s) 
{
 struct cse_object *AST_node;
 int val;
 if (true_type((*s)->next_node)!=INTEGER) {
  printf("Invalid tuple selection (non-integer)\n");
  exit(1);
 }
 val=(*s)->next_node->val.t->node_value;
 if ( (val <= 0) || (val > (*s)->val.tup->number) ) {
  printf("The tuple selection node_value %d out of range\n",val);
  exit(1);
 }

 AST_node=(*s)->val.tup->cse_mem;
 while(val-->1) AST_node=AST_node->next_node;
 AST_node=dcpy(AST_node);
 (*s)=(*s)->next_node->next_node; 
 make_root(AST_node,s);
}


void R13(struct cse_object *c,struct cse_object  **s) 

{
 struct zta *l=(struct zta *) malloc (sizeof(struct zta));
 struct cse_object *g11=new_root(GAMMA);
 struct cse_object *g12=new_root(GAMMA);
 struct cse_object *ldm1=new_root(LAMBDA); 

 c=lngt(c);
 c->next_node=g11;
 g11->next_node=g12; 
 ldm1->val.l=l;
 l->k=(*s)->val.l->k;
 l->c=(*s)->val.l->c;
 l->AST_node=dcpy((*s)->val.l->AST_node);
 make_root(ldm1,s); 
}


void R12(struct cse_object **s)  

{
 struct zta *e=(struct zta *) malloc (sizeof(struct zta));
 struct cse_object *AST_node=new_root(ETA);

 *s=(*s)->next_node; 

 
 
 AST_node->val.l=e;
 e->k=(*s)->val.l->k; 
 e->c=(*s)->val.l->c; 
 e->AST_node=dcpy((*s)->val.l->AST_node); 
 *s=(*s)->next_node; 
 make_root(AST_node,s); 
}


void Stem(struct cse_object **s) 

{
 struct cse_object *temp;
 char *t;



 temp=mke_it(STRING,0);
 t=(char *) malloc( (sizeof(char)*2) );
 t[0]=(*s)->next_node->val.t->name_p[0];
 t[1]='\0';
 temp->val.t->name_p=t;
 (*s)=(*s)->next_node->next_node;
 make_root(temp,s);
} 


void Stern(struct cse_object **s)  

{
 struct cse_object *temp;
 char *t,*AST_node;
 int len;
 
 AST_node=(*s)->next_node->val.t->name_p;

 temp=mke_it(STRING,0);
 len=strlen(AST_node);

 t=(char *) malloc( (sizeof(char) * len) );
 strcpy(t,AST_node+1);
 temp->val.t->name_p=t;
 (*s)=(*s)->next_node->next_node;
 make_root(temp,s);
}

 
void ItoS(struct cse_object **s)  

{
 struct cse_object *temp;
 int node_value,negative,rem,digit=20;  
 char *t=(char *) malloc(digit);
 
 node_value=(*s)->next_node->val.t->node_value;
 temp=mke_it(STRING,0);
 t[--digit]='\0';
 t[--digit]='0';
 if (node_value!=0) {
  negative=(node_value<0);
  node_value=abs(node_value);
  while(node_value>0) {
    rem=node_value % 10;
    t[digit--]='0'+rem;
    node_value=(node_value-rem)/10;
  }
  if (negative) t[digit--]='-';
  t+=(digit+1);
 }
 else 
  t+=digit;

 (*s)=(*s)->next_node->next_node;
 temp->val.t->name_p=t;
 make_root(temp,s);
}



struct cse_object *partition(struct exprAST *st,int *i,struct delta *d)
{
 struct cse_object *i_temp;
 struct cse_object *m;
 int counter=0;

 if (st==EXIT) return EXIT;

 m=new_root(ANY);
 switch (st->token_type) {
  case ID: 
  case INTEGER:
  case STRING:
  case TRUE:
  case FALSE: 
  case XNIL:
  case YSTAR:
  case DUMMY : { 
    m->val.t = st;
    m->next_node=EXIT;
    return m;
  }
  case LAMBDA : {  
    (*i)++;  
    m->val.l = acnt(*i,st->left); 
    m->token_type=LAMBDA;
    d=dmem(d); 
    d->next_node=dcnt(); 
    d=d->next_node;
    d->number=*i; 
    d->cse_st=partition(st->right,i,d);
    return m;
  }
  case NEG:
  case NOT : {
    m->val.t = st;
    m->token_type=UNARYOP;
    m->next_node=partition(st->left,i,d);
    return m;
  }
  case OR:
  case AND: {
    m->val.t = st;
    m->token_type=LOGICAL_BINOP;
    m->next_node=partition(st->left,i,d);
    i_temp=m;
    m=lngt(m);
    m->next_node=partition(st->right,i,d);
    return i_temp;
  }
  case GR:
  case GE:
  case LS:
  case LE:
  case EQ:
  case NE: 
  case PLUS:
  case MINUS:
  case TIMES:
  case DIVIDE:
  case EXPO : {
    m->val.t = st;
    m->token_type=ARITHMATIC_BINOP;
    m->next_node=partition(st->left,i,d);
    i_temp=m;
    m=lngt(m);
    m->next_node=partition(st->right,i,d);
    return i_temp;
  }
  case AUG : { 
    m->val.t = st;
    m->token_type=AUG;
    m->next_node=partition(st->left,i,d);
    i_temp=m;
    m=lngt(m);
    m->next_node=partition(st->right,i,d);
    return i_temp;
  }
  case CONDITIONAL : { 
    m->token_type = BETA;
    m->val.b=scnt(); 
    i_temp=m;
    (*i)++; 
    d=dmem(d);
    d->next_node=dcnt();
    d=d->next_node;
    d->number=*i;
    d->cse_st=partition(st->left->more,i,d);
    m->val.b->G19 = d; 
    (*i)++; 
    d=dmem(d);
    d->next_node=dcnt();
    d=d->next_node;
    d->number=*i;
    d->cse_st=partition(st->left->left->more,i,d);
    m->val.b->F = d; 
    m->next_node=partition(st->more,i,d);
    return i_temp;
  }
  case GAMMA : { 
    m->val.t = st;
    m->token_type=GAMMA;
    m->next_node=partition(st->left,i,d);
    i_temp=m;
    m=lngt(m);
    m->next_node=partition(st->right,i,d);
    return i_temp;
  }
  case TAU :  { 
    m->token_type=TAU;
    i_temp=m;
    while (st!=EXIT) {
      counter++; 
      m->next_node=partition(st->more,i,d);
      m=lngt(m);
      st=st->left;
    }
    i_temp->val.tau=counter;
    return i_temp;
  }
  default: break;
     exit(1);
  }
 return EXIT; 
}



void func_lup(struct cse_object **c,struct cse_object *s)

{
 struct env *e_temp=senv(s);
 while (e_temp!=EXIT) {
  if (e_temp->from==EXIT) break; 
  
  

  if (strcmp(e_temp->from->val.t->node_name,(*c)->val.t->node_name)==0) {
    
    *c=dcpy(e_temp->to);
    return; 
  }
 e_temp=e_temp->prev; 
 }

 if (chk_fn(*c)!=UNKNOWN) return; 

 
 exit(1);
}
 

void R1(struct cse_object *c,struct cse_object **s) 

{
 struct cse_object *temp_c=lngt(c); 
 
 pins(c);  
 if (true_type(temp_c)==ID) 
  func_lup(&temp_c,*s);  
 temp_c->next_node=*s; 
 *s=temp_c;
}


void R2(struct cse_object *c,struct cse_object **s) 

{
 struct cse_object *temp_c=lngt(c);
 struct cse_object *AST_node=new_root(LAMBDA);
 AST_node->val.l=acnt(temp_c->val.l->k,temp_c->val.l->AST_node->val.t);
 AST_node->val.l->c=senv(*s); 
 
 pins(c); 
 make_root(AST_node,s);
}


void R3E(struct cse_object *c,struct cse_object **s,struct delta *d,int *env) 

{
 int funct_type=chk_fn(*s); 
 pins(c); 
 switch (funct_type) { 
  case LAMBDA: R3(c,s,d,env); break;
  case PRINT: Print(s); break;
  case XNULL : 
  case ISINTEGER :
  case ISTUPLE : 
  case ISDUMMY :
  case ISTRUTHVALUE:
  case ISFUNCTION:
  case ISSTRING: IsSomething(funct_type,s); break;
  case CONC: func_conc(c,s); break;
  case ORDER: func_order(s); break;
  case TUPLE: func_ts(s); break;
  case YSTAR: R12(s);break;
  case STEM: Stem(s); break;
  case ITOS: ItoS(s); break;
  case STERN: Stern(s); break;
  case ETA: R13(c,s); break;
  default : break;


    exit(1);
 }
}


void R5(struct cse_object *c,struct cse_object **s) 
{
 struct cse_object *temp_s=*s;
 struct cse_object *temp2_s=*s;
 struct cse_object *temp_c=lngt(c);
 pins(c); 

 if (temp_s->token_type==ENV) *s=(*s)->next_node; 
 else {
  while (temp_s->token_type!=ENV) temp_s=temp_s->next_node;
  while (temp2_s->next_node!=temp_s) temp2_s=temp2_s->next_node;
  temp2_s->next_node=temp_s->next_node; 
 }
}


void R65(struct cse_object *c,struct cse_object **s) 
{
 struct tuple *tup;
 struct cse_object *temp,*miss1;
 int token_type=true_type(*s);
 
 if ( (token_type!=TUPLE) && (token_type!=XNIL)) { 
  exit(1);
 } 

 pins(c);  

 if (token_type==TUPLE) {
  tup=(*s)->val.tup;  
  tup->number++;    
  temp=tup->cse_mem; 
  while(temp->next_node!=EXIT) temp=temp->next_node; 
  temp->next_node=((*s)->next_node); 
  miss1=temp;
  temp=dcpy(*s);
  temp->val.tup=tup;
  (*s)=(*s)->next_node->next_node;
  miss1->next_node->next_node=EXIT; 
  }
 else { 
  tup=(struct tuple *) malloc(sizeof(struct tuple)); 
  tup->cse_mem=((*s)->next_node); 
  tup->number=1;
  temp=new_root(TUPLE);
  temp->val.tup=tup;
  (*s)=(*s)->next_node->next_node;
  tup->cse_mem->next_node=EXIT; 
  }
 make_root(temp,s);
}


void R6(struct cse_object *c,struct cse_object **s) 
{
 struct cse_object *temp_s=*s;
 struct cse_object *temp_c=lngt(c);
 int i1,i2;
 if (temp_c->token_type==ARITHMATIC_BINOP) {
  i1=temp_s->val.t->token_type;
  i2=temp_s->next_node->val.t->token_type;
 
  if (i1==STRING) {
    switch (temp_c->val.t->token_type) {
    case PLUS:
    case MINUS:
    case TIMES:
    case DIVIDE: break;
     exit(1);
      } 
  }

  if (i1==INTEGER) { 
    i1=(*s)->val.t->node_value;
    i2=(*s)->next_node->val.t->node_value;
  } 
  else i1=strcmp((*s)->val.t->name_p,(*s)->next_node->val.t->name_p);
  *s=(*s)->next_node->next_node; 
  pins(c); 
  if (temp_s->val.t->token_type==INTEGER) {

  switch (temp_c->val.t->token_type) { 
    case PLUS  :make_root(mke_it(INTEGER,i1+i2),s); break;
    case MINUS :make_root(mke_it(INTEGER,i1-i2),s); break;
    case TIMES :make_root(mke_it(INTEGER,i1*i2),s); break;
    case DIVIDE:make_root(mke_it(INTEGER,i1/i2),s); break;
    case EXPO  :make_root(mke_it(INTEGER,math_power(i1,i2)),s); break;
    case GR : make_root(generate_true_false(i1>i2),s);  break;
    case GE :   make_root(generate_true_false(i1>=i2),s); break;
    case LS :   make_root(generate_true_false(i1<i2),s);  break;
    case LE :   make_root(generate_true_false(i1<=i2),s); break;
    case EQ :   make_root(generate_true_false(i1==i2),s); break;
    case NE :   make_root(generate_true_false(i1!=i2),s); break;
    default : break;
      exit(1);
    }
  } 
  else {

  switch (temp_c->val.t->token_type) {  
    case GR : make_root(generate_true_false(i1>0),s);  break;
    case GE : make_root(generate_true_false(i1>=0),s); break;
    case LS : make_root(generate_true_false(i1<0),s);  break;
    case LE : make_root(generate_true_false(i1<=0),s); break;
    case EQ : make_root(generate_true_false(i1==0),s); break;
    case NE : make_root(generate_true_false(i1!=0),s); break;
    default : break;
      exit(1);
    }
  } 
 } 
 else { 

  i1=temp_s->val.t->token_type; 
  i2=temp_s->next_node->val.t->token_type; 
  if ((!boolean(temp_s->val.t->token_type))||(!boolean(temp_s->val.t->token_type))){
      
      exit(1);
  }
 
  i1=(*s)->val.t->node_value;
  i2=(*s)->next_node->val.t->node_value;

  *s=(*s)->next_node->next_node; 
  pins(c); 

  switch (temp_c->val.t->token_type) {
    case OR : make_root(generate_true_false(i1 || i2),s); break;
    case AND: make_root(generate_true_false(i1 && i2),s); break;
    default : break;
        exit(1);
  }
 }
}


void R7(struct cse_object *c,struct cse_object **s) 
{
 struct cse_object *temp_s=*s;
 struct cse_object *temp_c=lngt(c);
 

 if (temp_c->val.t->token_type==NOT) {
  if (!boolean(true_type(temp_s))) {
   
    exit(1);
  }
  (*s)=(*s)->next_node; 
  make_root(generate_true_false(! temp_s->val.t->node_value),s);
 }
 else {
  if (true_type(temp_s)!=INTEGER) {
 
    exit(1);
  }
  (*s)=(*s)->next_node; 
  make_root(mke_it(INTEGER,- temp_s->val.t->node_value),s);
 }
 pins(c);  
}


void R8(struct cse_object *c,struct cse_object **s) 

{
 struct cse_object *temp_s=*s;
 struct cse_object *temp_c=lngt(c);
 struct mta *temp_b;

 if (!(boolean(temp_s->val.t->token_type))) {
  exit(1);
 }
 temp_b=temp_c->val.b;
 pins(c); 
 temp_c=lngt(c);
 temp_c->next_node = dcpy_all ((temp_s->val.t->node_value) ? temp_b->G19->cse_st : 
           temp_b->F->cse_st);
 (*s)=(*s)->next_node; 
}


void R9(struct cse_object *c,struct cse_object **s) 

{
 struct cse_object *temp_c=lngt(c);
 struct tuple *tup = (struct tuple *) malloc(sizeof(struct tuple)); 
 struct cse_object *temp_s=(*s);
 int number=temp_c->val.tau;

 while(temp_c->val.tau > 1) { 
  if (temp_s==EXIT) {
    exit(1); 
  }
  temp_s=temp_s->next_node;
  temp_c->val.tau--;
 }
 pins(c); 
 tup->cse_mem=(*s);  
 (*s)=temp_s->next_node; 
 temp_s->next_node=EXIT;  
 temp_c=new_root(TUPLE); 
 temp_c->val.tup=tup; 
 temp_c->val.tup->number=number;
 make_root(temp_c,s);
}



struct env * cse_machine(struct delta *d_head)

{
 struct cse_object *vctl, *meh, *seh; 
 int env_num=0;  
 struct env *keh=gene_es(env_num,EXIT,EXIT,EXIT); 
 meh=vctl=create_environment(keh); 
 seh=create_environment(keh); 
 vctl->next_node=d_head->cse_st; 

 while (meh->token_type!=EMPTY) {
  if (env_num>10000) {
    exit(1);
 
  }
  vctl=lngt(meh);

  switch(vctl->token_type) { 
    case TUPLE:
    case ANY: R1(meh,&seh); break;
    case LAMBDA: R2(meh,&seh); break;
    case GAMMA: R3E(meh,&seh,d_head,&env_num); 
      break;
    case ENV: R5(meh,&seh);break;
    case ARITHMATIC_BINOP: 
    case LOGICAL_BINOP: R6(meh,&seh); break;
    case UNARYOP: R7(meh,&seh); break;
    case BETA: R8(meh,&seh); break;
    case TAU: R9(meh,&seh); break;
    case AUG: R65(meh,&seh); break; 
    default:  exit(1);
  }
 }
 
 printf("\n"); //cout
 return keh;
}


void interpret(struct exprAST *st) 

{
 struct delta *d_temp=dcnt();
 struct delta *d_head=d_temp;
 struct env *e_head;
 int counter=0;
 d_head->number=counter;
 d_head->cse_st=partition(st,&counter,d_temp);
 e_head = cse_machine(d_head);

}

//the mainloop to generate the AST in top down recursive decent parsing
void Mainloop(char *file_name){
 struct exprAST *AST,*st;
 Main(file_name);
 token_root=flatten(token_root);      
 AST=E_2();         
 new_node_t(AST,0); 
 st=standardize(AST); 
 interpret(st); 
  
}

}