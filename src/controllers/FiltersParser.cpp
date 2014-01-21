#include "FiltersParser.hpp"
#include <iostream>

FiltersParser::FiltersParser(const string& expression):
expression(expression), it(expression.cbegin())
{
	this->errorIt = this->expression.cbegin() - 1;
	this->parseExpression();
}

void FiltersParser::parseExpression()
{
	string value;
	Lexem lex;
	int nextCtx = Context::CTX_FILTER | Context::CTX_OP_BINARY | Context::CTX_OP_UNARY | Context::CTX_PARENTHESIS_OPEN;

	bool errorFound = false;
	
	int parenthesisCount = 0;
	queue<pair<Lexem, string>> &parserQueue = this->opQueue;
	stack<Lexem> parserStack;
	string prepareFilter = "";
	
	bool endMeansError = false;
	bool inFilter = false;
	int thisPriority;
	Lexem tempLexem;
	
	do
	{
		lex = this->getLexem(nextCtx, value);
		if (lex == Lexem::LEX_END && endMeansError)
		{
			lex = Lexem::LEX_BREAKPARSE;
			this->errorIt = this->it;
			continue;
		}
		
		endMeansError = false;
		switch (lex)
		{
			case Lexem::LEX_FILTER:
			{
				endMeansError = true;
				inFilter = true;
				nextCtx = Context::CTX_PARENTHESIS_OPEN;
				prepareFilter = value;
				prepareFilter += "|";
				break;
			}
			case Lexem::LEX_FILTER_ARG:
			{
				endMeansError = true;
				nextCtx = Context::CTX_PARENTHESIS_CLOSE;
				prepareFilter += value;
				parserQueue.push({Lexem::LEX_FILTER, prepareFilter});
				break;
			}
			case Lexem::LEX_PARENTHESIS_OPEN:
			{
				if (inFilter)
				{
					endMeansError = true;
					nextCtx = Context::CTX_FILTER_ARG;
				}
				else
				{
					++parenthesisCount;
					nextCtx = Context::CTX_ANYTHING;
					
					parserStack.push(lex);
				}
				break;
			}
			case Lexem::LEX_PARENTHESIS_CLOSE:
			{
				if (inFilter)
				{
					inFilter = false;
				}
				else
				{
					--parenthesisCount;
					if (parenthesisCount < 0)
					{
						lex = Lexem::LEX_BREAKPARSE;
						this->errorIt = this->it;
					}
					else
					{
						while((tempLexem = (!parserStack.empty() ?
							parserStack.top() :
							Lexem::LEX_ERROR)) != LEX_PARENTHESIS_OPEN)
						{
							parserQueue.push({parserStack.top(), ""});
							parserStack.pop();
						}
						if(tempLexem != LEX_ERROR)
						{
							parserStack.pop();
						}
						else
						{
							lex = Lexem::LEX_BREAKPARSE;
							this->errorIt = this->it;
						}
					}
				}
				nextCtx = Context::CTX_OP_BINARY | Context::CTX_PARENTHESIS_CLOSE;
				break;
			}
			case Lexem::LEX_OP_AND:
			case Lexem::LEX_OP_OR:
			{
				endMeansError = true;
				nextCtx = Context::CTX_FILTER | Context::CTX_OP_UNARY | Context::CTX_PARENTHESIS_OPEN;
				
				thisPriority = this->getLexemPriority(lex);
				while (!parserStack.empty() && thisPriority <= this->getLexemPriority(parserStack.top()))
				{
					parserQueue.push({parserStack.top(), ""});
					parserStack.pop();
				}
				parserStack.push(lex);
				
				break;
			}
			case Lexem::LEX_OP_NEGATE:
			{
				endMeansError = true;
				nextCtx = Context::CTX_FILTER;
				parserStack.push(lex);
				break;
			}
			default:
				break;
		}
	}
	while(lex != LEX_BREAKPARSE &&
			lex != Lexem::LEX_BADCTX &&
			lex != Lexem::LEX_END &&
			lex != Lexem::LEX_ERROR &&
			lex != Lexem::LEX_UNKNOWN &&
			lex != Lexem::LEX_UNKNOWN_FILTER);
	
	if (lex == Lexem::LEX_BADCTX)
	{
		errorFound = true;
		this->errorType = Error::ERR_BADCTX;
	}
	else if (lex == Lexem::LEX_ERROR)
	{
		errorFound = true;
		this->errorType = Error::ERR_ERROR;
	}
	else if (lex == Lexem::LEX_UNKNOWN)
	{
		errorFound = true;
		this->errorType = Error::ERR_UNKNOWN;
	}
	else if (lex == Lexem::LEX_UNKNOWN_FILTER)
	{
		errorFound = true;
		this->errorType = Error::ERR_UNKNOWN_FILTER;
	}	
	else if (lex == Lexem::LEX_BREAKPARSE)
	{
		errorFound = true;
		if (parenthesisCount != 0)
		{
			this->errorType = Error::ERR_PARENTHESIS_MISMATCH;
		}
		else if (endMeansError)
		{
			this->errorType = Error::ERR_UNEXPECTED_END;
		}
		else
		{
			this->errorType = Error::ERR_PARENTHESIS_USELESSCLOSE;
		}
	}
	else if(parenthesisCount != 0)
	{
		errorFound = true;
		this->errorType = Error::ERR_PARENTHESIS_MISMATCH;
	}
	
	if (!errorFound)
	{
		while(!parserStack.empty())
		{
			parserQueue.push({parserStack.top(), ""});
			parserStack.pop();
		}	
		
		this->success = true;
	}
}

