#include "stdafx.h"
#include "Camera6467_plate.h"
#include "HvDevice/HvDeviceBaseType.h"
#include "HvDevice/HvDeviceCommDef.h"
#include "HvDevice/HvDeviceNew.h"
#include "HvDevice/HvCamera.h"
#include "utilityTool/ToolFunction.h"
#include "utilityTool/log4z.h"
//#include"vpr_commondef_yn.h"
#include <process.h>
#include<shellapi.h>
#include <Dbghelp.h>
//#include <QDebug>
//#include<QDir>



#define CHECK_ARG(arg)\
if (arg == NULL) \
{\
    WriteFormatLog("%s is NULL", #arg); \
    return 0; \
}

unsigned int __stdcall Camera_SaveResultThread(LPVOID lpParam);

Camera6467_plate::Camera6467_plate() :
BaseCamera(),
m_iTimeInvl(3),
m_iCompressBigImgSize(COMPRESS_BIG_IMG_SIZE),
m_iCompressSamllImgSize(COMPRESS_PLATE_IMG_SIZE),
g_pUser(NULL),
g_func_ReconnectCallback(NULL),
g_ConnectStatusCallback(NULL),
g_func_DisconnectCallback(NULL),
g_func_ResultCallback(NULL),
m_CameraResult(NULL),
m_BufferResult(NULL),
m_bResultComplete(false),
m_bJpegComplete(false),
m_bSaveToBuffer(false),
m_bOverlay(false),
m_bCompress(false),
m_bStatusCheckThreadExit(false),
m_hStatusCheckThread(NULL),
m_pSideCamera(NULL),
m_pTailCamera(NULL)
{
    InitializeCriticalSection(&m_csResult);
    memset(m_chResultPath, '\0', sizeof(m_chResultPath));
    ReadConfig();
    
    m_h264Saver.initMode(0);

    m_hStatusCheckThread = (HANDLE)_beginthreadex(NULL, 0, Camera_StatusCheckThread, this, 0, NULL);
}


Camera6467_plate::Camera6467_plate(const char* chIP, HWND hWnd, int Msg) :
BaseCamera(chIP, hWnd, Msg),
m_iTimeInvl(3),
m_iCompressBigImgSize(COMPRESS_BIG_IMG_SIZE),
m_iCompressSamllImgSize(COMPRESS_PLATE_IMG_SIZE),
g_pUser(NULL),
g_func_ReconnectCallback(NULL),
g_ConnectStatusCallback(NULL),
g_func_DisconnectCallback(NULL),
g_func_ResultCallback(NULL),
m_CameraResult(NULL),
m_BufferResult(NULL),
m_bResultComplete(false),
m_bJpegComplete(false),
m_bSaveToBuffer(false),
m_bOverlay(false),
m_bCompress(false),
m_bStatusCheckThreadExit(false),
m_hStatusCheckThread(NULL),
  m_pSideCamera(NULL),
  m_pTailCamera(NULL)
{
    InitializeCriticalSection(&m_csResult);
    memset(m_chResultPath, '\0', sizeof(m_chResultPath));
    ReadConfig();

    m_h264Saver.initMode(0);

    m_hStatusCheckThread = (HANDLE)_beginthreadex(NULL, 0, Camera_StatusCheckThread, this, 0, NULL);
}

Camera6467_plate::~Camera6467_plate()
{
    SetCheckThreadExit(true);
    SetConnectStatus_Callback(NULL, NULL, 10);
    Tool_SafeCloseThread(m_hStatusCheckThread);
    //Tool_SafeCloseThread(m_hSaveResultThread);

    InterruptionConnection();

    CloseSubCamera(type_SideCam);
    CloseSubCamera(type_TailCam);

    SAFE_DELETE_OBJ(m_CameraResult);
    SAFE_DELETE_OBJ(m_BufferResult);

    DeleteCriticalSection(&m_csResult);
}

void Camera6467_plate::AnalysisAppendXML(CameraResult* CamResult)
{
    if (NULL == CamResult || strlen(CamResult->pcAppendInfo) <= 0)
        return;
    if (0 != CamResult->dw64TimeMS)
    {
         INT64 iTimeNow = CamResult->dw64TimeMS / 1000;
         struct tm tmNow = *localtime((time_t *)&iTimeNow);
         sprintf_s(CamResult->chPlateTime, sizeof(CamResult->chPlateTime), "%04d%02d%02d%02d%02d%02d",
             tmNow.tm_year +1900,
             tmNow.tm_mon +1,
             tmNow.tm_mday,
             tmNow.tm_hour,
             tmNow.tm_min,
             tmNow.tm_sec);
         //CTime tm(CamResult->dw64TimeMS / 1000);
        //sprintf_s(record->chPlateTime, "%04d%02d%02d%02d%02d%02d%03d", tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond(), record->dw64TimeMS%1000);
//        sprintf_s(CamResult->chPlateTime, sizeof(CamResult->chPlateTime), "%04d-%02d-%02d %02d:%02d:%02d",
//            tm.GetYear(),
//            tm.GetMonth(),
//            tm.GetDay(),
//            tm.GetHour(),
//            tm.GetMinute(),
//            tm.GetSecond());
    }
    else
    {
        SYSTEMTIME st;
        GetLocalTime(&st);
        //sprintf_s(record->chPlateTime, "%04d%02d%02d%02d%02d%02d%03d", st.wYear, st.wMonth, st.wDay	,st.wHour, st.wMinute,st.wSecond, st.wMilliseconds);
        sprintf_s(CamResult->chPlateTime, sizeof(CamResult->chPlateTime), "%04d%02d%02d%02d%02d%02d",
            st.wYear,
            st.wMonth,
            st.wDay,
            st.wHour,
            st.wMinute,
            st.wSecond);
    }
    //qDebug()<<"chPlateTime = "<< CamResult->chPlateTime;

    char chTemp[BUFFERLENTH] = { 0 };
    int iLenth = BUFFERLENTH;

    //if (Tool_GetDataFromAppenedInfo(CamResult->pcAppendInfo, "VehicleType", chTemp, &iLenth))
    //{
    //    CamResult->iVehTypeNo = AnalysisVelchType(chTemp);
    //}
    //memset(chTemp, 0, sizeof(chTemp));
    //iLenth = BUFFERLENTH;
    //if (Tool_GetDataFromAppenedInfo(CamResult->pcAppendInfo, "AxleCnt", chTemp, &iLenth))
    //{
    //    int iAxleCount = 0;
    //    sscanf(chTemp, "%d", &iAxleCount);
    //    CamResult->iAxletreeCount = iAxleCount;
    //    //printf("the Axletree count is %d.\n", iAxleCount);
    //}
    //memset(chTemp, 0, sizeof(chTemp));
    //iLenth = BUFFERLENTH;
    //if (Tool_GetDataFromAppenedInfo(CamResult->pcAppendInfo, "Wheelbase", chTemp, &iLenth))
    //{
    //    float fWheelbase = 0;
    //    sscanf(chTemp, "%f", &fWheelbase);
    //    CamResult->fDistanceBetweenAxles = fWheelbase;
    //    //printf("the Wheelbase  is %f.\n", fWheelbase);
    //}
    //memset(chTemp, 0, sizeof(chTemp));
    //iLenth = BUFFERLENTH;
    //if (Tool_GetDataFromAppenedInfo(CamResult->pcAppendInfo, "CarLength", chTemp, &iLenth))
    //{
    //    float fCarLength = 0;
    //    sscanf(chTemp, "%f", &fCarLength);
    //    CamResult->fVehLenth = fCarLength;
    //    //printf("the CarLength  is %f.\n", fCarLength);
    //}
    //memset(chTemp, 0, sizeof(chTemp));
    //iLenth = BUFFERLENTH;
    //if (Tool_GetDataFromAppenedInfo(CamResult->pcAppendInfo, "CarHeight", chTemp, &iLenth))
    //{
    //    float fCarHeight = 0;
    //    sscanf(chTemp, "%f", &fCarHeight);
    //    CamResult->fVehHeight = fCarHeight;
    //    //printf("the CarHeight  is %f.\n", fCarHeight);
    //}
    //memset(chTemp, 0, sizeof(chTemp));
    //iLenth = BUFFERLENTH;
    //if (Tool_GetDataFromAppenedInfo(CamResult->pcAppendInfo, "BackUp", chTemp, &iLenth))
    //{
    //    CamResult->bBackUpVeh = true;
    //}
    //iLenth = BUFFERLENTH;
    //if (Tool_GetDataFromAppenedInfo(CamResult->pcAppendInfo, "Confidence", chTemp, &iLenth))
    //{
    //    float fConfidence = 0;
    //    sscanf(chTemp, "%f", &fConfidence);
    //    CamResult->fConfidenceLevel = fConfidence;
    //    //printf("the CarHeight  is %f.\n", fCarHeight);
    //}

    TiXmlElement element = Tool_SelectElementByName(CamResult->pcAppendInfo, "PlateName", 2);
    if (strlen(element.GetText()) > 0)
    {
        memset(CamResult->chPlateNO, 0, sizeof(CamResult->chPlateNO));
        strcpy_s(CamResult->chPlateNO, sizeof(CamResult->chPlateNO), element.GetText());

        memset(chTemp, 0, sizeof(chTemp));
        strcpy_s(chTemp, sizeof(chTemp), element.GetText());

        iLenth = strlen(chTemp);        
        CamResult->iPlateColor = Tool_AnalysisPlateColorNo(chTemp);        
    }
    else
    {
        sprintf_s(CamResult->chPlateNO, sizeof(CamResult->chPlateNO), "无车牌");
        CamResult->iPlateColor = COLOR_UNKNOW;
    }
    WriteFormatLog("find the plate number = %s, plate length = %d, plate color type = %d \n", CamResult->chPlateNO, iLenth, CamResult->iPlateColor);
}

int Camera6467_plate::AnalysisVelchType(const char* vehType)
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
    else
    {
        return UNKOWN_TYPE;
    }
}

