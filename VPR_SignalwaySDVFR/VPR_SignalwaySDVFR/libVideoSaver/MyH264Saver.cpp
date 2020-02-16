#include "stdafx.h"
#include "MyH264Saver.h"
#include "utilityTool/ToolFunction.h"

//#include <QDebug>

#include "libVideoSaver/libvideosaver.h"
#pragma comment(lib, "libVideoSaver/libVideoSaver.lib")

#include <algorithm>

//#include"utilityTool/easylogging++.h"
#include"utilityTool/log4z.h"
using namespace zsummer::log4z;

#define  WINDOWS

typedef void (*IMAGE_FILE_CALLBACK)(void* , const char*);

//#ifdef WINDOWS
//#define WRITE_LOG(fmt, ...) WriteFormatLog("%s:: "fmt, __FUNCTION__, ##__VA_ARGS__);
//#else
//#define WRITE_LOG(...) Tool_WriteFormatLog("%s:: ", __FUNCTION__, ##__VA_ARGS__);
#define WRITE_LOG(fmt,...) WriteFormatLog("%s:: " fmt, __FUNCTION__,##__VA_ARGS__);
//#endif

MyH264Saver::MyH264Saver() :
m_bExit(false),
m_iSaveH264Flag(0),
m_iTimeFlag(0),
M_iStopTimeFlag(TIME_FLAG_UNDEFINE),
m_iTmpTime(0),
m_iMode(-1),
m_lastvideoidx(-1),
m_iFrameLogID(0),
m_iVideoLogID(1),
m_bFirstSave(false),
m_bWriteLog(false),
//m_pLocker(NULL),
m_hThreadSaveH264(NULL),
m_lDataStructVector(VIDEO_FRAME_LIST_SIZE),
m_hVideoSaver(NULL),
  m_pUserData(NULL),
  m_pCallbackFunc(NULL)
{
    m_hVideoSaver = Video_CreateProcessHandle(VIDEOTYPE_MP4);

    memset(m_chCurrentPath, '\0', sizeof(m_chCurrentPath));
    //sprintf_s(m_chCurrentPath, sizeof(m_chCurrentPath), "%s", Tool_GetCurrentPath());
    sprintf(m_chCurrentPath,  "%s", Tool_GetCurrentPath());

    InitializeCriticalSection(&m_Locker);
    InitializeCriticalSection(&m_DataListLocker);

    InitLogerConfig();
}

MyH264Saver::~MyH264Saver()
{
    SetIfExit(true);
    SetSaveFlag(false);
    Tool_SafeCloseThread(m_hThreadSaveH264);
    if (m_hVideoSaver != NULL)
    {
        Video_CloseProcessHandle(m_hVideoSaver);
    }

    DeleteCriticalSection(&m_Locker);
    DeleteCriticalSection(&m_DataListLocker);
}

bool MyH264Saver::initMode(int iType /*= 0*/)
{
    if (-1 != GetProcessMode())
    {
        return false;
    }
    SetProcessMode(iType);
    m_hThreadSaveH264 = CreateThread(NULL, 0, H264DataProceesor, this, 0, NULL);
    return true;
}

int MyH264Saver::GetProcessMode()
{
    int iValue = 0;
    EnterCriticalSection(&m_Locker);
    iValue = m_iMode;
    LeaveCriticalSection(&m_Locker);
    return iValue;
}

void MyH264Saver::SetProcessMode(int iValue)
{
    EnterCriticalSection(&m_Locker);
    m_iMode = iValue;
    LeaveCriticalSection(&m_Locker);
}

