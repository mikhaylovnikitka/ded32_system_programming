#include <iostream>

#include "Commands/GlobalVariables.h"
#include "CodeToByte/CodeToByte.h"

void SimpleTest() {
    Push push;
    CommandInterface* a = &push;
    ArgsInterface a_args{100, false};
    Memory registers{};
    registers.rax = 5;
    std::stack<int> s;
    a->Execute(s, &registers, &a_args);
    if (!s.empty()) {
        std::cout << "Stack's top: " << s.top() << std::endl;
    } else {
        std::cerr << "Stack is empty" << std::endl;
    }

    Push push_2;
    CommandInterface* a_2 = &push_2;
    ArgsInterface a_args_2{21, false};
    Memory registers_2{};
    registers_2.rax = 5;
    a->Execute(s, &registers_2, &a_args_2);
    if (!s.empty()) {
        std::cout << "Stack's top: " << s.top() << std::endl;
    } else {
        std::cerr << "Stack is empty" << std::endl;
    }

    Add add;
    CommandInterface* add_base = &add;
    add_base->Execute(s, nullptr, nullptr);
    std::cout << "Stack's top: " << s.top() << std::endl;

    Sqrt sqrt;
    CommandInterface* sqrt_base = &sqrt;
    sqrt_base->Execute(s, nullptr, nullptr);

    std::cout << "Stack's top: " << s.top() << std::endl;


    Pop pop;
    CommandInterface* b = &pop;
    ArgsInterface a_pop_args{"rbx"};
    registers.rax = 5;
    b->Execute(s, &registers, &a_pop_args);

    std::cout << "Stack size: " << s.size() << std::endl;
    std::cout << "rbx: " << registers.rbx << std::endl;
}

bool ValideMapping() noexcept {
    try {
        for (auto iter = gv::func_to_num.cbegin(); iter != gv::func_to_num.cend(); ++iter) {
            std::string label = iter->first;
            int num = iter->second;
            if (gv::num_to_comm[num]->GetLabel() != label) {
                std::cerr << gv::num_to_comm[num]->GetLabel() << ' ' << label << std::endl;
                return false;
            }
        }
        return true;
    } catch (std::exception e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

}
int main() {
    if (!ValideMapping()) {
        throw std::runtime_error("Mapping is not valid.");
    }
    std::cout << "It's The Powerful Processor" << std::endl;
    std::cout << "Enter scr filename:" << std::endl;
    std::string filename;
    std::cin >> filename;

    ConvertToByte(filename);
    ExecuteByteCode(filename);
    return 0;
}