#pragma once

#include <string>

#include "Logger.h"
#include "LoggerHTML.h"

#define POISON 0xDEADBEEF

template <typename T = int>
class ProtectedStack
{
public:
    explicit ProtectedStack(int);
    ~ProtectedStack();

    bool Ok();
    void Dump() const;

    bool Push(T item);
    bool Pop(T& element);
    bool IsEmpty();
    bool IsFull();
    int Size() { return elements_count_; }

    std::string error_message;

private:
    int CalculateChecksum();

    unsigned int canary_first_ = POISON;
    T* data_ = nullptr;
    int capacity_ = 0;
    int elements_count_ = 0; //actual number of items
    int checksum_ = 0;
    unsigned int canary_second_ = POISON;

    mutable Logger logger_;
    mutable LoggerHTML loggerHTML_;
    std::hash<T> hash_;
};

template <typename T>
ProtectedStack<T>::ProtectedStack(int initial_capacity) : loggerHTML_("dump.html", "STACK DUMP")
{
    if (initial_capacity < 0) {
        logger_.Write(logger_.ERROR, "attempt to create stack with negative size", __FUNCTION__);
    }
    capacity_ = initial_capacity;
    data_ = new T[initial_capacity];
    for (int i = 0; i < capacity_ ; ++i)
    {
        data_[i] = T();
    }
    logger_.Write(logger_.INF, "constructor ended", __FUNCTION__);
    checksum_ = CalculateChecksum();
}

template <typename T>
ProtectedStack<T>::~ProtectedStack()
{
    delete[] data_;
    logger_.Write(logger_.INF, "destructor ended", __FUNCTION__);
}


template <typename T>
int ProtectedStack<T>::CalculateChecksum()
{
    int current_check_sum = 0;
    current_check_sum += capacity_ + elements_count_ + canary_first_ + canary_second_;
    for (size_t i = 0; i < elements_count_; ++i) {
        current_check_sum += hash_(data_[i]);
    }
    return current_check_sum;
}

template <typename T>
bool ProtectedStack<T>::Ok()
{
    if (canary_first_ != POISON || canary_second_ != POISON)
    {
        error_message = "canary buffer protection was broken (memory was changed)";
        return false;
    }

    if (capacity_ < 0)
    {
        error_message = "stack has negative 'capacity_' field";
        return false;
    }

    if (elements_count_ < 0)
    {
        error_message = "stack has negative 'elements_count' field";
        return false;
    }

    if (capacity_ < elements_count_)
    {
        return false;
        error_message = "stack is larger than it should be";
    }

    int new_check_sum = CalculateChecksum();
    if (checksum_ != new_check_sum)
    {
        std::cout << checksum_ << ' ' << CalculateChecksum() << std::endl;
        error_message = "checksums are not equal";
        std::cout << checksum_ << ' ' << CalculateChecksum() << std::endl;
        return false;
    }
    return true;
}

template <typename T>
void ProtectedStack<T>::Dump() const
{
    #define BOLD_TEXT(text) std::string("<b>") + std::string(text) + std::string("</b>")
    #define RED_TEXT(text) std::string("<font color='red'>") + std::string(text) + std::string("</font>")
    #define GREEN_TEXT(text)  std::string("<font color='green'>") + std::string(text) + std::string("</font>")
    #define BEGIN_TAB std::string("<blockquote>")
    #define END_TAB std::string("</blockquote>")

    if (error_message != "")
    {
        loggerHTML_.WriteLine(BOLD_TEXT("STACK"), "[", this, "]",  RED_TEXT(BOLD_TEXT("ERROR")));
        loggerHTML_.WriteLine(BOLD_TEXT("ERROR MESSAGE:"), RED_TEXT(error_message));
    } else
    {
        loggerHTML_.WriteLine(BOLD_TEXT("STACK"), "[", this, "]",  GREEN_TEXT(BOLD_TEXT("ALL RIGHT")));
    }

    loggerHTML_.WriteLine(BOLD_TEXT("CAPACITY"), capacity_);
    loggerHTML_.WriteLine(BOLD_TEXT("ELEMENTS COUNT"), elements_count_);
    loggerHTML_.WriteLine(BOLD_TEXT("CANARY1"), canary_first_, BOLD_TEXT("CANARY2"), canary_second_);
    loggerHTML_.WriteLine(BOLD_TEXT("DATA"), "[", capacity_,  "]: ", data_, ":");

    if (capacity_ < 0 || elements_count_ < 0)
    {
        return;
    }

    for (int i = 0; i < elements_count_; ++i)
    {
        loggerHTML_.WriteLine(BEGIN_TAB, "*", "[", std::to_string(i), "] = ", data_[i], END_TAB);
    }

    for (int i = elements_count_; i < capacity_; ++i)
    {
        loggerHTML_.WriteLine(BEGIN_TAB, "*", "[", std::to_string(i), "] = ", data_[i], "(NOT USED)", END_TAB);
    }
}

template<typename T>
bool ProtectedStack<T>::IsEmpty() {
    return elements_count_ == 0;
}

template<typename T>
bool ProtectedStack<T>::IsFull() {
    return capacity_ == elements_count_;
}

template<typename T>
bool ProtectedStack<T>::Push(T item) {
    if (IsFull() || !Ok())
    {
        return false;
    }
    data_[elements_count_] = item;
    ++elements_count_;

    checksum_ = CalculateChecksum();
    return Ok();
}

template<typename T>
bool ProtectedStack<T>::Pop(T& element) {
    if (IsEmpty() || !Ok())
    {
        return false;
    }
    elements_count_--;
    element = data_[elements_count_];

    checksum_ = CalculateChecksum();
    return Ok();
}