bool MyH264Saver::addDataStruct(CustH264Struct* pDataStruct)
{
    if (pDataStruct == NULL)
    {
        return false;
    }
    //char buf[256] = { 0 };
    //sprintf_s(buf, "%ld\n", pDataStruct->m_llFrameTime);
    //OutputDebugString(buf);

    std::shared_ptr<CustH264Struct> pData = std::shared_ptr<CustH264Struct>(pDataStruct);
    if (GetProcessMode() == 1)
    {
        EnterCriticalSection(&m_DataListLocker);
        if (m_lDataStructList.size() > VIDEO_FRAME_LIST_SIZE)
        {
            auto tempData = m_lDataStructList.front();
            m_lDataStructList.pop_front();
            //WriteFormatLog("addDataStruct:: size > %d, erase front, frame time = %lld ", VIDEO_FRAME_LIST_SIZE, tempData->m_llFrameTime);
        }
        m_lDataStructList.push_back(pData);
        LeaveCriticalSection(&m_DataListLocker);
    }
    else
    {
        m_lDataStructVector.AddOneData(pData);
    }   

    if (GetLogEnable())
    {
        SYSTEMTIME  stimeNow = Tool_GetCurrentTime();

        char chBuffer1[256] = { 0 };
        //_getcwd(chBuffer1, sizeof(chBuffer1));

        sprintf(chBuffer1, /*sizeof(chBuffer1), */"%s\\XLWLog\\%04d-%02d-%02d\\",
            m_chCurrentPath,
            stimeNow.wYear,
            stimeNow.wMonth,
            stimeNow.wDay);
        Tool_MakeDir(chBuffer1);

        char chLogFileName[512] = { 0 };
        sprintf(chLogFileName, /*sizeof(chLogFileName), */"%s\\videoFrame_%d.log", chBuffer1,stimeNow.wHour);

        char chFrameTime[256] = {0};
        INT64 iTimeNow = pData->m_llFrameTime / 1000;
        struct tm tmFrame = *localtime((time_t *)&iTimeNow);
        sprintf(chFrameTime, /*sizeof(CamResult->chPlateTime),*/ "%04d-%02d-%02d %02d:%02d:%02d.%03d",
            tmFrame.tm_year +1900,
            tmFrame.tm_mon +1,
            tmFrame.tm_mday,
            tmFrame.tm_hour,
            tmFrame.tm_min,
            tmFrame.tm_sec,
            pData->m_llFrameTime % 1000);        

//        char chCurrentTime[256] = {0};
//        sprintf(chCurrentTime, /*sizeof(CamResult->chPlateTime),*/ "%04d-%02d-%02d %02d:%02d:%02d.%03d",
//            stimeNow.wYear,
//            stimeNow.wMonth,
//            stimeNow.wDay,
//            stimeNow.wHour,
//            stimeNow.wMinute,
//            stimeNow.wSecond,
//            stimeNow.wMilliseconds);


//        LOG(TRACE) << "[Thread: "<< GetCurrentThreadId() << " ]"
//                   << " frameIndex = "<< pData->index
//                   << " frameTimeStame = "<< pData->m_llFrameTime
//                   << " frame time string = "<<  chFrameTime;

//        LOGT("[Thread: "<< GetCurrentThreadId() << " ]"
//                                << " frameIndex = "<< pData->index
//                                << " frameTimeStame = "<< pData->m_llFrameTime
//                                << " frame time string = "<<  chFrameTime);



        LOG_INFO(m_iFrameLogID, " frameIndex = "<< pData->index
                                        << " frameTimeStame = "<< pData->m_llFrameTime
                                        << " frame time string = "<<  chFrameTime);

//        FILE *file = NULL;
//        file = fopen(chLogFileName, "a+");
//        if (file)
//        {
//            fprintf(file, "[%04d-%02d-%02d %02d:%02d:%02d:%03d] :frame index = %d m_llFrameTime =  %lld , frame time string = %s \n",
//                stimeNow.wYear,
//                stimeNow.wMonth,
//                stimeNow.wDay,
//                stimeNow.wHour,
//                stimeNow.wMinute,
//                stimeNow.wSecond,
//                stimeNow.wMilliseconds,
//                pData->index,
//                pData->m_llFrameTime,
//                chFrameTime);

//            fclose(file);
//            file = NULL;
//        }
    }

    return true;
}

bool MyH264Saver::StartSaveH264(INT64 beginTimeStamp, const char* pchFilePath)
{
    //qDebug()<< "StartSaveH264 :"<< "beginTimeStamp "<< beginTimeStamp;
    WRITE_LOG(" beginTimeStamp =  %lld , pchFilePath = %s", beginTimeStamp, pchFilePath);
    int iSaveFlag = GetSaveFlag();
    while(SAVING_FLAG_NOT_SAVE != iSaveFlag)
    {
        switch( iSaveFlag)
        {
            case SAVING_FLAG_SAVING:
                WRITE_LOG(" current save flag == SAVING_FLAG_SAVING, set to SAVING_FLAG_SHUT_DOWN to finish writing video file. ");
                SetSaveFlag(SAVING_FLAG_SHUT_DOWN);
            break;
        case SAVING_FLAG_SHUT_DOWN:
            WRITE_LOG(" current save flag == SAVING_FLAG_SHUT_DOWN wait finish. ");
            break;
        default:
            WRITE_LOG(" current save flag = %d, unknow. ", iSaveFlag);
            break;
        }
        iSaveFlag = GetSaveFlag();
        Sleep(100);
    }
    SetSavePath(pchFilePath, strlen(pchFilePath));
    SetStartTimeFlag(beginTimeStamp);
    SetIfFirstSave(true);
    SetStopTimeFlag(TIME_FLAG_UNDEFINE);
    SetSaveFlag(SAVING_FLAG_SAVING);

    m_lastvideoidx = -1;
    return true;
}

bool MyH264Saver::StopSaveH264(INT64 TimeFlag)
{
    //SetSaveFlag(SAVING_FLAG_SHUT_DOWN);
    SetStopTimeFlag(TimeFlag);
    return true;
}

DWORD WINAPI MyH264Saver::H264DataProceesor(LPVOID lpThreadParameter)
{
    if (NULL == lpThreadParameter)
    {
        return 0;
    }
    MyH264Saver* pSaver = (MyH264Saver*)lpThreadParameter;
    if (pSaver->GetProcessMode() == 0)
    {
        return pSaver->processH264Data_mp4();
    }
    else
    {
        return pSaver->processH264Data_mp4_new();
    }     
}

