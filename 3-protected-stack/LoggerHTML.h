#pragma once

#include "Logger.h"
#include <iostream>
#include <fstream>

class LoggerHTML{
public:
    LoggerHTML(const char * output_file_path = "log.html", const char* title = "LOGGER")
    {
        try
        {
            outfile.open(output_file_path);
        } catch (std::exception &e)
        {
            logger.Write(logger.ERROR, "error while redirecting out stream", __FUNCTION__);
        }

        outfile << "<!DOCTYPE html><html><head><title>" << title << "</title></head><body>" << std::endl;

        logger.Write(logger.INF, "constructor ended", __FUNCTION__);
    }

    ~LoggerHTML()
    {
        outfile << "</body></html>";
        outfile.close();
        logger.Write(logger.INF, "destructor ended", __FUNCTION__);
    }


    template <typename... OtherArgs>
    void WriteLine(OtherArgs... args)
    {
        size_t argument_count = sizeof...(args);
        if (argument_count < 1) {
            logger.Write(logger.ERROR, "function called with no parameters", __FUNCTION__);
        }

        outfile << "<p>";
        Write(std::forward<OtherArgs>(args)...);
        outfile << "</p>" << std::endl;
    }

private:
    Logger logger;
    std::ofstream outfile;

    template <typename T, typename... OtherArgs>
    void Write(T&& var, OtherArgs&&... args)
    {
        outfile << std::forward<T>(var) << ' ';
        Write(std::forward<OtherArgs>(args)...);
    }

    void Write() const
    {
        return;
    }
};