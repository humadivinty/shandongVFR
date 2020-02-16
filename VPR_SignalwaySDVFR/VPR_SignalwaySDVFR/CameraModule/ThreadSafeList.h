#ifndef THREADSAFELIST_H
#define THREADSAFELIST_H
//#include "utilityTool/log4z.h"
#include <windows.h>
#include<deque>


class MySafeLocker
{
public:
    MySafeLocker(CRITICAL_SECTION* inHandle) : m_handle(inHandle)
    {
        if (NULL != m_handle)
        {
            EnterCriticalSection(m_handle);
        }
    }

    ~MySafeLocker()
    {
        if (NULL != m_handle)
        {
            LeaveCriticalSection(m_handle);
            m_handle = NULL;
        }
    }
private:
    CRITICAL_SECTION* m_handle;
};

template <typename T>
class TemplateThreadSafeList
{
public:
    TemplateThreadSafeList();
    ~TemplateThreadSafeList();

public:
    size_t size();
    bool empty();
    void clear();

    T front();
    void pop_front();
    void pop_back();
    void push_back(T result);
private:
    CRITICAL_SECTION m_hcs;
    std::deque<T> m_list;
    
    friend class MySafeLocker;
};

template <typename T>
void TemplateThreadSafeList<T>::clear()
{
    MySafeLocker locker(&m_hcs);
    m_list.clear();
}

template <typename T>
size_t TemplateThreadSafeList<T>::size()
{
    MySafeLocker locker(&m_hcs);
    return m_list.size();
}

template <typename T>
bool TemplateThreadSafeList<T>::empty()
{
    MySafeLocker locker(&m_hcs);

    return m_list.empty();
}

template <typename T>
T TemplateThreadSafeList<T>::front()
{
    try
    {
        //std::unique_lock<std::mutex> locker(m_mtx);
        MySafeLocker locker(&m_hcs);
        T value;
        if (!m_list.empty())
        {
            value = m_list.front();
        }
        return value;
    }
    catch (std::bad_exception& e)
    {
        printf("TemplateList::front, bad_exception, error msg = %s", e.what());
        return T();
    }
    catch (std::bad_alloc& e)
    {
        printf("TemplateList::front, bad_alloc, error msg = %s", e.what());
        return T();
    }
    catch (std::exception& e)
    {
        printf("TemplateList::front, exception, error msg = %s.", e.what());
        return T();
    }
    catch (void*)
    {
        printf("TemplateList::front,  void* exception");
        return T();
    }
    catch (...)
    {
        printf("TemplateList::front,  unknown exception");
        return T();
    }
}

template <typename T>
void TemplateThreadSafeList<T>::pop_front()
{
    try
    {
        //std::unique_lock<std::mutex> locker(m_mtx);
        MySafeLocker locker(&m_hcs);

        if (!m_list.empty())
        {
            m_list.pop_front();
        }
    }
    catch (std::bad_exception& e)
    {
        printf("TemplateList::pop_front, bad_exception, error msg = %s", e.what());
    }
    catch (std::bad_alloc& e)
    {
        printf("TemplateList::pop_front, bad_alloc, error msg = %s", e.what());
    }
    catch (std::exception& e)
    {
        printf("TemplateList::pop_front, exception, error msg = %s.", e.what());
    }
    catch (void*)
    {
        printf("TemplateList::pop_front,  void* exception");
    }
    catch (...)
    {
        printf("TemplateList::pop_front,  unknown exception");
    }
}

template <typename T>
void TemplateThreadSafeList<T>::pop_back()
{
    try
    {
        //std::unique_lock<std::mutex> locker(m_mtx);
        MySafeLocker locker(&m_hcs);

        if (!m_list.empty())
        {
            m_list.pop_back();
        }
    }
    catch (std::bad_exception& e)
    {
        printf("TemplateList::pop_back, bad_exception, error msg = %s", e.what());
    }
    catch (std::bad_alloc& e)
    {
        printf("TemplateList::pop_back, bad_alloc, error msg = %s", e.what());
    }
    catch (std::exception& e)
    {
        printf("TemplateList::pop_back, exception, error msg = %s.", e.what());
    }
    catch (void*)
    {
        printf("TemplateList::pop_back,  void* exception");
    }
    catch (...)
    {
        printf("TemplateList::pop_back,  unknown exception");
    }
}

template <typename T>
void TemplateThreadSafeList<T>::push_back(T result)
{
    try
    {
        //std::unique_lock<std::mutex> locker(m_mtx);
        MySafeLocker locker(&m_hcs);

        m_list.push_back(result);
    }
    catch (std::bad_exception& e)
    {
        printf("TemplateList::push_back, bad_exception, error msg = %s", e.what());
    }
    catch (std::bad_alloc& e)
    {
        printf("TemplateList::push_back, bad_alloc, error msg = %s", e.what());
    }
    catch (std::exception& e)
    {
        printf("TemplateList::push_back, exception, error msg = %s.", e.what());
    }
    catch (void*)
    {
        printf("TemplateList::push_back,  void* exception");
    }
    catch (...)
    {
        printf("TemplateList::push_back,  unknown exception");
    }
}

template <typename T>
TemplateThreadSafeList<T>::~TemplateThreadSafeList()
{
    DeleteCriticalSection(&m_hcs);
}

template <typename T>
TemplateThreadSafeList<T>::TemplateThreadSafeList()
{
    InitializeCriticalSection(&m_hcs);
}


#endif