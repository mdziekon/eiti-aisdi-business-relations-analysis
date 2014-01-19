#ifndef FILTERSPARSER_HPP
#define	FILTERSPARSER_HPP

#include <string>
#include <sstream>
#include <unordered_set>
#include <queue>
#include <stack>
#include <algorithm>
#include <iterator>
#include <utility>

using namespace std;

class FiltersParser
{
protected:
	enum Lexem
	{
		LEX_BREAKPARSE = 1,
		LEX_ERROR = 2,
		LEX_UNKNOWN_FILTER = 3,
		LEX_UNKNOWN = 4,
		LEX_END = 5,
		LEX_BADCTX = 6,
		LEX_PARENTHESIS_OPEN = 7,
		LEX_PARENTHESIS_CLOSE = 8,
		LEX_FILTER = 9,
		LEX_FILTER_ARG = 10,
		LEX_OP_NEGATE = 11,
		LEX_OP_AND = 12,
		LEX_OP_OR = 13
	};
	enum Context
	{
		CTX_NOTHING = 0,
		CTX_FILTER = 1,
		CTX_FILTER_ARG = 2,
		CTX_PARENTHESIS_OPEN = 4,
		CTX_PARENTHESIS_CLOSE = 8,
		CTX_OP_UNARY = 16,
		CTX_OP_BINARY = 32,
		CTX_ANYTHING = 63
	};
	enum Error
	{
		ERR_NONE,
		ERR_UNKNOWN_FILTER,
		ERR_UNKNOWN,
		ERR_BADCTX,
		ERR_ERROR,
		ERR_PARENTHESIS_MISMATCH,
		ERR_PARENTHESIS_USELESSCLOSE,
		ERR_UNEXPECTED_END
	};
	unordered_set<string> knownFilters {"sender", "receiver", "datebefore", "dateafter", "subjecthas"};
	
	const string expression;
	string::const_iterator it;
	bool success = false;
	
	string::const_iterator errorIt;
	Error errorType = Error::ERR_NONE;
	
	void parseExpression();
	Lexem getLexem(int allowedCtx, string& getValue);
	int getLexemPriority(Lexem lex);
	
public:
	FiltersParser(const string& expression);
	
	bool hasSucceded();
	string getErrorInfo(int surrounding = 3);
};

#endif	/* FILTERSPARSER_HPP */