FiltersParser::Lexem FiltersParser::getLexem(int allowedCtx, string& getValue)
{
	getValue = "";
	string::const_iterator itStart;
	bool detected = false;
	Lexem detectedLex = Lexem::LEX_UNKNOWN;
	Context detectedCtx = Context::CTX_NOTHING;
	bool filterArgStartApostrophe;
	
	while(true)
	{
		if (!detected)
		{
			if (this->it == this->expression.cend())
			{
				detectedLex = Lexem::LEX_END;
				break;
			}
			else if (*(this->it) == ' ' || *(this->it) == '\n')
			{
				// Nothing
			}
			else
			{
				detected = true;
				if (*(this->it) == '(')
				{
					detectedLex = Lexem::LEX_PARENTHESIS_OPEN;
					detectedCtx = Context::CTX_PARENTHESIS_OPEN;
				}
				else if (*(this->it) == ')')
				{
					detectedLex = Lexem::LEX_PARENTHESIS_CLOSE;
					detectedCtx = Context::CTX_PARENTHESIS_CLOSE;
				}
				else if (*(this->it) == '!')
				{
					detectedLex = Lexem::LEX_OP_NEGATE;
					detectedCtx = Context::CTX_OP_UNARY;
				}
				else if (*(this->it) == '&')
				{
					detectedLex = Lexem::LEX_OP_AND;
					detectedCtx = Context::CTX_OP_BINARY;
				}
				else if (*(this->it) == '|')
				{
					detectedLex = Lexem::LEX_OP_OR;
					detectedCtx = Context::CTX_OP_BINARY;
				}
				else if (*(this->it) == '"' || *(this->it) == '\'')
				{
					detectedLex = Lexem::LEX_FILTER_ARG;
					detectedCtx = Context::CTX_FILTER_ARG;
					itStart = this->it;
					filterArgStartApostrophe = (*(this->it) == '\'' ? true : false);
				}
				else if ((*(this->it) >= 'a' && *(this->it) <= 'z') || (*(this->it) >= 'A' && *(this->it) <= 'Z'))
				{
					itStart = this->it;
				}
				else
				{
					this->errorIt = this->it;
					detectedLex = Lexem::LEX_ERROR;
					break;
					// Can skip iterator incrementation here because we stop parsing on error
				}
			}
		}
		else
		{
			if (detectedLex == Lexem::LEX_OP_AND ||
				detectedLex == Lexem::LEX_OP_OR ||
				detectedLex == Lexem::LEX_OP_NEGATE ||
				detectedLex == Lexem::LEX_PARENTHESIS_OPEN ||
				detectedLex == Lexem::LEX_PARENTHESIS_CLOSE)
			{
				break;
			}
			else if (detectedLex == Lexem::LEX_FILTER_ARG)
			{
				if (this->it == this->expression.end())
				{
					this->errorIt = this->it;
					detectedLex = Lexem::LEX_ERROR;
					break;
					// Can skip iterator incrementation here because we stop parsing on error
				}
				else if ((filterArgStartApostrophe && *(this->it) == '\'') || (!filterArgStartApostrophe && *(this->it) == '"'))
				{
					getValue = string(itStart + 1, this->it);
					++(this->it);
					break;
				}
			}
			else
			{
				// In FILTER or OR/AND functor detection
				if (this->it == this->expression.cend() ||
					!((*(this->it) >= 'a' && *(this->it) <= 'z') || (*(this->it) >= 'A' && *(this->it) <= 'Z')))
				{
					getValue = string(itStart, this->it);
					
					transform(getValue.begin(), getValue.end(), getValue.begin(), ::tolower);
					
					if (getValue == "or")
					{
						detectedLex = Lexem::LEX_OP_OR;
						detectedCtx = Context::CTX_OP_BINARY;
						getValue = "";
					}
					else if (getValue == "and")
					{
						detectedLex = Lexem::LEX_OP_AND;
						detectedCtx = Context::CTX_OP_BINARY;
						getValue = "";
					}
					else if (this->knownFilters.count(getValue) != 0)
					{
						detectedLex = Lexem::LEX_FILTER;
						detectedCtx = Context::CTX_FILTER;
					}
					else
					{
						this->errorIt = this->it;
						detectedLex = Lexem::LEX_UNKNOWN_FILTER;
					}
					
					break;
				}
			}
		}
		
		++(this->it);
	}
	
	if (detectedCtx != Context::CTX_NOTHING && !(detectedCtx & allowedCtx))
	{
		this->errorIt = this->it;
		detectedLex = Lexem::LEX_BADCTX;
	}
	
	return detectedLex;
}