void Camera6467_plate::ReadConfig()
{
    char iniFileName[MAX_PATH] = { 0 };
#ifdef GUANGXI_DLL
    sprintf_s(iniFileName, "..\\DevInterfaces\\HVCR_Signalway_V%d_%d\\HVCR_Config\\HVCR_Signalway_V%d_%d.ini", PROTOCAL_VERSION, DLL_VERSION, PROTOCAL_VERSION, DLL_VERSION);
#else
    //strcat_s(iniFileName, Tool_GetCurrentPath());
    strcat_s(iniFileName, Tool_GetDllDirPath());
    strcat_s(iniFileName, INI_FILE_NAME);
#endif
    //char szPath[MAX_PATH] = {0};
    //sprintf_s(szPath, sizeof(szPath), "%s\\Result\\", Tool_GetCurrentPath());
    //Tool_ReadKeyValueFromConfigFile(INI_FILE_NAME, "Result", "Path", szPath, MAX_PATH);
    //strcpy_s(m_chResultPath, sizeof(m_chResultPath), szPath);

    BaseCamera::ReadConfig();
}

void Camera6467_plate::SetCheckThreadExit(bool bExit)
{
    EnterCriticalSection(&m_csFuncCallback);
    m_bStatusCheckThreadExit = bExit;
    LeaveCriticalSection(&m_csFuncCallback);
}