DWORD MyH264Saver::processH264Data_mp4()
{
    WriteFormatLog("MyH264Saver::processH264Data begin.\n");
    int  iSaveFlag = 0;
    int iVideoWidth = -1;
    int iVideoHeight = -1;

    int iFlag = 1;

    long long iCurrentFrameTimeFlag = -1;
    long long iLastFrameTimeFlag = -1;
    long long iVideoStopTimeFlag = -1;

    long long iLastSaveFlag = -1;
    long long iVideoBeginTimeFlag = -1;
    long long iTimeNowFlag = -1;

    int iLastFrameIndex = -1;
    int iCurrentFrameIndex = -1;

    int iDataListIndex = -1;
    int iLastNullIndex = 0;
    bool bFindEmptyData = false;
    while (!GetIfExit())
    {

        char buf[256] = { 0 };
        //iVideoStopTimeFlag = GetStopTimeFlag();
        iVideoBeginTimeFlag = GetStartTimeFlag();
        iTimeNowFlag = GetTickCount();

        iSaveFlag = GetSaveFlag();


        if (iSaveFlag != iLastSaveFlag)
        {
            WriteFormatLog("stop time flag change, lastStopTimeFlag =  %lld  , current Stop time flag =  %lld  , system time Now =  %lld  ,  video beginTIme =  %lld , save flag = %d\n",
                iLastSaveFlag,
                iVideoStopTimeFlag,
                iTimeNowFlag,
                iVideoBeginTimeFlag,
                iSaveFlag);
            iLastSaveFlag = iSaveFlag;
        }

        std::shared_ptr<CustH264Struct > pData = nullptr;
        switch (iSaveFlag)
        {
        case SAVING_FLAG_NOT_SAVE:
            //usleep(10 * 1000);
            //printf("MyH264Saver::processH264Data SAVING_FLAG_NOT_SAVE, break \n");
            Sleep(1);
            break;
        case SAVING_FLAG_SAVING:
            //printf("MyH264Saver::processH264Data SAVING_FLAG_SAVING \n",GetSavePath() );

            if (-1 == iDataListIndex)
            {
                //查找所需的index
                if (bFindEmptyData)
                {
                    iLastNullIndex = iLastNullIndex == VIDEO_FRAME_LIST_SIZE - 1 ? 0 : iLastNullIndex;

                    WriteFormatLog("this is new search , iDataListIndex = %d  search index.\n", iDataListIndex);
                }
                else
                {
                    iLastNullIndex = m_lDataStructVector.GetOldestDataIndex();
                    WriteFormatLog("iDataListIndex = %d  search index.\n", iDataListIndex);
                }

                for (int i = 0; i < VIDEO_FRAME_LIST_SIZE; /*i++*/)
                {
                    int iSearchIndex = (i + iLastNullIndex) % VIDEO_FRAME_LIST_SIZE;
                    pData = m_lDataStructVector.GetOneDataByIndex(iSearchIndex);
                    if (pData == nullptr)
                    {
                        iLastNullIndex = iSearchIndex;
                        bFindEmptyData = true;
                        WriteFormatLog("NewSearch index = %d, pData == nullptr , finish search.\n", iSearchIndex);
                        break;
                    }
                    iCurrentFrameTimeFlag = pData->m_llFrameTime;
                    iCurrentFrameIndex = pData->index;
                    iVideoBeginTimeFlag = GetStartTimeFlag();
                    if (iCurrentFrameTimeFlag >= iVideoBeginTimeFlag)
                    {
                        iDataListIndex = iSearchIndex;
                        WriteFormatLog("NewSearch index = %d, iCurrentFrameIndex = %d, iCurrentFrameTimeFlag  %lld  >= iVideoBeginTimeFlag  %lld , set iDataListIndex = %d, break .\n",
                            iSearchIndex,
                            iCurrentFrameIndex,
                            iCurrentFrameTimeFlag,
                            iVideoBeginTimeFlag,
                            iDataListIndex);
                        iLastFrameIndex = iCurrentFrameIndex;
                        iLastFrameTimeFlag = iCurrentFrameTimeFlag;
                        break;
                    }
                    if (iVideoBeginTimeFlag - iCurrentFrameTimeFlag > 1000)
                    {
                        i = i + 25;
                        WriteFormatLog("NewSearch iVideoBeginTimeFlag (  %lld  )- iCurrentFrameTimeFlag(  %lld  ) > 1000 ms, next search index +25, continue .\n", iVideoBeginTimeFlag, iCurrentFrameTimeFlag);
                    }
                    else
                    {
                        i++;
                    }
                    WriteFormatLog("NewSearch search index = %d , iCurrentFrameTimeFlag  %lld  < iVideoBeginTimeFlag  %lld  , continue .\n", iSearchIndex, iCurrentFrameTimeFlag, iVideoBeginTimeFlag);
                    if ((VIDEO_FRAME_LIST_SIZE - 1) == i)
                    {
                        bFindEmptyData = false;
                    }
                }

                if(pData != nullptr
                        && pData->m_llFrameTime < GetStartTimeFlag()
                        )
                {
                    WriteFormatLog("NewSearch search finish, but still can`t find the frame, set frame data to null. iCurrentFrameTimeFlag  %lld  < iVideoBeginTimeFlag  %lld   .\n",
                                   pData->m_llFrameTime,
                                   GetStartTimeFlag());
                    pData = nullptr;
                }
            }
            else
            {
                iDataListIndex = (iDataListIndex >= (VIDEO_FRAME_LIST_SIZE - 1)) ? 0 : (iDataListIndex + 1);
                pData = m_lDataStructVector.GetOneDataByIndex(iDataListIndex);
                if (pData == nullptr)
                {
                    WriteFormatLog("search iDataListIndex = %d, but pData == nullptr, and index not change continue.\n", iDataListIndex);
                     iDataListIndex = (iDataListIndex == 0)?  VIDEO_FRAME_LIST_SIZE - 1: (iDataListIndex -1);
                     Sleep(200);
                    break;
                }

                iCurrentFrameTimeFlag = pData->m_llFrameTime;
                iCurrentFrameIndex = pData->index;
                iVideoBeginTimeFlag = GetStartTimeFlag();
                iVideoStopTimeFlag = GetStopTimeFlag();

                if (TIME_FLAG_UNDEFINE != iVideoStopTimeFlag
                    && iCurrentFrameTimeFlag >= iVideoStopTimeFlag)
                {
                    WriteFormatLog("search index = %d, iCurrentFrameIndex = %d, iLastFrameIndex = %d,  iCurrentFrameTimeFlag  %lld  >  iVideoStopTimeFlag  %lld , SetSaveFlag SAVING_FLAG_SHUT_DOWN .\n",
                        iDataListIndex,
                        iCurrentFrameIndex,
                        iLastFrameIndex,
                        iCurrentFrameTimeFlag,
                        iVideoStopTimeFlag);

                    SetSaveFlag(SAVING_FLAG_SHUT_DOWN);
                    iDataListIndex = -1;
                    iLastNullIndex = 0;
                    iLastFrameIndex = -1;
                }
                else
                {
                    if (iCurrentFrameTimeFlag >= iVideoBeginTimeFlag)
                    {
                        if (iLastFrameIndex == -1
//                            || TIME_FLAG_UNDEFINE == iVideoStopTimeFlag
//                            || (iCurrentFrameIndex == (iLastFrameIndex + 1) && iCurrentFrameTimeFlag > iLastFrameTimeFlag)
                            || (iCurrentFrameIndex > iLastFrameIndex  && iCurrentFrameTimeFlag > iLastFrameTimeFlag)
                            || ((iCurrentFrameTimeFlag > iLastFrameTimeFlag) && (iCurrentFrameIndex == 0) && iLastFrameIndex == VIDEO_FRAME_LIST_SIZE)
                            )
                        {
                            WriteFormatLog("search index = %d, iCurrentFrameIndex = %d, iCurrentFrameTimeFlag  %lld  >  iVideoBeginTimeFlag  %lld  and < iVideoStopTimeFlag (  %lld  ) ,   iLastFrameTimeFlag =  %lld  , iLastFrameIndex = %d ,Save frame .\n",
                                iDataListIndex,
                                iCurrentFrameIndex,
                                iCurrentFrameTimeFlag,
                                iVideoBeginTimeFlag,
                                iVideoStopTimeFlag,
                                iLastFrameTimeFlag,
                                iLastFrameIndex);

                            iLastFrameTimeFlag = iCurrentFrameTimeFlag;
                            iLastFrameIndex = iCurrentFrameIndex;
                        }
                        else
                        {
                            WriteFormatLog("search index = %d, iCurrentFrameTimeFlag (  %lld  ) > iVideoBeginTimeFlag (  %lld  ) ,and < iVideoStopTimeFlag (  %lld  ) but nosave frame, iCurrentFrameIndex = %d, iLastFrameTimeFlag =  %lld , iLastFrameIndex = %d, ",
                                iDataListIndex,
                                iCurrentFrameTimeFlag,
                                iVideoBeginTimeFlag,
                                iVideoStopTimeFlag,
                                iCurrentFrameIndex,
                                iLastFrameTimeFlag,
                                iLastFrameIndex
                                );
                            pData = nullptr;
                        }
                    }
                    else
                    {
                        WriteFormatLog("search index = %d , iCurrentFrameIndex = %d , iCurrentFrameTimeFlag  %lld  <  iVideoBeginTimeFlag  %lld  , no Save frame .\n",
                            iDataListIndex,
                            iCurrentFrameIndex,
                            iCurrentFrameTimeFlag,
                            iVideoBeginTimeFlag);
                        pData = nullptr;

//                        if (iVideoBeginTimeFlag - iCurrentFrameTimeFlag > 1000)
//                        {
//                            iDataListIndex = ((iDataListIndex + 24) >= (VIDEO_FRAME_LIST_SIZE - 1)) ? 0 : (iDataListIndex + 24);
//                            WriteFormatLog("search2, iVideoBeginTimeFlag ( %lld )- iCurrentFrameTimeFlag( %lld ) > 1000 ms, next search index +25, continue .\n", iVideoBeginTimeFlag, iCurrentFrameTimeFlag);
//                        }

                        WriteFormatLog("search index = %d, wait the next time ,do not go forward",iDataListIndex );
                        iDataListIndex = (iDataListIndex == 0)?  VIDEO_FRAME_LIST_SIZE - 1: (iDataListIndex -1);
                        Sleep(200);
                    }
                }
            }

            if (pData == nullptr)
            {
                WriteFormatLog("MyH264Saver::processH264Data SAVING_FLAG_SAVING  pData == nullptr\n");
                break;
            }

            iVideoWidth = pData->m_iWidth;
            iVideoHeight = pData->m_iHeight;
            if (GetIfFirstSave())
            {
                WriteFormatLog("GetIfFirstSave = true, Video_CreateVideoFile = %s \n", GetSavePath());
                iFlag = Video_CreateVideoFile(m_hVideoSaver, GetSavePath(), iVideoWidth, iVideoHeight, 25);
                if (0 != iFlag)
                {
                    WriteFormatLog("Video_CreateVideoFile failed  return code = %d\n", iFlag);
                }
                else
                {
                    WriteFormatLog("Video_CreateVideoFile success");
                }
            }

            if (NULL != m_hVideoSaver
                && pData->m_llFrameTime >= GetStartTimeFlag()
                )
            {
                iFlag = Video_WriteH264Frame(m_hVideoSaver, FRAMETYPE_MP4_VIDEO, pData->m_pbH264FrameData, pData->m_iDataSize);
                //iFlag = m_264AviLib.writeFrame((char*)pData->m_pbH264FrameData, pData->m_iDataSize, pData->m_isIFrame);
                if (iFlag != 0)
                {
                    WriteFormatLog(" Video_WriteH264Frame = %d\n", iFlag);
                }
                SetIfFirstSave(false);
            }
            else
            {
                //if (m_264AviLib.IsNULL())
                if (NULL == m_hVideoSaver)
                {
                    WriteFormatLog(" SAVING_FLAG_SAVING but m_hVideoSaver IsNULL\n");
                }
                if (pData->m_llFrameTime < GetStartTimeFlag())
                {
                    WriteFormatLog(" SAVING_FLAG_SAVING but pData->m_llFrameTime (  %lld  )< GetStartTimeFlag(  %lld  )\n", pData->m_llFrameTime, GetStartTimeFlag());
                }
                WriteFormatLog(" SAVING_FLAG_SAVING but some thing wrong.\n", iFlag);
            }
            pData = nullptr;
            break;
        case SAVING_FLAG_SHUT_DOWN:
            //if (!m_264AviLib.IsNULL())
            if (NULL != m_hVideoSaver)
            {
                m_iTmpTime = 0;
                //m_264AviLib.close();
                //SetSaveFlag(SAVING_FLAG_NOT_SAVE);
                iFlag = Video_CloseVideoFile(m_hVideoSaver);
                SendFileName( GetSavePath());
                WriteFormatLog("Video_CloseVideoFile SAVING_FLAG_SHUT_DOWN. iFlag = %d\n", iFlag);
            }
            else
            {
                WriteFormatLog("MyH264Saver::processH264Data SAVING_FLAG_SHUT_DOWN m_hVideoSaver Is NULL.\n");
            }
            SetSaveFlag(SAVING_FLAG_NOT_SAVE);
            iDataListIndex = -1;
            break;
        default:
            WriteFormatLog("MyH264Saver::processH264Data default break.\n");
            break;
        }

    }

    WriteFormatLog("MyH264Saver::processH264Data finish.\n");
    return 0;
}

