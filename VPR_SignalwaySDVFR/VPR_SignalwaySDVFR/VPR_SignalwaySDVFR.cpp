// VPR_SignalwaySDVFR.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "VPR_SignalwaySDVFR.h"
#include "utilityTool/ToolFunction.h"
#include "CameraModule/DeviceListManager.h"
#include "CameraModule/Camera6467_VFR.h"
#include <string>
#include <list>
#include<stdlib.h>
#include <algorithm>

#ifdef WINDOWS
#define WRITE_LOG(fmt, ...) Tool_WriteFormatLog("%s:: "fmt, __FUNCTION__, ##__VA_ARGS__);
#else
//#define WRITE_LOG(...) Tool_WriteFormatLog("%s:: ", __FUNCTION__, ##__VA_ARGS__);
#define WRITE_LOG(fmt,...) Tool_WriteFormatLog("%s:: " fmt, __FUNCTION__,##__VA_ARGS__);
#endif



#define ERROR_OK (0)
#define	ERROR_NORESPONSE (-100)
#define	ERROR_PARAMETERS_INVALID (-1000)
#define	ERROR_OCCUPIED	(-1001)
#define	ERROR_OPENFAILED (-1002)
#define	ERROR_OTHER (-2000)

std::list<int> g_lsLoginIDList;

int getRandID()
{
	int iNum = 0;
	auto it = std::end(g_lsLoginIDList);
	do 
	{
		iNum = ( rand() + 1 ) % 100; 
		it = std::find(std::begin(g_lsLoginIDList), std::end(g_lsLoginIDList), iNum);
	} while (std::end(g_lsLoginIDList) != it);
	return iNum;
}


VPR_SIGNALWAYSDVFR_API int D_CALLTYPE VLPR_Init()
{
	WRITE_LOG("begin ");
	srand((unsigned)time(NULL));//选取种子文件
	WRITE_LOG("finish ");
	return ERROR_OK;
}

VPR_SIGNALWAYSDVFR_API int D_CALLTYPE VLPR_Deinit()
{
	WRITE_LOG("begin ");
	DeviceListManager::GetInstance()->ClearAllDevice();
	WRITE_LOG("finish ");
	return ERROR_OK;
}

VPR_SIGNALWAYSDVFR_API int D_CALLTYPE VLPR_Login(int nType, char* sParas)
{
	WRITE_LOG("begin.nType = %d, sParas = %s", nType, sParas);
	if (nType == 0
		|| NULL == sParas
		|| NULL == strstr(sParas, "."))
	{
		WRITE_LOG("parameters is invalid.");
		return ERROR_PARAMETERS_INVALID;
	}
	std::string strIP(sParas);
	strIP = strIP.substr(0, strIP.find(","));
	WRITE_LOG("analysis sParas , the final ip = %s", strIP.c_str());

	int iLoginID = -1;

	BaseCamera* pTemp = NULL;
	pTemp = DeviceListManager::GetInstance()->GetDeviceByIpAddress(strIP.c_str());
	if (pTemp)
	{
		iLoginID = DeviceListManager::GetInstance()->GetDeviceIdByIpAddress(strIP.c_str());
		WRITE_LOG("Camera %s is already exist,login id = %d,  do nothing", strIP.c_str(), iLoginID);
		return iLoginID;
	}
	else
	{
		WRITE_LOG("Camera %s is not exist, create it", strIP.c_str());
		pTemp = new Camera6467_VFR();
		if (pTemp)
		{
			iLoginID = getRandID();
			g_lsLoginIDList.push_back(iLoginID);
			pTemp->SetCameraIP(strIP.c_str());
			pTemp->SetLoginID(iLoginID);
			int iConnect = pTemp->ConnectToCamera();
			pTemp->SetH264Callback(0, 0, 0, 0xffff0700);
			DeviceListManager::GetInstance()->AddOneDevice(iLoginID, pTemp);			

			WRITE_LOG("Camera %s  create finish, login id = %d, connect code = %d",
				strIP.c_str(),
				iLoginID,
				iConnect);
			return iLoginID;
		}
		else
		{
			WRITE_LOG("Camera %s  create failed ", strIP.c_str());
			return ERROR_OTHER;
		}		
	}
}

VPR_SIGNALWAYSDVFR_API int D_CALLTYPE VLPR_Logout(int nHandle)
{
	WRITE_LOG("begin, handle = %d ", nHandle);
	if (NULL != DeviceListManager::GetInstance()->GetDeviceById(nHandle))
	{
		WRITE_LOG("find the camera , and remove it. ");
		DeviceListManager::GetInstance()->EraseDevice(nHandle);
		g_lsLoginIDList.remove(nHandle);
	}
	WRITE_LOG("finish.");
	return 0;
}

