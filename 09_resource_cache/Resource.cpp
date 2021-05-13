#include <algorithm>
#include <Resource.h>

Resource::Resource( const std::string& name ) :
    m_name( name )
{
    // convert to lowercase
    std::transform( m_name.begin(),
                    m_name.end(), 
                    m_name.begin(), 
                    (int(*)(int)) std::tolower );
}
