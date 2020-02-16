#include "stdafx.h"
#include "ResultListManager.h"
#include "utilityTool/ToolFunction.h"
#include "utilityTool/log4z.h"
#include <new>
#include <exception>


ResultListManager::ResultListManager()
{
    InitializeCriticalSection(&m_hcs);
}

ResultListManager::~ResultListManager()
{
    //在vs2015之前的版本,在dll中 使用std::mutex 会有bug
    ClearALLResult();
    DeleteCriticalSection(&m_hcs);
}

size_t ResultListManager::size()
{
    std::unique_lock<std::mutex> locker(m_mtx);
    //MYLocker locker(&m_hcs);
    return m_list.size();   
}

bool ResultListManager::empty()
{
    std::unique_lock<std::mutex> locker(m_mtx);
    //MYLocker locker(&m_hcs);

    return m_list.empty();
}

void ResultListManager::front(Result_Type& result)
{
    try
    {
        std::unique_lock<std::mutex> locker(m_mtx);
        //MYLocker locker(&m_hcs);
        Result_Type value;
        if (!m_list.empty())
        {
            result = m_list.front();
            //m_list.pop_front();
        }
    }
    catch (std::bad_exception& e)
    {
        LOGFMTE("ResultListManager::front, bad_exception, error msg = %s", e.what());
        //return std::shared_ptr<CameraResult>();
    }
    catch (std::bad_alloc& e)
    {
        LOGFMTE("ResultListManager::front, bad_alloc, error msg = %s", e.what());
        //return std::shared_ptr<CameraResult>();
    }
    catch (std::exception& e)
    {
        LOGFMTE("ResultListManager::front, exception, error msg = %s.", e.what());
        //return std::shared_ptr<CameraResult>();
    }
    catch (void*)
    {
        LOGFMTE("ResultListManager::front,  void* exception");
        //return std::shared_ptr<CameraResult>();
    }
    catch (...)
    {
        LOGFMTE("ResultListManager::front,  unknown exception");
       // return std::shared_ptr<CameraResult>();
    }
}

void ResultListManager::back(Result_Type& result)
{
    try
    {
        std::unique_lock<std::mutex> locker(m_mtx);
        //MYLocker locker(&m_hcs);
        Result_Type value;
        if (!m_list.empty())
        {
            result = m_list.back();
            //m_list.pop_front();
        }
    }
    catch (std::bad_exception& e)
    {
        LOGFMTE("ResultListManager::front, bad_exception, error msg = %s", e.what());
        //return std::shared_ptr<CameraResult>();
    }
    catch (std::bad_alloc& e)
    {
        LOGFMTE("ResultListManager::front, bad_alloc, error msg = %s", e.what());
        //return std::shared_ptr<CameraResult>();
    }
    catch (std::exception& e)
    {
        LOGFMTE("ResultListManager::front, exception, error msg = %s.", e.what());
        //return std::shared_ptr<CameraResult>();
    }
    catch (void*)
    {
        LOGFMTE("ResultListManager::front,  void* exception");
        //return std::shared_ptr<CameraResult>();
    }
    catch (...)
    {
        LOGFMTE("ResultListManager::front,  unknown exception");
        // return std::shared_ptr<CameraResult>();
    }
}

ResultListManager::Result_Type ResultListManager::GetOneByIndex(int iPosition)
{
    try
    {
        std::unique_lock<std::mutex> locker(m_mtx);
        //MYLocker locker(&m_hcs);
        Result_Type value;
        if (m_list.empty())
        {
            return value;
        }

        int iPos = 0;
        for (list_Type::const_iterator it = m_list.cbegin(); it != m_list.cend(); it++)
        {
            if (iPos == iPosition)
            {
                value = *it;
                break;
            }
            iPos++;
        }
        return value;
    }
    catch (std::bad_exception& e)
    {
        LOGFMTE("GetOneByIndex, bad_exception, error msg = %s", e.what());
        return std::shared_ptr<CameraResult>();
    }
    catch (std::bad_alloc& e)
    {
        LOGFMTE("GetOneByIndex, bad_alloc, error msg = %s", e.what());
        return std::shared_ptr<CameraResult>();
    }
    catch (std::exception& e)
    {
        LOGFMTE("GetOneByIndex, exception, error msg = %s.", e.what());
        return std::shared_ptr<CameraResult>();
    }
    catch (void*)
    {
        LOGFMTE("ResultListManager::GetOneByIndex,index = %d,   void* exception", iPosition);
        return std::shared_ptr<CameraResult>();
    }
    catch (...)
    {
        LOGFMTE("ResultListManager::GetOneByIndex,index = %d,   unknown exception", iPosition);
        return std::shared_ptr<CameraResult>();
    }
}