int FiltersParser::getLexemPriority(Lexem lex)
{
	switch (lex)
	{
		case Lexem::LEX_OP_NEGATE:
			return 3;
			break;
		case Lexem::LEX_OP_AND:
			return 2;
			break;
		case Lexem::LEX_OP_OR:
			return 1;
			break;
		default:
			return -1;
			break;
	}
}

// Public methods

bool FiltersParser::hasSucceded()
{
	return this->success;
}

string FiltersParser::getErrorInfo(int surrounding)
{
	std::string errorString;
	std::stringstream errorText;
	errorText << "Error while parsing: ";
	switch(this->errorType)
	{
		case Error::ERR_BADCTX:
			errorString = "Bad context";
			break;
		case Error::ERR_ERROR:
			errorString = "Lexem incorrect";
			break;
		case Error::ERR_UNKNOWN_FILTER:
			errorString = "Unknown filter name";
			break;
		case Error::ERR_UNKNOWN:
			errorString = "Unknown";
			break;
		case Error::ERR_PARENTHESIS_MISMATCH:
			errorString = "Parenthesis mismatch";
			break;
		case Error::ERR_PARENTHESIS_USELESSCLOSE:
			errorString = "Useless close parenthesis";
			break;
		case Error::ERR_UNEXPECTED_END:
			errorString = "Unexpected expression end";
			break;
		default:
			errorString = "Undefined error";
			break;
	}
	errorText << errorString << std::endl;
	
	errorText << "Passed string: " << this->expression << std::endl;
	if (this->errorIt != (this->expression.cbegin() - 1))
	{
		errorText << "Error surrounding: ";
		if (std::distance(this->expression.cbegin(), this->errorIt - surrounding) > 0)
		{
			errorText << "...";
		}
		errorText << std::string(
			(std::distance(this->expression.cbegin(), this->errorIt - surrounding) >= 0 ?
				this->errorIt - surrounding :
				this->expression.cbegin()),
			(std::distance(this->expression.cend(), this->errorIt + surrounding) <= 0 ?
				this->errorIt + surrounding :
				this->expression.cend()));
		if (std::distance(this->expression.cend(), this->errorIt + surrounding) < 0)
		{
			errorText << "...";
		}
		errorText << std::endl;
		errorText << "(Error position: " << std::distance(this->expression.cbegin(), this->errorIt) << ")" << std::endl;
	}
	
	return errorText.str();
}

