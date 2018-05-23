#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <exception>
namespace leogon
{
	class CException : public std::exception
	{
	public:
		CException(const char* module, const char* fmt, ...);
		virtual const char *what( ) const throw( );
	private:
		char szError[256];
	};
}
#endif