ResultListManager::Result_Type ResultListManager::GetOneByCarid(DWORD dwCarID)
{
    try
    {
        std::unique_lock<std::mutex> locker(m_mtx);
        //MYLocker locker(&m_hcs);
        Result_Type value;
        if (m_list.empty())
        {
            return value;
        }

        for (list_Type::const_iterator it = m_list.cbegin(); it != m_list.cend(); it++)
        {
            Result_Type TempValue = *it;
            if (TempValue->dwCarID == dwCarID)
            {
                value = *it;
                break;
            }
        }
        return value;
    }
    catch (std::bad_exception& e)
    {
        LOGFMTE("GetOneByIndex, bad_exception, error msg = %s", e.what());
        return std::shared_ptr<CameraResult>();
    }
    catch (std::bad_alloc& e)
    {
        LOGFMTE("GetOneByIndex, bad_alloc, error msg = %s", e.what());
        return std::shared_ptr<CameraResult>();
    }
    catch (std::exception& e)
    {
        LOGFMTE("GetOneByIndex, exception, error msg = %s.", e.what());
        return std::shared_ptr<CameraResult>();
    }
    catch (void*)
    {
        LOGFMTE("ResultListManager::GetOneByIndex,dwCarID = %lu,   void* exception", dwCarID);
        return std::shared_ptr<CameraResult>();
    }
    catch (...)
    {
        LOGFMTE("ResultListManager::GetOneByIndex,dwCarID = %lu,   unknown exception", dwCarID);
        return std::shared_ptr<CameraResult>();
    }
}

ResultListManager::Result_Type ResultListManager::GetOneByPlateNumber(const char *plateNo)
{
    int iIndex = GetPositionByPlateNo(plateNo);
    if(iIndex == -1)
        return std::shared_ptr<CameraResult>();

    return GetOneByIndex(iIndex);
}

void ResultListManager::pop_front()
{
    try
    {
        //std::unique_lock<std::mutex> locker(m_mtx);
        MYLocker locker(&m_hcs);

        if (!m_list.empty())
        {
            m_list.pop_front();
        }
    }
    catch (std::bad_exception& e)
    {
        LOGFMTE("ResultListManager::pop_front, bad_exception, error msg = %s", e.what());
    }
    catch (std::bad_alloc& e)
    {
        LOGFMTE("ResultListManager::pop_front, bad_alloc, error msg = %s", e.what());
    }
    catch (std::exception& e)
    {
        LOGFMTE("ResultListManager::pop_front, exception, error msg = %s.", e.what());
    }
    catch (void*)
    {
        LOGFMTE("ResultListManager::pop_front,  void* exception");
    }
    catch (...)
    {
        LOGFMTE("ResultListManager::pop_front,  unknown exception");
    }
}

void ResultListManager::pop_back()
{
    try
    {
        std::unique_lock<std::mutex> locker(m_mtx);
        //MYLocker locker(&m_hcs);

        if (!m_list.empty())
        {
            m_list.pop_back();
        }
    }
    catch (std::bad_exception& e)
    {
        LOGFMTE("ResultListManager::pop_back, bad_exception, error msg = %s", e.what());
    }
    catch (std::bad_alloc& e)
    {
        LOGFMTE("ResultListManager::pop_back, bad_alloc, error msg = %s", e.what());
    }
    catch (std::exception& e)
    {
        LOGFMTE("ResultListManager::pop_back, exception, error msg = %s.", e.what());
    }
    catch (void*)
    {
        LOGFMTE("ResultListManager::pop_back,  void* exception");
    }
    catch (...)
    {
        LOGFMTE("ResultListManager::pop_back,  unknown exception");
    }
}

