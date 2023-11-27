#include "DecompressionProcess.h"
#include "ZipFile.h"

DecompressionProcess::DecompressionProcess() :
    m_bRunThread(true)
{
    m_DecompressRequestDelegate.reset(
        DECL_MBR_DELEGATE(
            &DecompressionProcess::DecompressRequestDelegate
        )
    );
    IEventManager::GetInstance()->VAddListener(
        m_DecompressRequestDelegate,
        EvtDataDecompressRequest::sk_EventType
    );
}

DecompressionProcess::~DecompressionProcess()
{
    IEventManager::GetInstance()->VRemoveListener(
        m_DecompressRequestDelegate,
        EvtDataDecompressRequest::sk_EventType
    );
    m_bRunThread = false;
}

void DecompressionProcess::DecompressRequestDelegate(IEventDataPtr pEventData)
{
    IEventDataPtr pEventClone = pEventData->VCopy();
    m_realtimeEventQueue.push(pEventClone);
}

void DecompressionProcess::VThreadProc()
{
    m_bRunThread = true;
    while (m_bRunThread)
    {
        // check the queue for events
        IEventDataPtr e;
        if (!m_realtimeEventQueue.try_pop(e)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
        if (e->VGetEventType() != EvtDataDecompressRequest::sk_EventType) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        std::shared_ptr<EvtDataDecompressRequest> pDecomp =
            std::static_pointer_cast<EvtDataDecompressRequest>(e);

        ZipFile zipFile;
        if (!zipFile.Init(pDecomp->m_zipFileName.c_str())) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        int size = 0;
        int resourceNum = zipFile.Find(pDecomp->m_fileName.c_str());
        if (resourceNum >= 0) {
            size = zipFile.GetFileLen(resourceNum);
            char* buffer = new char[size];
            zipFile.ReadFile(resourceNum, buffer);

            // send decompression result event
            IEventDataPtr pRes(
                new EvtDataDecompressProgress(
                    100,
                    pDecomp->m_zipFileName,
                    pDecomp->m_fileName,
                    buffer
                )
            );
            IEventManager::GetInstance()->VThreadSafeQueueEvent(e);
        }
    }
    Succeed();
}