VPR_SIGNALWAYSDVFR_API int D_CALLTYPE  VLPR_SetResultCallBack(
	int nHandle,
	CBFun_GetFrontResult pFunc1,
	CBFun_GetBackResult pFunc2,
	void *pUser)
{
	WRITE_LOG("begin, handle = %d, CBFun_GetFrontResult = %p, CBFun_GetBackResult = %p,  pUser = %p ", nHandle, pFunc1, pFunc2, pUser);
	BaseCamera* pCamera = DeviceListManager::GetInstance()->GetDeviceById(nHandle);
	if (NULL != pCamera)
	{
		WRITE_LOG("find the camera %d, ip = %s. ", nHandle, pCamera->GetCameraIP());
		((Camera6467_VFR*)pCamera)->SetFrontResultCallback(pFunc1, pUser);
		((Camera6467_VFR*)pCamera)->SetTailResultCallback(pFunc2, pUser);
		WRITE_LOG("set result call back finish.");
	}
	else
	{
		WRITE_LOG("can not find the camera, do nothing.");
		return ERROR_PARAMETERS_INVALID;
	}

	return ERROR_OK;
}

VPR_SIGNALWAYSDVFR_API int D_CALLTYPE VLPR_SetStatusCallBack(int nHandle,
	int nTimeInvl,
	CBFun_GetDevStatus pFunc,
	void* pUser)
{
	WRITE_LOG("begin, handle = %d, CBFun_GetDevStatus = %p, pUser= %p, nTimeInvl = %d",
		nHandle,
		pFunc,
		pUser,
		nTimeInvl);
	BaseCamera* pCamera = DeviceListManager::GetInstance()->GetDeviceById(nHandle);
	if (NULL != pCamera)
	{
		WRITE_LOG("find the camera %d, ip = %s. ", nHandle, pCamera->GetCameraIP());
		((Camera6467_VFR*)pCamera)->SetConnectStatus_Callback(pFunc, pUser, nTimeInvl);
		WRITE_LOG("set SetStatusCallBack  finish.");
		return ERROR_OK;
	}
	else
	{
		WRITE_LOG("can not find the camera, do nothing.");
		return ERROR_PARAMETERS_INVALID;
	}
}

VPR_SIGNALWAYSDVFR_API int D_CALLTYPE VLPR_GetStatus(int nHandle, int* pStatusCode)
{
	WRITE_LOG("begin, handle = %d, pStatusCode = %p", nHandle, pStatusCode);
	BaseCamera* pCamera = DeviceListManager::GetInstance()->GetDeviceById(nHandle);
	if (NULL != pCamera)
	{
		WRITE_LOG("find the camera %d, ip = %s. ", nHandle, pCamera->GetCameraIP());
		int iStatus = pCamera->GetCamStatus();
		WRITE_LOG("GetCamStatus = %d.", iStatus);
		*pStatusCode = iStatus;
		return ERROR_OK;
	}
	else
	{
		WRITE_LOG("can not find the camera, do nothing.");
		return ERROR_PARAMETERS_INVALID;
	}
}

VPR_SIGNALWAYSDVFR_API int D_CALLTYPE VLPR_GetStatusMsg(int nStatusCode, char* sStatusMsg, int nStatusMsgLen)
{
	WRITE_LOG("begin, nStatusCode = %d, sStatusMsg = %p,nStatusMsgLen = %d ",
		nStatusCode,
		sStatusMsg,
		nStatusMsgLen);
	memset(sStatusMsg, '\0', nStatusMsgLen);

	char chStatus[256] = {0};
	switch (nStatusCode)
	{
	case ERROR_OK:
		sprintf(chStatus, "operation success.");
		break;
	case ERROR_NORESPONSE:
		sprintf(chStatus, "Operation is not responding.");
		break;
	case ERROR_PARAMETERS_INVALID:
		sprintf(chStatus, "Parameter error.");
		break;
	case ERROR_OCCUPIED:
		sprintf(chStatus, "Device is occupied.");
		break;
	case ERROR_OTHER:
	default:
		sprintf(chStatus, "error unknown.");
		break;
	}
	if (strlen(chStatus) <= nStatusMsgLen)
	{
		memcpy(sStatusMsg, chStatus, strlen(chStatus));
	}
	else
	{
		WRITE_LOG("strlen(chStatus) > nStatusMsgLen, do nothing.");
		return ERROR_PARAMETERS_INVALID;
	}
	WRITE_LOG("finish, statues code = %d, string = %s", nStatusCode, chStatus);
	return ERROR_OK;
}

VPR_SIGNALWAYSDVFR_API int D_CALLTYPE VLPR_GetDevInfo(char* sCompany,
	int nCompanyLen,
	char* sDevMode,
	int nModeLen)
{
	WRITE_LOG("begin, sCompany = %p, nCompanyLen = %d,sDevMode = %p, nModeLen= %d ",
		sCompany,
		nCompanyLen,
		sDevMode,
		nModeLen);

	memset(sCompany, '\0', nCompanyLen);
	memset(sDevMode, '\0', nModeLen);

	char chCompany[256] = {"Signalway"};
	char chDevMode[256] = {"VFR"};
	if (strlen(chCompany) <= nCompanyLen)
	{
		memcpy(sCompany, chCompany, strlen(chCompany));
	}

	if (strlen(chDevMode) <= nModeLen)
	{
		memcpy(sDevMode, chDevMode, strlen(chDevMode));
	}
	WRITE_LOG("sCompany = %s,sDevMode = %s ", chCompany, chDevMode);
	return ERROR_OK;
}

