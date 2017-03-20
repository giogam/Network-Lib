#include "../../../include/lib_base.h"
#include "../../../include/thread.h"

static void* runThread(void* arg)
{
    return ((thread*)arg)->run();
}

thread::thread() : m_tid(0), m_running(0), m_detached(0) {  }

thread::~thread()
{
    if (m_running == 1 && m_detached == 0)
    {
        pthread_detach(m_tid);
    }
    if (m_running == 1)
    {
        pthread_cancel(m_tid);
    }
}

int thread::start()
{
    int result = pthread_create(&m_tid, NULL, runThread, this);
    if (result == 0)
    {
        m_running = 1;
    }
    return result;
}


int thread::join()
{
    int result = -1;
    if (m_running == 1)
    {
        result = pthread_join(m_tid, NULL);
        if (result == 0)
        {
            m_detached = 1;
        }
    }
    return result;
}

int thread::detach()
{
    int result = -1;
    if (m_running == 1 && m_detached == 0)
    {
        result = pthread_detach(m_tid);
        if (result == 0)
        {
            m_detached = 1;
        }
    }
    return result;
}

pthread_t thread::self()
{
    return m_tid;
}