bool Camera6467_plate::GetCheckThreadExit()
{
    bool bExit = false;
    EnterCriticalSection(&m_csFuncCallback);
    bExit = m_bStatusCheckThreadExit;
    LeaveCriticalSection(&m_csFuncCallback);
    return bExit;
}

bool Camera6467_plate::SetSubCamera(int Type, const char* camIP)
{
    if(NULL == camIP)
        return false;

    Camera6467_plate* pCamera = new Camera6467_plate();
    pCamera->SetCameraIP(camIP);
    pCamera->SetExtraDataSource(&m_lsResultList);
    pCamera->SetImageDir(m_chImageDir);

    switch (Type)
    {
    case type_SideCam:
        SAFE_DELETE_OBJ(m_pSideCamera);
        m_pSideCamera = pCamera;
        m_pSideCamera->SetLoginID(type_SideCam);
        m_pSideCamera->ConnectToCamera();
        m_pSideCamera->SetJpegStreamCallback();

        break;
    case type_TailCam:
        SAFE_DELETE_OBJ(m_pTailCamera);
        m_pTailCamera = pCamera;
        m_pTailCamera->SetLoginID(type_TailCam);
        m_pTailCamera->ConnectToCamera();
        break;
    default:
        return false;
        break;
    }

    return true;
}

void *Camera6467_plate::GetSubCamera(int Type)
{
    switch (Type)
    {
    case type_SideCam:
        return m_pSideCamera;
        break;
    case type_TailCam:
        return m_pTailCamera;
        break;
    default:
        return NULL;
        break;
    }
}

void Camera6467_plate::CloseSubCamera(int Type)
{
    switch (Type)
    {
    case type_SideCam:
        SAFE_DELETE_OBJ(m_pSideCamera);
        break;
    case type_TailCam:
        SAFE_DELETE_OBJ(m_pTailCamera);
        break;
    default:
        break;
    }
}

int Camera6467_plate::RecordInfoBegin(DWORD dwCarID)
{
    WriteFormatLog("RecordInfoBegin -begin- dwCarID = %lu", dwCarID);

    if (dwCarID == m_dwLastCarID)
    {
        WriteLog("相同carID,丢弃该结果");
        return 0;
    }
    SetResultComplete(false);
    SAFE_DELETE_OBJ(m_CameraResult);

    m_CameraResult = new CameraResult();
    if (m_CameraResult)
    {
        sprintf_s(m_CameraResult->chDeviceIp, sizeof(m_CameraResult->chDeviceIp), "%s", m_strIP.c_str());
        m_CameraResult->dwCarID = dwCarID;
    }
    //StartToSaveAviFile(0, )
    WriteFormatLog("RecordInfoBegin -end- dwCarID = %lu", dwCarID);
    return 0;
}

