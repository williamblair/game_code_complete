#ifndef GCC4_MATRIX_STACK_H_INCLUDED
#define GCC4_MATRIX_STACK_H_INCLUDED

#include <stack>
#include "GCCMath.h"

class MatrixStack
{
public:
    MatrixStack();
    ~MatrixStack();

    /** @brief pushes the current matrix onto the stack */
    void Push();

    /** @brief removes the current matrix from the top of the stack */
    void Pop();

    /** @brief determines the product of the given matrix and the current matrix */
    void MultMatrixLocal(const Mat4x4* pMat);

    /** @brief retrieves the current matrix */
    Mat4x4* GetTop() { return &m_CurMatrix; }

private:
    std::stack<Mat4x4> m_Stack;
    Mat4x4 m_CurMatrix;
};

#endif // GCC4_MATRIX_STACK_H_INCLUDED
