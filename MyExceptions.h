//
// Created by User on 27/06/2025.
//

#ifndef MYEXCEPTIONS_H
#define MYEXCEPTIONS_H
#include <exception>
#include <string>


class MyException : public std::exception {
private:
    std::string msg;
public:
    // Constructor to set the message
    explicit MyException(const std::string& message) : msg(message) {}

    // Override what()
    const char* what() const noexcept override {
        return msg.c_str();
    }
};

#endif //MYEXCEPTIONS_H