VPR_SIGNALWAYSDVFR_API int D_CALLTYPE VLPR_GetVersion(char* sDevVersion,
	int nDevVerLen,
	char* sAPIVersion,
	int nAPIVerLen)
{
	WRITE_LOG("begin, sDevVersion = %p,nDevVerLen = %d, sAPIVersion = %p, nAPIVerLen = %d", 
		sDevVersion,
		nDevVerLen,
		sAPIVersion,
		nAPIVerLen);

	TCHAR szFileName[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szFileName, MAX_PATH);	//取得包括程序名的全路径
	PathRemoveFileSpec(szFileName);				//去掉程序名
	char chPath[MAX_PATH] = { 0 };
	MY_SPRINTF(chPath, sizeof(chPath), "%s\\VPR_SignalwaySDVFR.dll", szFileName);
	std::string strVerion = Tool_GetSoftVersion(chPath);

	char chDevVersion[256] = { "VFR_PCC" };
	if (strlen(chDevVersion) <= nDevVerLen)
	{
		memcpy(sDevVersion, chDevVersion, strlen(chDevVersion));
	}

	if (strVerion.length() <= 0)
	{
		sprintf(sAPIVersion, "%s", DLL_VERSION);
	}
	else
	{
		memcpy(sAPIVersion, strVerion.c_str(), strVerion.length());
	}
	WRITE_LOG("finish, sDevVersion = %s, sAPIVersion = %s", sDevVersion, sAPIVersion);
	return ERROR_OK;
}

VPR_SIGNALWAYSDVFR_API int D_CALLTYPE VLPR_SyncTime(int nHandle, char* sSysTime)
{
	WRITE_LOG("begin, nHandle = %d,sSysTime = %s ", nHandle, sSysTime);
	std::string strTime(sSysTime);
	if (strTime.length() != 14 )
	{
		WRITE_LOG("sSysTime format is invalid");
		return ERROR_PARAMETERS_INVALID;
	}


	BaseCamera* pCamera = DeviceListManager::GetInstance()->GetDeviceById(nHandle);
	if (NULL != pCamera)
	{
		WRITE_LOG("find the camera %d, ip = %s. ", nHandle, pCamera->GetCameraIP());
		int iYear = atoi(strTime.substr(0, 4).c_str());
		int iMonth = atoi(strTime.substr(4, 2).c_str());
		int iDay = atoi(strTime.substr(6, 2).c_str());
		int iHour = atoi(strTime.substr(8, 2).c_str());
		int iMinute = atoi(strTime.substr(10, 2).c_str());
		int iSecond = atoi(strTime.substr(12, 2).c_str());
		WRITE_LOG("analysis time : year = %d, month = %d, day = %d, hours = %d, minutes = %d, second = %d",
			iYear,
			iMonth,
			iDay,
			iHour,
			iMinute,
			iSecond);
		if (pCamera->SynTime(iYear, iMonth, iDay, iHour, iMinute, iSecond, 0))
		{
			WRITE_LOG("syntime success.");
		}
		else
		{
			WRITE_LOG("syntime failed.");
		}
		return ERROR_OK;
	}
	else
	{
		WRITE_LOG("can not find the camera, do nothing.");
		return ERROR_PARAMETERS_INVALID;
	}
}

VPR_SIGNALWAYSDVFR_API int D_CALLTYPE VLPR_StartDisplay(int nHandle, int nWidth, int nHeight, int nFHandle)
{
	WRITE_LOG("DisplayStart, nHandle = %d,nWidth = %d,nHeight = %d,nFHandle = %d", nHandle, nWidth, nHeight, nFHandle);
	if (nHandle == NULL || nFHandle == NULL){
		return ERROR_PARAMETERS_INVALID;
	}
	BaseCamera* pCamera = DeviceListManager::GetInstance()->GetDeviceById(nHandle);
	const HWND h = (const HWND)nFHandle;
	if (pCamera)
	{
		((BaseCamera*)pCamera)->StartPlayVideoByChannel(3, h);
	}
	WRITE_LOG("StartPlayVideo end");
	return ERROR_OK;
}

VPR_SIGNALWAYSDVFR_API int D_CALLTYPE VLPR_StopDisplay(int nHandle)
{
	WRITE_LOG("StopPlayVideo begin.");

	BaseCamera* pCamera = DeviceListManager::GetInstance()->GetDeviceById(nHandle);
	((BaseCamera*)pCamera)->StopPlayVideoByChannel(3);

	WRITE_LOG("StopPlayVideo end.");
	return ERROR_OK;
}
