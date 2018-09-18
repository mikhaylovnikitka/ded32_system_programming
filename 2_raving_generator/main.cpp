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
#include <cctype>

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

class ToFilePrinter{
public:
    explicit ToFilePrinter(const char* output_file_path)
    {
        output = fopen(output_file_path, "w");

        if (output == nullptr)
        {
            logger.Write(logger.ERROR, "cannot open output file", __FUNCTION__);
        }
    }

    ~ToFilePrinter()
    {
        //nullptr
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
    Text() = delete;

    Text(const char* file_path, std::vector<char*>& text_lines)
    {
        logger_.Write(logger_.INF, "constructor begins", __FUNCTION__);

        file_descriptor_ = open(file_path, O_CREAT | O_RDWR);

        if (file_descriptor_ == -1)
        {
            logger_.Write(logger_.ERROR, "while opening file", __FUNCTION__);
            //exit(EXIT_FAILURE);
        }

        struct stat stat_buff{};
        //filelength
        if (fstat(file_descriptor_, &stat_buff) < 0)
        {
            close(file_descriptor_);
            logger_.Write(logger_.ERROR, "in calling function fstat", __FUNCTION__);
            //exit(EXIT_FAILURE);
        }

        file_size_ = static_cast<size_t>(stat_buff.st_size);

        //Memory mapping the file.
        mapped_memory_ = static_cast<char*>(mmap(nullptr, file_size_ + 1, PROT_READ | PROT_WRITE,
                                                 MAP_SHARED, file_descriptor_, 0));
        //We won't use file in near future
        close(file_descriptor_);

        if (mapped_memory_ == MAP_FAILED)
        {
            logger_.Write(logger_.ERROR, "while memory maping the file", __FUNCTION__);
            //exit(EXIT_FAILURE);
        }
        //memchr strchr
        size_t lines_ = 1;

        for (size_t i = 0; i < file_size_; i++)
        {
            if (mapped_memory_[i] == '\n' || mapped_memory_[i] == '\0')
            {
                mapped_memory_[i] = '\0';
                ++lines_quantity_;
                ++lines_;
            }
        }
        mapped_memory_[file_size_] = '\0';

        size_t lines_iter = 1;

        text_lines.assign(lines_quantity_, nullptr);
        text_lines[0] = mapped_memory_;

        for (size_t i = 0; i < file_size_; i++)
        {
            if (mapped_memory_[i] == '\0')
            {
                text_lines[lines_iter] = mapped_memory_ + i + 1;
                lines_iter++;
            }
        }

        logger_.Write(logger_.INF, "constructor ends", __FUNCTION__);
    }

    ~Text()
    {
        logger_.Write(logger_.INF, "destructor begins", __FUNCTION__);

        if (munmap(mapped_memory_, file_size_) == -1)
        {
            logger_.Write(logger_.ERROR, "while un-memory mapping the file", __FUNCTION__);
        }

        logger_.Write(logger_.INF, "destructor ends", __FUNCTION__);
    }

    size_t GetLinesQuantity() {
        return lines_quantity_;
    }

private:

    size_t lines_quantity_ = 1;
    size_t file_size_ = 0;
    char* mapped_memory_ = nullptr;
    int file_descriptor_ = 0;
    Logger logger_;
};


int main(int argc, char *argv[])
{
    const char *input_file_path = "onegin.txt";
    const char *output_file_path = "output.txt";

    if (argc >= 3) {
        input_file_path = argv[1];
        output_file_path = argv[2];
    }

    std::vector<char*> original_text;

    Text text(input_file_path, original_text);

    std::vector<char*> sorted_text = original_text;
    std::vector<char*> reverse_sorted_text = original_text;

    std::sort(sorted_text.begin(), sorted_text.end(), [](const char* first, const char* second)
    {
        if (first == nullptr && second != nullptr)
        {
            return true;
        }
        if (second == nullptr)
        {
            return false;
        }
        int iter = 0;
        while (true)
        {
            if (first[iter] == '\0' && second[iter] != '\0')
            {
                return true;
            }
            if (second[iter] == '\0')
            {
                return false;
            }
            if (first[iter] < second[iter])
            {
                return true;
            }
            if (second[iter] < first[iter])
            {
                return false;
            }
            iter++;
        }
    });

    std::sort(reverse_sorted_text.begin(), reverse_sorted_text.end(), [](const char* first, const char* second)
    {
        if (first == nullptr && second != nullptr)
        {
            return true;
        }
        if (second == nullptr)
        {
            return false;
        }
        int first_iter = 0;
        int second_iter = 0;
        while (first[first_iter] != '\0')
        {
            first_iter++;
        }
        while (second[second_iter] != '\0')
        {
            second_iter++;
        }

        while (true)
        {
            if (first_iter <= 0 && second_iter > 0)
            {
                return true;
            }
            if (second_iter <= 0)
            {
                return false;
            }
            while (!isalpha(first[first_iter]))
            {
                first_iter--;
            }
            while (!isalpha(second[second_iter]))
            {
                second_iter--;
            }
            if (first[first_iter] < second[second_iter])
            {
                return true;
            }
            if (second[second_iter] < first[first_iter])
            {
                return false;
            }
            first_iter--;
            second_iter--;
        }
    });

    ToFilePrinter printer(output_file_path);
    printer.PrintVector(sorted_text);
    printer.PrintVector(reverse_sorted_text);
    printer.PrintVector(original_text);

    return 0;
}