int Camera6467_plate::RecordInfoEnd(DWORD dwCarID)
{
    WriteFormatLog("RecordInfoEnd begin, dwCarID = %d", dwCarID);

    if (dwCarID != m_dwLastCarID)
    {
        m_dwLastCarID = dwCarID;
    }
    else
    {
        WriteLog("相同CarID, 丢弃该结果");
        return 0;
    }

    if (NULL == m_CameraResult)
    {
        return 0;
    }
    CopyResultToBuffer(m_CameraResult);

    SetResultComplete(true);

    //NotifyResultReady(dwCarID);
    if(m_hWnd != NULL)
    {
        WriteFormatLog("PostMessage winWnd = %p, msg = %d, laneID = %d", m_hWnd, m_iMsg, GetLoginID());
        ::PostMessage(m_hWnd, m_iMsg, GetLoginID(), 0);
    }    

//    if(strlen(m_chImageDir) > 0)
//    {
//        qDebug()<<"RecordInfoEnd::StopSaveAviFile";
//        StopSaveAviFile(0);
//        if(m_BufferResult != NULL
//                && m_BufferResult->CIMG_BestSnapshot.dwImgSize > 0)
//        {
//            m_BufferResult->CIMG_BeginCapture = m_BufferResult->CIMG_BestSnapshot;
//        }
//        if(m_pSideCamera  != NULL)
//        {
//            m_pSideCamera->GetOneJpegImg(m_BufferResult->CIMG_BestCapture);
//        }
//        if(m_pTailCamera != NULL)
//        {
//            m_pSideCamera->GetOneJpegImg(m_BufferResult->CIMG_LastCapture);
//        }
//        BaseCamera::SaveResult(m_BufferResult);
//        qDebug()<<"RecordInfoEnd::SaveResult finish";
//    }

//    char chSavePath[256] = {0};
//    std::string strPlateTime(m_BufferResult->chPlateTime);
//    std::shared_ptr<CameraResult> pTempResult;
//
//    switch (GetLoginID())
//    {
//    case type_FrontCam:
//        if(m_lsResultList.size() > 10)
//        {
//            m_lsResultList.pop_front();
//            WriteLog("Current size of result list is larger than 10 , abandon the first one.");
//        }
//        if(m_CameraResult)
//        {
//            m_lsResultList.push_back( std::shared_ptr<CameraResult>(m_CameraResult));
//            m_CameraResult = NULL;
//        }
//        SAFE_DELETE_OBJ(m_CameraResult);
//
//        if(m_BufferResult->CIMG_LastSnapshot.dwImgSize > 0)
//        {
//            memset(chSavePath, '\0', sizeof(chSavePath));
//            sprintf(chSavePath, "%s/%s/%s-1.jpg", m_chImageDir, strPlateTime.substr(0, 8).c_str(), m_BufferResult->chSaveFileName);
//            Tool_SaveFileToPath(chSavePath, m_BufferResult->CIMG_LastSnapshot.pbImgData, m_BufferResult->CIMG_LastSnapshot.dwImgSize);
//        }
//        if(m_pSideCamera != NULL
//                && m_pSideCamera->GetOneJpegImg(m_BufferResult->CIMG_BestCapture))
//        {
//            memset(chSavePath, '\0', sizeof(chSavePath));
//            sprintf(chSavePath, "%s/%s/%s-2.jpg", m_chImageDir, strPlateTime.substr(0, 8).c_str(), m_BufferResult->chSaveFileName);
//            Tool_SaveFileToPath(chSavePath, m_BufferResult->CIMG_BestCapture.pbImgData, m_BufferResult->CIMG_BestCapture.dwImgSize);
//        }
//        //Sleep(1000);
//        StopSaveAviFile(0, GetTickCount() + getVideoDelayTime()*1000);
//        break;
//    case type_SideCam:
//        //qDebug()<< "type_SideCam thread id =" << GetCurrentThreadId();
////         pTempResult = GetResultFromExtraSourceByPlateNumber(m_BufferResult->chPlateNO);
////        if(pTempResult != nullptr)
////        {
////            memset(chSavePath, '\0', sizeof(chSavePath));
////            strPlateTime = std::string(pTempResult->chPlateNO);
////            sprintf(chSavePath, "%s/%s/%s-2.jpg", m_chImageDir, strPlateTime.substr(0, 8).c_str(), pTempResult->chSaveFileName);
////        }
////        else
////        {
////            memset(chSavePath, '\0', sizeof(chSavePath));
////            sprintf(chSavePath, "%s/%s/%s-2.jpg", m_chImageDir, strPlateTime.substr(0, 8).c_str(), m_BufferResult->chSaveFileName);
////        }
////        Tool_SaveFileToPath(chSavePath, m_BufferResult->CIMG_LastSnapshot.pbImgData, m_BufferResult->CIMG_LastSnapshot.dwImgSize);
//        break;
//    case type_TailCam:
//        pTempResult = GetResultFromExtraSourceByPlateNumber(m_BufferResult->chPlateNO);
//       if(pTempResult != nullptr)
//       {
//           memset(chSavePath, '\0', sizeof(chSavePath));
//           strPlateTime = std::string(pTempResult->chPlateNO);
//           sprintf(chSavePath, "%s/%s/%s-3.jpg", m_chImageDir, strPlateTime.substr(0, 8).c_str(), pTempResult->chSaveFileName);
//       }
//       else
//       {
//           memset(chSavePath, '\0', sizeof(chSavePath));
//           sprintf(chSavePath, "%s/%s/%s-3.jpg", m_chImageDir, strPlateTime.substr(0, 8).c_str(), m_BufferResult->chSaveFileName);
//       }
//       Tool_SaveFileToPath(chSavePath, m_BufferResult->CIMG_LastSnapshot.pbImgData, m_BufferResult->CIMG_LastSnapshot.dwImgSize);
//
//        break;
//    default:
//        break;
//    }

    WriteFormatLog("RecordInfoEnd end, dwCarID = %lu", dwCarID);
    return 0;
}

int Camera6467_plate::RecordInfoPlate(DWORD dwCarID,
    LPCSTR pcPlateNo,
    LPCSTR pcAppendInfo,
    DWORD dwRecordType,
    DWORD64 dw64TimeMS)
{
    SetResultComplete(false);
    CHECK_ARG(m_CameraResult);

    WriteFormatLog("RecordInfoPlate -begin- dwCarID = %lu, recordType = %lu", dwCarID, dwRecordType);

    if (dwCarID == m_dwLastCarID)
    {
        BaseCamera::WriteLog("相同carID,丢弃该结果");
        return 0;
    }
//#ifdef DEBUG
//    char chName[256] = { 0 };
//    sprintf_s(chName, "%lu.xml", GetTickCount());
//    Tool_SaveFileToPath(chName, (void*)pcAppendInfo, strlen(pcAppendInfo));
//#endif // DEBUG

    if (m_CameraResult->dwCarID == dwCarID)
    {
        m_CameraResult->dw64TimeMS = dw64TimeMS;
        sprintf_s(m_CameraResult->chPlateNO, sizeof(m_CameraResult->chPlateNO), "%s", pcPlateNo);
        //m_CameraResult->strAppendInfo = std::string(pcAppendInfo);
        if (strlen(pcAppendInfo) < sizeof(m_CameraResult->pcAppendInfo) )
        {
            memset(m_CameraResult->pcAppendInfo, '\0', sizeof(m_CameraResult->pcAppendInfo));
            memcpy(m_CameraResult->pcAppendInfo, pcAppendInfo, strlen(pcAppendInfo));
        }
        else
        {
            WriteFormatLog("RecordInfoPlate -begin- strlen(pcAppendInfo) %u < %u sizeof(m_CameraResult->pcAppendInfo)",
                strlen(pcAppendInfo),
                sizeof(m_CameraResult->pcAppendInfo));
            memset(m_CameraResult->pcAppendInfo, '\0', sizeof(m_CameraResult->pcAppendInfo));
        }

        BaseCamera::WriteLog(m_CameraResult->chPlateNO);  
        BaseCamera::WriteLog(pcAppendInfo);
        AnalysisAppendXML(m_CameraResult);
    }
    generateFileName(m_CameraResult);

    //if(GetLoginID() == type_FrontCam &&  strlen(m_chImageDir) > 0)
    //{
    //    std::string strPlateTime(m_CameraResult->chPlateTime);
    //    char chAviPath[256] = {0};
    //    strPlateTime = strPlateTime.substr(0, 8);
    //    sprintf(chAviPath,  "%s/%s/", m_chImageDir,strPlateTime.c_str());
    //    qDebug()<<chAviPath;
    //    //MakeSureDirectoryPathExists(chAviPath);
    //    //OutputDebugStringA(chAviPath);
    //    QDir dir(chAviPath);
    //    dir.mkpath(chAviPath);
    //    qDebug()<<"MakeSureDirectoryPathExists ="<<chAviPath;

    //    memset(chAviPath, '\0', sizeof(chAviPath));
    //    sprintf(chAviPath, "%s/%s/%s-9.avi", m_chImageDir, strPlateTime.c_str(), m_CameraResult->chSaveFileName);
    //    qDebug()<<"chAviPath ="<<chAviPath;
    //    StartToSaveAviFile(0, chAviPath, getVideoAdvanceTime()*1000);
    //}

    WriteFormatLog("RecordInfoPlate -end- dwCarID = %lu", dwCarID);
    return 0;
}

