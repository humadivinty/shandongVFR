#include "stdafx.h"
#include "Camera6467_VFR.h"
#include "CameraResult.h"
#include "HvDevice/HvDeviceBaseType.h"
#include "HvDevice/HvDeviceCommDef.h"
#include "HvDevice/HvDeviceNew.h"
#include "HvDevice/HvCamera.h"
#include "utilityTool/ToolFunction.h"
#include "libCxImage/ximage.h"
//#include "utilityTool/log4z.h"
#include <process.h>
#include <exception>
#include <new>
#include <algorithm>
#include "VPR_SignalwaySDVFR_Commendef.h"

#define VEHICLE_HEAD_NODE_NAME "Image2"
#define VEHICLE_SIDE_NODE_NAME "Image3"
#define VEHICLE_TAIL_NODE_NAME "Image4"

#define VFR_WRITE_LOG(fmt, ...) WriteFormatLog("%s:: "fmt,  __FUNCTION__,##__VA_ARGS__);

#define CHECK_ARG(arg)\
    if (arg == NULL) \
    {\
        WriteFormatLog("%s is NULL", #arg); \
        return 0;\
    }

#ifndef LOGFMTE
#define LOGFMTE printf
#endif

#ifndef LOGFMTD
#define LOGFMTD printf
#endif

Camera6467_VFR::Camera6467_VFR() :
BaseCamera(),
m_dwLastCarID(-1),
m_iTimeInvl(10),
m_iSuperLenth(13),
m_iResultTimeOut(1500),
m_iWaitVfrTimeOut(3),
m_iResultMsg(WM_USER + 1),
g_pUser(NULL),
g_func_ReconnectCallback(NULL),
g_ConnectStatusCallback(NULL),
g_func_DisconnectCallback(NULL),
g_pFuncResultCallback(NULL),
g_pResultUserData(NULL),
g_pFuncFrontResultCallback(NULL),
g_pFrontResultUserData(NULL),
g_pFuncTailResultCallback(NULL),
g_pTailResultUserData(NULL),
m_hMsgHanldle(NULL),
m_pResult(NULL),
m_bStatusCheckThreadExit(false),
m_bJpegComplete(false),
m_bSentResult(false),
m_hStatusCheckThread(NULL),
m_hSendResultThread(NULL),
m_hDeleteLogThread(NULL),
m_hDeleteResultThread(NULL)
{
    InitializeCriticalSection(&m_csResult);
    ReadConfig();
    m_h264Saver.initMode(m_iVideoMode);
    //m_h264Saver_ex.initMode(0);
    m_hStatusCheckThread = (HANDLE)_beginthreadex(NULL, 0, Camera_StatusCheckThread, this, 0, NULL);
    m_hSendResultThread = (HANDLE)_beginthreadex(NULL, 0, s_SendResultThreadFunc, this, 0, NULL);
    //m_hDeleteLogThread = (HANDLE)_beginthreadex(NULL, 0, s_DeleteLogThreadFunc, this, 0, NULL);
    //m_hDeleteResultThread = (HANDLE)_beginthreadex(NULL, 0, s_DeleteResultThreadFunc, this, 0, NULL);
}


Camera6467_VFR::Camera6467_VFR(const char* chIP, HWND hWnd, int Msg):
BaseCamera(chIP, NULL, 0),
m_dwLastCarID(-1),
m_iTimeInvl(10),
m_iSuperLenth(13),
m_iResultTimeOut(1500),
m_iWaitVfrTimeOut(3),
m_iResultMsg(Msg),
g_pUser(NULL),
g_func_ReconnectCallback(NULL),
g_ConnectStatusCallback(NULL),
g_func_DisconnectCallback(NULL),
g_pFuncResultCallback(NULL),
g_pResultUserData(NULL),
g_pFuncFrontResultCallback(NULL),
g_pFrontResultUserData(NULL),
g_pFuncTailResultCallback(NULL),
g_pTailResultUserData(NULL),
m_hMsgHanldle(hWnd),
m_pResult(NULL),
m_bStatusCheckThreadExit(false),
m_bJpegComplete(false),
m_bSentResult(false),
m_hStatusCheckThread(NULL),
m_hSendResultThread(NULL),
m_hDeleteLogThread(NULL),
m_hDeleteResultThread(NULL)
{
    InitializeCriticalSection(&m_csResult);
    ReadConfig();

    m_h264Saver.initMode(m_iVideoMode);
    m_h264Saver_ex.initMode(0);
    m_hStatusCheckThread = (HANDLE)_beginthreadex(NULL, 0, Camera_StatusCheckThread, this, 0, NULL);
    m_hSendResultThread = (HANDLE)_beginthreadex(NULL, 0, s_SendResultThreadFunc, this, 0, NULL);
    //m_hDeleteLogThread = (HANDLE)_beginthreadex(NULL, 0, s_DeleteLogThreadFunc, this, 0, NULL);
    //m_hDeleteResultThread = (HANDLE)_beginthreadex(NULL, 0, s_DeleteResultThreadFunc, this, 0, NULL);
}

Camera6467_VFR::~Camera6467_VFR()
{
    InterruptionConnection();
    DisConnectCamera();

    m_Camera_Plate = nullptr;
    SetCheckThreadExit(true);
    //m_MySemaphore.notify(GetCurrentThreadId());
    SetResultCallback(NULL, NULL);
    Tool_SafeCloseThread(m_hStatusCheckThread);
    Tool_SafeCloseThread(m_hSendResultThread);
    Tool_SafeCloseThread(m_hDeleteLogThread);
    Tool_SafeCloseThread(m_hDeleteResultThread);

    SAFE_DELETE_OBJ(m_pResult);
    DeleteCriticalSection(&m_csResult);
}

void Camera6467_VFR::AnalysisAppendXML(CameraResult* CamResult)
{
    WriteFormatLog("AnalysisAppendXML");
    if (NULL == CamResult || strlen(CamResult->pcAppendInfo) <= 0)
        return;

    char chTemp[BUFFERLENTH] = { 0 };
    int iLenth = BUFFERLENTH;

	if (Tool_GetDataAttributFromAppenedInfo(CamResult->pcAppendInfo, VEHICLE_HEAD_NODE_NAME, "TimeHigh", chTemp, &iLenth))
    {
        DWORD64 iTime = 0;
        DWORD64 iTimeHight = 0;
        //sscanf(chTemp, "%d", &iAxleCount);
        sscanf_s(chTemp, "%I64u", &iTimeHight);
        iTime = iTimeHight << 32;

        memset(chTemp, 0, sizeof(chTemp));
        iLenth = BUFFERLENTH;

		if (Tool_GetDataAttributFromAppenedInfo(CamResult->pcAppendInfo, VEHICLE_HEAD_NODE_NAME, "TimeLow", chTemp, &iLenth))
        {
            //DWORD64 iTimeLow = 0;
            //sscanf(chTemp, "%d", &iAxleCount);
            //sscanf_s(chTemp, "%I64u", &iTimeLow);
            unsigned long iTimeLow = 0;
            sscanf_s(chTemp, "%lu", &iTimeLow);
            iTime += iTimeLow ;
        }
        WriteFormatLog("GET carArrive time iTimeLow %I64u", iTime);
        CamResult->dw64TimeMS = iTime;
    }

    if (0 != CamResult->dw64TimeMS)
    {
        //int iTimeNow = CamResult->dw64TimeMS / 1000;
        //struct tm tmNow = *localtime((time_t *)&iTimeNow);
        //sprintf_s(CamResult->chPlateTime, sizeof(CamResult->chPlateTime), "%04d-%02d-%02d %02d:%02d:%02d",
        //    tmNow.tm_year,
        //    tmNow.tm_mon,
        //    tmNow.tm_mday,
        //    tmNow.tm_hour,
        //    tmNow.tm_min,
        //    tmNow.tm_sec);
        CTime tm(CamResult->dw64TimeMS / 1000);
        sprintf_s(CamResult->chPlateTime, sizeof(CamResult->chPlateTime), "%04d%02d%02d%02d%02d%02d%03I64d",
            tm.GetYear(),
            tm.GetMonth(),
            tm.GetDay(),
            tm.GetHour(),
            tm.GetMinute(),
            tm.GetSecond(),
            CamResult->dw64TimeMS % 1000);
        //sprintf_s(CamResult->chPlateTime, sizeof(CamResult->chPlateTime), "%04d-%02d-%02d %02d:%02d:%02d",
        //    tm.GetYear(),
        //    tm.GetMonth(),
        //    tm.GetDay(),
        //    tm.GetHour(),
        //    tm.GetMinute(),
        //    tm.GetSecond());
    }
    else
    {
        SYSTEMTIME st;
        GetLocalTime(&st);
        sprintf_s(CamResult->chPlateTime, sizeof(CamResult->chPlateTime), "%04d%02d%02d%02d%02d%02d%03d",
            st.wYear,
            st.wMonth,
            st.wDay,
            st.wHour,
            st.wMinute,
            st.wSecond,
            st.wMilliseconds);
        //sprintf_s(CamResult->chPlateTime, sizeof(CamResult->chPlateTime), "%04d-%02d-%02d %02d:%02d:%02d:%03d",
        //    st.wYear,
        //    st.wMonth,
        //    st.wDay,
        //    st.wHour,
        //    st.wMinute,
        //    st.wSecond);
    }

    if (Tool_GetDataFromAppenedInfo(CamResult->pcAppendInfo, "VehicleType", chTemp, &iLenth))
    {
        //sprintf(CamResult->chVehTypeText,"%s", chTemp);
        sprintf_s(CamResult->chVehTypeText, sizeof(CamResult->chVehTypeText), "%s", chTemp);
        CamResult->iVehTypeNo = AnalysisVelchType(chTemp);
    }
    memset(chTemp, 0, sizeof(chTemp));
    iLenth = BUFFERLENTH;
    if (Tool_GetDataFromAppenedInfo(CamResult->pcAppendInfo, "AxleCnt", chTemp, &iLenth))
    {
        int iAxleCount = 0;
        //sscanf(chTemp, "%d", &iAxleCount);
        sscanf_s(chTemp, "%d", &iAxleCount);
        CamResult->iAxletreeCount = iAxleCount;
        //printf("the Axletree count is %d.\n", iAxleCount);
    }
    memset(chTemp, 0, sizeof(chTemp));
    iLenth = BUFFERLENTH;
    CamResult->iAxletreeType = 12;
    if (Tool_GetDataFromAppenedInfo(CamResult->pcAppendInfo, "AxleType", chTemp, &iLenth))
    {
        sprintf_s(CamResult->chAxleType, sizeof(CamResult->chAxleType), "%s", chTemp);
        CamResult->iAxletreeGroupCount = Tool_FindSubStrCount(chTemp, "+");
        CamResult->iAxletreeType = AnalysisVelchAxleType(chTemp, CamResult->iAxletreeGroupCount);
    }
    memset(chTemp, 0, sizeof(chTemp));
    iLenth = BUFFERLENTH;
    if (Tool_GetDataFromAppenedInfo(CamResult->pcAppendInfo, "Wheelbase", chTemp, &iLenth))
    {
        float fWheelbase = 0;
        //sscanf(chTemp, "%f", &fWheelbase);
        sscanf_s(chTemp, "%f", &fWheelbase);
        CamResult->fDistanceBetweenAxles = fWheelbase;
        //printf("the Wheelbase  is %f.\n", fWheelbase);
    }
    memset(chTemp, 0, sizeof(chTemp));
    iLenth = BUFFERLENTH;
    if (Tool_GetDataFromAppenedInfo(CamResult->pcAppendInfo, "CarLength", chTemp, &iLenth))
    {
        float fCarLength = 0;
        //sscanf(chTemp, "%f", &fCarLength);
        sscanf_s(chTemp, "%f", &fCarLength);
        CamResult->fVehLenth = fCarLength;
        //printf("the CarLength  is %f.\n", fCarLength);
    }
    memset(chTemp, 0, sizeof(chTemp));
    iLenth = BUFFERLENTH;
    if (Tool_GetDataFromAppenedInfo(CamResult->pcAppendInfo, "CarHeight", chTemp, &iLenth))
    {
        float fCarHeight = 0;
        //sscanf(chTemp, "%f", &fCarHeight);
        sscanf_s(chTemp, "%f", &fCarHeight);
        CamResult->fVehHeight = fCarHeight;
        //printf("the CarHeight  is %f.\n", fCarHeight);
    }
    memset(chTemp, 0, sizeof(chTemp));
    iLenth = BUFFERLENTH;
    if (Tool_GetDataFromAppenedInfo(CamResult->pcAppendInfo, "CarWidth", chTemp, &iLenth))
    {
        float fVehWidth = 0;
        //sscanf(chTemp, "%f", &fVehWidth);
        sscanf_s(chTemp, "%f", &fVehWidth);
        CamResult->fVehWidth = fVehWidth;
        //printf("the CarLength  is %f.\n", fCarLength);
    }
    memset(chTemp, 0, sizeof(chTemp));
    iLenth = BUFFERLENTH;
    if (Tool_GetDataFromAppenedInfo(CamResult->pcAppendInfo, "BackUp", chTemp, &iLenth))
    {
        CamResult->bBackUpVeh = true;
    }
	memset(chTemp, 0, sizeof(chTemp));
	iLenth = BUFFERLENTH;
    if (Tool_GetDataFromAppenedInfo(CamResult->pcAppendInfo, "Confidence", chTemp, &iLenth))
    {
        float fConfidence = 0;
        //sscanf(chTemp, "%f", &fConfidence);
        sscanf_s(chTemp, "%f", &fConfidence);
        CamResult->fConfidenceLevel = fConfidence;
        //printf("the CarHeight  is %f.\n", fCarHeight);
    }
	memset(chTemp, 0, sizeof(chTemp));
	iLenth = BUFFERLENTH;
	if (Tool_GetDataFromAppenedInfo(CamResult->pcAppendInfo, "TailPlateName", chTemp, &iLenth))
	{
		memset(CamResult->chTailPlateNO, '\0', sizeof(CamResult->chTailPlateNO));
		memcpy(CamResult->chTailPlateNO, chTemp, strlen(chTemp));
		WriteFormatLog("tail plate no = %s", chTemp);
	}
	
	CamResult->iPlateColor = Tool_AnalysisPlateColorNo(CamResult->chPlateNO);
	CamResult->iTailPlateColor = Tool_AnalysisPlateColorNo(CamResult->chTailPlateNO);

    //memset(chTemp, 0, sizeof(chTemp));
    //iLenth = BUFFERLENTH;
    //if (Tool_GetDataFromAppenedInfo(CamResult->pcAppendInfo, "VideoScaleSpeed", chTemp, &iLenth))
    //{
    //    int iSpeed = 0;
    //    sscanf_s(chTemp, "%d", &iSpeed);
    //    CamResult->iSpeed = iSpeed;
    //}

    //TiXmlElement element = Tool_SelectElementByName(CamResult->pcAppendInfo, "PlateName", 2);
    //if (strlen(element.GetText()) > 0)
    //{
    //    memset(CamResult->chPlateNO, 0, sizeof(CamResult->chPlateNO));
    //    strcpy_s(CamResult->chPlateNO, sizeof(CamResult->chPlateNO), element.GetText());

    //    memset(chTemp, 0, sizeof(chTemp));
    //    strcpy_s(chTemp, sizeof(chTemp), element.GetText());

    //    iLenth = strlen(chTemp);
    //    printf("find the plate number = %s, plate length = %d\n", chTemp, iLenth);
    //    if (strlen(chTemp) > 0)
    //    {
    //        CamResult->iPlateColor = Tool_AnalysisPlateColorNo(chTemp);
    //    }
    //    else
    //    {
    //        CamResult->iPlateColor = COLOR_UNKNOW;
    //    }

    //}
    //else
    //{
    //    sprintf_s(CamResult->chPlateNO, sizeof(CamResult->chPlateNO), "无车牌");
    //    CamResult->iPlateColor = COLOR_UNKNOW;
    //}
}

int Camera6467_VFR::AnalysisVelchType(const char* vehType)
{
    if (vehType == NULL)
    {
        return UNKOWN_TYPE;
    }
    if (strstr(vehType, "客1"))
    {
        return BUS_TYPE_1;
    }
    else if (strstr(vehType, "客2"))
    {
        return BUS_TYPE_2;
        //printf("the Vehicle type code is 2.\n");
    }
    else if (strstr(vehType, "客3"))
    {
        return BUS_TYPE_3;
    }
    else if (strstr(vehType, "客4"))
    {
        return BUS_TYPE_4;
    }
    else if (strstr(vehType, "客5"))
    {
        return BUS_TYPE_5;
    }
    else if (strstr(vehType, "货1"))
    {
        return TRUCK_TYPE_1;
    }
    else if (strstr(vehType, "货2"))
    {
        return TRUCK_TYPE_2;
    }
    else if (strstr(vehType, "货3"))
    {
        return TRUCK_TYPE_3;
    }
    else if (strstr(vehType, "货4"))
    {
        return TRUCK_TYPE_4;
    }
    else if (strstr(vehType, "货5"))
    {
        return TRUCK_TYPE_5;
    }
    else if (strstr(vehType, "货6"))
    {
        return TRUCK_TYPE_6;
    }
    else
    {
        return UNKOWN_TYPE;
    }
}

int Camera6467_VFR::AnalysisVelchAxleType(const char* AxleType, int iAxleGroupCount)
{
    if (NULL == AxleType
        || strlen(AxleType) <= 0
        || iAxleGroupCount <= 0
        || NULL == strstr(AxleType, "+"))
    {
        WriteFormatLog("AnalysisVelchAxleType, NULL == AxleType , return 12.");
        return 12;
    }
    WriteFormatLog("AnalysisVelchAxleType, AxleType = %s, iAxleGroupCount = %d", AxleType, iAxleGroupCount);

    char chTempAxle[256] = { 0 };
    //strcpy_s(chTempAxle, sizeof(chTempAxle), AxleType);
    strcpy(chTempAxle, AxleType);

    int iAxletreeType = 12;

    int iFirstValue = 0, iSeconed = 0, iThird = 0, iFourth = 0, iFifth = 0, iSix = 0, iSeventh = 0;
    int iAxletreeType1 = 0, iAxletreeType2 = 0, iAxletreeType3 = 0, iAxletreeType4 = 0, iAxletreeType5 = 0, iAxletreeType6 = 0, iAxletreeType7 = 0;
    char chAxletreeType[32] = { 0 };
    int iAxleCount = iAxleGroupCount +1;
    switch (iAxleCount)
    {
    case 1:
        iAxletreeType = 12;
        break;
    case 2:
        sscanf(chTempAxle, "%d+%d", &iFirstValue, &iSeconed);
        iAxletreeType1 = GetAlexType(iFirstValue);
        iAxletreeType2 = GetAlexType(iSeconed);

        sprintf(chAxletreeType, "%d%d", iAxletreeType1, iAxletreeType2);
        sscanf(chAxletreeType, "%d", &iAxletreeType);
        break;
    case 3:
        sscanf(chTempAxle, "%d+%d+%d", &iFirstValue, &iSeconed, &iThird);
        iAxletreeType1 = GetAlexType(iFirstValue);
        iAxletreeType2 = GetAlexType(iSeconed);
        iAxletreeType3 = GetAlexType(iThird);

        sprintf(chAxletreeType, "%d%d%d", iAxletreeType1, iAxletreeType2, iAxletreeType3);
        sscanf(chAxletreeType, "%d", &iAxletreeType);
        break;
    case 4:
        sscanf(chTempAxle, "%d+%d+%d+%d", &iFirstValue, &iSeconed, &iThird, &iFourth);
        iAxletreeType1 = GetAlexType(iFirstValue);
        iAxletreeType2 = GetAlexType(iSeconed);
        iAxletreeType3 = GetAlexType(iThird);
        iAxletreeType4 = GetAlexType(iFourth);

        sprintf(chAxletreeType, "%d%d%d%d", iAxletreeType1, iAxletreeType2, iAxletreeType3, iAxletreeType4);
        sscanf(chAxletreeType, "%d", &iAxletreeType);
        break;
    case 5:
        sscanf(chTempAxle, "%d+%d+%d+%d+%d", &iFirstValue, &iSeconed, &iThird, &iFourth, &iFifth);
        iAxletreeType1 = GetAlexType(iFirstValue);
        iAxletreeType2 = GetAlexType(iSeconed);
        iAxletreeType3 = GetAlexType(iThird);
        iAxletreeType4 = GetAlexType(iFourth);
        iAxletreeType5 = GetAlexType(iFifth);

        sprintf(chAxletreeType, "%d%d%d%d%d", iAxletreeType1, iAxletreeType2, iAxletreeType3, iAxletreeType4, iAxletreeType5);
        sscanf(chAxletreeType, "%d", &iAxletreeType);
        break;
    case 6:
        sscanf(chTempAxle, "%d+%d+%d+%d+%d+%d", &iFirstValue, &iSeconed, &iThird, &iFourth, &iFifth, &iSix);
        iAxletreeType1 = GetAlexType(iFirstValue);
        iAxletreeType2 = GetAlexType(iSeconed);
        iAxletreeType3 = GetAlexType(iThird);
        iAxletreeType4 = GetAlexType(iFourth);
        iAxletreeType5 = GetAlexType(iFifth);
        iAxletreeType6 = GetAlexType(iSix);


        sprintf(chAxletreeType, "%d%d%d%d%d%d",
            iAxletreeType1,
            iAxletreeType2,
            iAxletreeType3,
            iAxletreeType4,
            iAxletreeType5,
            iAxletreeType6);
        sscanf(chAxletreeType, "%d", &iAxletreeType);
        break;
    case 7:
        sscanf(chTempAxle, "%d+%d+%d+%d+%d+%d+%d", &iFirstValue, &iSeconed, &iThird, &iFourth, &iFifth, &iSix, &iSeventh);
        iAxletreeType1 = GetAlexType(iFirstValue);
        iAxletreeType2 = GetAlexType(iSeconed);
        iAxletreeType3 = GetAlexType(iThird);
        iAxletreeType4 = GetAlexType(iFourth);
        iAxletreeType5 = GetAlexType(iFifth);
        iAxletreeType6 = GetAlexType(iSix);
        iAxletreeType7 = GetAlexType(iSeventh);

        sprintf(chAxletreeType, "%d%d%d%d%d%d%d",
            iAxletreeType1,
            iAxletreeType2,
            iAxletreeType3,
            iAxletreeType4,
            iAxletreeType5,
            iAxletreeType6,
            iAxletreeType7);
        sscanf(chAxletreeType, "%d", &iAxletreeType);
        break;
    default:
        WriteLog("use default AxletreeType 2");
        iAxletreeType = 12;
        break;
    }

    WriteFormatLog("AnalysisVelchAxleType,finish,  AxleType = %d", iAxletreeType);
    return iAxletreeType;
}

int Camera6467_VFR::GetAlexType(int ivalue)
{
    int iType = 0;
    switch (ivalue)
    {
    case 1:
        iType = 1;
        break;
    case 2:
        iType = 2;
        break;
    case 11:
        iType = 11;
        break;
    case 12:
        iType = 12;
        break;
    case 22:
        iType = 5;
        break;
    case 111:
        iType = 111;
        break;
    case 112:
        iType = 112;
        break;
    case 122:
        iType = 15;
        break;
    case 222:
        iType = 7;
        break;
    case 1112:
        iType = 114;
        break;
    case 1122:
        iType = 115;
        break;
    default:
        iType = 1;
        break;
    }
    return iType;
}

bool Camera6467_VFR::CheckIfSuperLength(CameraResult* CamResult)
{
    CHECK_ARG(CamResult);
    if (CamResult->fVehLenth > m_iSuperLenth)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Camera6467_VFR::CheckIfBackUpVehicle(CameraResult* CamResult)
{
    CHECK_ARG(CamResult);
    if (CamResult->bBackUpVeh)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Camera6467_VFR::ReadConfig()
{
    char chTemp[MAX_PATH] = { 0 };
    int iTempValue = 0;

    iTempValue = 15;
    Tool_ReadIntValueFromConfigFile(INI_FILE_NAME, "Filter", "SuperLongVehicleLenth", iTempValue);
    m_iSuperLenth = iTempValue > 0 ? iTempValue : 15;

    //iTempValue = 1500;
    //Tool_ReadIntValueFromConfigFile(INI_FILE_NAME, "Filter", "ResultTimeOut", iTempValue);
    //m_iResultTimeOut = iTempValue > 0 ? iTempValue : 1500;    

    iTempValue = 1000;
    Tool_ReadIntValueFromConfigFile(INI_FILE_NAME, "Result", "WaitTimeOut", iTempValue);
    iTempValue = iTempValue > 0 ? iTempValue : 2000;
    SetResultWaitTime(iTempValue);

    //iTempValue = 0;
    //Tool_ReadIntValueFromConfigFile(INI_FILE_NAME, "Result", "HoldDays", iTempValue);
    //iTempValue = iTempValue > 0 ? iTempValue : 0;
    //SetReulstHoldDay(iTempValue);

    //值为1时，表示使用最早接收到的结果，
    //iTempValue = 1;
    //Tool_ReadIntValueFromConfigFile(INI_FILE_NAME, "Result", "GetMode", iTempValue);
    //m_iResultModule = iTempValue > 0 ? iTempValue : 0;

    iTempValue = 1;
    Tool_ReadIntValueFromConfigFile(INI_FILE_NAME, "Video", "mode", iTempValue);
    m_iVideoMode = iTempValue;

	iTempValue = 1;
	Tool_ReadIntValueFromConfigFile(INI_FILE_NAME, "Enable", "addplate", iTempValue);
	m_addplate = iTempValue;
	//1为开，其他为关
	iTempValue = 1;
	Tool_ReadIntValueFromConfigFile(INI_FILE_NAME, "Enable", "addtime", iTempValue);
	m_addtime = iTempValue;


	iTempValue = 255;
	//获取ini配置文件中的数值
	Tool_ReadIntValueFromConfigFile(INI_FILE_NAME, "FontAdd", "fontsize", iTempValue);
	//判断fontsize是否超过阈值，超过给默认值60
	if (iTempValue<24 || iTempValue>128)
	{
		m_iFontSize = 60;
	}
	else
	{
		m_iFontSize = iTempValue;
	}

	Tool_ReadIntValueFromConfigFile(INI_FILE_NAME, "FontAdd", "fontcolorR", iTempValue);
	//判断RGB是否超过阈值，超过给默认值255
	if (iTempValue<0 || iTempValue>255)
	{
		m_iFontAddcolorR = 255;
	}
	else
	{
		m_iFontAddcolorR = iTempValue;
	}


	Tool_ReadIntValueFromConfigFile(INI_FILE_NAME, "FontAdd", "fontcolorG", iTempValue);
	if (iTempValue<0 || iTempValue>255)
	{
		m_iFontAddcolorG = 255;
	}
	else
	{
		m_iFontAddcolorG = iTempValue;
	}


	Tool_ReadIntValueFromConfigFile(INI_FILE_NAME, "FontAdd", "fontcolorB", iTempValue);
	if (iTempValue<0 || iTempValue>255)
	{
		m_iFontAddcolorB = 255;
	}
	else
	{
		m_iFontAddcolorB = iTempValue;
	}

    BaseCamera::ReadConfig();
}

int Camera6467_VFR::GetTimeInterval()
{
    int iTimeInterval = 1;
    EnterCriticalSection(&m_csFuncCallback);
    iTimeInterval = m_iTimeInvl;
    LeaveCriticalSection(&m_csFuncCallback);
    return iTimeInterval;
}

void Camera6467_VFR::SetDisConnectCallback(void* funcDisc, void* pUser)
{
    EnterCriticalSection(&m_csFuncCallback);
    g_func_DisconnectCallback = funcDisc;
    g_pUser = pUser;
    LeaveCriticalSection(&m_csFuncCallback);
}

void Camera6467_VFR::SetReConnectCallback(void* funcReco, void* pUser)
{
    EnterCriticalSection(&m_csFuncCallback);
    g_func_ReconnectCallback = funcReco;
    g_pUser = pUser;
    LeaveCriticalSection(&m_csFuncCallback);
}

void Camera6467_VFR::SetConnectStatus_Callback(void* func, void* pUser, int TimeInterval)
{
    EnterCriticalSection(&m_csFuncCallback);
	g_ConnectStatusCallback = func;
    g_pUser = pUser;
    m_iTimeInvl = TimeInterval;
    LeaveCriticalSection(&m_csFuncCallback);
}

void Camera6467_VFR::SendConnetStateMsg(bool isConnect)
{
	int iLoginID = GetLoginID();
    if (isConnect)
    {
        EnterCriticalSection(&m_csFuncCallback);
        if (g_ConnectStatusCallback)
        {
            LeaveCriticalSection(&m_csFuncCallback);
            //char chIP[32] = { 0 };
            //sprintf_s(chIP, "%s", m_strIP.c_str());
			((CBFun_GetDevStatus)g_ConnectStatusCallback)(iLoginID, 0, g_pUser);
        }
        else
        {
            LeaveCriticalSection(&m_csFuncCallback);
        }

        //EnterCriticalSection(&m_csFuncCallback);
        //if (m_hWnd != NULL)
        //{
        //    ::PostMessage(m_hWnd, m_iConnectMsg, NULL, NULL);
        //}        
        //LeaveCriticalSection(&m_csFuncCallback);
    }
    else
    {
        EnterCriticalSection(&m_csFuncCallback);
        if (g_ConnectStatusCallback)
        {
            LeaveCriticalSection(&m_csFuncCallback);
            //char chIP[32] = { 0 };
            //sprintf_s(chIP, "%s", m_strIP.c_str());
			((CBFun_GetDevStatus)g_ConnectStatusCallback)(iLoginID, -100, g_pUser);
        }
        else
        {
            LeaveCriticalSection(&m_csFuncCallback);
        }

        //EnterCriticalSection(&m_csFuncCallback);
        //if (m_hWnd != NULL)
        //{
        //    ::PostMessage(m_hWnd, m_iDisConMsg, NULL, NULL);
        //}        
        //LeaveCriticalSection(&m_csFuncCallback);
    }
}

void Camera6467_VFR::SetResultCallback(void* func, void* pUser)
{
    EnterCriticalSection(&m_csFuncCallback);
    g_pFuncResultCallback = func;
    g_pResultUserData = pUser;
    LeaveCriticalSection(&m_csFuncCallback);
}

bool Camera6467_VFR::CheckIfSetResultCallback()
{
    bool bRet = false;
    EnterCriticalSection(&m_csFuncCallback);
    if (g_pFuncResultCallback != NULL)
    {
        bRet = true;
    }
    LeaveCriticalSection(&m_csFuncCallback);
    return bRet;
}

void Camera6467_VFR::SetFrontResultCallback(void* func, void* pUser)
{
	EnterCriticalSection(&m_csFuncCallback);
	g_pFuncFrontResultCallback = func;
	g_pFrontResultUserData = pUser;
	LeaveCriticalSection(&m_csFuncCallback);
}

bool Camera6467_VFR::CheckFrontResultCallback()
{
	bool bRet = false;
	EnterCriticalSection(&m_csFuncCallback);
	if (g_pFuncFrontResultCallback != NULL)
	{
		bRet = true;
	}
	LeaveCriticalSection(&m_csFuncCallback);
	return bRet;
}

void Camera6467_VFR::SetTailResultCallback(void* pfunc, void* pUser)
{
	EnterCriticalSection(&m_csFuncCallback);
	g_pFuncTailResultCallback = pfunc;
	g_pTailResultUserData = pUser;
	LeaveCriticalSection(&m_csFuncCallback);
}

bool Camera6467_VFR::CheckTailResultcallback()
{
	bool bRet = false;
	EnterCriticalSection(&m_csFuncCallback);
	if (g_pFuncTailResultCallback != NULL)
	{
		bRet = true;
	}
	LeaveCriticalSection(&m_csFuncCallback);
	return bRet;
}

void Camera6467_VFR::SendResultByCallback(std::shared_ptr<CameraResult> pResult)
{
	VFR_WRITE_LOG("SendResultByCallback begin.");
    
    if (CheckFrontResultCallback())
    {
		VFR_WRITE_LOG("SendResultByCallback process begin.");

		//获取文件名索引，并更新
		const char* pIndexFileName = "imgIndex.data";
		char chNumBuffer[64] = {0};
		size_t iBufLength = sizeof(chNumBuffer);
		int iIndex = 0;
		if (Tool_LoadFile(pIndexFileName, chNumBuffer, iBufLength))
		{
			iIndex = atoi(chNumBuffer);
		}
		iIndex++;
		memset(chNumBuffer, 0, sizeof(chNumBuffer));
		sprintf_s(chNumBuffer, sizeof(chNumBuffer), "%d", iIndex);
		Tool_SaveFileToPath(pIndexFileName, chNumBuffer, sizeof(chNumBuffer));
		
		T_VLPFRONTINFO vlpFrontInfo;
		memset(&vlpFrontInfo, '\0', sizeof(vlpFrontInfo));
		vlpFrontInfo.vlpInfoSize = sizeof(vlpFrontInfo);

		//车头车牌信息
		vlpFrontInfo.vlpColor[0] = 0;	//颜色
		vlpFrontInfo.vlpColor[1] = pResult->iPlateColor;
		char chPlateNO[64] = { 0 };//车牌号码
		Tool_ProcessPlateNo(pResult->chPlateNO, chPlateNO, sizeof(chPlateNO));
		memcpy(vlpFrontInfo.vlpText, chPlateNO, sizeof(vlpFrontInfo.vlpText));
		memcpy(vlpFrontInfo.vlpTime, pResult->chPlateTime, strlen(pResult->chPlateTime));//车头识别时间
		vlpFrontInfo.vlpReliability = (int(pResult->fConfidenceLevel * 10000)) % 10000;
		VFR_WRITE_LOG("process front plate info finish.");

		const char* pchImgRootPath = "idevlp";
		//图片信息
		char* pImgPath = (char*)vlpFrontInfo.imageFile[0];
		char* pSrcImgPath = NULL;
		int iPathLen = 128;
		//车头图
		SaveImgStructFunc(&pResult->CIMG_BeginCapture, type_frontImg, iIndex, pchImgRootPath);
		pSrcImgPath = pResult->CIMG_BeginCapture.chSavePath;
		Tool_CopyStringToBuffer(pImgPath, iPathLen, pSrcImgPath);
		VFR_WRITE_LOG("process type_frontImg info finish.");

		//车头车牌图
		SaveImgStructFunc(&pResult->CIMG_PlateImage, type_frontPlate, iIndex, pchImgRootPath);
		pImgPath = (char*)vlpFrontInfo.imageFile[1];
		pSrcImgPath = pResult->CIMG_PlateImage.chSavePath;
		Tool_CopyStringToBuffer(pImgPath, iPathLen, pSrcImgPath);
		VFR_WRITE_LOG("process type_frontPlate info finish.");

		//车头二值化图
		SaveImgStructFunc(&pResult->CIMG_BinImage, type_frontBin, iIndex, pchImgRootPath);
		pImgPath = (char*)vlpFrontInfo.imageFile[2];
		pSrcImgPath = pResult->CIMG_BinImage.chSavePath;
		Tool_CopyStringToBuffer(pImgPath, iPathLen, pSrcImgPath);
		VFR_WRITE_LOG("process type_frontBin info finish.");

		VFR_WRITE_LOG("send front callback data begin.");
		int iLoginID = GetLoginID();
		EnterCriticalSection(&m_csFuncCallback);
		((CBFun_GetFrontResult)g_pFuncFrontResultCallback)(iLoginID, &vlpFrontInfo, g_pFrontResultUserData);
		LeaveCriticalSection(&m_csFuncCallback);
		VFR_WRITE_LOG("send front callback data finish.");

		T_VLPBACKINFO vlpTailInfo;
		memset(&vlpTailInfo, '\0', sizeof(vlpTailInfo));
		vlpTailInfo.vlpInfoSize = sizeof(vlpTailInfo);

		//车尾车牌信息
		vlpTailInfo.vlpBackColor[0] = 0;	//颜色
		vlpTailInfo.vlpBackColor[1] = pResult->iTailPlateColor;
		memset(chPlateNO, '\0', sizeof(chPlateNO));
		Tool_ProcessPlateNo(pResult->chTailPlateNO, chPlateNO, sizeof(chPlateNO));
		memcpy(vlpTailInfo.vlpBackText, chPlateNO, sizeof(vlpTailInfo.vlpBackText));
		VFR_WRITE_LOG("process tail plate info finish.");

		//车型信息
		vlpTailInfo.vlpCarClass = pResult->iVehTypeNo;
		vlpTailInfo.vehLength = (int)(pResult->fVehLenth);
		vlpTailInfo.vehWidth = (int)(pResult->fVehWidth);
		vlpTailInfo.vehHigh = (int)(pResult->fVehHeight);
		vlpTailInfo.vehAxis = pResult->iAxletreeCount;
		vlpTailInfo.vlpReliability = (int(pResult->fConfidenceLevel * 10000)) % 10000;


		//车身图
		SaveImgStructFunc(&pResult->CIMG_BestCapture, type_SideImg, iIndex, pchImgRootPath);   //保存车身图
		pImgPath = (char*)vlpTailInfo.imageFile[0];
		pSrcImgPath = pResult->CIMG_BestCapture.chSavePath;             //车身图保存最终路径
		Tool_CopyStringToBuffer(pImgPath, iPathLen, pSrcImgPath);      
		VFR_WRITE_LOG("process type_SideImg info finish.");


		//车尾图
		SaveImgStructFunc(&pResult->CIMG_LastCapture, type_tailImg, iIndex, pchImgRootPath);
		pImgPath = (char*)vlpTailInfo.imageFile[1];
		pSrcImgPath = pResult->CIMG_LastCapture.chSavePath;
		Tool_CopyStringToBuffer(pImgPath, iPathLen, pSrcImgPath);
		VFR_WRITE_LOG("process type_tailImg info finish.");

		//车尾车牌图
		SaveImgStructFunc(&pResult->CIMG_BestSnapshot, type_tailPlate, iIndex, pchImgRootPath);
		pImgPath = (char*)vlpTailInfo.imageFile[2];
		pSrcImgPath = pResult->CIMG_BestSnapshot.chSavePath;
		Tool_CopyStringToBuffer(pImgPath, iPathLen, pSrcImgPath);
		VFR_WRITE_LOG("process type_tailPlate info finish.");

		//车尾车牌二值化图
		//SaveImgFunc(&pResult->CIMG_LastCapture, type_taileBin, iIndex, imgPath);
		pImgPath = (char*)vlpTailInfo.imageFile[3];
		vlpTailInfo.imageFile[3][0] = NULL;


		//五秒视频
		vlpTailInfo.imageFile[4][0] = NULL;
		pImgPath = (char*)vlpTailInfo.imageFile[4];
		if (FindIfFileNameInReciveList(pResult->chSaveFileName))
		{
			char chVideoName[256] = {0};
			sprintf_s(chVideoName, sizeof(chVideoName), ".\\%s\\vlp_7_%02d.jpg", pchImgRootPath, iIndex);
			if (CopyFile(pResult->chSaveFileName, chVideoName, FALSE))
			{
				remove(pResult->chSaveFileName);
				sprintf_s(pImgPath, iPathLen, "%s", pResult->chSaveFileName);
			}
			else
			{
				VFR_WRITE_LOG("video file %s remove failed.", pResult->chSaveFileName);
			}
		}
		else
		{
			VFR_WRITE_LOG("video file %s is not ready.", pResult->chSaveFileName);
		}
		
		if (CheckTailResultcallback())
		{
			VFR_WRITE_LOG("send tail callback data begin.");
			EnterCriticalSection(&m_csFuncCallback);
			((CBFun_GetBackResult)g_pFuncTailResultCallback)(iLoginID, &vlpTailInfo, g_pTailResultUserData);
			LeaveCriticalSection(&m_csFuncCallback);
			VFR_WRITE_LOG("send tail callback data finish.");
		}
		else
		{
			VFR_WRITE_LOG("tail callback func == NULL.");
		}		
    }
    else
    {
        WriteFormatLog("g_pFuncResultCallback == NULL.");
    }
}

void Camera6467_VFR::SetMsgHandleAngMsg(void* handle, int msg)
{
    WriteFormatLog("SetMsgHandleAngMsg, handle = %p, msg = %d", handle, msg);
    EnterCriticalSection(&m_csFuncCallback);
    m_hMsgHanldle = handle;
    m_iMsg = msg;
    LeaveCriticalSection(&m_csFuncCallback);
}

void Camera6467_VFR::SetCheckThreadExit(bool bExit)
{
    EnterCriticalSection(&m_csFuncCallback);
    m_bStatusCheckThreadExit = bExit;
    LeaveCriticalSection(&m_csFuncCallback);
}

bool Camera6467_VFR::GetCheckThreadExit()
{
    bool bExit = false;
    EnterCriticalSection(&m_csFuncCallback);
    bExit = m_bStatusCheckThreadExit;
    LeaveCriticalSection(&m_csFuncCallback);
    return bExit;
}

bool Camera6467_VFR::OpenPlateCamera(const char* ipAddress)
{
    CHECK_ARG(ipAddress);

    WriteFormatLog("OpenPlateCamera %s begin.", ipAddress);
    m_Camera_Plate = std::make_shared<Camera6467_plate>();
    m_Camera_Plate->SetCameraIP(ipAddress);
    if (0 == m_Camera_Plate->ConnectToCamera())
    {
        WriteFormatLog("OpenPlateCamera %s success.", ipAddress);
        return true;
    }
    else
    {
        WriteFormatLog("OpenPlateCamera %s failed.", ipAddress);
        return false;
    }
}

std::shared_ptr<CameraResult> Camera6467_VFR::GetFrontResult()
{
    try
    {
        std::shared_ptr<CameraResult> pResultPlate;
        std::shared_ptr<CameraResult> pResultVFR;
        bool bGetPlateNo = false;
        if (m_Camera_Plate)
        {
            CameraResult* pTemp = m_Camera_Plate->GetOneResult();
            if (pTemp != NULL)
            {
                pResultPlate = std::shared_ptr<CameraResult>(pTemp);
                bGetPlateNo = true;
                pTemp = NULL;
            }
            else
            {
                bGetPlateNo = false;
            }
        }
        else
        {
            if (!m_VfrResultList.empty())
            {
                //pResultPlate = m_resultList.front();
                 m_VfrResultList.front(pResultPlate);
            }
        }
        if (pResultPlate != nullptr)
        {
            if (bGetPlateNo)
            {
                WriteFormatLog("Get one result from plate camera , plate : %s.", pResultPlate->chPlateNO);
                WriteFormatLog("GetFrontResult, plate No list:\n");
                BaseCamera::WriteLog(m_VfrResultList.GetAllPlateString().c_str());
                int iIndex = -1;
                if (strstr(pResultPlate->chPlateNO, "无"))
                {
                    WriteFormatLog("GetFrontResult, current plate no is 无车牌, use first result .");
                    iIndex = -1;
                }
                else
                {
                    iIndex = m_VfrResultList.GetPositionByPlateNo(pResultPlate->chPlateNO);
                }
                if (-1 != iIndex)
                {
                    WriteFormatLog("find the result from list, index = %d.", iIndex);
                    pResultVFR = m_VfrResultList.GetOneByIndex(iIndex);
                }
                else
                {
                    if (!m_VfrResultList.empty())
                    {
                        WriteFormatLog("can not find result from list, Get first result.");
                        //pResultVFR = m_resultList.front();
                         m_VfrResultList.front(pResultVFR);
                    }
                    else
                    {
                        WriteFormatLog("can not find result from list, the list is empty.");
                    }
                }
            }
            else
            {
                WriteFormatLog("can not get result from plate camera, get from VFR list.");
                if (!m_VfrResultList.empty())
                {
                    WriteFormatLog("Get first result.");
                    //pResultVFR = m_resultList.front();
                     m_VfrResultList.front(pResultVFR);
                }
                else
                {
                    WriteFormatLog("The list is empty.");
                }
            }
        }
        else
        {
            WriteFormatLog("Can not get result from  camera .");
        }
        return pResultVFR;
    }
    catch (bad_exception& e)
    {
        LOGFMTE("GetFrontResult, bad_exception, error msg = %s", e.what());

        return std::make_shared<CameraResult>();
    }
    catch (bad_alloc& e)
    {
        LOGFMTE("GetFrontResult, bad_alloc, error msg = %s", e.what());

        return std::make_shared<CameraResult>();
    }
    catch (exception& e)
    {
        LOGFMTE("GetFrontResult, exception, error msg = %s.", e.what());

        return std::make_shared<CameraResult>();
    }
    catch (void*)
    {
        LOGFMTE("GetFrontResult,  void* exception");
        return std::make_shared<CameraResult>();
    }
    catch (...)
    {
        LOGFMTE("GetFrontResult,  unknown exception");
        return std::make_shared<CameraResult>();
    }
}

std::shared_ptr<CameraResult> Camera6467_VFR::GetFrontResultByPosition(int position)
{
    try
    {
        WriteFormatLog("GetFrontResultByPosition , position = %d", position);
        std::shared_ptr<CameraResult> tempResult;
        if (m_VfrResultList.empty())
        {
            WriteFormatLog("GetFrontResultByPosition , resultList is empty, return null.");
            return tempResult;
        }
        if (position <= 0)
        {
            tempResult = GetFrontResult();
        }
        else
        {
            if (position >= m_VfrResultList.size())
            {
                WriteFormatLog("GetFrontResultByPosition , position : %d is larger than  resultList size %d, get the last one.",
                    position,
                    m_VfrResultList.size());
                tempResult = m_VfrResultList.GetOneByIndex(m_VfrResultList.size() - 1);
            }
            else
            {
                tempResult = m_VfrResultList.GetOneByIndex(position);
            }
        }
        WriteFormatLog("GetFrontResultByPosition ,finish");
        return tempResult;
    }
    catch (bad_exception& e)
    {
        LOGFMTE("GetFrontResultByPosition, bad_exception, error msg = %s", e.what());

        return std::make_shared<CameraResult>();
    }
    catch (bad_alloc& e)
    {
        LOGFMTE("GetFrontResultByPosition, bad_alloc, error msg = %s", e.what());

        return std::make_shared<CameraResult>();
    }
    catch (exception& e)
    {
        LOGFMTE("GetFrontResultByPosition, exception, error msg = %s.", e.what());

        return std::make_shared<CameraResult>();
    }
    catch (void*)
    {
        LOGFMTE("GetFrontResultByPosition,  void* exception");
        return std::make_shared<CameraResult>();
    }
    catch (...)
    {
        LOGFMTE("GetFrontResultByPosition,  unknown exception");
        return std::make_shared<CameraResult>();
    }
}

std::shared_ptr<CameraResult> Camera6467_VFR::GetLastResult()
{
    std::shared_ptr<CameraResult> temp;
    EnterCriticalSection(&m_csResult);
    temp = m_pLastResult;
    LeaveCriticalSection(&m_csResult);
    return temp;
}

std::shared_ptr<CameraResult> Camera6467_VFR::GetResultByCarID(unsigned long dwCarID)
{
    return m_VfrResultList.GetOneByCarid(dwCarID);
}

bool Camera6467_VFR::GetLastResultIfReceiveComplete()
{
    bool bValue = false;
    EnterCriticalSection(&m_csResult);
    bValue = m_bLastResultComplete ;
    LeaveCriticalSection(&m_csResult);
    return bValue;
}

void Camera6467_VFR::SetLastResultIfReceiveComplete(bool bValue)
{
    EnterCriticalSection(&m_csResult);
    m_bLastResultComplete = bValue;
    LeaveCriticalSection(&m_csResult);
}

void Camera6467_VFR::DeleteFrontResult(const char* plateNo)
{
    try
    {
        WriteFormatLog("DeleteFrontResult, plate no = %p", plateNo);
        bool bHasPlateNo = false;
        if (NULL != plateNo && strlen(plateNo) > 0)
        {
            bHasPlateNo = true;
        }
        std::string strPlateNo;
        if (bHasPlateNo)
        {
            strPlateNo = plateNo;
            WriteFormatLog("DeleteFrontResult, has plate no : %s", plateNo);
        }
        else
        {            
            if (  m_Camera_Plate != nullptr  )
            {
                WriteFormatLog("DeleteFrontResult, get from plate camera:");
                CameraResult* pResult = m_Camera_Plate->GetOneResult();
                if (NULL != pResult)
                {
                    strPlateNo = pResult->chPlateNO;
                    SAFE_DELETE_OBJ(pResult);
                    WriteFormatLog("DeleteFrontResult, plate no : %s", strPlateNo.c_str());
                }
                else
                {
                    WriteFormatLog("DeleteFrontResult,can not get from plate camera.");
                }
            }
            else
            {
                WriteFormatLog("DeleteFrontResult, cant not get plate number from plate camera.");
            }
        }

        if (strPlateNo.empty())
        {
            WriteFormatLog("DeleteFrontResult, cant not get plate number , so delete front result.");
            m_VfrResultList.pop_front();
        }
        else
        {
            if (std::string::npos == strPlateNo.find("无"))
            {
                int iPosition = m_VfrResultList.GetPositionByPlateNo(strPlateNo.c_str());
                WriteFormatLog("DeleteFrontResult, GetPositionByPlateNo %d.", iPosition);
                m_VfrResultList.DeleteToPosition(iPosition);
            }
            else
            {
                WriteFormatLog("DeleteFrontResult, current plate  %s == ‘无车牌’, do nothing.", strPlateNo.c_str());
            }
        }
        WriteFormatLog("DeleteFrontResult, final list:");
        BaseCamera::WriteLog(m_VfrResultList.GetAllPlateString().c_str());

        WriteFormatLog("DeleteFrontResult, finish");
    }
    catch (bad_exception& e)
    {
        LOGFMTE("DeleteFrontResult, bad_exception, error msg = %s", e.what());
    }
    catch (bad_alloc& e)
    {
        LOGFMTE("DeleteFrontResult, bad_alloc, error msg = %s", e.what());
    }
    catch (exception& e)
    {
        LOGFMTE("DeleteFrontResult, exception, error msg = %s.", e.what());
    }
    catch (void*)
    {
        LOGFMTE("DeleteFrontResult,  void* exception");
    }
    catch (...)
    {
        LOGFMTE("DeleteFrontResult,  unknown exception");
    }
}

void Camera6467_VFR::ClearALLResult()
{
    WriteFormatLog("ClearALLResult begin.");
    //m_resultList.ClearALL();
    m_VfrResultList.ClearALLResult();
    WriteFormatLog("ClearALLResult finish.");
}

size_t Camera6467_VFR::GetResultListSize()
{
    //WriteFormatLog("GetResultListSize begin.");
    size_t iSize = m_VfrResultList.size();
    //WriteFormatLog("GetResultListSize finish, size = %d.", iSize);
    return iSize;
}

void Camera6467_VFR::TryWaitCondition()
{
    //m_MySemaphore.tryDecrease(GetCurrentThreadId());
}

int Camera6467_VFR::RecordInfoBegin(DWORD dwCarID)
{
    try
    {
        WriteFormatLog("RecordInfoBegin, dwCarID = %lu", dwCarID);
        SetLastResultIfReceiveComplete(false);

        SAFE_DELETE_OBJ(m_pResult);
        //m_Result = std::make_shared<CameraResult>();
        if (NULL == m_pResult)
        {
            m_pResult = new CameraResult();
        }
        CHECK_ARG(m_pResult);

        m_pResult->dwCarID = dwCarID;

        WriteFormatLog("RecordInfoBegin, finish.");
        return 0;
    }
    catch (bad_exception& e)
    {
        LOGFMTE("RecordInfoBegin, bad_exception, error msg = %s", e.what());
        return 0;
    }
    catch (bad_alloc& e)
    {
        LOGFMTE("RecordInfoBegin, bad_alloc, error msg = %s", e.what());
        return 0;
    }
    catch (exception& e)
    {
        LOGFMTE("RecordInfoBegin, exception, error msg = %s.", e.what());
        return 0;
    }
    catch (void*)
    {
        LOGFMTE("Camera6467_VFR::RecordInfoBegin,  void* exception");
        return 0;
    }
    catch (...)
    {
        LOGFMTE("Camera6467_VFR::DeleteFrontResult,  unknown exception");
        return 0;
    }
}

int Camera6467_VFR::RecordInfoEnd(DWORD dwCarID)
{
    try
    {
        WriteFormatLog("RecordInfoEnd, dwCarID = %lu", dwCarID);
        CHECK_ARG(m_pResult);

        EnterCriticalSection(&m_csResult);
        if (m_pLastResult
            && m_pLastResult->dwCarID == dwCarID)
        {            
            if (strlen(m_pLastResult->chSaveFileName) > 0)
            {
                memset(m_pResult->chSaveFileName, '\0', sizeof(m_pResult->chSaveFileName));
                memcpy(m_pResult->chSaveFileName, m_pLastResult->chSaveFileName, strlen(m_pLastResult->chSaveFileName));
            }
        }
        m_pLastResult = std::shared_ptr<CameraResult>(new CameraResult(*m_pResult));        
        LeaveCriticalSection(&m_csResult);

        SetLastResultIfReceiveComplete(true);

        if (dwCarID == m_pResult->dwCarID)
        {
            if (CheckIfBackUpVehicle(m_pResult))
            {
                WriteFormatLog("current result is reversing car, drop this result.");
            }
            else
            {
                //if (m_dwLastCarID != dwCarID)
                //{
                //    //StopSaveAviFile(0, GetTickCount() + getVideoDelayTime() * 1000);
                //    StopSaveAviFile(0, m_pResult->dw64TimeMS + getVideoDelayTime() * 1000);
                //}
                //SaveResult(m_pResult);

                if (CheckIfSuperLength(m_pResult))
                {
                    WriteFormatLog("current length %f is larger than max length %d, clear list first.", m_pResult->fVehLenth, m_iSuperLenth);
                   // m_resultList.ClearALL();
                   // m_MySemaphore.resetCount(GetCurrentThreadId());
                    m_VfrResultList.ClearALLResult();                    
                }
                WriteFormatLog("push one result to list, current list plate NO:\n");
                if (!m_VfrResultList.empty())
                {
                    BaseCamera::WriteLog(m_VfrResultList.GetAllPlateString().c_str());
                }
                else
                {
                    WriteFormatLog("list is empty.");
                }
                std::shared_ptr<CameraResult> pResult(m_pResult);

                std::shared_ptr<CameraResult> pLastSameResult = m_VfrResultList.GetOneByCarid(dwCarID);
                if (pLastSameResult)
                {
                    WriteFormatLog("current car ID  %lu is already receive, replace it.", dwCarID, m_dwLastCarID);

                    if (pLastSameResult
                        &&strlen(pLastSameResult->chSaveFileName) > 0
                        && pLastSameResult->dwCarID == dwCarID)
                    {
                        WriteFormatLog("last car ID  %lu , avi fileName = %s.", pLastSameResult->dwCarID, pLastSameResult->chSaveFileName);
                        strcpy_s(pResult->chSaveFileName, pLastSameResult->chSaveFileName);                       
                    } 
                    m_VfrResultList.ReplaceByCarID(dwCarID, pResult);
                }
                else
                {
                    m_dwLastCarID = dwCarID;
                    //m_MySemaphore.notify(GetCurrentThreadId());
                    m_VfrResultList.push_back(pResult);
                }                
                WriteFormatLog("after push, list plate NO:\n");
                BaseCamera::WriteLog(m_VfrResultList.GetAllPlateString().c_str());   
                //SendResultByCallback(pResult);

                //UpdateSentStatus(m_pResult);

                m_pResult = NULL;
            }

            //if (NULL != m_hMsgHanldle)
            //{
            //    WriteLog("PostMessage");
            //    //::PostMessage(*((HWND*)m_hWnd),m_iMsg, 1, 0);
            //    ::PostMessage((HWND)m_hMsgHanldle, m_iResultMsg, (WPARAM)1, 0);
            //}
        }
        else
        {
            WriteFormatLog("current car ID  %lu is not same wit result carID %lu.", dwCarID, m_pResult->dwCarID);
        }

        WriteFormatLog("RecordInfoEnd, finish");
        return 0;
    }
    catch (bad_exception& e)
    {
        LOGFMTE("RecordInfoEnd, bad_exception, error msg = %s", e.what());
        return 0;
    }
    catch (bad_alloc& e)
    {
        LOGFMTE("RecordInfoEnd, bad_alloc, error msg = %s", e.what());
        return 0;
    }
    catch (exception& e)
    {
        LOGFMTE("RecordInfoEnd, exception, error msg = %s.", e.what());
        return 0;
    }
    catch (void*)
    {
        LOGFMTE("RecordInfoEnd,  void* exception");
        return 0;
    }
    catch (...)
    {
        LOGFMTE("RecordInfoEnd, unknown exception");
        return 0;
    }
}

int Camera6467_VFR::RecordInfoPlate(DWORD dwCarID,
    LPCSTR pcPlateNo, 
    LPCSTR pcAppendInfo,
    DWORD dwRecordType,
    DWORD64 dw64TimeMS)
{
    try
    {
		VFR_WRITE_LOG("RecordInfoPlate, dwCarID = %lu, plateNo = %s, dwRecordType= %x, dw64TimeMS= %I64u, xmlLength = %u",
            dwCarID,
            pcPlateNo,
            dwRecordType,
            dw64TimeMS,
			strlen(pcAppendInfo));
        BaseCamera::WriteLog(pcAppendInfo);
        CHECK_ARG(m_pResult);

        SetLastResultIfReceiveComplete(false);

        if (dwCarID == m_pResult->dwCarID)
        {
            m_pResult->dw64TimeMS = dw64TimeMS;
            m_pResult->dwReceiveTime = GetTickCount();
            strcpy_s(m_pResult->chPlateNO, sizeof(m_pResult->chPlateNO), pcPlateNo);
            if (strlen(pcAppendInfo) < sizeof(m_pResult->pcAppendInfo))
            {
				VFR_WRITE_LOG("begin to copy append info to buffer.");
                strcpy_s(m_pResult->pcAppendInfo, sizeof(m_pResult->pcAppendInfo), pcAppendInfo);
                AnalysisAppendXML(m_pResult);
            }
            else
            {
				VFR_WRITE_LOG("strlen(pcAppendInfo)< sizeof(m_pResult->pcAppendInfo), can not AnalysisAppendXML.");
            }
        }
        else
        {
			VFR_WRITE_LOG("current car ID  %lu is not same wit result carID %lu.", dwCarID, m_pResult->dwCarID);
        }

        //generateFileName(m_pResult);

        
        //strPlateTime = strPlateTime.substr(0, 8);
  
        //QDir dir(chAviPath);
        //dir.mkpath(chAviPath);

        //if(m_dwLastCarID != dwCarID)
        /*if (!FindIfCarIDInTheList(dwCarID))*/
        {
			if (!FindIfCarIDInTheList(dwCarID))
			{
				InsertCarIDToTheList(dwCarID);
			}            

            char chImgDir[256] = { 0 };
            GetImageDir(chImgDir, sizeof(chImgDir));
            std::string strPlateTime(m_pResult->chPlateTime);
            char chAviPath[256] = { 0 };
            sprintf(chAviPath, "%s\\%s\\%s-%s-%s\\",
                chImgDir,
                RESULT_DIR_NAME,
                strPlateTime.substr(0, 4).c_str(),
                strPlateTime.substr(4, 2).c_str(),
                strPlateTime.substr(6, 2).c_str());
            MakeSureDirectoryPathExists(chAviPath);
            memset(chAviPath, '\0', sizeof(chAviPath));
            //sprintf(chAviPath, "%s\\%s\\%lu-%I64u.avi", m_chImageDir, strPlateTime.c_str(), dwCarID, m_pResult->dw64TimeMS);
            //sprintf_s(chAviPath, sizeof(chAviPath), "%s\\%s\\%s\\%s\\%lu-%I64u.avi", 
            //    m_chImageDir,
            //    strPlateTime.substr(0, 4).c_str(),
            //    strPlateTime.substr(4, 2).c_str(),
            //    strPlateTime.substr(6, 2).c_str(),
            //    dwCarID, 
            //    m_pResult->dw64TimeMS);

            sprintf_s(chAviPath, sizeof(chAviPath), "%s\\%s\\%s-%s-%s\\%s.mp4",
                chImgDir,
                RESULT_DIR_NAME,
                strPlateTime.substr(0, 4).c_str(),
                strPlateTime.substr(4, 2).c_str(),
                strPlateTime.substr(6, 2).c_str(),
                m_pResult->chPlateTime);
            //qDebug()<<"chAviPath ="<<chAviPath;
            //StartToSaveAviFile(0, chAviPath, getVideoAdvanceTime()*1000);
            //StartToSaveAviFile(0, chAviPath, (m_pResult->dw64TimeMS - getVideoAdvanceTime() * 1000));
            //memset(m_pResult->chSaveFileName, '\0', sizeof(m_pResult->chSaveFileName));
            //memcpy(m_pResult->chSaveFileName, chAviPath, strlen(chAviPath));

            //WriteFormatLog("current car ID  %lu , avi fileName = %s.", dwCarID, chAviPath);
            std::string strFinalString = Tool_ReplaceStringInStd(chAviPath, "\\\\", "\\");
			VFR_WRITE_LOG("final save path = %s  .", strFinalString.c_str());
            const char* pChVideoPath = strFinalString.c_str();
			memset(m_pResult->chSaveFileName, '\0', sizeof(m_pResult->chSaveFileName));
			memcpy(m_pResult->chSaveFileName, pChVideoPath, strlen(pChVideoPath));

            if (!Tool_CheckIfFileNameIntheList(m_lsVideoName, pChVideoPath, &m_csFuncCallback))
            {
                StartToSaveAviFile(0, pChVideoPath, (m_pResult->dw64TimeMS - getVideoAdvanceTime() * 1000));
                WriteFormatLog("current car ID  %lu , avi fileName = %s.", dwCarID, chAviPath);

                StopSaveAviFile(0, m_pResult->dw64TimeMS + getVideoDelayTime() * 1000);

                Tool_AddFileNameToTheList(m_lsVideoName, pChVideoPath, &m_csFuncCallback, MAX_FILENAME_LIST_SIZE);
            }
            else
            {
                WriteFormatLog("current car ID  %lu ,but avi fileName = %s is already save, so do not save it.", dwCarID, chAviPath);
            }
        }

        WriteFormatLog("RecordInfoPlate, finish.");
        return 0;
    }
    catch (bad_exception& e)
    {
        LOGFMTE("RecordInfoPlate, bad_exception, error msg = %s", e.what());
        return 0;
    }
    catch (bad_alloc& e)
    {
        LOGFMTE("RecordInfoPlate, bad_alloc, error msg = %s", e.what());
        return 0;
    }
    catch (exception& e)
    {
        LOGFMTE("RecordInfoPlate, exception, error msg = %s.", e.what());
        return 0;
    }
    catch (void*)
    {
        LOGFMTE("RecordInfoPlate,  void* exception");
        return 0;
    }
    catch (...)
    {
        LOGFMTE("RecordInfoPlate, unknown exception");
        return 0;
    }
}

int Camera6467_VFR::RecordInfoBigImage(DWORD dwCarID, 
    WORD wImgType,
    WORD wWidth, 
    WORD wHeight, 
    PBYTE pbPicData,
    DWORD dwImgDataLen,
    DWORD dwRecordType, 
    DWORD64 dw64TimeMS)
{
    WriteFormatLog("RecordInfoBigImage, dwCarID = %lu, wImgType = %u, wWidth= %u, wHeight= %u, \
        dwImgDataLen= %lu, dwRecordType = %x, dw64TimeMS = %I64u.",
        dwCarID,
        wImgType,
        wWidth,
        wHeight,
        dwImgDataLen,
        dwRecordType,
        dw64TimeMS);

    CHECK_ARG(m_pResult);    
    SetLastResultIfReceiveComplete(false);
    
    if (dwCarID == m_pResult->dwCarID)
    {        
        if (wImgType == RECORD_BIGIMG_BEST_SNAPSHOT)
        {
            WriteFormatLog("RecordInfoBigImage BEST_SNAPSHO  ");

            CopyDataToIMG(m_pResult->CIMG_BestSnapshot, pbPicData, wWidth, wHeight, dwImgDataLen, wImgType);
        }
        else if (wImgType == RECORD_BIGIMG_LAST_SNAPSHOT)
        {
            WriteFormatLog("RecordInfoBigImage LAST_SNAPSHOT  ");

            CopyDataToIMG(m_pResult->CIMG_LastSnapshot, pbPicData, wWidth, wHeight, dwImgDataLen, wImgType);

            if (m_pResult->CIMG_BeginCapture.dwImgSize <= 0 )
            {//当前小黄人有个问题，会出现只有最清晰大图和最后大图的情况，在这种情况下，根据酱油哥说，可以用最后大图代替车头大图
                CopyDataToIMG(m_pResult->CIMG_BeginCapture, pbPicData, wWidth, wHeight, dwImgDataLen, wImgType);
            }
        }
        else if (wImgType == RECORD_BIGIMG_BEGIN_CAPTURE)
        {
            WriteFormatLog("RecordInfoBigImage BEGIN_CAPTURE  ");

            CopyDataToIMG(m_pResult->CIMG_BeginCapture, pbPicData, wWidth, wHeight, dwImgDataLen, wImgType);
        }
        else if (wImgType == RECORD_BIGIMG_BEST_CAPTURE)
        {
            WriteFormatLog("RecordInfoBigImage BEST_CAPTURE  ");

            CopyDataToIMG(m_pResult->CIMG_BestCapture, pbPicData, wWidth, wHeight, dwImgDataLen, wImgType);
        }
        else if (wImgType == RECORD_BIGIMG_LAST_CAPTURE)
        {
            WriteFormatLog("RecordInfoBigImage LAST_CAPTURE  ");

            CopyDataToIMG(m_pResult->CIMG_LastCapture, pbPicData, wWidth, wHeight, dwImgDataLen, wImgType);
        }
        else
        {
            WriteFormatLog("RecordInfoBigImage other Image, put it to  LAST_CAPTURE .");
            CopyDataToIMG(m_pResult->CIMG_LastCapture, pbPicData, wWidth, wHeight, dwImgDataLen, wImgType);
        }
        //if (NULL == strstr(pSavePath, "front"))
        //{
        //    if (Tool_SaveFileToPath(pSavePath, pbPicData, dwImgDataLen))
        //    {
        //        WriteFormatLog("RecordInfoBigImage %s save success .", pSavePath);
        //    }
        //    else
        //    {
        //        WriteFormatLog("RecordInfoBigImage %s save failed .", pSavePath);
        //    }           
        //}
        //Tool_SaveFileToPath(pSavePath, pbPicData, dwImgDataLen);
    }
    else
    {
        WriteFormatLog("current car ID  %lu is not same wit result carID %lu.", dwCarID, m_pResult->dwCarID);
    }

    WriteFormatLog("RecordInfoBigImage , finish.");
    return 0;
}

int Camera6467_VFR::RecordInfoSmallImage(DWORD dwCarID,
    WORD wWidth,
    WORD wHeight,
    PBYTE pbPicData, 
    DWORD dwImgDataLen, 
    DWORD dwRecordType,
    DWORD64 dw64TimeMS)
{
    WriteFormatLog("RecordInfoSmallImage, dwCarID = %lu, wWidth= %u, wHeight= %u, \
                                                                                    dwImgDataLen= %lu, dwRecordType = %x, dw64TimeMS = %I64u.",
                                                                                    dwCarID,
                                                                                    wWidth,
                                                                                    wHeight,
                                                                                    dwImgDataLen,
                                                                                    dwRecordType,
                                                                                    dw64TimeMS);

    CHECK_ARG(m_pResult);
    SetLastResultIfReceiveComplete(false);

    int iBuffLen = dwImgDataLen;
    if (m_pResult->dwCarID == dwCarID)
    {
        if (m_Small_IMG_Temp.pbImgData == NULL)
        {
            WriteFormatLog("malloc first small image buffer .");
            m_Small_IMG_Temp.pbImgData = new BYTE[MAX_IMG_SIZE];
            memset(m_Small_IMG_Temp.pbImgData, 0, MAX_IMG_SIZE);
        }

        if (m_Small_IMG_Temp.pbImgData)
        {
            int iDestLenth = MAX_IMG_SIZE;
            memset(m_Small_IMG_Temp.pbImgData, 0, MAX_IMG_SIZE);
            WriteFormatLog("convert yuv to bmp , begin .");

            HRESULT Hr = HVAPIUTILS_SmallImageToBitmapEx(pbPicData,
                wWidth,
                wHeight,
                m_Small_IMG_Temp.pbImgData,
                &iDestLenth);

            if (Hr == S_OK)
            {
                WriteFormatLog("convert YUV to bmp, success .");
                CopyDataToIMG(m_pResult->CIMG_PlateImage, m_Small_IMG_Temp.pbImgData, wWidth, wHeight, iDestLenth, 0);

                memset(m_Small_IMG_Temp.pbImgData, 0, MAX_IMG_SIZE);
                WriteFormatLog("convert bmp to jpg , begin .");

                size_t iBufLength = MAX_IMG_SIZE;
                bool bScale = Tool_Img_ScaleJpg(m_pResult->CIMG_PlateImage.pbImgData,
                    m_pResult->CIMG_PlateImage.dwImgSize,
                    m_Small_IMG_Temp.pbImgData,
                    &iBufLength,
                    m_pResult->CIMG_PlateImage.wImgWidth,
                    m_pResult->CIMG_PlateImage.wImgHeight,
                    80);
                if (bScale)
                {
                    WriteFormatLog("convert bmp to jpeg success, begin copy.");
                    CopyDataToIMG(m_pResult->CIMG_PlateImage, m_Small_IMG_Temp.pbImgData, wWidth, wHeight, iBufLength, 0);
                    WriteFormatLog("convert bmp to jpeg success, finish copy.");
                }
                else
                {
                    WriteFormatLog("convert bmp to jpeg failed, use default.");
                }
            }
            else
            {
                WriteFormatLog("convert YUV to bmp , failed, use default.");
                CopyDataToIMG(m_pResult->CIMG_PlateImage, pbPicData, wWidth, wHeight, dwImgDataLen, 0);
            }

            //if (m_pResult->CIMG_PlateImage.pbImgData
            //    && m_pResult->CIMG_PlateImage.dwImgSize > 0)
            //{
            //    char* pSavePath = NULL;
            //    std::string strPlateTime(m_pResult->chPlateTime);

            //    pSavePath = m_pResult->CIMG_PlateImage.chSavePath;
            //    //sprintf_s(pSavePath, 256, "%s\\%s\\%lu-%I64u-plate.jpg",
            //    //    m_chImageDir,
            //    //    m_pResult->chPlateTime,
            //    //    dwCarID,
            //    //    m_pResult->dw64TimeMS);
            //    sprintf_s(pSavePath, 256, "%s\\%s\\%s\\%s\\%s-front-plate.jpg",
            //        m_chImageDir,
            //        strPlateTime.substr(0, 4).c_str(),
            //        strPlateTime.substr(4, 2).c_str(),
            //        strPlateTime.substr(6, 2).c_str(),
            //        m_pResult->chPlateTime);

            //    //Tool_SaveFileToPath(pSavePath, m_pResult->CIMG_PlateImage.pbImgData, m_pResult->CIMG_PlateImage.dwImgSize);
            //}
        }
        else
        {
            WriteFormatLog("malloc first small image buffer failed .");
        }
    }

    WriteFormatLog("RecordInfoSmallImage, finish.");
    return 0;
}

int Camera6467_VFR::RecordInfoBinaryImage(DWORD dwCarID, 
    WORD wWidth, 
    WORD wHeight, 
    PBYTE pbPicData, 
    DWORD dwImgDataLen,
    DWORD dwRecordType, 
    DWORD64 dw64TimeMS)
{
    WriteFormatLog("RecordInfoBinaryImage, dwCarID = %lu, wWidth= %u, wHeight= %u, \
                                                                                    dwImgDataLen= %lu, dwRecordType = %x, dw64TimeMS = %I64u.",
                                                                                    dwCarID,
                                                                                    wWidth,
                                                                                    wHeight,
                                                                                    dwImgDataLen,
                                                                                    dwRecordType,
                                                                                    dw64TimeMS);
    CHECK_ARG(m_pResult);
    SetLastResultIfReceiveComplete(false);
    if (dwCarID == m_pResult->dwCarID)
    {
		int iBufferlength = 1024 * 1024;
		if (m_pTempBin == NULL)
		{
			m_pTempBin = new BYTE[1024 * 1024];
			memset(m_pTempBin, 0x00, iBufferlength);
		}
		if (m_pTempBin)
		{
			memset(m_pTempBin, 0x00, iBufferlength);

			HRESULT hRet = HVAPIUTILS_BinImageToBitmapEx(pbPicData, m_pTempBin, &iBufferlength);
			if (hRet == S_OK)
			{
				CopyDataToIMG(m_pResult->CIMG_BinImage, m_pTempBin, wWidth, wHeight, iBufferlength, 0);
			}
			else
			{
				WriteLog("HVAPIUTILS_BinImageToBitmapEx, failed, use default.");
				CopyDataToIMG(m_pResult->CIMG_BinImage, pbPicData, wWidth, wHeight, dwImgDataLen, 0);
			}
		}
		else
		{
			WriteFormatLog("m_pTempBin == NULL.");
		}
        //CopyDataToIMG(m_pResult->CIMG_BinImage, pbPicData, wWidth, wHeight, dwImgDataLen, 0);
    }
    else
    {
        WriteFormatLog("current car ID  %lu is not same wit result carID %lu.", dwCarID, m_pResult->dwCarID);
    }
    WriteFormatLog("RecordInfoBinaryImage, finish.");
    return 0;
}

int Camera6467_VFR::DeviceJPEGStream(PBYTE pbImageData, 
    DWORD dwImageDataLen,
    DWORD dwImageType, 
    LPCSTR szImageExtInfo)
{
    static int iCout = 0;
    if (iCout++ > 100)
    {
        WriteFormatLog("DeviceJPEGStream, pbImageData = %p, plateNo = %s, dwImageDataLen= %lu, dwImageType= %lu",
            pbImageData,
            dwImageDataLen,
            dwImageType);

        iCout = 0;
    }

    EnterCriticalSection(&m_csResult);
    m_bJpegComplete = false;

    m_CIMG_StreamJPEG.dwImgSize = dwImageDataLen;
    m_CIMG_StreamJPEG.wImgWidth = 1920;
    m_CIMG_StreamJPEG.wImgHeight = 1080;
    if (NULL == m_CIMG_StreamJPEG.pbImgData)
    {
        m_CIMG_StreamJPEG.pbImgData = new unsigned char[MAX_IMG_SIZE];
        memset(m_CIMG_StreamJPEG.pbImgData, 0, MAX_IMG_SIZE);
    }
    if (m_CIMG_StreamJPEG.pbImgData)
    {
        memset(m_CIMG_StreamJPEG.pbImgData, 0, MAX_IMG_SIZE);
        memcpy(m_CIMG_StreamJPEG.pbImgData, pbImageData, dwImageDataLen);
        m_bJpegComplete = true;
    }
    LeaveCriticalSection(&m_csResult);

    return 0;
}

void Camera6467_VFR::CheckStatus()
{
    int iLastStatus = -1;
    INT64 iLastTick = 0, iCurrentTick = 0;
    int iFirstConnctSuccess = -1;

    while (!GetCheckThreadExit())
    {
        Sleep(1000);
        iCurrentTick = GetTickCount();
        int iTimeInterval = GetTimeInterval();
        if ((iCurrentTick - iLastTick) >= (iTimeInterval * 1000)
			|| -1 == iFirstConnctSuccess)
        {
            int iStatus = GetCamStatus();
            //if (iStatus != iLastStatus)
            
                if (iStatus == 0)
                {
					//if (iStatus != iLastStatus)
					//{
					//	pThis->SendConnetStateMsg(true);
					//}
					SendConnetStateMsg(true);
                    WriteLog("设备连接正常.");
                    iFirstConnctSuccess = 0;
                }
                else
                {
                    SendConnetStateMsg(false);
                    WriteLog("设备连接失败, 尝试重连");

                    if (iFirstConnctSuccess == -1)
                    {
                        //pThis->ConnectToCamera();
                    }
                }
            
            iLastStatus = iStatus;

            iLastTick = iCurrentTick;
        }
    }
}

bool Camera6467_VFR::checkIfHasThreePic(std::shared_ptr<CameraResult> result)
{
    //if ( result->CIMG_BestCapture.dwImgSize > 0
    //    && result->CIMG_LastCapture.dwImgSize > 0)
    //{
    //    return true;
    //}

    //if (result->CIMG_LastSnapshot.dwImgSize > 0/*
    //    && result->CIMG_BestCapture.dwImgSize <= 0*/)
    //{
    //    return true;
    //}

	if (result->CIMG_BeginCapture.dwImgSize > 0
		&& result->CIMG_BestCapture.dwImgSize > 0
		&& result->CIMG_LastCapture.dwImgSize > 0
		&& FindIfFileNameInReciveList(result->chSaveFileName))
	{
		return true;
	}
    return false;
}

int Camera6467_VFR::getResultWaitTime()
{
    int iValue = 0;
    EnterCriticalSection(&m_csFuncCallback);
    iValue = m_iWaitVfrTimeOut;
    LeaveCriticalSection(&m_csFuncCallback);
    return iValue;
}

void Camera6467_VFR::SetResultWaitTime(int iValue)
{
    EnterCriticalSection(&m_csFuncCallback);
    m_iWaitVfrTimeOut = iValue;
    LeaveCriticalSection(&m_csFuncCallback);
}

//unsigned int Camera6467_VFR::SendResultThreadFunc()
//{
//    WriteFormatLog("SendResultThreadFunc begin.");
//    std::list<DWORD> lsSentCarIdList;
//	std::list<ResultSentStatus >sentStausList;
//	std::shared_ptr<CameraResult> pResult;
//    bool bNeedSendResult = false;
//	int iWaitTimeOut = getResultWaitTime();
//
//	auto FindIfInTheListFunc = [](std::list<ResultSentStatus >& sendList, unsigned long dwCarID)
//	{
//		return (std::end(sendList) != std::find_if(std::begin(sendList), std::end(sendList),
//			[dwCarID](ResultSentStatus sta)
//		{ return  (sta.dwCarID == dwCarID); }
//		));
//	};
//
//	ResultSentStatus status;
//	unsigned long dwCurrentTick = GetTickCount();
//	const int iMaxListSize = 10;
//    while (!GetCheckThreadExit())
//    {
//        Sleep(100);
//		dwCurrentTick = GetTickCount();
//		if (!m_lsStatusList.getFirstElement(status))
//		{
//			continue;
//		}
//		if (FindIfInTheListFunc(sentStausList, status.dwCarID))
//		{
//			//如果在已发送的队列中，但是没有新的内容更新，则删除该结果
//			VFR_WRITE_LOG("login id = %d , carId = %u , is send finish remove it.", GetLoginID(), status.dwCarID);
//
//			m_VfrResultList.DeleteByCarID(status.dwCarID);
//			m_lsStatusList.RemoveElement(GetLoginID(), status.dwCarID);
//
//			continue;
//		}
//		else
//		{
//			//不在已发送的队列中，则判断内容是否接收完毕，且是否超时
//			if (dwCurrentTick - status.uTimeReceive >= iWaitTimeOut
//				|| ( status.iFrontImgSendStatus == sta_receiveDone
//						&& status.iSidImgSendStatus == sta_receiveDone
//						&& status.iTailImgSendStatus == sta_receiveDone
//						&& status.iVideoSendStatus == sta_receiveDone
//					)
//				)
//			{
//				pResult = m_VfrResultList.GetOneByCarid(status.dwCarID);
//				SendResultByCallback(pResult);
//				pResult = nullptr;
//
//				m_VfrResultList.DeleteByCarID(status.dwCarID);
//				m_lsStatusList.RemoveElement(GetLoginID(), status.dwCarID);
//
//				if (sentStausList.size() > iMaxListSize)
//				{
//					VFR_WRITE_LOG("ex sent list size is  %d larger than %d, remove first one.", sentStausList.size(), iMaxListSize);
//					sentStausList.pop_front();
//				}
//				sentStausList.push_back(status);
//			}
//		}
//    }
//    WriteFormatLog("SendResultThreadFunc finish.");
//    return 0;
//}


//unsigned int Camera6467_VFR::SendResultThreadFunc_WithNoSignal()
//{
//    VFR_WRITE_LOG("SendResultThreadFunc begin.");
//    std::list<DWORD> lsSentCarIdList;
//    bool bSendResult = false;
//    std::shared_ptr<CameraResult> pResult;
//    std::list<ResultSentStatus >sentStausList;
//    int iWaitTimeOut = getResultWaitTime();
//    unsigned long dwCurrentTick = GetTickCount();
//    unsigned long dwLastTick = GetTickCount();
//    int iSendItem = 0;
//    ResultSentStatus status;
//    const int iMaxListSize = 4;
//
//    auto FindIfInTheListFunc = [](std::list<ResultSentStatus >& sendList, unsigned long dwCarID)
//    {
//        return (std::end(sendList) != std::find_if(std::begin(sendList), std::end(sendList),
//            [dwCarID](ResultSentStatus sta) 
//        { return  (sta.dwCarID == dwCarID) ; }
//        ));
//    };
//
//    auto UpdateStatusInTheListFunc = [](std::list<ResultSentStatus >& sendList, ResultSentStatus newSta)
//    {
//        for (auto it = sendList.begin(); it != sendList.end(); it++)
//        {
//            if (it->dwCarID == newSta.dwCarID)
//            {
//                it->iFrontImgSendStatus = newSta.iFrontImgSendStatus;
//                it->iSidImgSendStatus = newSta.iSidImgSendStatus;
//                it->iTailImgSendStatus = newSta.iTailImgSendStatus;
//                it->iVideoSendStatus = newSta.iVideoSendStatus;
//            }
//        }
//    };
//
//    auto GetStatusFromListFunc = [](std::list<ResultSentStatus >& sendList, ResultSentStatus& newSta)
//    {
//        for (auto it = sendList.begin(); it != sendList.end(); it++)
//        {
//            if (it->dwCarID == newSta.dwCarID)
//            {
//                newSta = *it;
//                return true;
//            }
//        }
//        return false;
//    };    
//
//    while (!GetCheckThreadExit())
//    {
//        Sleep(50);
//        dwCurrentTick = GetTickCount();
//
//        if (GetIfSendResult()
//            && dwCurrentTick - dwLastTick < 2000)
//        {
//            continue;
//        }
//        dwLastTick = GetTickCount();
//
//        //if (sentStausList.size() > 0)
//        //{
//        //    ResultSentStatus tempStatus = sentStausList.front();
//        //    if (dwCurrentTick - tempStatus.uTimeReceive > 2 * 60 * 1000)
//        //    {
//        //        VFR_WRITE_LOG("firt status of sentList carID =  %lu, receive time %lu is time out 120s, remove it.", tempStatus.dwCarID, tempStatus.uTimeReceive);
//        //        sentStausList.pop_front();
//        //    }
//        //}
//
//        if (!m_lsStatusList.getFirstElement(status))
//        {
//            continue;
//        }
//
//        //if (dwCurrentTick - status.uTimeReceive >= iWaitTimeOut
//        //    || FindIfCarIDInTheList(status.dwCarID))
//        //{            
//        //    VFR_WRITE_LOG("login id = %d , carId = %u , is time out, remove the status.", GetLoginID(), status.dwCarID);
//        //    m_VfrResultList.DeleteByCarID(status.dwCarID);
//        //    m_lsStatusList.RemoveElement(GetLoginID(), status.dwCarID);
//
//        //    if (!FindIfCarIDInTheList(status.dwCarID))
//        //    {
//        //        InsertCarIDToTheList(status.dwCarID);
//        //    }
//        //    continue;
//        //}
//
//
//        if (dwCurrentTick - status.uTimeReceive >= iWaitTimeOut)
//        {
//            //当前结果在队列中保留的时间超时了
//            if (FindIfInTheListFunc(sentStausList, status.dwCarID))
//            {
//                ResultSentStatus statusSent;
//                if (GetStatusFromListFunc(sentStausList, statusSent))
//                {
//                    if (statusSent.iFrontImgSendStatus == sta_sentFinish)
//                    {
//                        status.iFrontImgSendStatus = sta_sentFinish;
//                    }
//
//                    if (statusSent.iSidImgSendStatus == sta_sentFinish)
//                    {
//                        status.iSidImgSendStatus = sta_sentFinish;
//                    }
//
//                    if (statusSent.iTailImgSendStatus == sta_sentFinish)
//                    {
//                        status.iTailImgSendStatus = sta_sentFinish;
//                    }
//
//                    if (statusSent.iVideoSendStatus == sta_sentFinish)
//                    {
//                        status.iVideoSendStatus = sta_sentFinish;
//                    }
//                }
//                //在已发送的队列中
//                if (status.iFrontImgSendStatus == sta_receiveDone
//                    || status.iSidImgSendStatus == sta_receiveDone
//                    || status.iTailImgSendStatus == sta_receiveDone
//                    || status.iVideoSendStatus == sta_receiveDone)
//                {
//                    //如果在已发送的队列中，但是有新的内容更新，则推送更新内容
//                    VFR_WRITE_LOG("login id = %d , carId = %u , is time out and send before, but some of the item , still send it.", GetLoginID(), status.dwCarID);
//                    VFR_WRITE_LOG("-------------------------------------------------------------------");
//                    VFR_WRITE_LOG("status :     int DeviceID = %d,   dwCarID = %lu, iFrontImgSendStatus = %d , iSidImgSendStatus = %d , \
//                                                                                                                                                      iTailImgSendStatus = %d ,  iVideoSendStatus = %d , iSmallImgSendStatus = %d iBinaryImgSendStatus = %d , uTimeReceive = %lu ",
//                                                                                                                                                      status.DeviceID,
//                                                                                                                                                      status.dwCarID,
//                                                                                                                                                      status.iFrontImgSendStatus,
//                                                                                                                                                      status.iSidImgSendStatus,
//                                                                                                                                                      status.iTailImgSendStatus,
//                                                                                                                                                      status.iVideoSendStatus,
//                                                                                                                                                      status.iSmallImgSendStatus,
//                                                                                                                                                      status.iBinaryImgSendStatus,
//                                                                                                                                                      status.uTimeReceive);
//                    VFR_WRITE_LOG("-------------------------------------------------------------------");
//                }
//                else
//                {
//                    //如果在已发送的队列中，但是没有新的内容更新，则删除该结果
//                    VFR_WRITE_LOG("login id = %d , carId = %u , is time out and send finish remove it.", GetLoginID(), status.dwCarID);
//
//                    m_VfrResultList.DeleteByCarID(status.dwCarID);
//                    m_lsStatusList.RemoveElement(GetLoginID(), status.dwCarID);
//
//                    continue;
//                }
//            }
//            else
//            {
//                //不在已发送的队列中
//                if (status.iFrontImgSendStatus == sta_receiveDone
//                    || status.iSidImgSendStatus == sta_receiveDone
//                    || status.iTailImgSendStatus == sta_receiveDone
//                    || status.iVideoSendStatus == sta_receiveDone)
//                {
//                    //超时，但有内容更新，发送该结果
//                    VFR_WRITE_LOG("login id = %d , carId = %u , is time out, but some status update , still send it.", GetLoginID(), status.dwCarID);
//                    VFR_WRITE_LOG("-------------------------------------------------------------------");
//                    VFR_WRITE_LOG("status :     int DeviceID = %d,   dwCarID = %lu, iFrontImgSendStatus = %d , iSidImgSendStatus = %d ,iTailImgSendStatus = %d ,  iVideoSendStatus = %d , iSmallImgSendStatus = %d iBinaryImgSendStatus = %d , uTimeReceive = %lu ",
//                                                                                                                                                                                                                          status.DeviceID,
//                                                                                                                                                                                                                          status.dwCarID,
//                                                                                                                                                                                                                          status.iFrontImgSendStatus,
//                                                                                                                                                                                                                          status.iSidImgSendStatus,
//                                                                                                                                                                                                                          status.iTailImgSendStatus,
//                                                                                                                                                                                                                          status.iVideoSendStatus,
//                                                                                                                                                                                                                          status.iSmallImgSendStatus,
//                                                                                                                                                                                                                          status.iBinaryImgSendStatus,
//                                                                                                                                                                                                                          status.uTimeReceive);
//                    VFR_WRITE_LOG("-------------------------------------------------------------------");
//                }
//                else
//                {
//                    //超时，且没有内容更新，删除队列中的这个结果
//                    VFR_WRITE_LOG("login id = %d , carId = %u , is time out, and no status update remove it.", GetLoginID(), status.dwCarID);
//                    m_VfrResultList.DeleteByCarID(status.dwCarID);
//                    m_lsStatusList.RemoveElement(GetLoginID(), status.dwCarID);
//
//                    if (sentStausList.size() > iMaxListSize)
//                    {
//                        VFR_WRITE_LOG("ex sent list size is  %d larger than 10, remove first one.", sentStausList.size());
//                        sentStausList.pop_front();
//                    }
//                    sentStausList.push_back(status);
//                    continue;
//                }
//            }
//        }
//        else
//        {
//            if (FindIfInTheListFunc(sentStausList, status.dwCarID))
//            {
//                VFR_WRITE_LOG("login id = %d, carId = %u  is sent send before, check status.", GetLoginID(), status.dwCarID);
//
//                ResultSentStatus statusSent;
//                statusSent.dwCarID = status.dwCarID;
//                if (GetStatusFromListFunc(sentStausList, statusSent))
//                {
//                    VFR_WRITE_LOG("status :     int DeviceID = %d,   dwCarID = %lu, iFrontImgSendStatus = %d , iSidImgSendStatus = %d ,iTailImgSendStatus = %d ,  iVideoSendStatus = %d , iSmallImgSendStatus = %d iBinaryImgSendStatus = %d , uTimeReceive = %lu ",
//                        status.DeviceID,
//                        status.dwCarID,
//                        status.iFrontImgSendStatus,
//                        status.iSidImgSendStatus,
//                        status.iTailImgSendStatus,
//                        status.iVideoSendStatus,
//                        status.iSmallImgSendStatus,
//                        status.iBinaryImgSendStatus,
//                        status.uTimeReceive);
//
//                    if (statusSent.iFrontImgSendStatus == sta_sentFinish)
//                    {
//                        status.iFrontImgSendStatus = sta_sentFinish;
//                        VFR_WRITE_LOG("check item iFrontImgSendStatus, staus value =sta_sentFinish , not send again .");
//                    }
//
//                    if (statusSent.iSidImgSendStatus == sta_sentFinish)
//                    {
//                        status.iSidImgSendStatus = sta_sentFinish;
//                        VFR_WRITE_LOG("check item iSidImgSendStatus, staus value =sta_sentFinish , not send again .");
//                    }
//
//                    if (statusSent.iTailImgSendStatus == sta_sentFinish)
//                    {
//                        status.iTailImgSendStatus = sta_sentFinish;
//                        VFR_WRITE_LOG("check item iTailImgSendStatus, staus value =sta_sentFinish , not send again .");
//                    }
//
//                    if (statusSent.iVideoSendStatus == sta_sentFinish)
//                    {
//                        status.iVideoSendStatus = sta_sentFinish;
//                        VFR_WRITE_LOG("check item iVideoSendStatus, staus value =sta_sentFinish , not send again .");
//                    }
//                }
//                else
//                {
//                    VFR_WRITE_LOG("login id = %d, carId = %u  is sent send before, get status from sent list failed", GetLoginID(), status.dwCarID);
//                }
//            }
//        }
//
//        if (status.iFrontImgSendStatus == sta_receiveDone)
//        {
//            iSendItem = item_frontImg;
//            if (m_hWnd != NULL)
//            {
//                VFR_WRITE_LOG("PostMessage winWnd = %p, msg = %d, MsgType = %d, carID = %lu", m_hWnd, m_iMsg, iSendItem, status.dwCarID);
//                ::PostMessage(m_hWnd, m_iMsg, iSendItem, 0);
//            }
//            UpdateSendStatus(GetLoginID(), status.dwCarID, iSendItem, sta_waitGet);
//            SetIfSendResult(true);
//            continue;
//        }
//
//        if (status.iSidImgSendStatus == sta_receiveDone)
//        {
//            iSendItem = item_sideImg;
//            if (m_hWnd != NULL)
//            {
//                VFR_WRITE_LOG("PostMessage winWnd = %p, msg = %d, MsgType = %d, carID = %lu", m_hWnd, m_iMsg, iSendItem, status.dwCarID);
//                ::PostMessage(m_hWnd, m_iMsg, iSendItem, 0);
//            }
//            UpdateSendStatus(GetLoginID(), status.dwCarID, iSendItem, sta_waitGet);
//            SetIfSendResult(true);
//            continue;
//        }
//
//        if (status.iTailImgSendStatus == sta_receiveDone)
//        {
//            iSendItem = item_tailImg;
//            if (m_hWnd != NULL)
//            {
//                VFR_WRITE_LOG("PostMessage winWnd = %p, msg = %d, MsgType = %d, carID = %lu", m_hWnd, m_iMsg, iSendItem, status.dwCarID);
//                ::PostMessage(m_hWnd, m_iMsg, iSendItem, 0);
//            }
//            UpdateSendStatus(GetLoginID(), status.dwCarID, iSendItem, sta_waitGet);
//            SetIfSendResult(true);
//            continue;
//        }
//
//        if (status.iVideoSendStatus == sta_receiveDone)
//        {
//            iSendItem = item_video;
//            if (m_hWnd != NULL)
//            {
//                VFR_WRITE_LOG("PostMessage winWnd = %p, msg = %d, MsgType = %d, carID = %lu", m_hWnd, m_iMsg, iSendItem, status.dwCarID);
//                ::PostMessage(m_hWnd, m_iMsg, iSendItem, 0);
//            }
//            UpdateSendStatus(GetLoginID(), status.dwCarID, iSendItem, sta_waitGet);
//            SetIfSendResult(true);
//            continue;
//        }
//
//        if (sta_sentFinish == status.iFrontImgSendStatus
//            && sta_sentFinish == status.iFrontImgSendStatus
//            && sta_sentFinish == status.iTailImgSendStatus)
//        {
//            VFR_WRITE_LOG("login id = %d, carId = %u  is sent finish  remove the status.", GetLoginID(), status.dwCarID);
//            m_VfrResultList.DeleteByCarID(status.dwCarID);
//            m_lsStatusList.RemoveElement(GetLoginID(), status.dwCarID);
//
//            if (FindIfInTheListFunc(sentStausList, status.dwCarID))
//            {
//                VFR_WRITE_LOG("carID %lu is in the sent list , update it.", status.dwCarID);
//                UpdateStatusInTheListFunc(sentStausList, status);
//
//            }
//            else
//            {
//                VFR_WRITE_LOG("carID %lu is not in the sent list , add it.", status.dwCarID);
//                if (sentStausList.size() > iMaxListSize)
//                {
//                    VFR_WRITE_LOG("sent list size is  %d larger than 10, remove first one.", sentStausList.size());
//                    sentStausList.pop_front();
//                }
//                sentStausList.push_back(status);
//            }
//            continue;
//        }
//
//    }
//    VFR_WRITE_LOG("SendResultThreadFunc finish.");
//    return 0;
//}

unsigned int Camera6467_VFR::SendResultThreadFunc_Separate()
{
	WriteFormatLog("SendResultThreadFunc begin.");

	typedef struct _SendFlag
	{
		int index;
		unsigned long dwCarID;
		bool bSendFront;
		bool bSendSide;
		bool bSendTail;
		bool bSendAppendInfo;		

		_SendFlag()
		{
			index = 0;
			dwCarID = 0;
			bSendFront = false;
			bSendSide = false;
			bSendTail = false;
			bSendAppendInfo = false;
		}
	}SendFlag;

	std::list<SendFlag> sentList;
	enum itemType{ item_carID, item_front, item_side, item_tail };

	auto findIfIntheList = [](std::list<SendFlag>& staList, unsigned long carID)
	{
		for (auto it = std::begin(staList); it != std::end(staList); it++)
		{
			if (it->dwCarID == carID)
			{
				return true;
			}
		}
		return false;
	};

	auto updateStaInTheList = [](std::list<SendFlag>& staList, unsigned long carID, int item, int value)
	{
		for (auto it = std::begin(staList); it != std::end(staList); it++)
		{
			if (it->dwCarID == carID)
			{
				switch (item)
				{
					case item_carID:
						it->dwCarID = value;
						break;
					case item_front:
						it->bSendFront = value == 0 ? false : true;
						break;
					case item_side:
						it->bSendSide = value == 0 ? false : true;
						break;
					case item_tail:
						it->bSendTail = value == 0 ? false : true;
						break;
					default:
						break;
				}
				break;
			}
		}
	};

	auto addStatusToList = [](std::list<SendFlag>& staList, SendFlag sta)
	{
		if (staList.size() > 5)
		{
			staList.pop_front();
		}
		staList.push_back(sta);
	};

	auto getStatusInTheList = [](std::list<SendFlag>& staList, unsigned long carID, SendFlag& statues)
	{
		for (auto it = std::begin(staList); it != std::end(staList); it++)
		{
			if (it->dwCarID == carID)
			{
				statues = *it;
				return true;
			}			
		}
		return false;
	};
	
	int iIndex = 0;
	SendFlag Currentflag;
	DWORD dwReceiveTime = 0;
	DWORD dwCurrentTime = 0;
	DWORD dwTimeDif = 0;
	int iResultTimeOut = getResultWaitTime();
	while (!GetCheckThreadExit())
	{
		Sleep(100);
		if (m_VfrResultList.empty())
		{
			continue;
		}

		std::shared_ptr<CameraResult> pResult = nullptr;
		m_VfrResultList.front(pResult);
		if (!pResult)
		{
			continue;
		}
		memset(&Currentflag, 0, sizeof(Currentflag));
		Currentflag.index = -1;
		dwReceiveTime = pResult->dwReceiveTime;
		dwCurrentTime = GetTickCount();
		dwTimeDif = dwCurrentTime - dwReceiveTime;

		if (findIfIntheList(sentList, pResult->dwCarID))
		{
			VFR_WRITE_LOG("current result is sent befor, carid = %lu, plateNO = %s.", pResult->dwCarID, pResult->chPlateNO);
			SendFlag flag;
			if (getStatusInTheList(sentList, pResult->dwCarID, flag))
			{
				VFR_WRITE_LOG("getStatusInTheList success, index = %d, carID = %lu, bSendFront = %d, bSendTail = %d",
					flag.index,
					flag.dwCarID,
					flag.bSendFront,
					flag.bSendTail);

				if (flag.bSendFront == false
					&& pResult->CIMG_BeginCapture.dwImgSize > 0)
				{
					SendFrontResultByCallback(pResult, flag.index);
					updateStaInTheList(sentList, pResult->dwCarID, item_front, 1);
					flag.bSendFront = true;
				}

				if (flag.bSendTail == false
					&& checkIfHasThreePic(pResult))
				{
					SendTailResultByCallback(pResult, flag.index);
					updateStaInTheList(sentList, pResult->dwCarID, item_tail, 1);
					flag.bSendTail = true;
				}

				Currentflag = flag;
			}
			else
			{
				VFR_WRITE_LOG("can not get status form sentStatus list.");
			}
		}
		else
		{
			VFR_WRITE_LOG("current result is new, carid = %lu, plateNO = %s.", pResult->dwCarID, pResult->chPlateNO);
			SendFlag flag;
			flag.dwCarID = pResult->dwCarID;
			flag.index = iIndex;
			if (pResult->CIMG_BeginCapture.dwImgSize > 0
				&& pResult->CIMG_BeginCapture.pbImgData != NULL)
			{
				SendFrontResultByCallback(pResult, flag.index);
				flag.bSendFront = true;
			}

			if (checkIfHasThreePic(pResult))
			{
				SendTailResultByCallback(pResult, flag.index);
				flag.bSendTail = true;
			}
			Currentflag = flag;
			addStatusToList(sentList, flag);

			if (CheckFrontResultCallback())
			{
				iIndex = (iIndex >= 99) ? 0 : iIndex + 1;
			}			
		}		
		
		if (Currentflag.index == -1
			|| (Currentflag.bSendFront&& Currentflag.bSendTail)
			|| (dwTimeDif > iResultTimeOut)
			)
		{
			VFR_WRITE_LOG("Currentflag index = %d, carID = %lu, bSendFront = %d, bSendTail = %d",
				Currentflag.index,
				Currentflag.dwCarID,
				Currentflag.bSendFront,
				Currentflag.bSendTail);

			if (dwTimeDif > iResultTimeOut)
			{
				VFR_WRITE_LOG("result timeout , receive time = %lu, current time = %lu.", dwReceiveTime, dwCurrentTime);
			}

			VFR_WRITE_LOG("Currentflag.index = %d, index = -1 or have send all data, remove first result ,carID = %lu, plateNO = %s ",
				Currentflag.index, 
				pResult->dwCarID,
				pResult->chPlateNO);
			m_VfrResultList.DeleteToPosition(0);
			VFR_WRITE_LOG("remove finish.");
		}
		pResult = nullptr;
	}
	WriteFormatLog("SendResultThreadFunc finish.");

	return 0;
}

void Camera6467_VFR::SendFrontResultByCallback(std::shared_ptr<CameraResult> pResult, int index)
{
	if (CheckFrontResultCallback())
	{
		VFR_WRITE_LOG("Send Front ResultByCallback process begin.");

		T_VLPFRONTINFO vlpFrontInfo;
		memset(&vlpFrontInfo, '\0', sizeof(vlpFrontInfo));
		vlpFrontInfo.vlpInfoSize = sizeof(vlpFrontInfo);

		//车头车牌信息
		vlpFrontInfo.vehNo = index;

		char chPlateColor[3] = { 0 };
		sprintf(chPlateColor, "%02d", pResult->iPlateColor);
		//vlpFrontInfo.vlpColor[0] = 0;	//颜色
		//vlpFrontInfo.vlpColor[1] = pResult->iPlateColor;
		memcpy(vlpFrontInfo.vlpColor, chPlateColor, 2);

		char chPlateNO[64] = { 0 };//车牌号码
		Tool_ProcessPlateNo(pResult->chPlateNO, chPlateNO, sizeof(chPlateNO));
		memcpy(vlpFrontInfo.vlpText, chPlateNO, strlen(chPlateNO));
		memcpy(vlpFrontInfo.vlpTime, pResult->chPlateTime, strlen(pResult->chPlateTime));//车头识别时间
		vlpFrontInfo.vlpReliability = (int(pResult->fConfidenceLevel * 10000)) % 10000;
		VFR_WRITE_LOG("process front plate info finish.");

		const char* pchImgRootPath = "idevlp";
		//图片信息
		char* pImgPath = (char*)vlpFrontInfo.imageFile[0];
		char* pSrcImgPath = NULL;
		int iPathLen = 128;
		//车头图
		SaveImgStructFunc(&pResult->CIMG_BeginCapture, type_frontImg, index, pchImgRootPath);
		pSrcImgPath = pResult->CIMG_BeginCapture.chSavePath;
		Tool_CopyStringToBuffer(pImgPath, iPathLen, pSrcImgPath);
		VFR_WRITE_LOG("process type_frontImg info finish.");
		
		BeginFontAdd(pResult, index, pchImgRootPath);     //车头图片字符叠加
		
		//车头车牌图
		SaveImgStructFunc(&pResult->CIMG_PlateImage, type_frontPlate, index, pchImgRootPath);
		pImgPath = (char*)vlpFrontInfo.imageFile[1];
		pSrcImgPath = pResult->CIMG_PlateImage.chSavePath;
		Tool_CopyStringToBuffer(pImgPath, iPathLen, pSrcImgPath);
		VFR_WRITE_LOG("process type_frontPlate info finish.");

		//车头二值化图
		SaveImgStructFunc(&pResult->CIMG_BinImage, type_frontBin, index, pchImgRootPath);
		pImgPath = (char*)vlpFrontInfo.imageFile[2];
		pSrcImgPath = pResult->CIMG_BinImage.chSavePath;
		Tool_CopyStringToBuffer(pImgPath, iPathLen, pSrcImgPath);
		VFR_WRITE_LOG("process type_frontBin info finish.");

		VFR_WRITE_LOG("send front callback data begin,index = %d, carID = %lu, plateNO = %s, plateTime = %s",
			index, 
			pResult->dwCarID, 
			pResult->chPlateNO,
			pResult->chPlateTime);
		int iLoginID = GetLoginID();
		EnterCriticalSection(&m_csFuncCallback);
		((CBFun_GetFrontResult)g_pFuncFrontResultCallback)(iLoginID, &vlpFrontInfo, g_pFrontResultUserData);
		LeaveCriticalSection(&m_csFuncCallback);
		VFR_WRITE_LOG("send front callback data finish.");
	}
	else
	{
		VFR_WRITE_LOG("front callback == null.");
	}
}

void Camera6467_VFR::SendTailResultByCallback(std::shared_ptr<CameraResult> pResult, int index)
{
	if (CheckTailResultcallback())
	{
		VFR_WRITE_LOG("Send Tail ResultByCallback process begin.");

		T_VLPBACKINFO vlpTailInfo;
		memset(&vlpTailInfo, '\0', sizeof(vlpTailInfo));
		vlpTailInfo.vlpInfoSize = sizeof(vlpTailInfo);

		const char* pchImgRootPath = "idevlp";
		//图片信息
		char* pImgPath = NULL;
		char* pSrcImgPath = NULL;
		int iPathLen = 128;

		//车尾车牌信息
		vlpTailInfo.vehNo = index;
		char chPlateNO[64] = { 0 };//车牌号码

		//vlpTailInfo.vlpBackColor[0] = 0;	//颜色
		//vlpTailInfo.vlpBackColor[1] = pResult->iTailPlateColor;

		char chPlateColor[3] = { 0 };
		sprintf(chPlateColor, "%02d", pResult->iTailPlateColor);
		memcpy(vlpTailInfo.vlpBackColor, chPlateColor, 2);

		memset(chPlateNO, '\0', sizeof(chPlateNO));
		Tool_ProcessPlateNo(pResult->chTailPlateNO, chPlateNO, sizeof(chPlateNO));
		memcpy(vlpTailInfo.vlpBackText, chPlateNO, strlen(chPlateNO));
		memcpy(vlpTailInfo.vlpBackTime, pResult->chPlateTime, strlen(pResult->chPlateTime));
		VFR_WRITE_LOG("process tail plate info finish.");

		//车型信息
		vlpTailInfo.vlpCarClass = pResult->iVehTypeNo;
		vlpTailInfo.vehLength = (int)(pResult->fVehLenth);
		vlpTailInfo.vehWidth = (int)(pResult->fVehWidth);
		vlpTailInfo.vehHigh = (int)(pResult->fVehHeight);
		vlpTailInfo.vehAxis = pResult->iAxletreeCount;
		vlpTailInfo.vlpReliability = (int(pResult->fConfidenceLevel * 10000)) % 10000;

		//车身图
		SaveImgStructFunc(&pResult->CIMG_BestCapture, type_SideImg, index, pchImgRootPath);
		pImgPath = (char*)vlpTailInfo.imageFile[0];
		pSrcImgPath = pResult->CIMG_BestCapture.chSavePath;
		Tool_CopyStringToBuffer(pImgPath, iPathLen, pSrcImgPath);

		FontAdd(pResult, index, pchImgRootPath);//路径问题
		
		VFR_WRITE_LOG("process type_SideImg info finish.");
		
		//车尾图
		SaveImgStructFunc(&pResult->CIMG_LastCapture, type_tailImg, index, pchImgRootPath);
		pImgPath = (char*)vlpTailInfo.imageFile[1];
		pSrcImgPath = pResult->CIMG_LastCapture.chSavePath;
		Tool_CopyStringToBuffer(pImgPath, iPathLen, pSrcImgPath);
		VFR_WRITE_LOG("process type_tailImg info finish.");
	
		LastFontAdd(pResult, index, pchImgRootPath);
		
		//车尾车牌图
		SaveImgStructFunc(&pResult->CIMG_BestSnapshot, type_tailPlate, index, pchImgRootPath);
		pImgPath = (char*)vlpTailInfo.imageFile[2];
		pSrcImgPath = pResult->CIMG_BestSnapshot.chSavePath;
		Tool_CopyStringToBuffer(pImgPath, iPathLen, pSrcImgPath);
		VFR_WRITE_LOG("process type_tailPlate info finish.");

		//车尾车牌二值化图
		//SaveImgFunc(&pResult->CIMG_LastCapture, type_taileBin, iIndex, imgPath);
		pImgPath = (char*)vlpTailInfo.imageFile[3];
		vlpTailInfo.imageFile[3][0] = NULL;

		//五秒视频
		vlpTailInfo.imageFile[4][0] = NULL;
		pImgPath = (char*)vlpTailInfo.imageFile[4];
		if (FindIfFileNameInReciveList(pResult->chSaveFileName))
		{
			char chVideoName[256] = { 0 };
			sprintf_s(chVideoName, sizeof(chVideoName), ".\\%s\\vlp_7_%02d.mp4", pchImgRootPath, index);
			VFR_WRITE_LOG("copy and remove video file %s.", pResult->chSaveFileName);
			if (CopyFile(pResult->chSaveFileName, chVideoName, FALSE))
			{
				remove(pResult->chSaveFileName);
				sprintf_s(pImgPath, iPathLen, "%s", chVideoName);
			}
			else
			{
				VFR_WRITE_LOG("video file %s remove failed.", pResult->chSaveFileName);
			}
		}
		else
		{
			VFR_WRITE_LOG("video file %s is not ready.", pResult->chSaveFileName);
		}
		int iLoginID = GetLoginID();

		VFR_WRITE_LOG("send tail callback data begin,index = %d, carID = %lu, plateNO = %s, plateTime = %s",
			index,
			pResult->dwCarID,
			pResult->chPlateNO,
			pResult->chPlateTime);

		EnterCriticalSection(&m_csFuncCallback);
		((CBFun_GetBackResult)g_pFuncTailResultCallback)(iLoginID, &vlpTailInfo, g_pTailResultUserData);
		LeaveCriticalSection(&m_csFuncCallback);
		VFR_WRITE_LOG("send tail callback data finish.");
	}
	else
	{
		VFR_WRITE_LOG("tail callback func == NULL.");
	}
}
void Camera6467_VFR::BeginFontAdd(std::shared_ptr<CameraResult> pResult, int index, const char* imgPath)  //车头字符叠加功能函数
{
	CxImage imageMix(pResult->CIMG_BeginCapture.pbImgData, pResult->CIMG_BeginCapture.dwImgSize, CXIMAGE_FORMAT_JPG);
	CxImage::CXTEXTINFO Plate, Time;//图片叠加文字
		if (imageMix.IsEnabled())
		{
			//imageMix.Save("before.jpg", CXIMAGE_FORMAT_JPG);   //用于test

			//初始化叠加文字结构体
			imageMix.InitTextInfo(&Plate);
			imageMix.InitTextInfo(&Time);

			//设置文字的属性
			_stprintf(Plate.lfont.lfFaceName, _T("微软雅黑"));//字体
			Plate.lfont.lfCharSet = GB2312_CHARSET;//字符集
			Plate.lfont.lfHeight = m_iFontSize;//根据ini配置字体高度大小
			Plate.fcolor = RGB(m_iFontAddcolorR, m_iFontAddcolorG, m_iFontAddcolorB);  //根据ini配置颜色
			Plate.opaque = 0;  //背景

			_stprintf(Time.lfont.lfFaceName, _T("微软雅黑"));
			Time.lfont.lfCharSet = GB2312_CHARSET;
			Time.lfont.lfHeight = m_iFontSize;
			Time.fcolor = RGB(m_iFontAddcolorR, m_iFontAddcolorG, m_iFontAddcolorB);
			Time.opaque = 0;

			std::string strPlate = pResult->chPlateNO;
			std::string strTime = pResult->chPlateTime;

			sprintf_s(Plate.text, "%s", strPlate.c_str());
			sprintf_s(Time.text, "%s", strTime.c_str());

			//开始将文字叠加到图片
			//判断叠加车牌信息
			if (m_addplate == 1){
				imageMix.DrawStringEx(0, 10, 100, &Plate);
			}
			//判断叠加时间信息
			if (m_addtime == 1){
				imageMix.DrawStringEx(0, 10, 200, &Time);
			}
			
			long size = 0;
			BYTE* buffer = 0;
			imageMix.Encode(buffer, size, CXIMAGE_FORMAT_JPG);

			CameraIMG img;
			img.pbImgData = new unsigned char[size];
			memcpy(img.pbImgData, buffer, size);
			img.dwImgSize = size;

			const char* pchImgRootPath = imgPath;   //保存结果的指定路径
			imageMix.FreeMemory(buffer);
			CameraIMG *p;
			p = &img;
			SaveImgStructFunc(p, type_frontImg, index, pchImgRootPath);    //保存车头图
		}
}
void Camera6467_VFR::FontAdd(std::shared_ptr<CameraResult> pResult, int index, const char* imgPath)  //车身字符叠加功能函数
{
	CxImage imageMix(pResult->CIMG_BestCapture.pbImgData, pResult->CIMG_BestCapture.dwImgSize, CXIMAGE_FORMAT_JPG);
	CxImage::CXTEXTINFO Plate, Time;//图片叠加文字
		if (imageMix.IsEnabled())
		{
			//imageMix.Save("before.jpg", CXIMAGE_FORMAT_JPG);   //用于test

			//初始化叠加文字结构体
			imageMix.InitTextInfo(&Plate);
			imageMix.InitTextInfo(&Time);

			//设置文字的属性
			_stprintf(Plate.lfont.lfFaceName, _T("微软雅黑"));//字体
			Plate.lfont.lfCharSet = GB2312_CHARSET;//字符集
			Plate.lfont.lfHeight = m_iFontSize;//根据ini配置字体高度大小
			Plate.fcolor = RGB(m_iFontAddcolorR, m_iFontAddcolorG, m_iFontAddcolorB);  //根据ini配置颜色
			Plate.opaque = 0;  //背景

			_stprintf(Time.lfont.lfFaceName, _T("微软雅黑"));
			Time.lfont.lfCharSet = GB2312_CHARSET;
			Time.lfont.lfHeight = m_iFontSize;
			Time.fcolor = RGB(m_iFontAddcolorR, m_iFontAddcolorG, m_iFontAddcolorB);
			Time.opaque = 0;

			std::string strPlate = pResult->chPlateNO;
			std::string strTime = pResult->chPlateTime;

			sprintf_s(Plate.text, "%s", strPlate.c_str());
			sprintf_s(Time.text, "%s", strTime.c_str());
			//判断叠加车牌信息
			if (m_addplate == 1){
				imageMix.DrawStringEx(0, 10, 100, &Plate);
			}
			//判断叠加时间信息
			if (m_addtime == 1){
				imageMix.DrawStringEx(0, 10, 200, &Time);
			}

			long size = 0;
			BYTE* buffer = 0;
			imageMix.Encode(buffer, size, CXIMAGE_FORMAT_JPG);

			CameraIMG img;
			img.pbImgData = new unsigned char[size];
			memcpy(img.pbImgData, buffer, size);
			img.dwImgSize = size;

			const char* pchImgRootPath = imgPath;   //保存结果的指定路径
			imageMix.FreeMemory(buffer);
			CameraIMG *p;
			p = &img;
			SaveImgStructFunc(p, type_SideImg, index, pchImgRootPath);    //保存车身图
		}
	
}
void Camera6467_VFR::LastFontAdd(std::shared_ptr<CameraResult> pResult, int index, const char* imgPath)  //车尾字符叠加功能函数
{

	CxImage imageMix(pResult->CIMG_LastCapture.pbImgData, pResult->CIMG_LastCapture.dwImgSize, CXIMAGE_FORMAT_JPG);
	CxImage::CXTEXTINFO Plate, Time;//图片叠加文字
		if (imageMix.IsEnabled())
		{
			//imageMix.Save("before.jpg", CXIMAGE_FORMAT_JPG);   //用于test

			//初始化叠加文字结构体
			imageMix.InitTextInfo(&Plate);
			imageMix.InitTextInfo(&Time);

			//设置文字的属性
			_stprintf(Plate.lfont.lfFaceName, _T("微软雅黑"));//字体
			Plate.lfont.lfCharSet = GB2312_CHARSET;//字符集
			Plate.lfont.lfHeight = m_iFontSize;//根据ini配置字体高度大小
			Plate.fcolor = RGB(m_iFontAddcolorR, m_iFontAddcolorG, m_iFontAddcolorB);  //根据ini配置颜色
			Plate.opaque = 0;  //背景

			_stprintf(Time.lfont.lfFaceName, _T("微软雅黑"));
			Time.lfont.lfCharSet = GB2312_CHARSET;
			Time.lfont.lfHeight = m_iFontSize;
			Time.fcolor = RGB(m_iFontAddcolorR, m_iFontAddcolorG, m_iFontAddcolorB);
			Time.opaque = 0;

			std::string strPlate = pResult->chPlateNO;
			std::string strTime = pResult->chPlateTime;

			sprintf_s(Plate.text, "%s", strPlate.c_str());
			sprintf_s(Time.text, "%s", strTime.c_str());

			//开始将文字叠加到图片
			//判断叠加车牌信息
			if (m_addplate == 1){
				imageMix.DrawStringEx(0, 10, 100, &Plate);
			}
			//判断叠加时间信息
			if (m_addtime == 1){
				imageMix.DrawStringEx(0, 10, 200, &Time);
			}
			long size = 0;
			BYTE* buffer = 0;
			imageMix.Encode(buffer, size, CXIMAGE_FORMAT_JPG);

			CameraIMG img;
			img.pbImgData = new unsigned char[size];
			memcpy(img.pbImgData, buffer, size);
			img.dwImgSize = size;

			const char* pchImgRootPath = imgPath;   //保存结果的指定路径
			imageMix.FreeMemory(buffer);
			CameraIMG *p;
			p = &img;
			SaveImgStructFunc(p, type_tailImg, index, pchImgRootPath);    //保存车尾图
		}
}


void Camera6467_VFR::copyStringToBuffer(char* bufer, size_t bufLen, const char * srcStr)
{
    if (NULL == bufer
        || bufLen == 0
        || NULL == srcStr)
    {
        return;
    }
    memset(bufer, '\0', bufLen);
    if (bufLen > strlen(srcStr))
    {
        memcpy(bufer, srcStr, strlen(srcStr));
    }
    else
    {
        memcpy(bufer, srcStr, bufLen-1);
    }
    return;
}

bool Camera6467_VFR::SaveImgStructFunc(CameraIMG* pImg, int imgType, int iIndex, const char* imgPath)
{
	if (pImg == NULL
		|| pImg->dwImgSize <= 0
		|| NULL == pImg->pbImgData)
	{
		return false;
	}
	char chFileName[128] = { 0 };
	switch (imgType)
	{
		case type_frontImg:
			sprintf_s(chFileName, sizeof(chFileName), ".\\%s\\vlp_0_%02d", imgPath, iIndex);
			break;
		case type_SideImg:
			sprintf_s(chFileName, sizeof(chFileName), ".\\%s\\vlp_3_%02d", imgPath, iIndex);
			break;
		case type_tailImg:
			sprintf_s(chFileName, sizeof(chFileName), ".\\%s\\vlp_4_%02d", imgPath, iIndex);
			break;
		case type_frontPlate:
			sprintf_s(chFileName, sizeof(chFileName), ".\\%s\\vlp_1_%02d", imgPath, iIndex);
			break;
		case type_frontBin:
			sprintf_s(chFileName, sizeof(chFileName), ".\\%s\\vlp_2_%02d", imgPath, iIndex);
			break;
		case type_tailPlate:
			sprintf_s(chFileName, sizeof(chFileName), ".\\%s\\vlp_5_%02d", imgPath, iIndex);
			break;
		case type_taileBin:
			sprintf_s(chFileName, sizeof(chFileName), ".\\%s\\vlp_6_%02d", imgPath, iIndex);
			break;
		default:
			break;
	}
	strcat(chFileName, ".jpg");
	if (strlen(chFileName) > 0
		&& Tool_SaveFileToPath(chFileName, pImg->pbImgData, pImg->dwImgSize))
	{
		memcpy((void*)pImg->chSavePath, chFileName, strlen(chFileName));
		return true;
	}
	return false;
}

unsigned int Camera6467_VFR::DeleteLogThreadFunc()
{
    char chDirPath[256] = {0};
    int iTryTime = 3600;
    while (!GetCheckThreadExit())
    {
        Sleep(100);
        if (iTryTime ++ < 10*10)
        {
            continue;
        }
        iTryTime = 0;
        if (GetLogPath(chDirPath, sizeof(chDirPath))
            && strlen(chDirPath) > 0)
        {
            std::string strLogDir(chDirPath);
            strLogDir.append("\\").append(LOG_DIR_NAME).append("\\");
            Tool_LoopDeleteSpecificFormatDirectory(strLogDir.c_str(), GetLogHoldDays());
        }       
    }
    return 0;
}

unsigned int Camera6467_VFR::DeleteResultThreadFunc()
{
    char chDirPath[256] = { 0 };
    int iTryTime = 3600;
    while (!GetCheckThreadExit())
    {
        Sleep(100);
        if (GetResultHoldDay() <= 0)
        {
            continue;
        }
        if (iTryTime++ < 3600 * 1 )
        {
            continue;
        }

        iTryTime = 0;
        GetImageDir(chDirPath, sizeof(chDirPath));
        if (strlen(chDirPath) > 0)
        {
            std::string strResultDir(chDirPath);
            strResultDir.append("\\").append(RESULT_DIR_NAME).append("\\");
            Tool_LoopDeleteSpecificFormatDirectory(strResultDir.c_str(), GetResultHoldDay());
        }
    }
    return 0;
}

void Camera6467_VFR::UpdateSentStatus(CameraResult* pResult)
{
    
    int iLogginID = GetLoginID();
    int iItem = 0;
    long iValue = sta_undefine;
    DWORD dwCarID = pResult->dwCarID;
    CameraIMG* pImg = &(pResult->CIMG_BeginCapture);
    if (m_lsStatusList.GetRsultStatus(iLogginID, dwCarID, item_receiveTime, iValue))
    {
        auto funcUpdate = [](int iLogginID, int dwCarID, int iItem, long & iValue, ResultSentStatusManager& manager, CameraIMG* pImgStruct)
        {
            if (pImgStruct->dwImgSize > 0
                && manager.GetRsultStatus(iLogginID, dwCarID, iItem, iValue)
                && iValue == sta_undefine)
            {
                manager.UpdateStatus(iLogginID, dwCarID, iItem, sta_receiveDone);
            }
        };

        iItem = item_frontImg;
        iValue = sta_undefine;
        pImg = &(pResult->CIMG_BeginCapture);
        funcUpdate(iLogginID, dwCarID, iItem, iValue, m_lsStatusList, pImg);

        iItem = item_sideImg;
        iValue = sta_undefine;
        pImg = &(pResult->CIMG_BestCapture);
        funcUpdate(iLogginID, dwCarID, iItem, iValue, m_lsStatusList, pImg);

        iItem = item_tailImg;
        iValue = sta_undefine;
        pImg = &(pResult->CIMG_LastCapture);
        funcUpdate(iLogginID, dwCarID, iItem, iValue, m_lsStatusList, pImg);

        iItem = item_smallImg;
        iValue = sta_undefine;
        pImg = &(pResult->CIMG_PlateImage);
        funcUpdate(iLogginID, dwCarID, iItem, iValue, m_lsStatusList, pImg);

        iItem = item_binImg;
        iValue = sta_undefine;
        pImg = &(pResult->CIMG_BinImage);
        funcUpdate(iLogginID, dwCarID, iItem, iValue, m_lsStatusList, pImg);

        iItem = item_video;
        iValue = sta_undefine;
        if (strlen(pResult->chSaveFileName) > 0
            && m_lsStatusList.GetRsultStatus(iLogginID, dwCarID, iItem, iValue)
            && iValue == sta_undefine
            && FindIfFileNameInReciveList(pResult->chSaveFileName))
        {
            m_lsStatusList.UpdateStatus(iLogginID, dwCarID, iItem, sta_receiveDone);
        }
    }
    else
    {
        if (pResult->CIMG_BeginCapture.dwImgSize > 0)
        {
            m_lsStatusList.UpdateStatus(iLogginID, dwCarID, item_frontImg, sta_receiveDone);
        }

        if (pResult->CIMG_BestCapture.dwImgSize > 0)
        {
            m_lsStatusList.UpdateStatus(iLogginID, dwCarID, item_sideImg, sta_receiveDone);
        }

        if (pResult->CIMG_LastCapture.dwImgSize > 0)
        {
            m_lsStatusList.UpdateStatus(iLogginID, dwCarID, item_tailImg, sta_receiveDone);
        }

        if (pResult->CIMG_PlateImage.dwImgSize > 0)
        {
            m_lsStatusList.UpdateStatus(iLogginID, dwCarID, item_smallImg, sta_receiveDone);
        }

        if (pResult->CIMG_BinImage.dwImgSize > 0)
        {
            m_lsStatusList.UpdateStatus(iLogginID, dwCarID, item_binImg, sta_receiveDone);
        }
        if (strlen(pResult->chSaveFileName) > 0
            && FindIfFileNameInReciveList(pResult->chSaveFileName))
        {
            m_lsStatusList.UpdateStatus(iLogginID, dwCarID, item_video, sta_receiveDone);
        }
    }
}

bool Camera6467_VFR::GetIfSendResult()
{
    bool bValue = false;
    EnterCriticalSection(&m_csFuncCallback);
    bValue = m_bSentResult;
    LeaveCriticalSection(&m_csFuncCallback);
    return bValue;
}

void Camera6467_VFR::SetIfSendResult(bool bValue)
{
    EnterCriticalSection(&m_csFuncCallback);
    m_bSentResult = bValue;
    LeaveCriticalSection(&m_csFuncCallback);
}

bool Camera6467_VFR::GetFrontSendStatus(ResultSentStatus& status) //获取状态
{
    if (m_lsStatusList.size() > 0)
    {
        return m_lsStatusList.getFirstElement(status);
    }
    return false;
}

void Camera6467_VFR::UpdateSendStatus(int iLogginID, unsigned long dwCarID, int iItem, long value)
{
    m_lsStatusList.UpdateStatus(iLogginID, dwCarID, iItem, value);
}

int Camera6467_VFR::GetResultMode()
{
    return m_iResultModule;
}

