#ifndef GCC4_PROCESS_TYPES_H_INCLUDED
#define GCC4_PROCESS_TYPES_H_INCLUDED

#include <memory>

// forward declaration
class Process;

typedef std::shared_ptr<Process> StrongProcessPtr;
typedef std::weak_ptr<Process> WeakProcessPtr;

#endif // GCC4_PROCESS_TYPES_H_INCLUDED