int Camera6467_plate::RecordInfoBigImage(DWORD dwCarID,
    WORD wImgType,
    WORD wWidth,
    WORD wHeight,
    PBYTE pbPicData,
    DWORD dwImgDataLen,
    DWORD dwRecordType,
    DWORD64 /*dw64TimeMS*/)
{
    SetResultComplete(false);

    CHECK_ARG(m_CameraResult);

    WriteFormatLog("RecordInfoBigImage -begin- dwCarID = %ld, dwRecordType = %#x， ImgType=%d, size = %ld",
        dwCarID, 
        dwRecordType, 
        wImgType,
        dwImgDataLen);

    if (dwCarID == m_dwLastCarID)
    {
        WriteLog("相同carID,丢弃该结果");
        return 0;
    }
    if (m_CameraResult->dwCarID == dwCarID)
    {
        if (wImgType == RECORD_BIGIMG_BEST_SNAPSHOT)
        {
            WriteLog("RecordInfoBigImage BEST_SNAPSHO  ");

            CopyDataToIMG(m_CameraResult->CIMG_BestSnapshot, pbPicData, wWidth, wHeight, dwImgDataLen, wImgType);
        }
        else if (wImgType == RECORD_BIGIMG_LAST_SNAPSHOT)
        {
            WriteLog("RecordInfoBigImage LAST_SNAPSHOT  ");

            CopyDataToIMG(m_CameraResult->CIMG_LastSnapshot, pbPicData, wWidth, wHeight, dwImgDataLen, wImgType);
        }
        else if (wImgType == RECORD_BIGIMG_BEGIN_CAPTURE)
        {
            WriteLog("RecordInfoBigImage BEGIN_CAPTURE  ");

            CopyDataToIMG(m_CameraResult->CIMG_BeginCapture, pbPicData, wWidth, wHeight, dwImgDataLen, wImgType);
        }
        else if (wImgType == RECORD_BIGIMG_BEST_CAPTURE)
        {
            WriteLog("RecordInfoBigImage BEST_CAPTURE  ");

            CopyDataToIMG(m_CameraResult->CIMG_BestCapture, pbPicData, wWidth, wHeight, dwImgDataLen, wImgType);
        }
        else if (wImgType == RECORD_BIGIMG_LAST_CAPTURE)
        {
            WriteLog("RecordInfoBigImage LAST_CAPTURE  ");

            CopyDataToIMG(m_CameraResult->CIMG_LastCapture, pbPicData, wWidth, wHeight, dwImgDataLen, wImgType);
        }
        else
        {
            WriteLog("RecordInfoBigImage other Image, put it to  LAST_CAPTURE .");
            CopyDataToIMG(m_CameraResult->CIMG_LastCapture, pbPicData, wWidth, wHeight, dwImgDataLen, wImgType);
        }
    }

    WriteFormatLog("RecordInfoBigImage -end- dwCarID = %lu", dwCarID);

    return 0;
}

