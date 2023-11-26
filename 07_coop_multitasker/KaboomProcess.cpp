#include "KaboomProcess.h"
#include <iostream>

KaboomProcess::KaboomProcess()
{
    m_message = "Kaboom process exploded!";
}

void KaboomProcess::VOnUpdate(uint64_t deltaMs)
{
    std::cout << m_message << std::endl;
    Succeed();
}
