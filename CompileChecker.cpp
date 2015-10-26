#include <cmath>
#include <cstdio>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include "CompileChecker.h"

using namespace std;
/**
 * Constructor
 */
CompileChecker :: CompileChecker(){
	varCount=0;
	labelCount=0;
	tokenNumber=0;
}
/**
 * Pushes given string to stringStack(Stack that holds string according to input order from file)
 * 
 * @param token this is a one word string
 */		
void CompileChecker :: pushToStack(string token){
	stringStack.push(token);
}
/**
 * pushes given string to outputQueue (in form of assemble code)
 * 
 * @param outputString
 */
void CompileChecker :: pushToQueue(string outputString){
	outputQueue.push(outputString);
}
/**
 * checks if given variable is in varTable, adds it to varTable if not
 * 
 * @param variable
 */
int CompileChecker :: checkVarTable(string variable){
	
	int flag=0;	// flag that check is given variable is available at varTable
	for(int i=0;i<varCount;i++){
		if(varTable[i]==variable){
			flag=1;	// means varTable has given variable 
			break;
		}
	}
	if(flag==1)return 1;
	else{
		string var=variable;
		varTable.push_back(var);	// if there is no variable in varTable this will add newly created variable to varTable	
		varCount++;
		return 0;
	}
}
/**
 * Checks following strings in stack according to myLanguage Grammar, checks statement
 * 
 */