int Camera6467_plate::RecordInfoSmallImage(DWORD dwCarID,
    WORD wWidth,
    WORD wHeight,
    PBYTE pbPicData,
    DWORD dwImgDataLen,
    DWORD dwRecordType,
    DWORD64 /*dw64TimeMS*/)
{
    SetResultComplete(false);
    if (NULL == m_CameraResult)
    {
        return -1;
    }
    WriteFormatLog( "RecordInfoSmallImage  -begin- dwCarID = %lu, imgLength = %lu, recordType = %lu",\
                    dwCarID, dwImgDataLen, dwRecordType);

    if (dwCarID == m_dwLastCarID)
    {
        WriteLog("相同carID,丢弃该结果");
        return 0;
    }

    int iBuffLen = 1024 * 1024;
    if (m_CameraResult->dwCarID == dwCarID)
    {
        if (NULL != m_CameraResult->CIMG_PlateImage.pbImgData)
        {
            delete[] m_CameraResult->CIMG_PlateImage.pbImgData;
            m_CameraResult->CIMG_PlateImage.pbImgData = NULL;
        }
        m_CameraResult->CIMG_PlateImage.pbImgData = new BYTE[iBuffLen];
        WriteLog("RecordInfoSmallImage 内存申请.");
        if (m_CameraResult->CIMG_PlateImage.pbImgData != NULL)
        {
            WriteLog("RecordInfoSmallImage 内存申请成功.");
            memset(m_CameraResult->CIMG_PlateImage.pbImgData, 0, iBuffLen);
            HRESULT Hr = HVAPIUTILS_SmallImageToBitmapEx(pbPicData,
                wWidth,
                wHeight,
                m_CameraResult->CIMG_PlateImage.pbImgData,
                &iBuffLen);
            if (Hr == S_OK)
            {
                m_CameraResult->CIMG_PlateImage.wImgWidth = wWidth;
                m_CameraResult->CIMG_PlateImage.wImgHeight = wHeight;
                m_CameraResult->CIMG_PlateImage.dwImgSize = iBuffLen;
                if (m_Small_IMG_Temp.pbImgData == NULL)
                {
                    m_Small_IMG_Temp.pbImgData = new BYTE[MAX_IMG_SIZE];
                    memset(m_Small_IMG_Temp.pbImgData, 0, MAX_IMG_SIZE);
                }
//                if (m_Small_IMG_Temp.pbImgData)
//                {
//                    size_t iDestLenth = MAX_IMG_SIZE;
//                    memset(m_Small_IMG_Temp.pbImgData, 0, MAX_IMG_SIZE);
//                    WriteLog("convert bmp to jpeg , begin .");
//                    bool bScale = Tool_Img_ScaleJpg(m_CameraResult->CIMG_PlateImage.pbImgData,
//                        m_CameraResult->CIMG_PlateImage.dwImgSize,
//                        m_Small_IMG_Temp.pbImgData,
//                        &iDestLenth,
//                        m_CameraResult->CIMG_PlateImage.wImgWidth,
//                        m_CameraResult->CIMG_PlateImage.wImgHeight,
//                        80);
//                    if (bScale)
//                    {
//                        WriteLog("convert bmp to jpeg success, begin copy.");
//                        memset(m_CameraResult->CIMG_PlateImage.pbImgData, 0, m_CameraResult->CIMG_PlateImage.dwImgSize);
//                        memcpy(m_CameraResult->CIMG_PlateImage.pbImgData, m_Small_IMG_Temp.pbImgData, iDestLenth);
//                        m_CameraResult->CIMG_PlateImage.dwImgSize = iDestLenth;
//                        WriteLog("convert bmp to jpeg success, finish copy.");
//                    }
//                    else
//                    {
//                        WriteLog("convert bmp to jpeg failed, use default.");
//                    }
//                }
            }
            else
            {
                WriteLog("HVAPIUTILS_SmallImageToBitmapEx 失败.");
            }
        }
        else
        {
            WriteLog("RecordInfoSmallImage 内存申请失败.");
        }
    }
    WriteFormatLog("RecordInfoSmallImage  -end- dwCarID = %lu", dwCarID);
    return 0;
}

int Camera6467_plate::RecordInfoBinaryImage(DWORD dwCarID,
    WORD wWidth,
    WORD wHeight,
    PBYTE pbPicData,
    DWORD dwImgDataLen,
    DWORD dwRecordType,
    DWORD64 /*dw64TimeMS*/)
{
    SetResultComplete(false);

    if (NULL == m_CameraResult)
    {
        return -1;
    }
    WriteFormatLog("RecordInfoBinaryImage -begin- dwCarID = %lu,  imgLength = %lu, recordType = %lu",
                    dwCarID, dwImgDataLen, dwRecordType);


    if (dwCarID == m_dwLastCarID)
    {
        WriteLog("相同carID,丢弃该结果");
        return 0;
    }
    //int iBufferlength = 1024 * 1024;
    //if (m_pTempBin == NULL)
    //{
    //    m_pTempBin = new BYTE[1024 * 1024];
    //    memset(m_pTempBin, 0x00, iBufferlength);
    //}
    //if (m_pTempBin)
    //{
    //    memset(m_pTempBin, 0x00, iBufferlength);

    //    HRESULT hRet = HVAPIUTILS_BinImageToBitmapEx(pbPicData, m_pTempBin, &iBufferlength);
    //    if (hRet == S_OK)
    //    {
    //        if (m_Bin_IMG_Temp.pbImgData == NULL)
    //        {
    //            m_Bin_IMG_Temp.pbImgData = new BYTE[MAX_IMG_SIZE];
    //            memset(m_Bin_IMG_Temp.pbImgData, 0x00, MAX_IMG_SIZE);
    //        }
    //        if (m_Bin_IMG_Temp.pbImgData)
    //        {
    //            DWORD iDestLenth = MAX_IMG_SIZE;
    //            memset(m_Bin_IMG_Temp.pbImgData, 0x00, MAX_IMG_SIZE);
    //            WriteLog("bin, convert bmp to jpeg , begin .");
    //            bool bScale = Tool_Img_ScaleJpg(m_pTempBin,
    //                iBufferlength,
    //                m_Bin_IMG_Temp.pbImgData,
    //                &iDestLenth,
    //                wWidth,
    //                wHeight,
    //                90);
    //            if (bScale)
    //            {
    //                WriteLog("bin, convert bmp to jpeg success, begin copy.");
    //                CopyDataToIMG(m_CameraResult->CIMG_BinImage, m_Bin_IMG_Temp.pbImgData, wWidth, wHeight, iDestLenth, 0);
    //                WriteLog("bin, convert bmp to jpeg success, finish copy.");
    //            }
    //            else
    //            {
    //                WriteLog("bin, convert bmp to jpeg failed, use default.");
    //            }
    //        }
    //        else
    //        {
    //            WriteLog("m_Bin_IMG_Temp  is null.");
    //        }
    //    }
    //    else
    //    {
    //        WriteLog("HVAPIUTILS_BinImageToBitmapEx, failed, use default.");
    //        CopyDataToIMG(m_CameraResult->CIMG_BinImage, pbPicData, wWidth, wHeight, dwImgDataLen, 0);
    //    }
    //}
    //else
    {
        //WriteLog("m_pTempBin is NULL ,  use default.");
        CopyDataToIMG(m_CameraResult->CIMG_BinImage, pbPicData, wWidth, wHeight, dwImgDataLen, 0);
    }
    WriteFormatLog("RecordInfoBinaryImage -end- dwCarID = %lu", dwCarID);
    return 0;
}

