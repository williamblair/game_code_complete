#include <cassert>
#include <cstdlib>
#include "Logger.h"
#include "xml.h"
#include "CriticalSection.h"

static const char* ERRORLOG_FILENAME = "error.log";

const unsigned char ERRORFLAG_DEFAULT = (LOGFLAG_WRITE_TO_DEBUGGER | LOGFLAG_WRITE_TO_LOG_FILE);
const unsigned char WARNINGFLAG_DEFAULT = (LOGFLAG_WRITE_TO_DEBUGGER | LOGFLAG_WRITE_TO_LOG_FILE);
const unsigned char LOGFLAG_DEFAULT = (LOGFLAG_WRITE_TO_DEBUGGER | LOGFLAG_WRITE_TO_LOG_FILE);

// singleton
class LogMgr;
static LogMgr* s_pLogMgr = nullptr;

class LogMgr
{
public:
    enum ErrorDialogResult
    {
        LOGMGR_ERROR_ABORT,
        LOGMGR_ERROR_RETRY,
        LOGMGR_ERROR_IGNORE
    };

    typedef std::map<std::string, unsigned char> Tags;
    typedef std::list<Logger::ErrorMessenger*> ErrorMessengerList;

    Tags m_tags;
    ErrorMessengerList m_errorMessengers;

    // thread safety
    CriticalSection m_tagCriticalSection;
    CriticalSection m_messengerCriticalSection;


    LogMgr();
    ~LogMgr();
    void Init(const char* loggingConfigFilename);

    void Log(
        const std::string& tag,
        const std::string& message,
        const char* funcName,
        const char* sourceFile,
        unsigned int lineNum
    );
    void SetDisplayFlags(const std::string& tag, unsigned char flags);

    void AddErrorMessenger(Logger::ErrorMessenger* pMessenger);
    LogMgr::ErrorDialogResult Error(
        const std::string& errorMessage,
        bool isFatal,
        const char* funcName,
        const char* sourceFile,
        unsigned int lineNum
    );

private:
    void OutputFinalBufferToLogs(const std::string& finalBuffer, unsigned char flags);
    void WriteToLogFile(const std::string& data);
    void GetOutputBuffer(
        std::string& outOutputBuffer,
        const std::string& tag,
        const std::string& message,
        const char* funcName,
        const char* sourceFile,
        unsigned int lineNum
    );
};

LogMgr::LogMgr()
{
    SetDisplayFlags("ERROR", ERRORFLAG_DEFAULT);
    SetDisplayFlags("WARNING", WARNINGFLAG_DEFAULT);
    SetDisplayFlags("INFO", LOGFLAG_DEFAULT);
}

LogMgr::~LogMgr()
{
    m_messengerCriticalSection.Lock();
    for (auto it = m_errorMessengers.begin(); it != m_errorMessengers.end(); ++it)
    {
        Logger::ErrorMessenger* pMessenger = *it;
        delete pMessenger;
    }
    m_errorMessengers.clear();
    m_messengerCriticalSection.Unlock();
}