DWORD MyH264Saver::processH264Data_mp4_new()
{
    WriteFormatLog("MyH264Saver::processH264Data begin.\n");
    int  iSaveFlag = 0;
    int iVideoWidth = -1;
    int iVideoHeight = -1;

    int iFlag = 1;

    long long iCurrentFrameTimeFlag = -1;
    long long iLastFrameTimeFlag = -1;
    long long iVideoStopTimeFlag = -1;

    long long iLastSaveFlag = -1;
    long long iVideoBeginTimeFlag = -1;
    long long iTimeNowFlag = -1;

    int iLastFrameIndex = -1;
    int iCurrentFrameIndex = -1;

    int iDataListIndex = -1;
    int iLastNullIndex = 0;
    bool bFindEmptyData = false;


    while (!GetIfExit())
    {

        char buf[256] = { 0 };
        //iVideoStopTimeFlag = GetStopTimeFlag();
        iVideoBeginTimeFlag = GetStartTimeFlag();
        iTimeNowFlag = GetTickCount();

        iSaveFlag = GetSaveFlag();


        if (iSaveFlag != iLastSaveFlag)
        {
            WriteFormatLog("stop time flag change, lastStopTimeFlag =  %lld  , current Stop time flag =  %lld  , system time Now =  %lld  ,  video beginTIme =  %lld , save flag = %d\n",
                iLastSaveFlag,
                iVideoStopTimeFlag,
                iTimeNowFlag,
                iVideoBeginTimeFlag,
                iSaveFlag);
            iLastSaveFlag = iSaveFlag;
        }

        std::shared_ptr<CustH264Struct > pData = nullptr;
        std::deque<std::shared_ptr<CustH264Struct > >::iterator iter;
        std::deque<std::shared_ptr<CustH264Struct > >::iterator iterEnd;
        size_t iBeginIndex = 0;
        int iFrameCount = 0;
        bool bFirstFrame = false;

        switch (iSaveFlag)
        {
        case SAVING_FLAG_NOT_SAVE:
            //usleep(10 * 1000);
            //printf("MyH264Saver::processH264Data SAVING_FLAG_NOT_SAVE, break \n");
            Sleep(10);
            break;
        case SAVING_FLAG_SAVING:
            if(TIME_FLAG_UNDEFINE == GetStopTimeFlag())
            {
                Sleep(50);
                WriteFormatLog("SAVING_FLAG_SAVING , but stopTime is undefined wait to 50 ms.");
                continue;
            }

#ifdef WINDOWS
            EnterCriticalSection(&m_DataListLocker);
#endif

           iter = std::find_if(std::begin(m_lDataStructList),
                         std::end(m_lDataStructList),
                         [iVideoBeginTimeFlag] (std::shared_ptr<CustH264Struct > h264Data)
            {
                return (h264Data->m_llFrameTime >= iVideoBeginTimeFlag);
            }
            );
          iBeginIndex = m_lDataStructList.size();

          if(iter != std::end(m_lDataStructList))
          {
              iBeginIndex = std::distance(std::begin(m_lDataStructList), iter);
              WriteFormatLog("find the first frame , list index = %d ", iBeginIndex);
          }
          else
          {
              WriteFormatLog("can not find the first frame , use list size to replace index = %d ", iBeginIndex);
          }
          iFrameCount = (GetStopTimeFlag() - GetStartTimeFlag() )/40;
          WriteFormatLog("frame count = %d ", iFrameCount);

          iterEnd =  std::end(m_lDataStructList);

          if( iBeginIndex+iFrameCount >= m_lDataStructList.size())
          {
              if(m_lDataStructList.size() >= iFrameCount  )
              {
                   iBeginIndex = m_lDataStructList.size() - iFrameCount;

                   WriteFormatLog("iBeginIndex+iFrameSize = %d >= data list size %d, use m_lDataStructList.size() - iFrameSize = %d to begin.",
                                  iBeginIndex+iFrameCount,
                                  m_lDataStructList.size(),
                                  iBeginIndex);
              }
             else
              {
                  iBeginIndex = 0;

                  WriteFormatLog("iBeginIndex+iFrameSize = %d > data list size %d,and list size is smaller than it, use index 0",iBeginIndex+iFrameCount, m_lDataStructList.size() );
              }
          }
          else
          {
              iterEnd = std::begin(m_lDataStructList) + iBeginIndex + iFrameCount;

              WriteFormatLog("use list index = %d to begin, and %d to end", iBeginIndex,  iBeginIndex + iFrameCount);
          }


          bFirstFrame = true;
          for(auto it = std::begin(m_lDataStructList) + iBeginIndex; it != iterEnd ; it++)
          {
              pData = *it;

              if(bFirstFrame
                      && NULL != m_hVideoSaver )
              {
                  WriteFormatLog("begin to save video,  Video_CreateVideoFile = %s \n", GetSavePath());
                  iVideoWidth = pData->m_iWidth;
                  iVideoHeight = pData->m_iHeight;
                  iFlag = Video_CreateVideoFile(m_hVideoSaver, GetSavePath(), iVideoWidth, iVideoHeight, 25);
                  if (0 != iFlag)
                  {
                      WriteFormatLog("Video_CreateVideoFile failed  return code = %d\n", iFlag);
                  }
                  else
                  {
                      WriteFormatLog("Video_CreateVideoFile success");
                      bFirstFrame = false;
                  }
              }

              WriteFormatLog("SAVING_FLAG_SAVING write frame , frame index = %d, frame time = %I64d  ", pData->index ,pData->m_llFrameTime);
              iFlag = Video_WriteH264Frame(m_hVideoSaver, FRAMETYPE_MP4_VIDEO, pData->m_pbH264FrameData, pData->m_iDataSize);
              //iFlag = m_264AviLib.writeFrame((char*)pData->m_pbH264FrameData, pData->m_iDataSize, pData->m_isIFrame);
              if (iFlag != 0)
              {
                  WriteFormatLog(" Video_WriteH264Frame = %d\n", iFlag);
              }
              pData = nullptr;
          }


#ifdef WINDOWS
            LeaveCriticalSection(&m_DataListLocker);
#endif

            SetSaveFlag(SAVING_FLAG_SHUT_DOWN);

            break;
        case SAVING_FLAG_SHUT_DOWN:
            //if (!m_264AviLib.IsNULL())
            if (NULL != m_hVideoSaver)
            {
                m_iTmpTime = 0;
                //m_264AviLib.close();
                //SetSaveFlag(SAVING_FLAG_NOT_SAVE);
                iFlag = Video_CloseVideoFile(m_hVideoSaver);
                SendFileName( GetSavePath());
                WriteFormatLog("Video_CloseVideoFile SAVING_FLAG_SHUT_DOWN. iFlag = %d\n", iFlag);
            }
            else
            {
                WriteFormatLog("MyH264Saver::processH264Data SAVING_FLAG_SHUT_DOWN m_hVideoSaver Is NULL.\n");
            }
            SetSaveFlag(SAVING_FLAG_NOT_SAVE);
            iDataListIndex = -1;
            break;
        default:
            WriteFormatLog("MyH264Saver::processH264Data default break.\n");
            break;
        }

    }

    WriteFormatLog("MyH264Saver::processH264Data finish.\n");
    return 0;
}

