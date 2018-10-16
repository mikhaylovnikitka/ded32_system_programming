#pragma once

#include <cstdio>

//font color
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define BLUE    "\x1b[34m"
#define RESET   "\x1b[0m"

class Logger {
public:
    enum TypeMessage
    {
        ERROR,
        WARNING,
        INF
    };

    //макрос
    void Write(TypeMessage type, const char* message, const char* func_name)
    {
        switch(type) {
            case ERROR:
            {
                printf("%sERROR :: %s", RED, RESET);
                break;
            }
            case WARNING:
            {
                printf("%sWARNING :: %s", BLUE, RESET);
                break;
            }
            case INF:
            {
                printf("%sINFORMATION :: %s", GREEN, RESET);
                break;
            }
            default : break;
        }
        printf("%s :: %s\n", message, func_name);
    }
};

