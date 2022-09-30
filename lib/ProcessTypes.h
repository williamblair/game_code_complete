#ifndef PROCESS_TYPES_H_INCLUDED
#define PROCESS_TYPES_H_INCLUDED

#include <memory>

// forward declaration
class Process;

typedef std::shared_ptr<Process> StrongProcessPtr;
typedef std::weak_ptr<Process> WeakProcessPtr;

#endif // PROCESS_TYPES_H_INCLUDED

