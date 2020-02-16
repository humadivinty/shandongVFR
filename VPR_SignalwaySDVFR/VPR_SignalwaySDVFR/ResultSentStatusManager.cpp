#include "stdafx.h"
#include "ResultSentStatusManager.h"
#include <algorithm>


ResultSentStatusManager::ResultSentStatusManager()
{
}


ResultSentStatusManager::~ResultSentStatusManager()
{
    ClearAll();
}

void ResultSentStatusManager::UpdateStatus(int DeviceID, unsigned long carID, int itemID, int iStatus)
{
    std::unique_lock<std::mutex> locker(m_mtx);

    std::list<ResultSentStatus>::iterator iter = std::find_if(std::begin(m_lsStatusList),
        std::end(m_lsStatusList),
        [DeviceID, carID](ResultSentStatus sta)
    {
        if (sta.DeviceID == DeviceID
            && sta.dwCarID == carID)
        {
            return true;
        }
        return false;
    });


    if (iter != std::end(m_lsStatusList))
    {
        switch (itemID)
        {
        case item_frontImg:
            iter->iFrontImgSendStatus = iStatus;
            break;
        case item_sideImg:
            iter->iSidImgSendStatus = iStatus;
            break;
        case item_tailImg:
            iter->iTailImgSendStatus = iStatus;
            break;
        case item_video:
            iter->iVideoSendStatus = iStatus;
            break;
        case item_smallImg:
            iter->iSmallImgSendStatus = iStatus;
            break;
        case item_binImg:
            iter->iBinaryImgSendStatus = iStatus;
            break;
        default:
            break;
        }
    }
    else
    {
        ResultSentStatus sta;
        sta.uTimeReceive = GetTickCount();
        sta.DeviceID = DeviceID;
        sta.dwCarID = carID;
        switch (itemID)
        {
        case item_frontImg:
            sta.iFrontImgSendStatus = iStatus;
            break;
        case item_sideImg:
            sta.iSidImgSendStatus = iStatus;
            break;
        case item_tailImg:
            sta.iTailImgSendStatus = iStatus;
            break;
        case item_video:
            sta.iVideoSendStatus = iStatus;
            break;
        case item_smallImg:
            sta.iSmallImgSendStatus = iStatus;
            break;
        case item_binImg:
            sta.iBinaryImgSendStatus = iStatus;
            break;
        default:
            break;
        }
        m_lsStatusList.push_back(sta);
    }

}

bool ResultSentStatusManager::GetRsultStatus(int DeviceID, unsigned long carID, int itemID, long& iStatus)
{
    std::unique_lock<std::mutex> locker(m_mtx);

    std::list<ResultSentStatus>::iterator iter = std::find_if(std::begin(m_lsStatusList),
        std::end(m_lsStatusList),
        [DeviceID, carID](ResultSentStatus sta)
    {
        if (sta.DeviceID == DeviceID
            && sta.dwCarID == carID)
        {
            return true;
        }
        return false;
    });

    bool bRet = true;
    if (iter != std::end(m_lsStatusList))
    {
        switch (itemID)
        {
        case item_frontImg: 
            iStatus = iter->iFrontImgSendStatus;
            break;
        case item_sideImg: 
            iStatus = iter->iSidImgSendStatus;
            break;
        case item_tailImg:
            iStatus = iter->iTailImgSendStatus;
            break;
        case item_video:
            iStatus = iter->iVideoSendStatus;
            break;
        case item_smallImg:
            iStatus = iter->iSmallImgSendStatus;
            break;
        case item_binImg:
            iStatus = iter->iBinaryImgSendStatus;
            break;
        case item_receiveTime:
            iStatus = iter->uTimeReceive;
            break;
        default:
            bRet = false;
            break;
        }
    }
    else
    {
        bRet = false;
    }
    return bRet;
}

void ResultSentStatusManager::RemoveElement(int DeviceID, unsigned long carID)
{
    std::unique_lock<std::mutex> locker(m_mtx);

    std::list<ResultSentStatus>::iterator iter = std::find_if(std::begin(m_lsStatusList),
        std::end(m_lsStatusList),
        [DeviceID, carID](ResultSentStatus sta)
    {
        if (sta.DeviceID == DeviceID
            && sta.dwCarID == carID)
        {
            return true;
        }
        return false;
    });

    if (iter != std::end(m_lsStatusList))
    {
        m_lsStatusList.erase(iter);
    }
}

bool ResultSentStatusManager::getFirstElement(ResultSentStatus& element)
{
    std::unique_lock<std::mutex> locker(m_mtx);
    if (m_lsStatusList.size() > 0)
    {
        element = m_lsStatusList.front();
        return true;
    }
    return false;
}

bool ResultSentStatusManager::RemoveFirstElement()
{
    std::unique_lock<std::mutex> locker(m_mtx);
    if (m_lsStatusList.size() > 0)
    {
        m_lsStatusList.pop_front();
        return true;
    }
    return false;
}

void ResultSentStatusManager::ClearAll()
{
    std::unique_lock<std::mutex> locker(m_mtx);
    m_lsStatusList.clear();
}

size_t ResultSentStatusManager::size()
{
    std::unique_lock<std::mutex> locker(m_mtx);
    return m_lsStatusList.size();
}
