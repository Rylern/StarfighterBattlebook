#include "exception.hpp"


namespace Server {
    Exception::Exception(std::string message) : std::exception(), _message(message) {
    }

    std::string Exception::getMessage() const {
        return this->_message;
    }

    const char* Exception::what() const throw() {
    	return this->_message.c_str();
    }
}