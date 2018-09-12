#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <cstring>
#include <vector>
#include <iostream>

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

    void Write(TypeMessage type, const char* message)
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
        printf("%s \n", message);
    }
};

class ToFilePrinter{
public:
    explicit ToFilePrinter(const char* output_file_path)
    {
        output = fopen(output_file_path, "w");

        if (output == nullptr)
        {
            logger.Write(logger.ERROR, "cannot open output file");
        }
    }

    ~ToFilePrinter()
    {
        fclose(output);
    }

    void PrintVector(std::vector<char*>& text)
    {
        for (auto &i : text)
        {
            fprintf(output, "%s \n", i);
        }
    }

private:
    Logger logger;
    FILE* output;
};

class Text
{
public:
    explicit Text() = delete;

    explicit Text(const char* file_path, std::vector<char*>& text_lines)
    {
        logger_.Write(logger_.INF, "constructor begins");

        file_descriptor_ = open(file_path, O_CREAT | O_RDWR);

        if (file_descriptor_ == -1)
        {
            logger_.Write(logger_.ERROR, "while opening file");
            //exit(EXIT_FAILURE);
        }

        struct stat stat_buff{};
        if (fstat(file_descriptor_, &stat_buff) < 0)
        {
            close(file_descriptor_);
            logger_.Write(logger_.ERROR, "in calling function fstat");
            //exit(EXIT_FAILURE);
        }

        file_size_ = static_cast<size_t>(stat_buff.st_size);

        //Memory mapping the file.
        maped_memory_ = static_cast<char*>(mmap(nullptr, file_size_ + 1, PROT_READ | PROT_WRITE,
                                                MAP_SHARED, file_descriptor_, 0));
        //We won't use file in near future
        close(file_descriptor_);

        if (maped_memory_ == MAP_FAILED)
        {
            logger_.Write(logger_.ERROR, "while memory maping the file");
            //exit(EXIT_FAILURE);
        }

        size_t lines_ = 1;

        for (size_t i = 0; i < file_size_; i++)
        {
            if (maped_memory_[i] == '\n' || maped_memory_[i] == '\0')
            {
                maped_memory_[i] = '\0';
                ++lines_quantity_;
                ++lines_;
            }
        }
        maped_memory_[file_size_] = '\0';

        size_t lines_iter = 1;

        text_lines.assign(lines_quantity_, nullptr);
        text_lines[0] = maped_memory_;

        for (size_t i = 0; i < file_size_; i++)
        {
            if (maped_memory_[i] == '\0')
            {
                text_lines[lines_iter] = maped_memory_ + i + 1;
                lines_iter++;
            }
        }

        logger_.Write(logger_.INF, "constructor ends");
    }

    ~Text()
    {
        logger_.Write(logger_.INF, "destructor begins");

        if (munmap(maped_memory_, file_size_) == -1)
        {
            logger_.Write(logger_.ERROR, "while un-memory mapping the file");
        }

        logger_.Write(logger_.INF, "destructor ends");
    }

    size_t GetLinesQuantity() {
        return lines_quantity_;
    }

private:

    size_t lines_quantity_ = 1;
    size_t file_size_ = 0;
    char* maped_memory_ = nullptr;
    int file_descriptor_ = 0;
    Logger logger_;
};


int main() {

    const char *input_file_path = "onegin.txt";
    std::vector<char*> original_text;

    Text text(input_file_path, original_text);

    std::vector<char*> sorted_text = original_text;
    std::vector<char*> reverse_sorted_text = original_text;

    std::sort(sorted_text.begin(), sorted_text.end(), [](const char* first, const char* second)
    {
        std::string s_first = first;
        std::string s_second = second;
        return s_first < s_second;
    });

    std::sort(reverse_sorted_text.begin(), reverse_sorted_text.end(), [](const char* first, const char* second)
    {
        std::string s_first = first;
        std::string s_second = second;
        return s_first > s_second;
    });

    ToFilePrinter printer("output.txt");
    printer.PrintVector(sorted_text);
    printer.PrintVector(reverse_sorted_text);
    printer.PrintVector(original_text);

    return 0;
}