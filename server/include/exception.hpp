#ifndef SERVER_EXCEPTION_H
#define SERVER_EXCEPTION_H

#include <exception>
#include <string>

namespace Server {
    class Exception : public std::exception
    {
    private:
        std::string _message;
    public:
        Exception(std::string message);
        std::string getMessage() const;
        const char* what() const throw();
    };
}




#endif