#include "MatrixStack.h"

MatrixStack::MatrixStack()
{}

MatrixStack::~MatrixStack()
{}

void MatrixStack::Push() { m_Stack.push(m_CurMatrix); }
void MatrixStack::Pop() {
    m_Stack.pop();
    if (m_Stack.size() > 0) {
        m_CurMatrix = m_Stack.top();
    } else {
        // identity
        m_CurMatrix = Mat4x4();
    }
}

void MatrixStack::MultMatrixLocal(const Mat4x4* pMat) {
    m_CurMatrix = (*pMat) * m_CurMatrix;
}

