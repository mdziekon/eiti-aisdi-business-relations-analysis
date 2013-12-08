/* 
 * File:   Exceptions.hpp
 * Author: michal
 */

#ifndef EXCEPTIONS_HPP
#define	EXCEPTIONS_HPP

#include <string>

using namespace std;

class GenericException: public exception
{
	protected:
		const string text;

	public:
		GenericException(const char* text);
		virtual const char* what() const noexcept;
};

#endif	/* EXCEPTIONS_HPP */

