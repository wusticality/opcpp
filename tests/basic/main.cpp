#include <iostream>
#include "generated/Generated.oohindex"

int main()
{
    test::Test test;

    std::cout << "\tm_transient_member" << test.m_transient_member << std::endl;
    std::cout << "\tm_native_member" << test.m_native_member << std::endl;

    return 0;
}