void MyH264Saver::SetLogEnable(bool bValue)
{
    EnterCriticalSection(&m_Locker);
    m_bWriteLog = bValue;
    LeaveCriticalSection(&m_Locker);
}

bool MyH264Saver::GetLogEnable()
{
    bool bValue = false;
    EnterCriticalSection(&m_Locker);
    bValue = m_bWriteLog;
    LeaveCriticalSection(&m_Locker);
    return bValue;
}

void MyH264Saver::SetFileNameCallback(void *pUserData, void *pCallbackFunc)
{
    EnterCriticalSection(&m_Locker);
    m_pUserData = pUserData;
    m_pCallbackFunc = pCallbackFunc;
    LeaveCriticalSection(&m_Locker);
}

void MyH264Saver::SendFileName(const char *fileName)
{
    EnterCriticalSection(&m_Locker);
    if(m_pCallbackFunc)
    {
        ((IMAGE_FILE_CALLBACK)m_pCallbackFunc)(m_pUserData, fileName);
    }
    LeaveCriticalSection(&m_Locker);
}

void MyH264Saver::SetIfExit(bool bValue)
{
    EnterCriticalSection(&m_Locker);
    m_bExit = bValue;
    LeaveCriticalSection(&m_Locker);
}

bool MyH264Saver::GetIfExit()
{
    bool bValue = false;
    EnterCriticalSection(&m_Locker);
    bValue = m_bExit;
    LeaveCriticalSection(&m_Locker);
    return bValue;
}