int CompileChecker :: checkStm(){
	string s;
	if(!stringStack.empty()){
		tokenNumber++;
		s= stringStack.top();
		stringStack.pop();
	}
	else {
		cout<<"Token expected after "<<tokenNumber<<endl;
		return 0;
	}
	if(s=="print"){
		int syntaxCorrect = checkExpr();
		outputQueue.push("\
 \t pop ax \t \t ; \n\
 \t call myprint \t ; \n");	// prints expression ( checkExpr pushes a value to stack of assembly, we pop it here and use it)
		return syntaxCorrect;
	}
	else if (s=="read"){
		string s2;	// "id" , s2 is variable name
		if(!stringStack.empty()){
			tokenNumber++;
			s2 = stringStack.top();
			stringStack.pop();
		}
		else {
			cout<<"Token expected after "<<tokenNumber<<endl;
			return 0;
		}
		outputQueue.push(" \t call myread \t ; ");
		string readOutput="read " + s2 + "\n";
		outputQueue.push(readOutput);
		string s3;
		if(!checkVarTable(s2))s3=" \t mov  v" +s2+ ",cx \t ; \n";	// creates new variable
		else{
			s3=" \t push offset v"+ s2 + " \t ; \n\
 \t pop bp \t \t ; \n\
 \t mov [bp],cx \t ; \n";  // change value of old variable
		}
		outputQueue.push(s3);
		return 1;
	 }
	else if (s=="if"){
		int ifindex=labelCount;	// a jump label for if statement
		labelCount++;
		//outputQueue.push(s);
		stringstream ss; // for converting integer to string
		ss << ifindex;
		string str = ss.str();
		string iflabel1="Lab"+str+" \t ; \n";	// will be put after jump statement
		string iflabel2="Lab"+str+": \t \t\t ; \n";	//label that will be put on end of if block
		int syntaxCorrect1 = checkExpr();	//pushes a value to stack of assembly compiler
		string s2;
		if(!stringStack.empty()){
			tokenNumber++;
			s2 = stringStack.top();
			stringStack.pop();
		 }
		else {
			cout<<"Token expected after "<<tokenNumber<<endl;
			return 0;
		}
		if(s2!="then") {
			cout<<"then token expected at token "<<tokenNumber<<endl;
			return 0;
		}
		outputQueue.push("\
 \t pop ax \t \t ; \n\
 \t cmp ax,0 \t \t ; \n\
 \t IF Z jmp ");		//pops value from stack, if its 0, jumps to end of if block 
		outputQueue.push(iflabel1);	
		int syntaxCorrect2 = checkStm();
		outputQueue.push(iflabel2);		// label after if block that can be used to jump outside
		return syntaxCorrect1 && syntaxCorrect2 ;
	}
	else if (s=="while"){
		int whilePreIndex=labelCount;	//label before start of while block, will be used for recursion
		labelCount++;
		int whileAfterIndex=labelCount;	//label after while block, will be used to skip while statement
		labelCount++;		
		stringstream ss;				
		stringstream ss2;
		ss << whilePreIndex;		//for string conversation
		ss2 <<whileAfterIndex;
		string str,str2;
		ss>>str;
		ss2>>str2;
		string whilePreLabel1="Lab"+str+": \t\t\t ; while begin \n";
		string whilePreLabel2="\t jmp Lab"+str+" \t \t ; jump to while beginning \n";
		string whileAfterLabel1="Lab"+str2+" \t \t ; \n";
		string whileAfterLabel2="Lab"+str2+": \t\t\t ; \n";
		outputQueue.push(whilePreLabel1);
		int syntaxCorrect1 = checkExpr();
		string s2;
		if(!stringStack.empty()){
			tokenNumber++;
			s2 = stringStack.top();
			stringStack.pop();
		 }
		else {
			cout<<"Token expected after "<<tokenNumber<<endl;
			return 0;
		}
		if(s2!="do"){
			cout<<"do Token expected at "<<tokenNumber<<endl;
			return 0;
		}
		outputQueue.push("\
 \t pop ax \t \t ; \n\
 \t cmp ax,0 \t \t ; \n\
 \t IF Z jmp ");	//while statement check. Jumps if value taken from expression is 0
		outputQueue.push(whileAfterLabel1);
		int syntaxCorrect2 = checkStm();
		outputQueue.push(whilePreLabel2);	//Jumps to label at start of while block.
		outputQueue.push(whileAfterLabel2);	//label after while block to exit while loop
		return syntaxCorrect1 && syntaxCorrect2 ;
	}
	else if (s=="begin"){
		int syntaxCorrect =checkOp_Stms();	// controls statements between begin and end
		string s2;
		if(!stringStack.empty()){
			 tokenNumber++;
			 s2 = stringStack.top();
			 stringStack.pop();
		 }
		else {
			cout<<"end token expected after "<<tokenNumber<<endl;
			return 0;
		}
		if(s2!="end") {
			cout<<"end token expected at "<<tokenNumber<<endl;
			return 0;
		}
		return syntaxCorrect;
	}
	else {
		//s=id
		//add s1 to variable list		
		stringstream ss;
		ss << s;
		string str = ss.str();
		string s2;
		if(!stringStack.empty()){
			tokenNumber++;
			s2 = stringStack.top();
			stringStack.pop();
		 }
		else {
			cout<<" '=' expected after token "<<tokenNumber<<endl;
			return 0;
		}
		if(s2!="=") {
			cout<<" '=' expected at token "<<tokenNumber<<endl;
			return 0;
		}
		int syntaxCorrect =checkExpr();
		string s3;
		if(!checkVarTable(s))s3=" \t pop cx \t \t ; \n\
 \t mov  v" +str+ ",cx \t ; \n";	// creates new variable
		else{
			s3=" \t pop ax \t \t ; \n\
 \t push offset v"+ str + " \t ; \n\
 \t pop bp \t \t ; \n\
 \t mov [bp],ax  \t ; \n";  // change value of old variable
		}
		outputQueue.push(s3);
		return syntaxCorrect;
	}
	return 0;
}
/**
 *  Checks following strings in stack according to myLanguage Grammar, checks if there are multiple statements.
 */
int CompileChecker :: checkOp_Stms(){
	string s;
	if(!stringStack.empty()){
		 s = stringStack.top();
	}
	else return 0;
	if(s=="end"){
		return 1;
	}
	else{
		return checkStm_List();
	}
}
/**
 *  Checks following strings in stack according to myLanguage Grammar, checks if there are multiple statements.
 */
