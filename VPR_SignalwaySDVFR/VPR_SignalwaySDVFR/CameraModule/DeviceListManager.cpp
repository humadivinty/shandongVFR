#include "stdafx.h"
#include "DeviceListManager.h"
//#include <condition_variable>    // std::condition_variable
#include "ThreadSafeList.h"


DeviceListManager* DeviceListManager::GetInstance()
{
    static DeviceListManager g_instant;
    return &g_instant;
}

DeviceListManager::DeviceListManager()
{
	InitializeCriticalSection( &m_csLock );
    printf("create DeviceListManager.\n");
}
DeviceListManager::~DeviceListManager()
{	
    printf("destroy DeviceListManager.\n");
    ClearAllDevice();
	DeleteCriticalSection( &m_csLock );
}

size_t DeviceListManager::GetDeviceCount()
{
    MySafeLocker locker(&m_csLock);
    return m_CameraMap.size();
}

void DeviceListManager::ClearAllDevice()
{    
    //m_CameraMap.clear();
	MySafeLocker locker(&m_csLock);
	for (std::map<int , OBJ_TYPE* >::iterator tempIt = m_CameraMap.begin(); tempIt != m_CameraMap.end(); tempIt++)
	{
		OBJ_TYPE* pTemp = tempIt->second;
		if (pTemp != NULL)
		{
			delete pTemp;
			pTemp = NULL;
		}
	}
	m_CameraMap.clear();
}

void DeviceListManager::AddOneDevice(int iCamId, OBJ_TYPE* pCamera)
{
    if (!FindIfExsit(iCamId))
    {
        MySafeLocker locker(&m_csLock);
		m_CameraMap.insert(std::pair<int, OBJ_TYPE*>( iCamId , pCamera));
    }
}

bool DeviceListManager::FindIfExsit(int camID)
{
    MySafeLocker locker(&m_csLock);
	if (m_CameraMap.end() !=  m_CameraMap.find(camID))
	{
		return true;
	}
    return false;
}

bool DeviceListManager::EraseDevice(int camID)
{
    if (FindIfExsit(camID))
    {
        MySafeLocker locker(&m_csLock);
		std::map<int , OBJ_TYPE* >::iterator pIt = m_CameraMap.find(camID);
		OBJ_TYPE* pTemp = pIt->second;
		if (pTemp != NULL)
		{
			delete pTemp;
			pTemp = NULL;
		}
		m_CameraMap.erase(pIt);
		return true;
    }
    return false;
}

OBJ_TYPE* DeviceListManager::GetDeviceById( int iCamId )
{
	OBJ_TYPE* pTemp = NULL;
	if (FindIfExsit(iCamId))
	{
		MySafeLocker locker(&m_csLock);
		std::map<int , OBJ_TYPE* >::iterator pIt = m_CameraMap.find(iCamId);
		if (pIt != m_CameraMap.end())
		{
			pTemp = pIt->second;
		}
	}
	return pTemp;
}

OBJ_TYPE* DeviceListManager::GetDeviceByIpAddress( const char* ipAddress )
{
	OBJ_TYPE* pTemp = NULL;

	MySafeLocker locker(&m_csLock);
	for (std::map<int , OBJ_TYPE* >::iterator tempIt = m_CameraMap.begin(); tempIt != m_CameraMap.end(); tempIt++)
	{
		pTemp = NULL;
		pTemp = tempIt->second;
		if (pTemp != NULL
			&& strcmp(pTemp->GetCameraIP(), ipAddress) == 0)
		{
			break;
		}
		else
		{
			pTemp = NULL;
		}
	}
	return pTemp;
}

int DeviceListManager::GetDeviceIdByIpAddress(const char* ipAddress)
{
    OBJ_TYPE* pTemp = NULL;
    int iDeviceID = -1;

    MySafeLocker locker(&m_csLock);
    for (std::map<int, OBJ_TYPE* >::iterator tempIt = m_CameraMap.begin(); tempIt != m_CameraMap.end(); tempIt++)
    {
        pTemp = NULL;
        pTemp = tempIt->second;
        if (pTemp != NULL
            && strcmp(pTemp->GetCameraIP(), ipAddress) == 0)
        {
            iDeviceID = tempIt->first;
            break;
        }
        else
        {
            pTemp = NULL;
        }
    }
    return iDeviceID;
}
