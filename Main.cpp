#include <cmath>
#include <cstdio>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "CompileChecker.h"
/**
 *	Checks if  given file is myLanguage file( ends with .my)
 * 
 * 	@param fileName given fileName
 */
int checkFormat(string fileName){
	int index=0;
	
	while(fileName[index]!='.' && index<fileName.length()){
		index++;
	}// for cases where there is no dot or length after dot isn't suitable for ".my" ending
	if(index!=fileName.length()-3) return 0;		
	 
	
	if(fileName[index+1]=='m' && fileName[index+2] == 'y') return 1;	// Last two characters are 'm' and 'y' 
	else return 0;
}
using namespace std;
/**
 * gives file name without ".my" ending 
 */
string extractFileName(string fileName){
	int index=0;
	while(fileName[index]!='.') index++;
	return fileName.substr(0,index);
}
int main(int argc,  char** argv){
	
	if(argc<2){	// We need arguments for compiling myLanguage Code.
		cout<<"Argument Expected";
		return 0;
	}
	string filePath=argv[1];
	if(!checkFormat(filePath)){	// Files should be myLanguage files.
		cout<<"Unexpected format, Inputs are required to end with \".my\" .(It's case sensitive)" ;
		return 0;
	}
	
	ifstream inputEntry;
	inputEntry.open(filePath.c_str());
	stack<string> inputTakingStack;	//stack for taking inputs.
	string inputString;
	while(inputEntry>>inputString){
		inputTakingStack.push(inputString);
	}
	CompileChecker codeChecker;		// for Checking syntax errors and Preparing assembly code
	while(!inputTakingStack.empty()){
		inputString=inputTakingStack.top();
		inputTakingStack.pop();
		codeChecker.pushToStack(inputString);	// stack of compiler
	}
	inputEntry.close();
	codeChecker.pushToQueue("code segment \n");	//Start of Assembly Code
	int properMyCode = codeChecker.checkStm();
	if(!properMyCode){
		cout<<"There is a syntax error in code\n";
		return 0;
	}
	codeChecker.pushRemainingAssemblyCode();
	string outputName=extractFileName(filePath);
	codeChecker.giveOutput(outputName);
	
	return 0;
}