void ResultListManager::push_back(Result_Type result)
{
    try
    {
        std::unique_lock<std::mutex> locker(m_mtx);
        //MYLocker locker(&m_hcs);
        if (result != NULL)
        {
            m_list.push_back(result);
        }
    }
    catch (std::bad_exception& e)
    {
        LOGFMTE("ResultListManager::push_back, bad_exception, error msg = %s", e.what());
    }
    catch (std::bad_alloc& e)
    {
        LOGFMTE("ResultListManager::push_back, bad_alloc, error msg = %s", e.what());
    }
    catch (std::exception& e)
    {
        LOGFMTE("ResultListManager::push_back, exception, error msg = %s.", e.what());
    }
    catch (void*)
    {
        LOGFMTE("ResultListManager::push_back,  void* exception");
    }
    catch (...)
    {
        LOGFMTE("ResultListManager::push_back,  unknown exception");
    }
}

int ResultListManager::GetPositionByPlateNo(const char* plateNo)
{
    try
    {
        std::unique_lock<std::mutex> locker(m_mtx);
        //MYLocker locker(&m_hcs);
        int iRet = -1;
        if (m_list.empty() || NULL == plateNo)
        {
            return iRet;
        }
        int iPos = 0;
        std::string strSrcPlate(plateNo);
        for (list_Type::const_iterator it = m_list.cbegin(); it != m_list.cend(); it++)
        {
            Result_Type value = *it;
            std::string strDestPlate(value->chPlateNO);
            if (std::string::npos != strSrcPlate.find(strDestPlate))
            {
                iRet = iPos;
                break;
            }
            else if (Tool_DimCompare(strSrcPlate.c_str(), strDestPlate.c_str()))
            {
                iRet = iPos;
                break;
            }
            else
            {
                iPos++;
            }
        }
        return iRet;
    }
    catch (std::bad_exception& e)
    {
        LOGFMTE("ResultListManager::GetPositionByPlateNo, bad_exception, error msg = %s", e.what());
        return -1;
    }
    catch (std::bad_alloc& e)
    {
        LOGFMTE("ResultListManager::GetPositionByPlateNo, bad_alloc, error msg = %s", e.what());
        return -1;
    }
    catch (std::exception& e)
    {
        LOGFMTE("ResultListManager::GetPositionByPlateNo, exception, error msg = %s.", e.what());
        return -1;
    }
    catch (void*)
    {
        LOGFMTE("ResultListManager::GetPositionByPlateNo,  void* exception");
        return -1;
    }
    catch (...)
    {
        LOGFMTE("ResultListManager::GetPositionByPlateNo,  unknown exception");
        return -1;
    }
}

void ResultListManager::DeleteToPosition(int position)
{
    try
    {
        if (position < 0)
        {
            return;
        }
        std::unique_lock<std::mutex> locker(m_mtx);
        //MYLocker locker(&m_hcs);
        size_t iPosition = (position >= m_list.size()) ? (m_list.size() - 1) : position;
        if (!m_list.empty())
        {
            for (int i = 0; i <= iPosition; i++)
            {
                m_list.pop_front();
            }
        }
    }
    catch (std::bad_exception& e)
    {
        LOGFMTE("ResultListManager::DeleteToPosition, bad_exception, error msg = %s", e.what());
    }
    catch (std::bad_alloc& e)
    {
        LOGFMTE("ResultListManager::DeleteToPosition, bad_alloc, error msg = %s", e.what());
    }
    catch (std::exception& e)
    {
        LOGFMTE("ResultListManager::DeleteToPosition, exception, error msg = %s.", e.what());
    }
    catch (void*)
    {
        LOGFMTE("ResultListManager::DeleteToPosition,  void* exception");
    }
    catch (...)
    {
        LOGFMTE("ResultListManager::DeleteToPosition,  unknown exception");
    }
}