int Camera6467_plate::DeviceJPEGStream(PBYTE pbImageData,
    DWORD dwImageDataLen,
    DWORD /*dwImageType*/,
    LPCSTR /*szImageExtInfo*/)
{
    static int iCout = 0;
    if (iCout++ > 100)
    {
        WriteLog("receive one jpeg frame.");
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

void Camera6467_plate::SetDisConnectCallback(void* funcDisc, void* pUser)
{
    EnterCriticalSection(&m_csFuncCallback);
    g_func_DisconnectCallback = funcDisc;
    g_pUser = pUser;
    LeaveCriticalSection(&m_csFuncCallback);
}

void Camera6467_plate::SetReConnectCallback(void* funcReco, void* pUser)
{
    EnterCriticalSection(&m_csFuncCallback);
    g_func_ReconnectCallback = funcReco;
    g_pUser = pUser;
    LeaveCriticalSection(&m_csFuncCallback);
}

bool Camera6467_plate::GetOneJpegImg(CameraIMG &destImg)
{
    WriteLog("GetOneJpegImg::begin.");
    bool bRet = false;

    if (!destImg.pbImgData)
    {
        WriteLog("GetOneJpegImg:: allocate memory.");
        destImg.pbImgData = new unsigned char[MAX_IMG_SIZE];
        memset(destImg.pbImgData, 0, MAX_IMG_SIZE);
        WriteLog("GetOneJpegImg:: allocate memory success.");
    }

    EnterCriticalSection(&m_csResult);
    if (m_bJpegComplete)
    {
        if (destImg.pbImgData)
        {
            memset(destImg.pbImgData, 0, MAX_IMG_SIZE);
            memcpy(destImg.pbImgData, m_CIMG_StreamJPEG.pbImgData, m_CIMG_StreamJPEG.dwImgSize);

            destImg.dwImgSize = m_CIMG_StreamJPEG.dwImgSize;
            destImg.wImgHeight = m_CIMG_StreamJPEG.wImgHeight;
            destImg.wImgWidth = m_CIMG_StreamJPEG.wImgWidth;
            bRet = true;
            WriteLog("GetOneJpegImg success.");
            m_bJpegComplete = false;
        }
        else
        {
            WriteLog("GetOneJpegImg:: allocate memory failed.");
        }
    }
    else
    {
        WriteLog("GetOneJpegImg the image is not ready.");
    }
    LeaveCriticalSection(&m_csResult);
    WriteLog("GetOneJpegImg:: end.");

    return bRet;
}

void Camera6467_plate::SendConnetStateMsg(bool isConnect)
{
    //if (m_hWnd == NULL)
    //	return;

    if (isConnect)
    {
        //EnterCriticalSection(&m_csFuncCallback);
        //if (g_ConnectStatusCallback)
        //{
        //    LeaveCriticalSection(&m_csFuncCallback);
        //    //char chIP[32] = { 0 };
        //    //sprintf_s(chIP, "%s", m_strIP.c_str());
        //    //g_ConnectStatusCallback(m_iIndex, 0, g_pUser);
        //}
        //else
        //{
        //    LeaveCriticalSection(&m_csFuncCallback);
        //}
        if (m_hWnd)
        {
            EnterCriticalSection(&m_csFuncCallback);
            ::PostMessage(m_hWnd, m_iConnectMsg, 0, 0);
            LeaveCriticalSection(&m_csFuncCallback);
        }

    }
    else
    {
        //EnterCriticalSection(&m_csFuncCallback);
        //if (g_ConnectStatusCallback)
        //{
        //    LeaveCriticalSection(&m_csFuncCallback);
        //    char chIP[32] = { 0 };
        //    sprintf_s(chIP, "%s", m_strIP.c_str());
        //    //g_ConnectStatusCallback(m_iIndex, -100, g_pUser);
        //}
        //else
        //{
        //    LeaveCriticalSection(&m_csFuncCallback);
        //}
        if (m_hWnd)
        {
            EnterCriticalSection(&m_csFuncCallback);
            ::PostMessage(m_hWnd, m_iDisConMsg, 0, 0);
            LeaveCriticalSection(&m_csFuncCallback);
        }
    }
}

void Camera6467_plate::SetConnectStatus_Callback(void* func, void* pUser, int TimeInterval)
{
    EnterCriticalSection(&m_csFuncCallback);
    g_ConnectStatusCallback = func;
    g_pUser = pUser;
    m_iTimeInvl = TimeInterval;
    LeaveCriticalSection(&m_csFuncCallback);
}

void Camera6467_plate::SetResultCallbackFunc(void *func)
{
    EnterCriticalSection(&m_csFuncCallback);
    g_ConnectStatusCallback = func;
    LeaveCriticalSection(&m_csFuncCallback);
}

bool Camera6467_plate::GetResultComplete()
{
    bool bFinish = false;
    EnterCriticalSection(&m_csResult);
    bFinish = m_bResultComplete;
    LeaveCriticalSection(&m_csResult);
    return bFinish;
}

CameraResult* Camera6467_plate::GetOneResult()
{
    CameraResult* tempResult = NULL;
    //EnterCriticalSection(&m_csResult);
    //if (m_ResultList.size() > 0)
    //{
    //	tempResult = m_ResultList.front();
    //	m_ResultList.pop_front();
    //}
    //LeaveCriticalSection(&m_csResult);

    if (GetResultComplete())
    {
        EnterCriticalSection(&m_csResult);
        tempResult = new CameraResult(*m_BufferResult);
        LeaveCriticalSection(&m_csResult);
    }
    return tempResult;
}

CameraResult* Camera6467_plate::GetOneResultFromBuffer()
{
    CameraResult* tempResult = NULL;
    //EnterCriticalSection(&m_csResult);
    //if (m_ResultList.size() > 0)
    //{
    //	tempResult = m_ResultList.front();
    //	m_ResultList.pop_front();
    //}
    //LeaveCriticalSection(&m_csResult);

    if (GetResultComplete())
    {
        EnterCriticalSection(&m_csResult);
        tempResult = new CameraResult(*m_BufferResult);
        LeaveCriticalSection(&m_csResult);
    }
    return tempResult;
}

void Camera6467_plate::CopyResultToBuffer(CameraResult *pResult)
{
    if(pResult == NULL)
        return;

    EnterCriticalSection(&m_csResult);

    CameraResult *pTemp = new CameraResult(*pResult);
    if(pTemp != NULL)
    {
        SAFE_DELETE_OBJ(m_BufferResult);
        m_BufferResult = pTemp;
        pTemp = NULL;
    }
    LeaveCriticalSection(&m_csResult);
}

std::shared_ptr<CameraResult> Camera6467_plate::GetOneResultByCarId(DWORD dwCarID)
{
    std::shared_ptr<CameraResult> tempResult;
    if(m_lsResultList.empty())
    {
        return tempResult;
    }
    else
    {
        tempResult = m_lsResultList.GetOneByCarid(dwCarID);
    }

    return tempResult;
}

void Camera6467_plate::NotifyResultReady(DWORD index)
{
    WriteFormatLog("Notify Result Ready by callback function, index = %lu", index);
//    EnterCriticalSection(&m_csFuncCallback);
//    if(g_ConnectStatusCallback)
//    {
//        ((TNotifyFunc)g_ConnectStatusCallback)(index);
//        WriteFormatLog("Notify Result Ready by callback function, finish");
//    }
//    else
//    {
//        WriteFormatLog(" callback function is not set yet.");
//    }
//    LeaveCriticalSection(&m_csFuncCallback);
}

void Camera6467_plate::SaveResult()
{

}

void Camera6467_plate::SetExtraDataSource(void *pDataSource)
{
    EnterCriticalSection(&m_csFuncCallback);
    m_pExternalDataSource = pDataSource;
    LeaveCriticalSection(&m_csFuncCallback);
}

std::shared_ptr<CameraResult> Camera6467_plate::GetResultFromExtraSourceByPlateNumber(const char *pChPlateNumber)
{
    std::shared_ptr<CameraResult> pResult = nullptr;
    EnterCriticalSection(&m_csFuncCallback);
    if(pChPlateNumber == NULL)
    {
        LeaveCriticalSection(&m_csFuncCallback);
        return pResult;
    }
    ResultListManager* pResultListManager = (ResultListManager*)pChPlateNumber;
    pResult = pResultListManager->GetOneByPlateNumber(pChPlateNumber);
    LeaveCriticalSection(&m_csFuncCallback);

    return pResult;
}

void Camera6467_plate::SetResultComplete(bool bfinish)
{
    EnterCriticalSection(&m_csResult);
    m_bResultComplete = bfinish;
    LeaveCriticalSection(&m_csResult);
}

int Camera6467_plate::GetTimeInterval()
{
    int iTimeInterval = 1;
    EnterCriticalSection(&m_csFuncCallback);
    iTimeInterval = m_iTimeInvl;
    LeaveCriticalSection(&m_csFuncCallback);
    return iTimeInterval;
}

void Camera6467_plate::CheckStatus()
{
    int iLastStatus = -1;
    INT64 iLastTick = 0, iCurrentTick = 0;
    int iFirstConnctSuccess = -1;

    while (!GetCheckThreadExit())
    {
        Sleep(50);
        iCurrentTick = GetTickCount();
        int iTimeInterval = GetTimeInterval();
        if ((iCurrentTick - iLastTick) >= (iTimeInterval * 1000))
        {
            int iStatus = GetCamStatus();
            if (iStatus != iLastStatus)
            {
                if (iStatus == 0)
                {
                    //if (iStatus != iLastStatus)
                    //{
                    //	pThis->SendConnetStateMsg(true);
                    //}
                    //SendConnetStateMsg(true);
                    WriteLog("设备连接正常.");
                    iFirstConnctSuccess = 0;
                }
                else
                {
                    //SendConnetStateMsg(false);
                    WriteLog("设备连接失败, 尝试重连");

                    if (iFirstConnctSuccess == -1)
                    {
                        ConnectToCamera();
                    }
                }
            }
            iLastStatus = iStatus;

            iLastTick = iCurrentTick;
        }
    }
}

unsigned int __stdcall Camera_SaveResultThread(LPVOID lpParam)
{
    if (lpParam == NULL)
    {
        return 0;
    }
    LOGFMTD("Camera_SaveResultThread, begin");
    Camera6467_plate* pCamera = (Camera6467_plate*)lpParam;
    pCamera->SaveResult();   
    LOGFMTD("Camera_SaveResultThread, finish");
    return 0;
}