void LogMgr::Init(const char* loggingConfigFilename)
{
    if (!loggingConfigFilename) {
        return;
    }

    //tinyxml2::XMLDocument loggingConfigFile(loggingConfigFilename);
    //if (!loggingConfigFile.LoadFile()) {
    tinyxml2::XMLDocument loggingConfigFile;
    if (!loggingConfigFile.LoadFile(loggingConfigFilename)) {
        return;
    }

    tinyxml2::XMLElement* pRoot = loggingConfigFile.RootElement();
    if (!pRoot) {
        return;
    }

    // loop through each child element and load the component
    for (tinyxml2::XMLElement* pNode = pRoot->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
    {
        unsigned char flags = 0;
        std::string tag(pNode->Attribute("tag"));

        int debugger = 0;
        debugger = atoi(pNode->Attribute("debugger"));
        if (debugger) {
            flags |= LOGFLAG_WRITE_TO_DEBUGGER;
        }

        int logfile = 0;
        logfile = atoi(pNode->Attribute("file"));
        if (logfile) {
            flags |= LOGFLAG_WRITE_TO_LOG_FILE;
        }

        SetDisplayFlags(tag, flags);
    }
}

void LogMgr::Log(
    const std::string& tag,
    const std::string& message,
    const char* funcName,
    const char* sourceFile,
    unsigned int lineNum)
{
    m_tagCriticalSection.Lock();
    Tags::iterator findIt = m_tags.find(tag);
    if (findIt != m_tags.end())
    {
        m_tagCriticalSection.Unlock();

        std::string buffer;
        GetOutputBuffer(buffer, tag, message, funcName, sourceFile, lineNum);
        OutputFinalBufferToLogs(buffer, findIt->second);
    }
    else
    {
        m_tagCriticalSection.Unlock();
    }
}

void LogMgr::SetDisplayFlags(const std::string& tag, unsigned char flags)
{
    m_tagCriticalSection.Lock();
    if (flags != 0) {
        Tags::iterator findIt = m_tags.find(tag);
        if (findIt == m_tags.end()) {
            m_tags.insert(std::make_pair(tag,flags));
        } else {
            findIt->second = flags;
        }
    } else {
        m_tags.erase(tag);
    }
    m_tagCriticalSection.Unlock();
}

void LogMgr::AddErrorMessenger(Logger::ErrorMessenger* pMessenger)
{
    m_messengerCriticalSection.Lock();
    m_errorMessengers.push_back(pMessenger);
    m_messengerCriticalSection.Unlock();
}

LogMgr::ErrorDialogResult LogMgr::Error(
    const std::string& errorMessage,
    bool isFatal,
    const char* funcName,
    const char* sourceFile,
    unsigned int lineNum)
{
    std::string tag = ((isFatal) ? ("FATAL") : ("ERROR"));

    // buffer for out final output string
    std::string buffer;
    GetOutputBuffer(buffer, tag, errorMessage, funcName, sourceFile, lineNum);

    // write the final buffer to all the various logs
    m_tagCriticalSection.Lock();
    Tags::iterator findIt = m_tags.find(tag);
    if (findIt != m_tags.end()) {
        OutputFinalBufferToLogs(buffer, findIt->second);
    }
    m_tagCriticalSection.Unlock();

    // show the dialog box
    //TODO
    //int result = ::MessageBoxA(NULL, buffer.c_str(), tag.c_str(), MB_ABORTRETRYIGNORE|MB_ICONERROR|MB_DEFBUTTON3);
    //switch (result)
    //{
    //case IDIGNORE: return LogMgr::LOGMGR_ERROR_IGNORE;
    //case IDABORT: __debugbreak(); return LogMgr::LOGMGR_ERROR_RETRY;
    //case IDRERTY: return LogMgr::LOGMGR_ERROR_RETRY;
    //default:
    //    break;
    //}
    return LogMgr::LOGMGR_ERROR_RETRY;
}

void LogMgr::OutputFinalBufferToLogs(
    const std::string& finalBuffer,
    unsigned char flags)
{
    if ((flags & LOGFLAG_WRITE_TO_LOG_FILE) > 0) {
        WriteToLogFile(finalBuffer);
    }
    //TODO
    //if ((flags & LOGFLAG_WRITE_TO_DEBUGGER) > 0) {
    //    ::OutputDebugStringA(finalBuffer.c_str());
    //}
}

void LogMgr::WriteToLogFile(const std::string& data)
{
    FILE* pLogFile = nullptr;
    pLogFile = fopen(ERRORLOG_FILENAME, "a+");
    if (!pLogFile) {
        return;
    }
    fprintf(pLogFile, data.c_str());
    fclose(pLogFile);
}

void LogMgr::GetOutputBuffer(
    std::string& outOutputBuffer,
    const std::string& tag,
    const std::string& message,
    const char* funcName,
    const char* sourceFile,
    unsigned int lineNum)
{
    if (!tag.empty()) {
        outOutputBuffer = "["+tag+"]" + message;
    } else {
        outOutputBuffer = message;
    }

    if (funcName) {
        outOutputBuffer += "\nFunction: ";
        outOutputBuffer += funcName;
    }

    if (sourceFile) {
        outOutputBuffer += "\n";
        outOutputBuffer += sourceFile;
    }

    if (lineNum) {
        outOutputBuffer += "\nLine: ";
        char lineNumBuffer[11];
        memset(lineNumBuffer, 0, sizeof(char));
        //outOutputBuffer += itoa(lineNum, lineNumBuffer, 10);
        sprintf(lineNumBuffer, "%d", lineNum);
        outOutputBuffer += lineNumBuffer;
    }

    outOutputBuffer += "\n";
}

namespace Logger
{

ErrorMessenger::ErrorMessenger()
{
    s_pLogMgr->AddErrorMessenger(this);
    m_enabled = true;
}

void ErrorMessenger::Show(
    const std::string& errorMessage,
    bool isFatal,
    const char* funcName,
    const char* sourceFile,
    unsigned int lineNum)
{
    if (!m_enabled) {
        return;
    }

    if (s_pLogMgr->Error(
            errorMessage,
            isFatal,
            funcName,
            sourceFile,
            lineNum) == LogMgr::LOGMGR_ERROR_IGNORE) {
        m_enabled = false;
    }
}

void Init(const char* loggingConfigFilename)
{
    if (!s_pLogMgr) {
        s_pLogMgr = new LogMgr;
        s_pLogMgr->Init(loggingConfigFilename);
    }
}

void Destroy()
{
    delete s_pLogMgr;
    s_pLogMgr = nullptr;
}

void Log(
    const std::string& tag,
    const std::string& message,
    const char* funcName,
    const char* sourceFile,
    unsigned int lineNum)
{
    assert(s_pLogMgr);
    s_pLogMgr->Log(tag, message, funcName, sourceFile, lineNum);
}

void SetDisplayFlags(const std::string& tag, unsigned char flags)
{
    assert(s_pLogMgr);
    s_pLogMgr->SetDisplayFlags(tag, flags);
}

} // namespace Logger