int CompileChecker :: checkStm_List(){ 		// in gramar , Stmt_List -->  Stmt ; Stmt_List  or Stmt_List --> Stmt
	int syntaxCheck1 = checkStm();			//Checks For statemement
	string s;
	if(!stringStack.empty()){
		 s = stringStack.top();
	}
	else return 0;
	int syntaxCheck2=1;
	if(s==";"){			// check for ;  , if there is a ; it means there are more statement inside
		tokenNumber++;
		stringStack.pop();
		syntaxCheck2=checkStm_List();	//checks for statement.
	}
	return syntaxCheck1 && syntaxCheck2 ;
}
/**
 * This function along with checkTerm,checkMoreTerm,checkFactor,checkMoreFactor are used for parsing expressions.
 */
int CompileChecker :: checkExpr(){		// In grammar ,Expr --> Term MoreTerm.
	int syntaxCheck1=checkTerm();	//checks Term
	int syntaxCheck2=checkMoreTerm();	//checks MoreTerm
	return syntaxCheck1 && syntaxCheck2;
}
/**
 * 	Checks For Term, its for parsing expression
 */ 
int CompileChecker :: checkTerm(){		// In grammar ,Term --> Factor MoreFactor
	int syntaxCheck1 = checkFactor();	//checks Factor
	int syntaxCheck2 = checkMoreFactor();	//checks MoreFactor
	return syntaxCheck1 && syntaxCheck2;	
}
/**
 * 	Checks For MoreTerm, its for parsing expression
 */ 
int CompileChecker :: checkMoreTerm(){ // In grammer ,MoreTerm --> ("+" | "-") Term MoreTerm or it's empty.   
	string s;
	if(!stringStack.empty()){
		s = stringStack.top();
	} else return 1;
	int syntaxCheck1=1;
	int syntaxCheck2=1;
	if (s=="+" || s=="-"){		// if s is + or - , it means MoreTerm is not empty.
		tokenNumber++;
		stringStack.pop();
		syntaxCheck1=checkTerm();	//checks Term
		outputQueue.push(" \t pop cx \t \t ; \n\
 \t pop ax \t \t ; \n");	//pops last pushed value to cx and value before it to ax.
		if( s=="+")outputQueue.push(" \t add ax,cx  \t ; \n\
 \t push ax \t \t ; \n");	// add them and push their sum to stack
		else if (s=="-")outputQueue.push(" \t sub ax,cx  \t ; \n\
 \t push ax \t \t ; \n");	// sub them and push their subtraction to stack
		syntaxCheck2= checkMoreTerm();	//checks for MoreTerm
		return syntaxCheck1 && syntaxCheck2;
	}
	else return 1;
}
/**
 * 	Checks For Factor, its for parsing expression
 */ 
int CompileChecker :: checkFactor(){ //In grammar , Factor --> '( ' Expr ' ) ' or id | num 
	string s;
	if(!stringStack.empty()){
		tokenNumber++;
		s = stringStack.top();
		stringStack.pop();
	}else {
		cout<<"token expected after "<<tokenNumber<<endl;
		return 0;
	}
	int syntaxCheck1=1;
	if(s=="("){ // if first word is ( it means there is a "( Expr )" here 
		syntaxCheck1=checkExpr(); //checks for expression
		string s2;
		if(!stringStack.empty()){
			tokenNumber++;
			s2 = stringStack.top();
			stringStack.pop();
		}else {			//there has to be ) after end of expression
			cout<<" ')' expected after "<<tokenNumber<<endl;
			return 0;
		}
		if(s2!=")"){
			cout<<" ')' expected at "<<tokenNumber<<endl;
			return 0;
		}
		return syntaxCheck1;
	}
	else{
		int hasOnlyDigits = 1; // finds if s is Id or Number
		for(unsigned int i = 0; i < s.length(); i++)
		{
			if (!isdigit(s[i]))
			{
				hasOnlyDigits = 0;
				break;
			}
		}
		if(hasOnlyDigits){ // if s is number
			if(s=="0"){
				outputQueue.push(" \t xor ax,ax  \t \t ; \n \t push ax \n");
			}
			else outputQueue.push(" \t push "+s+"d \n");
		}
		else outputQueue.push(" \t push v"+s+" w\t \t ; \n"); // s is variable.
		return 1;
	}
}
/**
 * 	Checks For MoreFactor, its for parsing expression
 */ 
