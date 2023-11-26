#ifndef GCC4_GENERIC_FACTORY_H_INCLUDED
#define GCC4_GENERIC_FACTORY_H_INCLUDED

#include <map>

template<class BaseType, class SubType>
BaseType* GenericObjectCreationFunction() { return new SubType; }

template<class BaseClass, class IdType>
class GenericObjectFactory
{
    typedef BaseClass* (*ObjectCreationFunction)(void);

public:
    template<class SubClass>
    bool Register(IdType id) {
        auto findIt = m_CreationFunctions.find(id);
        if (findIt == m_CreationFunctions.end())
        {
            m_CreationFunctions[id] = &GenericObjectCreationFunction<BaseClass,SubClass>;
            return true;
        }
        return false;
    }

    BaseClass* Create(IdType id) {
        auto findIt = m_CreationFunctions.find(id);
        if (findIt != m_CreationFunctions.end())
        {
            ObjectCreationFunction pFunc = findIt->second;
            return pFunc();
        }
        return nullptr;
    }
private:    
    std::map<IdType, ObjectCreationFunction> m_CreationFunctions;
};

#endif // GCC4_GENERIC_FACTORY_H_INCLUDED
