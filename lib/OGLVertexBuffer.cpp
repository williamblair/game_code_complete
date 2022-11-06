#include <cstdio>
#include "OGLVertexBuffer.h"

OGLVertexBuffer::OGLVertexBuffer() :
    mType(OGLVertexBuffer::UNINITIALIZED),
    mVAO(0), mVBO(0), mEBO(0),
    mNumVerts(0),
    mVertStride(0),
    mNumIndices(0)
{}

OGLVertexBuffer::~OGLVertexBuffer()
{
    if (mNumVerts != 0) {
        glDeleteBuffers(1, &mVBO);
    }
    if (mNumIndices != 0) {
        glDeleteBuffers(1, &mEBO);
    }
    if (mVAO != 0) {
        glDeleteVertexArrays(1, &mVAO);
    }
}

bool OGLVertexBuffer::Init(
    Type type,
    void* pVerts, size_t numVerts,
    uint32_t* pIndices, size_t numIndices,
    Usage usage
)
{
    int attribLocation; // aPos, where we set location = 0
    int dataType;
    int shouldNormalize;
    int floatsPerVertex;
    void* beginOffset;

    switch (type)
    {
    case POS: mVertStride = sizeof(VertexPos); break;
    case POS_COLOR: mVertStride = sizeof(VertexColored); break;
    case POS_TEXCOORD: mVertStride = sizeof(VertexTextured); break;
    default:
        printf("Unhandled Vertex Type: %d\n", (int)type);
        return false;
    }
    
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    if (pIndices != nullptr && numIndices > 0) {
        glGenBuffers(1, &mEBO);
    }
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        numVerts * mVertStride,
        pVerts,
        (usage == USAGE_STATIC) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW
    );
    if (pIndices != nullptr && numIndices > 0)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            numIndices * sizeof(uint32_t),
            pIndices,
            GL_STATIC_DRAW
        );
    }
    
    mType = type;
    mNumVerts = numVerts;
    mNumIndices = numIndices;
    switch (type)
    {
    case POS:
        // position
        attribLocation = 0; // aPos, where we set location = 0
        dataType = GL_FLOAT;
        shouldNormalize = GL_FALSE;
        floatsPerVertex = 3; // xyz
        beginOffset = (void*)0;
        glVertexAttribPointer(
            attribLocation,
            floatsPerVertex,
            dataType,
            shouldNormalize,
            mVertStride,
            beginOffset
        );
        glEnableVertexAttribArray(attribLocation);
        break;
    case POS_COLOR:
        // position
        attribLocation = 0; // aPos, where we set location = 0
        dataType = GL_FLOAT;
        shouldNormalize = GL_FALSE;
        floatsPerVertex = 3; // xyz
        beginOffset = (void*)0;
        glVertexAttribPointer(
            attribLocation,
            floatsPerVertex,
            dataType,
            shouldNormalize,
            mVertStride,
            beginOffset
        );
        glEnableVertexAttribArray(attribLocation);
        // normal 
        attribLocation = 1; // aNormal, where we set location = 1
        dataType = GL_FLOAT;
        shouldNormalize = GL_FALSE;
        floatsPerVertex = 3; // nx,ny,nz
        beginOffset = (void*)(3*sizeof(float)); // skip xyz
        glVertexAttribPointer(
            attribLocation,
            floatsPerVertex,
            dataType,
            shouldNormalize,
            mVertStride,
            beginOffset
        );
        glEnableVertexAttribArray(attribLocation);
        
        // color
        attribLocation = 2; // aColor, where we set location = 2
        dataType = GL_FLOAT;
        shouldNormalize = GL_FALSE;
        floatsPerVertex = 3; // rgb
        beginOffset = (void*)(6*sizeof(float)); // skip xyz, nxnynz
        glVertexAttribPointer(
            attribLocation,
            floatsPerVertex,
            dataType,
            shouldNormalize,
            mVertStride,
            beginOffset
        );
        glEnableVertexAttribArray(attribLocation);
        break;
    case POS_TEXCOORD:
        // position
        attribLocation = 0; // aPos, where we set location = 0
        dataType = GL_FLOAT;
        shouldNormalize = GL_FALSE;
        floatsPerVertex = 3; // xyz
        beginOffset = (void*)0;
        glVertexAttribPointer(
            attribLocation,
            floatsPerVertex,
            dataType,
            shouldNormalize,
            mVertStride,
            beginOffset
        );
        glEnableVertexAttribArray(attribLocation);
        // normal 
        attribLocation = 1; // aNormal, where we set location = 1
        dataType = GL_FLOAT;
        shouldNormalize = GL_FALSE;
        floatsPerVertex = 3; // nx,ny,nz
        beginOffset = (void*)(3*sizeof(float)); // skip xyz
        glVertexAttribPointer(
            attribLocation,
            floatsPerVertex,
            dataType,
            shouldNormalize,
            mVertStride,
            beginOffset
        );
        glEnableVertexAttribArray(attribLocation);
        
        // tex coord
        attribLocation = 2; // aTexCoord, where we set location = 2
        dataType = GL_FLOAT;
        shouldNormalize = GL_FALSE;
        floatsPerVertex = 2; // uv
        beginOffset = (void*)(6*sizeof(float)); // skip xyz, nxnynz
        glVertexAttribPointer(
            attribLocation,
            floatsPerVertex,
            dataType,
            shouldNormalize,
            mVertStride,
            beginOffset
        );
        glEnableVertexAttribArray(attribLocation);
        break;
    default:
        printf("Unhandled VertexBuffer type\n");
        return false;
        break;
    }
    
    mNumVerts = numVerts;
    
    // unbind the current buffers
    // ORDER MATTERS - the VAO must be unbinded FIRST!
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if (pIndices != nullptr && numIndices > 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    
    return true;
}