Graph* FiltersParser::applyExpression(Graph* originalGraph)
{
	if (!this->hasSucceded())
	{
		throw GenericException("Applying broken expression");
	}
	
	stack<unordered_set<Containers::Mail*>> resStack;
	
	pair<Lexem, string> el;
	string filterName, filterArgument;
	while(!this->opQueue.empty())
	{
		el = this->opQueue.front();
		
		if (el.first == Lexem::LEX_FILTER)
		{
			filterName = string(el.second.begin(), el.second.begin() + el.second.find_first_of("|"));
			filterArgument = string(el.second.begin() + el.second.find_first_of("|") + 1, el.second.end());
			
			if (filterName == "sender")
			{
				resStack.push(PeopleFilter(originalGraph->findPerson(filterArgument), true).findMails(originalGraph));
			}
			else if (filterName == "receiver")
			{
				resStack.push(PeopleFilter(originalGraph->findPerson(filterArgument), false).findMails(originalGraph));
			}
			else if (filterName == "datebefore")
			{
				resStack.push(DateFilter(std::stoi(filterArgument), true).findMails(originalGraph));
			}
			else if (filterName == "dateafter")
			{
				resStack.push(DateFilter(std::stoi(filterArgument), false).findMails(originalGraph));
			}
			else if (filterName == "subjecthas")
			{
				resStack.push(TopicSubstringFilter(filterArgument).findMails(originalGraph));
			}
		}
		else if (el.first == Lexem::LEX_OP_NEGATE)
		{
			auto allMails = originalGraph->getMailsHashset();
			allMails.erase(resStack.top().begin(), resStack.top().end());
			resStack.pop();
			resStack.push(allMails);
		}
		else if (el.first == Lexem::LEX_OP_AND)
		{
			unordered_set<Containers::Mail*> newCont;
			
			auto arg2 = resStack.top();
			resStack.pop();
			auto arg1 = resStack.top();
			resStack.pop();
			auto &bigger = (arg1.size() > arg2.size() ? arg1 : arg2);
			auto &smaller = (arg1.size() > arg2.size() ? arg2 : arg1);
			for(auto x: smaller)
			{
				if (bigger.count(x) == 1)
				{
					newCont.insert(x);
				}
			}
			resStack.push(newCont);
		}
		else if (el.first == Lexem::LEX_OP_OR)
		{			
			auto arg2 = resStack.top();
			resStack.pop();
			auto arg1 = resStack.top();
			resStack.pop();
			
			arg2.insert(arg1.begin(), arg1.end());
			resStack.push(arg2);
		}
		else
		{
			throw GenericException("Unknown element in FiltersParser Queue");
		}
		
		this->opQueue.pop();
	}
	
	if (resStack.size() != 1)
	{
		std::stringstream info;
		info << "FiltersParser resStack size is ";
		info << resStack.size();
		throw GenericException(info.str().c_str());
	}
	
	FilterSet fSet;
	fSet.addNewFilter(new MailsFilter(resStack.top()));
	fSet.processAll(originalGraph);
	
	return originalGraph;
}
