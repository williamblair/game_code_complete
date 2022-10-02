#ifndef MATRIX_STACK_H_INCLUDED
#define MATRIX_STACK_H_INCLUDED

#include <stack>
#include <GCCMath.h>

class MatrixStack
{
public:
    MatrixStack();
    ~MatrixStack();
private:
    std::stack<Mat4x4> m_Stack;
};

#endif // MATRIX_STACK_H_INCLUDED
