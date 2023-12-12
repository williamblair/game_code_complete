#ifndef GCC4_LOGGER_H_INCLUDED
#define GCC4_LOGGER_H_INCLUDED

#include <string>
#include <map>
#include <list>

#define LOGFLAG_WRITE_TO_LOG_FILE (1<<0)
#define LOGFLAG_WRITE_TO_DEBUGGER (1<<1)

namespace Logger
{

class ErrorMessenger
{
public:
    ErrorMessenger();
    void Show(
        const std::string& errorMessage,
        bool isFatal,
        const char* funcName,
        const char* sourceFile,
        unsigned int lineNum
    );
private:
    bool m_enabled;
};

void Init(const char* loggingConfigFilename);
void Destroy();

void Log(
    const std::string& tag,
    const std::string& message,
    const char* funcName,
    const char* sourceFile,
    unsigned int lineNum
);
void SetDisplayFlags( const std::string& tag, unsigned char flags);

} // namespace Logger

#define GCC_FATAL(str) \
    { \
        static Logger::ErrorMessenger* pErrorMessenger = new Logger::ErrorMessenger; \
        std::string s((str)); \
        pErrorMessenger->Show(s, true, __FUNCTION__, __FILE__, __LINE__); \
    }

#define GCC_ERROR(str) \
    { \
        static Logger::ErrorMessenger* pErrorMessenger = new Logger::ErrorMessenger; \
        std::string s((str)); \
        pErrorMessenger->Show(s, false, __FUNCTION__, __FILE__, __LINE__); \
    }

#define GCC_WARNING(str) \
    { \
        std::string s((str)); \
        Logger::Log("WARNING", s, __FUNCTION__, __FILE__, __LINE__); \
    }

#define GCC_INFO(str) \
    { \
        std::string s((str)); \
        Logger::Log("INFO", s, NULL, NULL, 0); \
    }

#define GCC_LOG(tag, str) \
    { \
        std::string s((str)); \
        Logger::Log(tag, s, NULL, NULL, 0); \
    }

#define GCC_ASSERT(expr) \
    { \
        if (!(expr)) \
        { \
            static Logger::ErrorMessenger* pErrorMessenger = new Logger::ErrorMessenger; \
            pErrorMessenger->Show(#expr, false, __FUNCTION__, __FILE__, __LINE__); \
        } \
    }

#endif // GCC4_LOGGER_H_INCLUDED