void MyH264Saver::SetSaveFlag(int iValue)
{
    WriteFormatLog("MyH264Saver::SetSaveFlag %d", iValue);
    EnterCriticalSection(&m_Locker);
    m_iSaveH264Flag = iValue;
    LeaveCriticalSection(&m_Locker);
}

int MyH264Saver::GetSaveFlag()
{
    int iValue = false;
    EnterCriticalSection(&m_Locker);
    iValue = m_iSaveH264Flag;
    LeaveCriticalSection(&m_Locker);
    return iValue;
}

void MyH264Saver::SetStartTimeFlag(INT64 iValue)
{
    WriteFormatLog("MyH264Saver::SetStartTimeFlag  %lld ", iValue);
    EnterCriticalSection(&m_Locker);
    m_iTimeFlag = iValue;
    m_iTmpTime = iValue;
    LeaveCriticalSection(&m_Locker);
}

INT64 MyH264Saver::GetStartTimeFlag()
{
    INT64 iValue = false;
    EnterCriticalSection(&m_Locker);
    iValue = m_iTimeFlag;
    LeaveCriticalSection(&m_Locker);
    return iValue;
}

void MyH264Saver::SetStopTimeFlag(INT64 iValue)
{
    WriteFormatLog("MyH264Saver::SetStopTimeFlag %lld ", iValue);
    EnterCriticalSection(&m_Locker);
    M_iStopTimeFlag = iValue;
    LeaveCriticalSection(&m_Locker);
}

