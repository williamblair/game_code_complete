#ifndef GCC4_OGL_VERTEX_BUFFER_H_INCLUDED
#define GCC4_OGL_VERTEX_BUFFER_H_INCLUDED

#include <cstdint>
#include <GL/glew.h>

class OGLVertexBuffer
{
    friend class OGLRenderer;
public:
    OGLVertexBuffer();
    ~OGLVertexBuffer();

    /** @brief The format of the vertex data, corresponding to one of the Vertex* structs */
    enum Type
    {
        POS,
        POS_COLOR,
        POS_TEXCOORD,
        UNINITIALIZED
    };

    /** @brief How often this buffer will be updated */
    enum Usage
    {
        USAGE_STATIC,
        USAGE_DYNAMIC
    };

    struct VertexPos
    {
        float x,y,z;
    };
    struct VertexColored
    {
        float x,y,z;
        float nx,ny,nz;
        float r,g,b;
    };
    struct VertexTextured
    {
        float x,y,z;
        float nx,ny,nz;
        float u,v;
    };

     /**
     * @brief Create the internal buffer and copy the given vertex data into it
     * 
     * @param type the type of vertex data contained in the buffer
     * @param pVerts pointer array of one of the VertexColored/VertexTextured struct data formats
     * @param numVerts the number of VertexColored/VertexTextured structs in vertices
     * @param pIndices pointer to indices for the buffer, or null
     * @param numIndices size of the indices array (0 if none)
     * @param usage wether this buffer will be updated often (USAGE_DYNAMIC) or not (USAGE_STATIC)
     * @return true on success, false on failure
     */
    bool Init(
        Type type,
        void* pVerts, size_t numVerts,
        uint32_t* pIndices, size_t numIndices,
        Usage usage = USAGE_STATIC
    );

    /**
     * @brief store new input data in the vertex buffer
     * 
     * @param pVerts the new data to store; must be same type used in Init()
     * @param numVerts the number of VertexColored/VertexTextured structs in vertices.
     *      must be same size as what was given in Init()
     * @return true on success, false on failure
     */
    bool Update(void* pVerts, size_t numVerts);

private:
    Type mType;
    GLuint mVAO, mVBO, mEBO;
    size_t mNumVerts;
    size_t mVertStride;
    size_t mNumIndices;
};

#endif // GCC4_OGL_VERTEX_BUFFER_H_INCLUDED
