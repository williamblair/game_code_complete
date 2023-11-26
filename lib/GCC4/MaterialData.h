#ifndef GCC4_MATERIAL_DATA_H_INCLUDED
#define GCC4_MATERIAL_DATA_H_INCLUDED

struct MaterialData
{
    float restitution;
    float friction;
    
    MaterialData(float _restitution, float _friction) :
        restitution(_restitution),
        friction(_friction)
    {}
    
    MaterialData(const MaterialData& other) :
        restitution(other.restitution),
        friction(other.friction)
    {}
};

#endif // GCC4_MATERIAL_DATA_H_INCLUDED