INT64 MyH264Saver::GetStopTimeFlag()
{
    INT64 iValue = false;
    EnterCriticalSection(&m_Locker);
    iValue = M_iStopTimeFlag;
    LeaveCriticalSection(&m_Locker);
    return iValue;
}

void MyH264Saver::SetIfFirstSave(bool bValue)
{
    WriteFormatLog("MyH264Saver::SetIfFirstSave %d", bValue);
    EnterCriticalSection(&m_Locker);
    m_bFirstSave = bValue;
    LeaveCriticalSection(&m_Locker);
}

bool MyH264Saver::GetIfFirstSave()
{
    bool bValue = false;
    EnterCriticalSection(&m_Locker);
    bValue = m_bFirstSave;
    LeaveCriticalSection(&m_Locker);
    return bValue;
}

void MyH264Saver::SetSavePath(const char* filePath, size_t bufLength)
{
    WriteFormatLog("MyH264Saver::SetSavePath %s", filePath);
    EnterCriticalSection(&m_Locker);
    if (bufLength < sizeof(m_chFilePath))
    {
        memcpy(m_chFilePath, filePath, bufLength);
        m_chFilePath[bufLength] = '\0';
    }
    LeaveCriticalSection(&m_Locker);
}

const char* MyH264Saver::GetSavePath()
{
    const char* pValue = NULL;
    EnterCriticalSection(&m_Locker);
    pValue = m_chFilePath;
    LeaveCriticalSection(&m_Locker);
    return pValue;
}

