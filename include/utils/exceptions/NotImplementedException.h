//
// Created by eric on 19-1-24.
//

#ifndef NOT_IMPLEMENTED_EXCEPTION_H
#define NOT_IMPLEMENTED_EXCEPTION_H

#include <exception>

class NotImplementedException : public std::exception {

public:

    const char * what() const noexcept override {
        return "This method has NOT been implement!";
    }

};

#endif //NOT_IMPLEMENTED_EXCEPTION_H