int CompileChecker :: checkMoreFactor(){ // In grammar, MoreFactor --> (+|-|/) factor MoreFactor or is empty 
	string s;
	if(!stringStack.empty()){
		 s = stringStack.top();
	}else return 1;
	if(s=="*"||s=="/"||s=="%"){ // if s is one of this, it means MoreFactor is not empty.
		tokenNumber++;
		stringStack.pop();
		int syntaxCheck1=checkFactor();
		outputQueue.push(" \t pop cx \t \t ; \n\
 \t pop ax \t \t ; \n");
		if( s=="*")outputQueue.push(" \t Mul cx \t \t ; \n\
 \t push ax \t \t ; \n"); //mul cx multiplies ax with cx and put value on ax, we push ax.
		else{
			outputQueue.push(" \t mov dx,0 \t \t ; \n\
 \t div cx \t \t ; \n");	//divides ax with cx, puts their division on ax and their mod on dx. 
			if (s=="/")outputQueue.push(" \t push ax \t \t ; \n");	// if / is asked, we push ax
			else if (s=="%")outputQueue.push(" \t push dx \t \t ; \n"); // if % is asked we push dx.
		}
		int syntaxCheck2=checkMoreFactor();
		
		return syntaxCheck1&& syntaxCheck2;
	}else return 1;
}
/**
 * Puts the code in (given string).asm 
 * 
 * @param fileName output file name
 */
void CompileChecker :: giveOutput (string fileName){
	ofstream out;
	string outFile=fileName+".asm";
	out.open(outFile.c_str());
	while(!outputQueue.empty()){	//empties OutputQueue to fileName.asm
		string s;
		s=outputQueue.front();
		outputQueue.pop();
		out<<s;
	}
	out.close();
}
/**
 * Pushes remaining necessary code to outputQueue
 */
void CompileChecker :: pushRemainingAssemblyCode(){
	outputQueue.push("\
\t int 20h \t\t ; exit to dos \n\
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; \n\
;reads a nonnegative integer and puts \n\
;it in cx register  \n\
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n" );
	outputQueue.push("\
myread: \n\
\t MOV  CX,0 \n\
morechar: \n\
\t mov  ah,01h \n\
\t int  21H \n\
\t mov  dx,0 \n\
\t mov  dl,al \n\
\t mov  ax,cx \n\
\t cmp  dl,0D \n");	// read function
	outputQueue.push("\
\t je   myret \n\
\t sub  dx,48d \n\
\t mov  bp,dx  \n\
\t mov  ax,cx \n\
\t mov  cx,10d \n\
\t mul  cx  \n\
\t add  ax,bp \n\
\t mov  cx,ax \n\
\t jmp  morechar \n\
myret: \n\
\t ret \n"); // read function continued
	outputQueue.push("\
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n\
; number in AX register is printed\n\
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n\
myprint: \n\
\t mov    si,10d \n\
\t xor    dx,dx \n\
\t push   ' ' \t\t ; push newline \n\
\t mov    cx,1d \n\
nonzero: \n");	//print function
	outputQueue.push("\
\t div    si \n\
\t add    dx,48d \n\
\t push   dx \n\
\t inc    cx \n\
\t xor    dx,dx \n\
\t cmp    ax,0h \n\
\t jne    nonzero \n\
writeloop: \n\
\t pop    dx \n");
	outputQueue.push("\
\t mov    ah,02h \n\
\t int    21h \n\
\t dec    cx \n\
\t jnz    writeloop \n\
\t ret \n\
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n\
; Variables are put in this area \n\
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; \n");
// variables that are used are put in here
	for(int j=0;j<varCount;j++){
		string s;
		s="v"+varTable[j]+" \t dw ?  ;  variable "+varTable[j]+"\n";
		outputQueue.push(s);
	}
	outputQueue.push("code ends");	//end of code block

}

