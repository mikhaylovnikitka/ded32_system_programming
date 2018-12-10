#pragma once

//<tt>
//<pre>
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

        outfile << "<pre>" << std::endl;
        outfile << "<pp>" << std::endl;
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

        Write(std::forward<OtherArgs>(args)...);
    }

private:

    void Write()
    {
        outfile << std::endl;
    }

    template <typename T, typename... OtherArgs>
    void Write(T&& var, OtherArgs&&... args)
    {
        outfile << std::forward<T>(var) << ' ';
        Write(std::forward<OtherArgs>(args)...);
    }

    Logger logger;
    std::ofstream outfile;
};