void ResultListManager::DeleteByCarID(DWORD dwCarID)
{
    try
    {
        std::unique_lock<std::mutex> locker(m_mtx);
        //MYLocker locker(&m_hcs);
        Result_Type value;
        if (m_list.empty())
        {
            return ;
        }

        for (list_Type::iterator it = m_list.begin(); it != m_list.end(); it++)
        {
            Result_Type TempValue = *it;
            if (TempValue->dwCarID == dwCarID)
            {
                m_list.erase(it);
                break;
            }
        }
    }
    catch (std::bad_exception& e)
    {
        LOGFMTE(" bad_exception, error msg = %s", e.what());
    }
    catch (std::bad_alloc& e)
    {
        LOGFMTE("bad_alloc, error msg = %s", e.what());
    }
    catch (std::exception& e)
    {
        LOGFMTE("exception, error msg = %s.", e.what());
    }
    catch (void*)
    {
        LOGFMTE("dwCarID = %lu,   void* exception", dwCarID);
    }
    catch (...)
    {
        LOGFMTE("  unknown exception", dwCarID);
    }
}

bool ResultListManager::ReplaceByCarID(DWORD dwCarID, Result_Type result)
{
    bool bet = false;
    try
    {
        std::unique_lock<std::mutex> locker(m_mtx);
        if (m_list.empty())
        {
            return bet;
        }

        for (list_Type::iterator it = m_list.begin(); it != m_list.end(); it++)
        {
            Result_Type TempValue = *it;
            if (TempValue->dwCarID == dwCarID)
            {
                *it = result;
                bet = true;
                break;
            }
        }
        return bet;
    }
    catch (std::bad_exception& e)
    {
        LOGFMTE(" bad_exception, error msg = %s", e.what());
        return bet;
    }
    catch (std::bad_alloc& e)
    {
        LOGFMTE(" bad_alloc, error msg = %s", e.what());
        return bet;
    }
    catch (std::exception& e)
    {
        LOGFMTE(" exception, error msg = %s.", e.what());
        return bet;
    }
    catch (void*)
    {
        LOGFMTE("dwCarID = %lu,   void* exception", dwCarID);
        return bet;
    }
    catch (...)
    {
        LOGFMTE("dwCarID = %lu,   unknown exception", dwCarID);
        return bet;
    }
}

std::string ResultListManager::GetAllPlateString()
{
    try
    {
        std::unique_lock<std::mutex> locker(m_mtx);
        //MYLocker locker(&m_hcs);
        std::string strPlateListString;
        if (m_list.empty())
        {
            return strPlateListString;
        }
        else
        {
            char chValue[256] = {0};
            for (list_Type::const_iterator it = m_list.cbegin(); it != m_list.cend(); it++)
            {
                Result_Type value = *it;
                strPlateListString.append(value->chPlateNO);

                memset(chValue, '\0', sizeof(chValue));
                sprintf(chValue, "  CarID: %lu \n", value->dwCarID);
                strPlateListString.append(chValue);
            }
            return strPlateListString;
        }
    }
    catch (std::bad_exception& e)
    {
        LOGFMTE("ResultListManager::GetAllPlateString, bad_exception, error msg = %s", e.what());
        return std::string();
    }
    catch (std::bad_alloc& e)
    {
        LOGFMTE("ResultListManager::GetAllPlateString, bad_alloc, error msg = %s", e.what());
        return std::string();
    }
    catch (std::exception& e)
    {
        LOGFMTE("ResultListManager::GetAllPlateString, exception, error msg = %s.", e.what());
        return std::string();
    }
    catch (void*)
    {
        LOGFMTE("ResultListManager::GetAllPlateString,  void* exception");
        return std::string();
    }
    catch (...)
    {
        LOGFMTE("ResultListManager::DeleteToPosition,  unknown exception");
        return std::string();
    }
}

void ResultListManager::ClearALLResult()
{
    try
    {
        std::unique_lock<std::mutex> locker(m_mtx);
        //MYLocker locker(&m_hcs);
        if (!m_list.empty())
        {
            m_list.clear();
        }
    }
    catch (std::bad_exception& e)
    {
        LOGFMTE("ResultListManager::DeleteToPosition, ClearALL, error msg = %s", e.what());
    }
    catch (std::bad_alloc& e)
    {
        LOGFMTE("ResultListManager::DeleteToPosition, ClearALL, error msg = %s", e.what());
    }
    catch (std::exception& e)
    {
        LOGFMTE("ResultListManager::DeleteToPosition, ClearALL, error msg = %s.", e.what());
    }
    catch (void*)
    {
        LOGFMTE("ResultListManager::ClearALL,  void* exception");
    }
    catch (...)
    {
        LOGFMTE("ResultListManager::ClearALL,  unknown exception");
    }
}