void MyH264Saver::WriteFormatLog(const char *szfmt, ...)
{
    if (!GetLogEnable())
        return;
    //std::lock_guard<std::mutex> locker(m_mtx);

    //char m_chLogBuf[10240] = { 0 };
    memset(m_chLogBuf, '\0', sizeof(m_chLogBuf));

    va_list arg_ptr;
    va_start(arg_ptr, szfmt);
    //vsnprintf_s(g_szPbString, sizeof(g_szPbString), szfmt, arg_ptr);
    //_vsnprintf(g_szPbString,  sizeof(g_szPbString),  szfmt, arg_ptr);
    vsnprintf(m_chLogBuf, sizeof(m_chLogBuf), szfmt, arg_ptr);
    va_end(arg_ptr);

    //LOG(INFO) << "[Thread: "<< GetCurrentThreadId() << " ]"<<g_szString;

    //LOGD("[Thread: "<< GetCurrentThreadId() << " ]"<<g_szString);

    //LoggerId logid_videoSave = ILog4zManager::getRef().findLogger("videoSave");

    LOG_INFO(m_iVideoLogID, m_chLogBuf);

//    SYSTEMTIME  stimeNow = Tool_GetCurrentTime();

//    char chBuffer1[256] = { 0 };
//    //_getcwd(chBuffer1, sizeof(chBuffer1));

//    sprintf(chBuffer1, /*sizeof(chBuffer1), */"%s\\XLWLog\\%04d-%02d-%02d\\",
//        m_chCurrentPath,
//        stimeNow.wYear,
//        stimeNow.wMonth,
//        stimeNow.wDay);
//    Tool_MakeDir(chBuffer1);

//    char chLogFileName[512] = { 0 };
//    sprintf(chLogFileName, /*sizeof(chLogFileName),*/ "%s/Video_%d-%02d_%02d_%02d_%02dm.log",
//        chBuffer1,
//        stimeNow.wYear,
//        stimeNow.wMonth,
//        stimeNow.wDay,
//        stimeNow.wHour,
//        stimeNow.wMinute/10);

//    FILE *file = NULL;
//    file = fopen(chLogFileName, "a+");
//    //fopen_s(&file, chLogFileName, "a+");
//    if (file)
//    {
//        //fprintf(file, "[%04d-%02d-%02d %02d:%02d:%02d:%03lld] : %s\n",
//        //    stimeNow.tm_year + 1900,
//        //    stimeNow.tm_mon + 1,
//        //    stimeNow.tm_mday,
//        //    stimeNow.tm_hour,
//        //    stimeNow.tm_min,
//        //    stimeNow.tm_sec,
//        //    iTimeInMilliseconds % 1000,
//        //    g_szString);

//        fprintf(file, "[%04d-%02d-%02d %02d:%02d:%02d:%03d] [threadID: %lu] : %s\n",
//            stimeNow.wYear,
//            stimeNow.wMonth,
//            stimeNow.wDay,
//            stimeNow.wHour,
//            stimeNow.wMinute,
//            stimeNow.wSecond,
//            stimeNow.wMilliseconds,
//                GetCurrentThreadId(),
//            g_szString);

//        fclose(file);
//        file = NULL;
//    }
}

void MyH264Saver::InitLogerConfig()
{
    //ILog4zManager::getRef().setLoggerPath(LOG4Z_MAIN_LOGGER_ID, "./XLWLog/");
    //ILog4zManager::getRef().setLoggerMonthdir(LOG4Z_MAIN_LOGGER_ID, true);

    srand((int)GetTickCount());
    unsigned int iLogID = rand();

    char chLogerName[256] = {0};
    sprintf(chLogerName, "videoFrame_%lu", iLogID);

    m_iFrameLogID = ILog4zManager::getRef().createLogger(chLogerName);
    ILog4zManager::getRef().setLoggerDisplay(m_iFrameLogID, false);
    ILog4zManager::getRef().setLoggerLevel(m_iFrameLogID, LOG_LEVEL_DEBUG);
    ILog4zManager::getRef().setLoggerMonthdir(m_iFrameLogID, true);
    ILog4zManager::getRef().setLoggerPath(m_iFrameLogID, "./XLWLog/");
    ILog4zManager::getRef().setLoggerName(m_iFrameLogID, chLogerName);
    ILog4zManager::getRef().setLoggerOutFile(m_iFrameLogID, true);

    memset(chLogerName, '\0', sizeof(chLogerName));
    sprintf(chLogerName, "videoSave_%lu", iLogID);
    m_iVideoLogID = ILog4zManager::getRef().createLogger(chLogerName);
    ILog4zManager::getRef().setLoggerDisplay(m_iVideoLogID, false);
    ILog4zManager::getRef().setLoggerLevel(m_iVideoLogID, LOG_LEVEL_DEBUG);
    ILog4zManager::getRef().setLoggerMonthdir(m_iVideoLogID, true);
    ILog4zManager::getRef().setLoggerPath(m_iVideoLogID, "./XLWLog/");
    ILog4zManager::getRef().setLoggerName(m_iVideoLogID, chLogerName);
    ILog4zManager::getRef().setLoggerOutFile(m_iVideoLogID, true);

    //ILog4zManager::getRef().start();
}
