#ifndef COMPILECHECKER_H
#define COMPILECHECKER_H
#include <vector>
#include <queue>
#include <stack>
#include <string>

using namespace std;
class CompileChecker
{
	private:
		std::vector<string> varTable;
		int varCount;
		int labelCount;
		int tokenNumber;
		std::stack<string> stringStack;
		std::queue<string> outputQueue;
	public:
		CompileChecker();
		void pushToStack(string s);
		void pushToQueue(string s);
		int checkVarTable(string s);
		int checkStm();
		int checkOp_Stms();
		int checkStm_List();
		int checkExpr();
		int checkTerm();
		int checkMoreTerm();
		int checkFactor();
		int checkMoreFactor();
		void giveOutput(string fileName);
		void pushRemainingAssemblyCode();

};

#endif
