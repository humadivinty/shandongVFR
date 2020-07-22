#include "stdafx.h"
#include "ToolFunction.h"
//#include "log4z.h"
#include <string>
#include<shellapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <iostream>
#include <map>
#include <sys/stat.h>
#include<Dbghelp.h>
#include<shlwapi.h>
#include <time.h>
//#include<windows.h>
#include "CameraModule/CameraResult.h"


#ifdef _WIN32
#include <direct.h>
#include <io.h>
#include <strsafe.h>
#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <regex>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

#elif _LINUX
#include <stdarg.h>
#include <sys/stat.h>
#endif
//#include <afx.h>
#ifdef USE_GID_PLUS
#include <gdiplus.h>
using namespace Gdiplus;
#pragma  comment(lib, "gdiplus.lib")
#endif

#pragma comment(lib, "Version.lib")

//#define INI_FILE_NAME "Config.ini"
//#define DLL_LOG_NAME "SLW.log"

#ifdef _WIN32
#define ACCESS _access
#define MKDIR(a) _mkdir((a))
#elif _LINUX
#define ACCESS access
#define MKDIR(a) mkdir((a),0755)
#endif
#pragma warning(disable : 4995)
char g_chDllPath[256] = {0};
char g_chLogPath[256] = { 0 };

#ifndef LOGFMTE
#define LOGFMTE printf
#endif

#ifndef LOGFMTD
#define LOGFMTD printf
#endif


TiXmlElement Tool_SelectElementByName(const char* InputInfo, const char* pName, int iXMLType)
{
    //注：XMLTYPE 为1时，InputInfo为XML路径，当为2时,InputInfo为二进制文件内容
    TiXmlDocument cXmlDoc;
    TiXmlElement* pRootElement = NULL;
    if (iXMLType == 1)
    {
        if (!cXmlDoc.LoadFile(InputInfo))
        {
            printf("parse XML file failed \n");
            return TiXmlElement("");
        }
    }
    else if (iXMLType == 2)
    {
        if (!cXmlDoc.Parse(InputInfo))
        {
            printf("parse XML failed \n");
            return TiXmlElement("");
        }
    }

    pRootElement = cXmlDoc.RootElement();
    if (NULL == pRootElement)
    {
        printf("no have root Element\n");
        return TiXmlElement("");
    }
    else
    {
        TiXmlElement* pTempElement = NULL;
        pTempElement = Tool_ReadElememt(pRootElement, pName);
        if (pTempElement)
        {
            printf("find the Name : %s, Text = %s\n", pTempElement->Value(), pTempElement->GetText());
            return *pTempElement;
        }
        else
        {
            return TiXmlElement("");
        }
    }
}

TiXmlElement* Tool_ReadElememt(TiXmlElement* InputElement, const char* pName)
{
    TiXmlElement* ptemp = NULL;
    if (InputElement && 0 == strcmp(pName, InputElement->Value()))
    {
        printf("Find the element :%s \n", InputElement->Value());
        ptemp = InputElement;
        return ptemp;
    }
    else
    {
        printf("%s \n", InputElement->Value());
    }

    TiXmlElement* tmpElement = InputElement;
    if (tmpElement->FirstChildElement())
    {
        ptemp = Tool_ReadElememt(tmpElement->FirstChildElement(), pName);
    }
    if (NULL == ptemp)
    {
        tmpElement = tmpElement->NextSiblingElement();
        if (tmpElement)
        {
            ptemp = Tool_ReadElememt(tmpElement, pName);
        }
    }
    return ptemp;
}

bool Tool_GetElementTextByName(const char* InputInfo,const char* pName, int iXMLType, char* chTextValue , size_t& bufferLength)
{
    TiXmlElement tempEle = Tool_SelectElementByName(InputInfo, pName, iXMLType);
    if (NULL == tempEle.GetText())
    {
        LOGFMTE("Tool_GetElementTextByName,  failed, NULL == tempEle.GetText()");
        return false;
    }
    size_t iLength = strlen(tempEle.GetText());
    if (iLength> 0 && iLength < bufferLength)
    {
        bufferLength = iLength;
        memcpy(chTextValue, tempEle.GetText(), iLength);
        chTextValue[iLength] = '\0';
        return true;
    }
    else
    {
        bufferLength = iLength;

        LOGFMTE("Tool_GetElementTextByName,  failed, iLength < 0 or iLength >= bufferLength");
        return false;
    }
}

bool Tool_InsertElementByName(const char* InputInfo, const char* pName, int iXMLType,
    const char* nodeName, const char* textValue,
    std::string& outputString)
{
    //注：XMLTYPE 为1时，InputInfo为XML路径，当为2时,InputInfo为二进制文件内容
    if (InputInfo == NULL || strlen(InputInfo) <= 0)
    {
        return false;
    }
    TiXmlDocument cXmlDoc;
    TiXmlElement* pRootElement = NULL;
    if (iXMLType == 1)
    {
        if (!cXmlDoc.LoadFile(InputInfo))
        {
            printf("parse XML file failed \n");
            return false;
        }
    }
    else if (iXMLType == 2)
    {
        if (!cXmlDoc.Parse(InputInfo))
        {
            printf("parse XML failed \n");
            return false;
        }
    }

    pRootElement = cXmlDoc.RootElement();
    if (NULL == pRootElement)
    {
        printf("no have root Element\n");
        return false;
    }
    else
    {
        TiXmlElement* pTempElement = NULL;
        pTempElement = Tool_ReadElememt(pRootElement, pName);
        if (pTempElement)
        { 
            char szText[256] = {0};
            sprintf_s(szText, sizeof(szText), "find the Name : %s, Text = %s\n", pTempElement->Value(), pTempElement->GetText());
            OutputDebugStringA(szText);
            TiXmlElement *pTextEle = new TiXmlElement(nodeName);
            TiXmlText *pTextValue = new TiXmlText(textValue);
            pTextEle->LinkEndChild(pTextValue);
            pTempElement->LinkEndChild(pTextEle);

            TiXmlPrinter Xmlprinter;
            cXmlDoc.Accept(&Xmlprinter);
            outputString = Xmlprinter.CStr();
            return true;
        }
        else
        {
            return false;
        }
    }
}

void Tool_ReadKeyValueFromConfigFile(const char* IniFileName, const char* nodeName, const char* keyName, char* keyValue, size_t bufferSize)
{
    if (strlen(keyValue) > bufferSize)
    {
        return;
    }
    char iniFileName[MAX_PATH] = { 0 };
    //sprintf_s(iniFileName, sizeof(iniFileName), "%s\\%s", Tool_GetCurrentPath(), IniFileName);
    sprintf_s(iniFileName, sizeof(iniFileName), "%s\\%s", Tool_GetDllDirPath(), IniFileName);

    char chTemp[256] = { 0 };
    GetPrivateProfileStringA(nodeName, keyName, "0", chTemp, bufferSize, iniFileName);
    if (strcmp(chTemp, "0") == 0)
    {
        WritePrivateProfileStringA(nodeName, keyName, keyValue, iniFileName);
    }
    else
    {
        strcpy_s(keyValue, bufferSize, chTemp);
    }
}

void Tool_ReadIntValueFromConfigFile(const char* IniFileName, const char* nodeName, const char* keyName, int&keyValue)
{
    char iniFileName[MAX_PATH] = { 0 };
    //sprintf_s(iniFileName, sizeof(iniFileName), "%s\\%s", Tool_GetCurrentPath(), IniFileName);
    sprintf_s(iniFileName, sizeof(iniFileName), "%s\\%s", Tool_GetDllDirPath(), IniFileName);

    int iValue = GetPrivateProfileIntA(nodeName, keyName, keyValue, iniFileName);
    keyValue = iValue;

    char chTemp[128] = { 0 };
    sprintf_s(chTemp, sizeof(chTemp), "%d", iValue);
    WritePrivateProfileStringA(nodeName, keyName, chTemp, iniFileName);
}

void Tool_WriteKeyValueFromConfigFile(const char* INIFileName, const char* nodeName, const char* keyName, char* keyValue, size_t bufferSize)
{
    if (strlen(keyValue) > bufferSize)
    {
        return;
    }

    char iniFileName[MAX_PATH] = { 0 };
    //strcat_s(iniFileName, Tool_GetCurrentPath());
    strcat_s(iniFileName, Tool_GetDllDirPath());
    strcat_s(iniFileName, INIFileName);

    //GetPrivateProfileStringA(nodeName, keyName, "172.18.109.97", keyValue, bufferSize, iniFileName);

    WritePrivateProfileStringA(nodeName, keyName, keyValue, iniFileName);
}

int Tool_checkIP(const char* p)
{
    int n[4];
    char c[4];
    if (sscanf(p, "%d%c%d%c%d%c%d%c",
        &n[0], &c[0], &n[1], &c[1],
        &n[2], &c[2], &n[3], &c[3])
        == 7)
//    if (sscanf_s(p, "%d%c%d%c%d%c%d%c",
//        &n[0], &c[0], 1,
//        &n[1], &c[1], 1,
//        &n[2], &c[2], 1,
//        &n[3], &c[3], 1)
//        == 7)
    {
        int i;
        for (i = 0; i < 3; ++i)
        if (c[i] != '.')
            return 0;
        for (i = 0; i < 4; ++i)
        if (n[i] > 255 || n[i] < 0)
            return 0;
        if (n[0] == 0 && n[1] == 0 && n[2] == 0 && n[3] == 0)
        {
            return 0;
        }
        return 1;
    }
    else
        return 0;
}

bool Tool_IsFileExist(const char* FilePath)
{
    if (FilePath == NULL)
    {
        return false;
    }
    FILE* tempFile = NULL;
    bool bRet = false;
    //tempFile = fopen(FilePath, "r");
    errno_t errCode;
    _set_errno(0);
    errCode = fopen_s(&tempFile, FilePath, "r");
    if (tempFile)
    {
        bRet = true;
        fclose(tempFile);
        tempFile = NULL;
    }
    else
    {
        LOGFMTE("Tool_IsFileExist, failed, error code = %d", errCode);
    }
    return bRet;
}

bool Tool_MakeDir(const char* chPath)
{
    if (NULL == chPath)
    {
        //WriteLog("the path is null ,Create Dir failed.");
        return false;
    }
    char pszDir[256] = {0};
    strcpy_s(pszDir, sizeof(pszDir), chPath);
    INT32 i = 0;
    INT32 iRet;
    INT32 iLen = strlen(pszDir);
    //在末尾加/
    if (pszDir[iLen - 1] != '\\' && pszDir[iLen - 1] != '/')
    {
        pszDir[iLen] = '/';
        pszDir[iLen + 1] = '\0';
    }

    // 创建目录
    for (i = 0;i < iLen;i ++)
    {
        if (pszDir[i] == '\\' || pszDir[i] == '/')
        {
            pszDir[i] = '\0';

            //如果不存在,创建
            iRet = ACCESS(pszDir,0);
            if (iRet != 0)
            {
                iRet = MKDIR(pszDir);
                if (iRet != 0)
                {
                    return false;
                }
            }
            //支持linux,将所有\换成/
            pszDir[i] = '/';
        }
    }
    return true;
}

long Tool_GetFileSize(const char *FileName)
{
    //FILE* tmpFile = fopen(FileName, "rb");    
    FILE* tmpFile = NULL;
    errno_t errCode;
    _set_errno(0);
    errCode = fopen_s(&tmpFile, FileName, "rb");
    if (tmpFile)
    {
        //fseek(tmpFile, 0, SEEK_END);
        //long fileSize = ftell(tmpFile);
        //fclose(tmpFile);
        //tmpFile = NULL;
        //return fileSize;

        long fileSize = _filelength(_fileno(tmpFile));
        fclose(tmpFile);
        tmpFile = NULL;
        return fileSize;
    }
    else
    {
        LOGFMTE("Tool_GetFileSize, failed, error code = %d", errCode);
        return 0;
    }
}

bool Tool_PingIPaddress(const char* IpAddress)
{
    //FILE* pfile;
    //char chBuffer[1024] = {0};
    char chCMD[256] = { 0 };
    sprintf_s(chCMD, sizeof(chCMD), "ping %s -n 1", IpAddress);
    //std::wstring wstrIpAddr = Tool_string2wstring(IpAddress);
    //wchar_t chCMD[256] = { 0 };
    //swprintf_s(chCMD, sizeof(chCMD), L"ping %s -n 1", wstrIpAddr.c_str());
    //std::string strPingResult;
    //pfile = _popen(chCMD, "r");
    //if (pfile != NULL)
    //{
    //	while(fgets(chBuffer, 1024, pfile) != NULL)
    //	{
    //		strPingResult.append(chBuffer);
    //	}
    //}
    //else
    //{
    //	printf("popen failed. \n");
    //	return false;
    //}
    //_pclose(pfile);
    //printf("%s", strPingResult.c_str());
    //if (std::string::npos != strPingResult.find("TTL") || std::string::npos != strPingResult.find("ttl"))
    //{
    //	return true;
    //}
    //else
    //{
    //	return false;
    //}


    char pbuf[1024]; // 缓存  
    DWORD len;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    HANDLE hRead1 = NULL, hWrite1 = NULL;  // 管道读写句柄  
    BOOL b;
    SECURITY_ATTRIBUTES saAttr;

    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE; // 管道句柄是可被继承的  
    saAttr.lpSecurityDescriptor = NULL;

    // 创建匿名管道，管道句柄是可被继承的  
    b = CreatePipe(&hRead1, &hWrite1, &saAttr, 1024);
    if (!b)
    {
        //MessageBox(hwnd, "管道创建失败。","Information",0);  
        printf("管道创建失败\n");
        return false;
    }

    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    si.hStdOutput = hWrite1; // 设置需要传递到子进程的管道写句柄  


    // 创建子进程，运行ping命令，子进程是可继承的  
    if (!CreateProcess(NULL, chCMD, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
    {
        //itoa(GetLastError(), pbuf, 10); 
        sprintf_s(pbuf, sizeof(pbuf), "%d", GetLastError());
        //MessageBox(hwnd, pbuf,"Information",0);
        printf("%s\n", pbuf);
        CloseHandle(hRead1);
        hRead1 = NULL;
        CloseHandle(hWrite1);
        hWrite1 = NULL;
        return false;
    }

    // 写端句柄已被继承，本地则可关闭，不然读管道时将被阻塞  
    CloseHandle(hWrite1);
    hWrite1 = NULL;

    // 读管道内容，并用消息框显示  
    len = 1000;
    DWORD l;

    std::string strInfo;
    while (ReadFile(hRead1, pbuf, len, &l, NULL))
    {
        if (l == 0) break;
        pbuf[l] = '\0';
        //MessageBox(hwnd, pbuf, "Information",0);  
        //printf("Information2:\n%s\n", pbuf);
        strInfo.append(pbuf);
        len = 1000;
    }

    //MessageBox(hwnd, "ReadFile Exit","Information",0);  
    printf("finish ReadFile buffer = %s\n", strInfo.c_str());
    CloseHandle(hRead1);
    hRead1 = NULL;

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hThread);
    pi.hThread = NULL;
    CloseHandle(pi.hProcess);
    pi.hProcess = NULL;

    if (std::string::npos != strInfo.find("TTL") || std::string::npos != strInfo.find("ttl"))
    {
        return true;
    }
    else
    {
        return false;
    }
}

#ifdef USE_GID_PLUS
bool Tool_Img_ScaleJpg(PBYTE pbSrc, int iSrcLen, PBYTE pbDst, size_t *iDstLen, int iDstWidth, int iDstHeight, int compressQuality)
{
    if (pbSrc == NULL || iSrcLen <= 0)
    {
        return false;
    }
    if (pbDst == NULL || iDstLen == NULL || *iDstLen <= 0)
    {
        return false;
    }
    if (iDstWidth <= 0 || iDstHeight <= 0)
    {
        return false;
    }

    // init gdi+
    ULONG_PTR gdiplusToken = NULL;
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // 创建流
    IStream *pstmp = NULL;
    CreateStreamOnHGlobal(NULL, TRUE, &pstmp);
    if (pstmp == NULL)
    {
        GdiplusShutdown(gdiplusToken);
        gdiplusToken = NULL;
        return false;
    }

    // 初始化流
    LARGE_INTEGER liTemp = { 0 };
    ULARGE_INTEGER uLiZero = { 0 };
    pstmp->Seek(liTemp, STREAM_SEEK_SET, NULL);
    pstmp->SetSize(uLiZero);

    // 将图像放入流中
    ULONG ulRealSize = 0;
    pstmp->Write(pbSrc, iSrcLen, &ulRealSize);

    // 从流创建位图
    Bitmap bmpSrc(pstmp);
    Bitmap bmpDst(iDstWidth, iDstHeight, PixelFormat24bppRGB);

    // 创建画图对象
    Graphics grDraw(&bmpDst);

    // 绘图
    //grDraw.DrawImage(&bmpSrc, 0, 0, bmpSrc.GetWidth(), bmpSrc.GetHeight());
    Rect destRect(0, 0, iDstWidth, iDstHeight);
    grDraw.DrawImage(&bmpSrc, destRect);
    if (Ok != grDraw.GetLastStatus())
    {
        pstmp->Release();
        pstmp = NULL;
        GdiplusShutdown(gdiplusToken);
        gdiplusToken = NULL;
        return false;
    }

    // 创建输出流
    IStream* pStreamOut = NULL;
    if (CreateStreamOnHGlobal(NULL, TRUE, &pStreamOut) != S_OK)
    {
        pstmp->Release();
        pstmp = NULL;
        GdiplusShutdown(gdiplusToken);
        gdiplusToken = NULL;
        return false;
    }

    CLSID jpgClsid;
    Tool_GetEncoderClsid(L"image/jpeg", &jpgClsid);

    // 初始化输出流
    pStreamOut->Seek(liTemp, STREAM_SEEK_SET, NULL);
    pStreamOut->SetSize(uLiZero);

    // 将位图按照JPG的格式保存到输出流中
    int iQuality = compressQuality % 100;
    EncoderParameters encoderParameters;
    encoderParameters.Count = 1;
    encoderParameters.Parameter[0].Guid = EncoderQuality;
    encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
    encoderParameters.Parameter[0].NumberOfValues = 1;
    encoderParameters.Parameter[0].Value = &iQuality;
    bmpDst.Save(pStreamOut, &jpgClsid, &encoderParameters);
    //bmpDst.Save(pStreamOut, &jpgClsid, 0);

    // 获取输出流大小
    bool bRet = false;
    ULARGE_INTEGER libNewPos = { 0 };
    pStreamOut->Seek(liTemp, STREAM_SEEK_END, &libNewPos);      // 将流指针指向结束位置，从而获取流的大小
    if (*iDstLen < (int)libNewPos.LowPart)                     // 用户分配的缓冲区不足
    {
        *iDstLen = libNewPos.LowPart;
        bRet = false;
    }
    else
    {
        pStreamOut->Seek(liTemp, STREAM_SEEK_SET, NULL);                   // 将流指针指向开始位置
        pStreamOut->Read(pbDst, libNewPos.LowPart, &ulRealSize);           // 将转换后的JPG图片拷贝给用户
        *iDstLen = ulRealSize;
        bRet = true;
    }


    // 释放内存
    if (pstmp != NULL)
    {
        pstmp->Release();
        pstmp = NULL;
    }
    if (pStreamOut != NULL)
    {
        pStreamOut->Release();
        pStreamOut = NULL;
    }

    GdiplusShutdown(gdiplusToken);
    gdiplusToken = NULL;

    return bRet;
}
#endif

#ifdef USE_GID_PLUS
int Tool_GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT  num = 0;          // number of image encoders
    UINT  size = 0;         // size of the image encoder array in BYTEs

    ImageCodecInfo* pImageCodecInfo = NULL;

    GetImageEncodersSize(&num, &size);
    if (size == 0)
        return -1;  // Failure

    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == NULL)
        return -1;  // Failure

    GetImageEncoders(num, size, pImageCodecInfo);

    for (UINT j = 0; j < num; ++j)
    {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;  // Success
        }
    }
    free(pImageCodecInfo);
    return -1;  // Failure
}
#endif
//void Tool_ExcuteShellCMD(wchar_t* pChCommand)
//{
//    if (NULL == pChCommand)
//    {
//        return;
//    }
//    ShellExecute(NULL, L"open", L"C:\\WINDOWS\\system32\\cmd.exe", pChCommand, L"", SW_HIDE);
//}

void Tool_ExcuteShellCMD(char* pChCommand)
{
    if (NULL == pChCommand)
    {
        return;
    }
    ShellExecute(NULL, "open", "C:\\WINDOWS\\system32\\cmd.exe", pChCommand, "", SW_HIDE);
}

bool Tool_ExcuteCMDbyCreateProcess(const char* CmdName)
{
    if(CmdName == NULL)
        return false;

    //std::wstring strCmd = Tool_string2wstring(CmdName);
    //wchar_t chCMD[256] = {0};
    // wcscpy_s(chCMD, sizeof(chCMD), strCmd.c_str());

    char chCMD[256] = { 0 };
    strcpy_s(chCMD, sizeof(chCMD), CmdName);

    char pbuf[1024]; // 缓存  
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    si.hStdOutput = NULL; // 设置需要传递到子进程的管道写句柄  

    // 创建子进程，运行ping命令，子进程是可继承的  
#ifdef  UNICODE
    if (!CreateProcess(L"C:\\WINDOWS\\system32\\cmd.exe", chCMD, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
#else
    if (!CreateProcess("C:\\WINDOWS\\system32\\cmd.exe", chCMD, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
#endif //  UNICODE
    {
        //itoa(GetLastError(), pbuf, 10); 
        sprintf_s(pbuf, sizeof(pbuf), "%d", GetLastError());
        //MessageBox(hwnd, pbuf,"Information",0);
        printf("%s\n", pbuf);

        return false;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hThread);
    pi.hThread = NULL;
    CloseHandle(pi.hProcess);
    pi.hProcess = NULL;

    return true;
}

#ifdef USE_GID_PLUS
bool Tool_OverlayStringToImg(unsigned char** pImgsrc, long srcSize,
    unsigned char** pImgDest, long& DestSize,
    const wchar_t* DestString, int FontSize,
    int x, int y, int colorR, int colorG, int colorB,
    int compressQuality)
{
    if (!pImgsrc || !pImgDest || srcSize <= 0 || DestSize <= 0)
    {
        //WriteLog("传入参数为非法值");
        return false;
    }
    if (wcslen(DestString) <= 0 || x < 0 || y < 0)
    {
        //WriteLog("字符串长度为0");
        return false;
    }

    //构造图像
    IStream *pSrcStream = NULL;
    IStream *pDestStream = NULL;
    CreateStreamOnHGlobal(NULL, TRUE, &pSrcStream);
    CreateStreamOnHGlobal(NULL, TRUE, &pDestStream);
    if (!pSrcStream || !pDestStream)
    {
        //WriteLog("流创建失败.");
        return false;
    }
    LARGE_INTEGER liTemp = { 0 };
    pSrcStream->Seek(liTemp, STREAM_SEEK_SET, NULL);
    pSrcStream->Write(*pImgsrc, srcSize, NULL);
    Bitmap bmp(pSrcStream);
    int iImgWith = bmp.GetWidth();
    int iImgHeight = bmp.GetHeight();

    Graphics grp(&bmp);

    SolidBrush brush(Color(colorR, colorG, colorB));
    FontFamily fontFamily(L"宋体");
    //Gdiplus::Font font(&fontFamily, (REAL)FontSize);
    Gdiplus::Font font(&fontFamily, (REAL)FontSize, FontStyleRegular, UnitPixel);

    RectF layoutRect(x, y, iImgWith - x, 0);
    RectF FinalRect;
    INT codePointsFitted = 0;
    INT linesFitted = 0;
    int strLenth = wcslen(DestString);
    grp.MeasureString(DestString, strLenth, &font, layoutRect, NULL, &FinalRect, &codePointsFitted, &linesFitted);
    grp.DrawString(DestString, -1, &font, FinalRect, NULL, &brush);
    Gdiplus::Status iState = grp.GetLastStatus();
    if (iState == Ok)
    {
        //WriteLog("字符叠加成功");
    }
    else
    {
        //char chLog[260] = { 0 };
        //sprintf(chLog, "字符叠加失败， 错误码为%d", iState);
        //WriteLog(chLog);
    }

    pSrcStream->Seek(liTemp, STREAM_SEEK_SET, NULL);
    pDestStream->Seek(liTemp, STREAM_SEEK_SET, NULL);

    // 将位图按照JPG的格式保存到输出流中
    CLSID jpgClsid;
    Tool_GetEncoderClsid(L"image/jpeg", &jpgClsid);
    int iQuality = compressQuality;
    EncoderParameters encoderParameters;
    encoderParameters.Count = 1;
    encoderParameters.Parameter[0].Guid = EncoderQuality;
    encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
    encoderParameters.Parameter[0].NumberOfValues = 1;
    encoderParameters.Parameter[0].Value = &iQuality;
    bmp.Save(pDestStream, &jpgClsid, &encoderParameters);

    ULARGE_INTEGER uiSize;
    pDestStream->Seek(liTemp, STREAM_SEEK_CUR, &uiSize);
    long iFinalSize = (long)uiSize.QuadPart;
    if (iFinalSize <= DestSize)
    {
        pDestStream->Seek(liTemp, STREAM_SEEK_SET, NULL);
        pDestStream->Read(*pImgDest, iFinalSize, NULL);
        DestSize = iFinalSize;
    }
    else
    {
        DestSize = 0;
        if (pSrcStream)
        {
            pSrcStream->Release();
            pSrcStream = NULL;
        }
        if (pDestStream)
        {
            pDestStream->Release();
            pDestStream = NULL;
        }
        //WriteLog("传入空间不足，字符叠加失败");
        return false;
    }

    if (pSrcStream)
    {
        pSrcStream->Release();
        pSrcStream = NULL;
    }
    if (pDestStream)
    {
        pDestStream->Release();
        pDestStream = NULL;
    }
    return true;
}
#endif

bool Tool_GetDataFromAppenedInfo(char *pszAppendInfo, std::string strItemName, char *pszRstBuf, int *piRstBufLen)
{
    if (pszAppendInfo == NULL || piRstBufLen == NULL || *piRstBufLen <= 0)
    {
        return false;
    }

    // <RoadNumber value="0" chnname="车道" />
    // <StreetName value="" chnname="路口名称" />
    try
    {
        std::string strAppendInfo = pszAppendInfo;
        size_t siStart = strAppendInfo.find(strItemName);
        if (siStart == std::string::npos)
        {
            return false;
        }
        siStart = strAppendInfo.find("\"", siStart + 1);
        if (siStart == std::string::npos)
        {
            return false;
        }
        size_t siEnd = strAppendInfo.find("\"", siStart + 1);
        if (siEnd == std::string::npos)
        {
            return false;
        }

        std::string strRst = strAppendInfo.substr(siStart + 1, siEnd - siStart - 1);
        if (*piRstBufLen < (int)strRst.length())
        {
            *piRstBufLen = (int)strRst.length();
            return false;
        }

        strncpy_s(pszRstBuf, *piRstBufLen, strRst.c_str(), (int)strRst.length());
        *piRstBufLen = (int)strRst.length();
        return true;
    }
    catch (std::bad_exception& e)
    {
        LOGFMTE("Tool_GetDataFromAppenedInfo, bad_exception, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::overflow_error& e)
    {
        LOGFMTE("Tool_GetDataFromAppenedInfo, overflow_error, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::domain_error& e)
    {
        LOGFMTE("Tool_GetDataFromAppenedInfo, domain_error, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::length_error& e)
    {
        LOGFMTE("Tool_GetDataFromAppenedInfo, length_error, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::out_of_range& e)
    {
        LOGFMTE("Tool_GetDataFromAppenedInfo, out_of_range, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::range_error& e)
    {
        LOGFMTE("Tool_GetDataFromAppenedInfo, range_error, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::runtime_error& e)
    {
        LOGFMTE("Tool_GetDataFromAppenedInfo, runtime_error, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::logic_error& e)
    {
        LOGFMTE("Tool_GetDataFromAppenedInfo, logic_error, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::bad_alloc& e)
    {
        LOGFMTE("Tool_GetDataFromAppenedInfo, bad_alloc, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::exception& e)
    {
        LOGFMTE("Tool_GetDataFromAppenedInfo, exception, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (void*)
    {
        LOGFMTE("Tool_GetDataFromAppenedInfo,  void* exception, error code = %lu.", GetLastError());
        return false;
    }
    catch (...)
    {
        LOGFMTE("Tool_GetDataFromAppenedInfo,  unknown exception, error code = %lu.", GetLastError());
        return false;
    }
}

bool Tool_GetDataAttributFromAppenedInfo(char *pszAppendInfo, std::string strItemName, std::string  strAttributeName,char *pszRstBuf, int *piRstBufLen)
{
    if (pszAppendInfo == NULL || piRstBufLen == NULL || *piRstBufLen <= 0)
    {
        return false;
    }

    // <RoadNumber value="0" chnname="车道" />
    // <StreetName value="" chnname="路口名称" />
    try
    {
        std::string strAppendInfo = pszAppendInfo;
        size_t siStart = strAppendInfo.find(strItemName);
        if (siStart == std::string::npos)
        {
            return false;
        }
        siStart = strAppendInfo.find(strAttributeName, siStart+1);

        siStart = strAppendInfo.find("\"", siStart + 1);
        if (siStart == std::string::npos)
        {
            return false;
        }
        size_t siEnd = strAppendInfo.find("\"", siStart + 1);
        if (siEnd == std::string::npos)
        {
            return false;
        }

        std::string strRst = strAppendInfo.substr(siStart + 1, siEnd - siStart - 1);
        if (*piRstBufLen < (int)strRst.length())
        {
            *piRstBufLen = (int)strRst.length();
            return false;
        }

        strncpy_s(pszRstBuf, *piRstBufLen, strRst.c_str(), (int)strRst.length());
        *piRstBufLen = (int)strRst.length();
        return true;
    }
    catch (std::bad_exception& e)
    {
        LOGFMTE("Tool_GetDataFromAppenedInfo, bad_exception, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::overflow_error& e)
    {
        LOGFMTE("Tool_GetDataFromAppenedInfo, overflow_error, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::domain_error& e)
    {
        LOGFMTE("Tool_GetDataFromAppenedInfo, domain_error, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::length_error& e)
    {
        LOGFMTE("Tool_GetDataFromAppenedInfo, length_error, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::out_of_range& e)
    {
        LOGFMTE("Tool_GetDataFromAppenedInfo, out_of_range, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::range_error& e)
    {
        LOGFMTE("Tool_GetDataFromAppenedInfo, range_error, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::runtime_error& e)
    {
        LOGFMTE("Tool_GetDataFromAppenedInfo, runtime_error, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::logic_error& e)
    {
        LOGFMTE("Tool_GetDataFromAppenedInfo, logic_error, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::bad_alloc& e)
    {
        LOGFMTE("Tool_GetDataFromAppenedInfo, bad_alloc, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::exception& e)
    {
        LOGFMTE("Tool_GetDataFromAppenedInfo, exception, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (void*)
    {
        LOGFMTE("Tool_GetDataFromAppenedInfo,  void* exception, error code = %lu.", GetLastError());
        return false;
    }
    catch (...)
    {
        LOGFMTE("Tool_GetDataFromAppenedInfo,  unknown exception, error code = %lu.", GetLastError());
        return false;
    }
}


void Tool_ExcuteCMD(char* pChCommand)
{
#ifdef WIN32

    if (NULL == pChCommand)
    {
        return;
    }
#ifdef UNICODE
    std::wstring wstrCommand = Tool_string2wstring(pChCommand);
    ShellExecute(NULL, L"open", L"C:\\WINDOWS\\system32\\cmd.exe", wstrCommand.c_str(), L"", SW_HIDE);
#else
    ShellExecute(NULL, "open", "C:\\WINDOWS\\system32\\cmd.exe", pChCommand, "", SW_HIDE);
#endif // UNICODE

#endif // WIN32
}

std::wstring Tool_string2wstring(std::string strSrc)
{
    std::wstring wstrDst;
    int iWstrLen = MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), strSrc.size(), NULL, 0);
    wchar_t* pwcharBuf = new wchar_t[iWstrLen + sizeof(wchar_t)];   // 多一个结束符
    if (pwcharBuf == NULL || iWstrLen <= 0)
    {
        return L"";
    }
    memset(pwcharBuf, 0, iWstrLen*sizeof(wchar_t)+sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), strSrc.size(), pwcharBuf, iWstrLen);
    pwcharBuf[iWstrLen] = L'\0';
    wstrDst.append(pwcharBuf);
    delete[] pwcharBuf;
    pwcharBuf = NULL;
    return wstrDst;
}

#ifdef USE_MFC
bool Tool_DeleteDirectory(char* strDirName)
{
    CFileFind tempFind;

    char strTempFileFind[MAX_PATH];

    sprintf_s(strTempFileFind, sizeof(strTempFileFind), "%s//*.*", strDirName);

    BOOL IsFinded = tempFind.FindFile(strTempFileFind);

    while (IsFinded)
    {
        IsFinded = tempFind.FindNextFile();

        if (!tempFind.IsDots())
        {
            char strFoundFileName[MAX_PATH];

            //strcpy(strFoundFileName, tempFind.GetFileName().GetBuffer(MAX_PATH));
            strcpy_s(strFoundFileName, tempFind.GetFileName().GetBuffer(MAX_PATH));

            if (tempFind.IsDirectory())
            {
                char strTempDir[MAX_PATH];

                sprintf_s(strTempDir, sizeof(strTempDir), "%s//%s", strDirName, strFoundFileName);

                Tool_DeleteDirectory(strTempDir);
            }
            else
            {
                char strTempFileName[MAX_PATH];

                sprintf_s(strTempFileName, sizeof(strTempFileName), "%s//%s", strDirName, strFoundFileName);

                DeleteFile(strTempFileName);
            }
        }
    }

    tempFind.Close();

    if (!RemoveDirectory(strDirName))
    {
        return FALSE;
    }

    return TRUE;
}

int Tool_CirclelaryDelete(const char* folderPath, int iBackUpDays)
{
    printf("进入环覆盖线程主函数,开始查找制定目录下的文件夹");
    char myPath[MAX_PATH] = { 0 };
    //sprintf(myPath, "%s\\*", folderPath);
    sprintf_s(myPath, sizeof(myPath), "%s\\*", folderPath);

    CTime tmCurrentTime = CTime::GetCurrentTime();
    CTime tmLastMonthTime = tmCurrentTime - CTimeSpan(iBackUpDays, 0, 0, 0);
    int Last_Year = tmLastMonthTime.GetYear();
    int Last_Month = tmLastMonthTime.GetMonth();
    int Last_Day = tmLastMonthTime.GetDay();
    //cout<<Last_Year<<"-"<<Last_Month<<"-"<<Last_Day<<endl;

    CFileFind myFileFind;
    BOOL bFinded = myFileFind.FindFile(myPath);
    char DirectoryName[MAX_PATH] = { 0 };
    while (bFinded)
    {
        bFinded = myFileFind.FindNextFileA();
        if (!myFileFind.IsDots())
        {
            sprintf_s(DirectoryName, sizeof(DirectoryName), "%s", myFileFind.GetFileName().GetBuffer());
            if (myFileFind.IsDirectory())
            {
                int iYear, iMonth, iDay;
                iYear = iMonth = iDay = 0;
                //sscanf(DirectoryName,"%d-%d-%d",&iYear, &iMonth, &iDay);
                sscanf_s(DirectoryName, "%d-%d-%d", &iYear, &iMonth, &iDay);
                if (iYear == 0 && iMonth ==0 && iDay == 0)
                {
                    continue;
                }
                if (iYear < Last_Year)
                {
                    sprintf_s(DirectoryName, sizeof(DirectoryName), "%s\\%s", folderPath, myFileFind.GetFileName().GetBuffer());
                    printf("delete the DirectoryB :%s\n", DirectoryName);
                    Tool_DeleteDirectory(DirectoryName);

                    char chLog[MAX_PATH] = { 0 };
                    sprintf_s(chLog, sizeof(chLog), "年份小于当前年份，删除文件夹%s", DirectoryName);
                    printf(chLog);
                }
                else if (iYear == Last_Year)
                {
                    if (iMonth < Last_Month)
                    {
                        sprintf(DirectoryName, sizeof(DirectoryName), "%s\\%s", folderPath, myFileFind.GetFileName().GetBuffer());
                        printf("delete the DirectoryB :%s\n", DirectoryName);
                        Tool_DeleteDirectory(DirectoryName);

                        char chLog[MAX_PATH] = { 0 };
                        sprintf_s(chLog, sizeof(chLog), "月份小于上一月，删除文件夹%s", DirectoryName);
                        printf(chLog);
                    }
                    else if (iMonth == Last_Month)
                    {
                        if (iDay < Last_Day)
                        {
                            sprintf_s(DirectoryName, sizeof(DirectoryName), "%s\\%s", folderPath, myFileFind.GetFileName().GetBuffer());
                            printf("delete the DirectoryB :%s\n", DirectoryName);
                            Tool_DeleteDirectory(DirectoryName);

                            char chLog[MAX_PATH] = { 0 };
                            sprintf_s(chLog, sizeof(chLog), "日号小于指定天数，删除文件夹%s", DirectoryName);
                            printf(chLog);
                        }
                    }
                }
            }
        }
    }
    myFileFind.Close();
    printf("查询结束，退出环覆盖线程主函数..");
    return 0;
}

#endif

int Tool_SafeCloseThread(HANDLE& threadHandle)
{
    if (threadHandle == NULL)
    {
        return -1;
    }
    MSG msg;
    DWORD dwRet = -1;
    while (NULL != threadHandle && WAIT_OBJECT_0 != dwRet) // INFINITE
    {
        dwRet = MsgWaitForMultipleObjects(1, &threadHandle, FALSE, 100, QS_ALLINPUT);
        if (dwRet == WAIT_OBJECT_0 + 1)
        {
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
    CloseHandle(threadHandle);
    threadHandle = NULL;
    return 0;
}

const char* Tool_GetCurrentPath()
{
    //static TCHAR szPath[256] = { 0 };
    //if (strlen(szPath) <= 0)
//    static wchar_t szPath[256] = { 0 };
//    if (wcslen(szPath) <= 0)
//    {
//        GetModuleFileName(NULL, szPath, MAX_PATH - 1);
//        PathRemoveFileSpec(szPath);
//    }
    static CHAR szPath[256] = { 0 };
    //getcwd(szPath,sizeof(szPath));
    memset(szPath, '\0', sizeof(szPath));
    _getcwd(szPath, sizeof(szPath));
    return szPath;
}

#ifdef USE_MSVC
SYSTEMTIME Tool_GetCurrentTime()
{
    SYSTEMTIME systime;
    GetLocalTime(&systime);//本地时间
    return systime;
}
#endif

bool Tool_DimCompare(const char *szSrcPlateNo, const char *szDesPlateNo)
{

    if (!szSrcPlateNo || !szDesPlateNo)
        return false;

    if (strstr(szSrcPlateNo, "无") || strstr(szDesPlateNo, "无"))
    {
        printf("Info: NoPlate not Compare!!!!!!!!!!");
        return false;
    }
    char chLog[MAX_PATH] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "DimCompare(%s, %s)", szSrcPlateNo, szDesPlateNo);
    printf(chLog);
    //获取6字节标准车牌
    char strStandardCarChar[10] = { 0 };
    int  nPlateNo = strlen(szSrcPlateNo);
    if (nPlateNo > 10)
    {
        printf("Error: szPlateNo!!!!!!!!!!");
        return false;
    }
    else if (nPlateNo > 6)
    {
        strcpy_s(strStandardCarChar, szSrcPlateNo + 2);
        //WriteLog("strcpy iCurPlateNo > 6 ");
    }
    else
    {
        strcpy_s(strStandardCarChar, szSrcPlateNo);
        //WriteLog("strcpy iCurPlateNo <= 6 ");
    }

    int iMaxMatchCnt = 0;
    int iMaxMatchRate = 0;
    int iStandardPlateLen = strlen(strStandardCarChar);//比对车牌长度
    int i = 0;
    int j = 0;
    int nFlagCompare = -1;
    char strComparePlateChar[10] = { 0 };
    int  iComparePlateLen = 0;
    int  iCurPlateNo = 0;

    iCurPlateNo = strlen(szDesPlateNo);
    if (iCurPlateNo > 10)
    {
        printf("Error: szPlateNo!!!!!!!!!!");
        return false;
    }
    else if (iCurPlateNo > 6)
    {
        strcpy_s(strComparePlateChar, szDesPlateNo + 2);
        //WriteLog("strcpy iCurPlateNo > 6 ");
    }
    else
    {
        strcpy_s(strComparePlateChar, szDesPlateNo);
        //WriteLog("strcpy iCurPlateNo <= 6 ");
    }

    //取出list中需要作对比的牌号字符数组
    iComparePlateLen = strlen(strComparePlateChar);//list选中车牌长度

    //取少位数的来遍历,同位匹配
    int iLoopTimes = iComparePlateLen < iStandardPlateLen ? iComparePlateLen : iStandardPlateLen;
    int iEqualCount = 0;
    for (j = 0; j < iLoopTimes; j++)
    {
        if (strComparePlateChar[j] == strStandardCarChar[j])
        {
            ++iEqualCount; //匹配数
        }
    }

    if (iEqualCount >= 5 && iEqualCount > iMaxMatchCnt) //车牌匹配5个或以上，算是匹配了，但仍然循环完，已查找最佳匹配率的记录
    {
        iMaxMatchCnt = iEqualCount;

        int iDenominator = iComparePlateLen > iStandardPlateLen ? iComparePlateLen : iStandardPlateLen;
        int iMatchRate = (int)(iMaxMatchCnt * 100 / iDenominator);

        if (iMatchRate > iMaxMatchRate)
        {
            iMaxMatchRate = iMatchRate;
            nFlagCompare = i;
        }
    }

    //同位匹配5个以上直接认为是同个车牌
    if (nFlagCompare != -1)
    {
        printf("nFlagCompare != -1（同位匹配5个以上直接认为是同个车牌） return true.");
        return true;
    }

    //同位匹配不上,只能继续错位匹配
    iMaxMatchCnt = 0;
    iMaxMatchRate = 0;
    nFlagCompare = -1;
    iLoopTimes = 4; //错位比较中间连续的4位
    if (iStandardPlateLen < 6 || iComparePlateLen < 6)
    {
        printf("iStandardPlateLen < 6 or iComparePlateLen < 6");
        return false;
    }

    //A12345
    //假如车辆有三辆，分别为粤G15678、川B23456、云C12456
    int iEqualCount1 = 0, iEqualCount2 = 0;
    bool bCompare = false;
    for (j = 0; j < iLoopTimes; j++)
    {
        // A1234与15678、23456、12456比较,后5位与主的前5位比较,但中间连续4位必须全匹配
        if (strComparePlateChar[j + 2] == strStandardCarChar[j + 1])
        {
            ++iEqualCount1; //匹配数
            if (iEqualCount1 == iLoopTimes) bCompare = true;
        }

        // 12345与G15678、B23456、C12456比较,前5位与主的后5位比较,但中间连续4位必须全匹配
        if (strComparePlateChar[j] == strStandardCarChar[j + 1])
        {
            ++iEqualCount2; //匹配数
            if (iEqualCount2 == iLoopTimes) bCompare = true;
        }
    }
    if (strComparePlateChar[1] == strStandardCarChar[0])
        ++iEqualCount1;
    if (strComparePlateChar[4] == strStandardCarChar[5])
        ++iEqualCount2;


    //车牌匹配4个或以上，算是匹配了，但仍然循环完，已查找最佳匹配率的记录
    if ((iEqualCount1 >= 4 && iEqualCount1 > iMaxMatchCnt && bCompare)
        || (iEqualCount2 >= 4 && iEqualCount2 > iMaxMatchCnt && bCompare))
    {
        iMaxMatchCnt = iEqualCount1 > iEqualCount2 ? iEqualCount1 : iEqualCount2;

        int iDenominator = iComparePlateLen > iStandardPlateLen ? iComparePlateLen : iStandardPlateLen;
        int iMatchRate = (int)(iMaxMatchCnt * 100 / iDenominator);

        if (iMatchRate > iMaxMatchRate)
        {
            iMaxMatchRate = iMatchRate;
            nFlagCompare = i;
        }
    }
    if (nFlagCompare == -1)
    {
        printf("DimCompare failed.\n");
        return false;
    }
    else
    {
        printf("DimCompare success.\n");
        return true;
    }
}

void Tool_SetLogPath(const char* path)
{
    memset(g_chLogPath, '\0', sizeof(g_chLogPath));
    if (path != NULL
        && strlen(path) < sizeof(g_chLogPath))
    {
        //printf("Tool_SetLogPath(%s) \n", path);
        //strcpy(g_chLogPath, path);
        strcpy_s(g_chLogPath, path);
    }
    else
    {
        //printf("Tool_SetLogPath() something wrong strlen(path) = %d, sizeof(g_chLogPath) = %d \n",  strlen(path), sizeof(g_chLogPath));
    }
}

void Tool_WriteLog(const char* chlog)
{
    //取得当前的精确毫秒的时间
    SYSTEMTIME systime;
    GetLocalTime(&systime);//本地时间

	char chLogPath[512] = { 0 };

	static char chLogRoot[256] = { 0 };
	if (strlen(chLogRoot) > 0)
	{
		sprintf_s(chLogPath, sizeof(chLogPath), "%s\\%04d-%02d-%02d\\",
			chLogRoot,
			systime.wYear,
			systime.wMonth,
			systime.wDay);
	}
	else
	{
		Tool_ReadKeyValueFromConfigFile(INI_FILE_NAME, "Log", "Path", chLogRoot, sizeof(chLogRoot));
		if (strlen(chLogRoot) <= 0)
		{
			sprintf_s(chLogRoot, sizeof(chLogRoot), "%s\\XLWLog\\", Tool_GetCurrentPath());
		}

		sprintf_s(chLogPath, sizeof(chLogPath), "%s\\%04d-%02d-%02d\\",
			chLogRoot,
			systime.wYear,
			systime.wMonth,
			systime.wDay);
	}
	Tool_MakeDir(chLogPath);

    char chLogFileName[512] = { 0 };
    //sprintf_s(chLogFileName, "%s\\CameraLog-%d-%02d_%02d.log",chLogPath, pTM->tm_year + 1900, pTM->tm_mon+1, pTM->tm_mday);
    sprintf_s(chLogFileName, sizeof(chLogFileName), "%s\\%s%04d%02d%02d.log", 
        chLogPath, 
        DLL_LOG_NAME,
        systime.wYear,
        systime.wMonth,
        systime.wDay);

    FILE *file = NULL;
    //file = fopen(chLogFileName, "a+");
    fopen_s(&file, chLogFileName, "a+");
    if (file)
    {
        fprintf(file, "%04d-%02d-%02d %02d:%02d:%02d:%03d[Version: %s] [thread ID: %lu] : %s\n",
            systime.wYear,
            systime.wMonth,
            systime.wDay,
            systime.wHour,
            systime.wMinute,
            systime.wSecond,
            systime.wMilliseconds,
            DLL_VERSION,
            GetCurrentThreadId(),
            chlog);
        fclose(file);
        file = NULL;
    }
}

void Tool_WriteLog_Ex(const char* chlog)
{
    //取得当前的精确毫秒的时间
    SYSTEMTIME systime;
    GetLocalTime(&systime);//本地时间

    char chLogPath[512] = { 0 };
    char chLogRoot[256] = { 0 };

    if (strlen(g_chLogPath) <= 0)
    {
        Tool_ReadKeyValueFromConfigFile(INI_FILE_NAME, "Log", "Path", chLogRoot, sizeof(chLogRoot));
        if (strlen(chLogRoot) > 0)
        {
            sprintf_s(chLogPath, sizeof(chLogPath), "%s\\XLWLog\\%04d-%02d-%02d\\",
                chLogRoot,
                systime.wYear,
                systime.wMonth,
                systime.wDay);
        }
        else
        {
            sprintf_s(chLogPath, sizeof(chLogPath), "%s\\XLWLog\\%04d-%02d-%02d\\",
                Tool_GetCurrentPath(),
                systime.wYear,
                systime.wMonth,
                systime.wDay);
        }
    }
    else
    {
        sprintf_s(chLogPath, sizeof(chLogPath), "%s\\XLWLog\\%04d-%02d-%02d\\",
            g_chLogPath,
            systime.wYear,
            systime.wMonth,
            systime.wDay);
    }  
    Tool_MakeDir(chLogPath);

    //每次只保留10天以内的日志文件
    time_t now = time(NULL);
    tm* ts = localtime(&now);
    ts->tm_mday = ts->tm_mday - 30;
    mktime(ts); /* Normalise ts */
    int Last_Year = ts->tm_year + 1900;
    int Last_Month = ts->tm_mon + 1;
    int Last_Day = ts->tm_wday;

    char chOldLogFileName[MAX_PATH] = { 0 };
    //sprintf_s(chOldLogFileName, "%s\\XLWLog\\%04d-%02d-%02d\\",szFileName, Last_Year, Last_Month, Last_Day);
    sprintf_s(chOldLogFileName, sizeof(chOldLogFileName), "%s\\XLWLog\\%04d-%02d-%02d\\",
        Tool_GetCurrentPath(),
        Last_Year,
        Last_Month,
        Last_Day);

    if (Tool_IsDirExist(chOldLogFileName))
    {
        char chCommand[512] = { 0 };
        //sprintf_s(chCommand, "/c rd /s/q %s", chOldLogFileName);
        sprintf_s(chCommand, sizeof(chCommand), "/c rd /s/q %s", chOldLogFileName);
        Tool_ExcuteCMD(chCommand);
    }

    char chLogFileName[512] = { 0 };
    //sprintf_s(chLogFileName, "%s\\CameraLog-%d-%02d_%02d.log",chLogPath, pTM->tm_year + 1900, pTM->tm_mon+1, pTM->tm_mday);
    sprintf_s(chLogFileName, sizeof(chLogFileName), "%s\\%s", chLogPath, DLL_LOG_NAME);

    FILE *file = NULL;
    //file = fopen(chLogFileName, "a+");
    fopen_s(&file, chLogFileName, "a+");
    if (file)
    {
        fprintf(file, "%04d-%02d-%02d %02d:%02d:%02d:%03d[Version: %s] [thread ID: %lu] : %s\n",
            systime.wYear,
            systime.wMonth,
            systime.wDay,
            systime.wHour,
            systime.wMinute,
            systime.wSecond,
            systime.wMilliseconds,
            DLL_VERSION,
            GetCurrentThreadId(),
            chlog);
        fclose(file);
        file = NULL;
    }
}

void Tool_WriteFormatLog(const char* szfmt, ...)
{
    char g_szPbString[1024] = { 0 };
    memset(g_szPbString, '\0', sizeof(g_szPbString));

    va_list arg_ptr;
    va_start(arg_ptr, szfmt);
    vsnprintf_s(g_szPbString, sizeof(g_szPbString), szfmt, arg_ptr);
    va_end(arg_ptr);

    Tool_WriteLog(g_szPbString);

    //Tool_WriteLog_Ex(g_szPbString);
}

bool Tool_SaveFileToPath(const char* szPath, void* fileData, size_t fileSize)
{
    //LOGFMTD("begin Tool_SaveFileToPath");
    if (NULL == fileData || NULL == szPath)
    {
        LOGFMTE("Tool_SaveFileToPath, failed.NULL == pImgData || NULL == chImgPath");
        return false;
    }
    char chLogBuff[MAX_PATH] = { 0 };
    bool bRet = false;

    if (NULL != strstr(szPath, "\\") || NULL != strstr(szPath, "/"))
    {
        std::string tempFile(szPath);
        size_t iPosition = std::string::npos;
        if (NULL != strstr(szPath, "\\"))
        {
            iPosition = tempFile.rfind("\\");
        }
        else
        {
            iPosition = tempFile.rfind("/");
        }
        std::string tempDir = tempFile.substr(0, iPosition + 1);
        if (!MakeSureDirectoryPathExists(tempDir.c_str()))
        {
            memset(chLogBuff, '\0', sizeof(chLogBuff));
            //sprintf_s(chLogBuff, "%s save failed", chImgPath);
            sprintf_s(chLogBuff, sizeof(chLogBuff), "%s save failed, create path failed.", szPath);
            LOGFMTE(chLogBuff);
            return false;
        }
    }

    size_t iWritedSpecialSize = 0;
    FILE* fp = NULL;
    //fp = fopen(chImgPath, "wb+");
    errno_t errCode;
    _set_errno(0);
    errCode = fopen_s(&fp, szPath, "wb+");
    if (fp)
    {
        //iWritedSpecialSize = fwrite(pImgData, dwImgSize , 1, fp);
        iWritedSpecialSize = fwrite(fileData, sizeof(BYTE), fileSize, fp);
        fflush(fp);
        fclose(fp);
        fp = NULL;
        bRet = true;
    }
    else
    {
        memset(chLogBuff, '\0', sizeof(chLogBuff));
        //sprintf_s(chLogBuff, "%s save failed", chImgPath);
        sprintf_s(chLogBuff, sizeof(chLogBuff), "%s open failed, error code = %d", szPath, errCode);
        LOGFMTE(chLogBuff);
    }
    if (iWritedSpecialSize == fileSize)
    {
        memset(chLogBuff, '\0', sizeof(chLogBuff));
        //sprintf_s(chLogBuff, "%s save success", chImgPath);
        sprintf_s(chLogBuff, sizeof(chLogBuff), "%s save success", szPath);
        LOGFMTD(chLogBuff);
    }
    else
    {
        memset(chLogBuff, '\0', sizeof(chLogBuff));
        //sprintf_s(chLogBuff, "%s save success", chImgPath);
        _get_errno(&errCode);
        sprintf_s(chLogBuff, sizeof(chLogBuff), "%s write no match, size = %lu, write size = %lu, error code = %d.",
            szPath,
            fileSize,
            iWritedSpecialSize,
            errCode);
        LOGFMTE(chLogBuff);
    }

    //LOGFMTD("end SaveImgToDisk");
    return bRet;
}

std::list<std::string> getFilesPath(const std::string& cate_dir, const std::string& filter)
{
    std::list<std::string> strFilesList;//存放文件名
    std::string strDir = cate_dir + filter;
#ifdef WIN32
    _finddata_t file;
    long lf;
    //输入文件夹路径
    if ((lf = _findfirst(strDir.c_str(), &file)) == -1)
    {
        std::cout << strDir << " not found!!!" << std::endl;
    }
    else
    {
        std::string strPath = cate_dir;
        do
        {
            //输出文件名
            //cout<<file.name<<endl;
            if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0 || strlen(file.name) <= 0)
                continue;
            strFilesList.push_back(strPath + "\\" + file.name);
        } while (_findnext(lf, &file) == 0);
    }
    _findclose(lf);
#endif

#ifdef linux
    DIR *dir;
    struct dirent *ptr;
    char base[1000];

    if ((dir = opendir(cate_dir.c_str())) == NULL)
    {
        perror("Open dir error...");
        exit(1);
    }

    while ((ptr = readdir(dir)) != NULL)
    {
        if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)    ///current dir OR parrent dir
            continue;
        else if (ptr->d_type == 8)    ///file
            //printf("d_name:%s/%s\n",basePath,ptr->d_name);
            strFilesList.push_back(ptr->d_name);
        else if (ptr->d_type == 10)    ///link file
            //printf("d_name:%s/%s\n",basePath,ptr->d_name);
            continue;
        else if (ptr->d_type == 4)    ///dir
        {
            strFilesList.push_back(ptr->d_name);
            /*
            memset(base,'\0',sizeof(base));
            strcpy(base,basePath);
            strcat(base,"/");
            strcat(base,ptr->d_nSame);
            readFileList(base);
            */
        }
    }
    closedir(dir);
#endif

    //排序，按从小到大排序
    //std::sort(strFilesList.begin(), strFilesList.end());
    return strFilesList;
}

bool Tool_LoadFile(const char* fileName, void* pBuffer, size_t& inputOutputFileSize)
{
    if (NULL == fileName || NULL == pBuffer)
    {
        printf("Tool_LoadFile, NULL == fileName || NULL == pBuffer.\n");
        return false;
    }

    try
    {
        if (!Tool_IsFileExist(fileName))
        {
            printf("Tool_LoadFile, File not Exist.\n");
            return false;
        }

        size_t iFileSize = Tool_GetFileSize(fileName);

        if (iFileSize <= 0 || iFileSize > inputOutputFileSize)
        {
            printf("Tool_LoadFile, iFileSize= %d,  iFileSize <= 0 || iFileSize > inputFileSize \n", iFileSize);
            return false;
        }
        inputOutputFileSize = iFileSize;

        FILE* pFile = NULL;
        errno_t errCode;
        _set_errno(0);
        errCode = fopen_s(&pFile, fileName, "rb");
        if (NULL != pFile)
        {
            fread(pBuffer, 1, iFileSize, pFile);
            fclose(pFile);
            pFile = NULL;

            return true;
        }
        else
        {
            LOGFMTE("Tool_LoadFile, open file failed, error code = %d.\n", errCode);
            return false;
        }
    }
    catch (std::bad_exception& e)
    {
        LOGFMTE("Tool_LoadFile, bad_exception, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::domain_error& e)
    {
        LOGFMTE("Tool_LoadFile, domain_error, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::length_error& e)
    {
        LOGFMTE("Tool_LoadFile, length_error, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::range_error& e)
    {
        LOGFMTE("Tool_LoadFile, range_error, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::out_of_range& e)
    {
        LOGFMTE("Tool_LoadFile, out_of_range, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::overflow_error& e)
    {
        LOGFMTE("Tool_LoadFile, overflow_error, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::logic_error& e)
    {
        LOGFMTE("Tool_LoadFile, logic_error, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::runtime_error& e)
    {
        LOGFMTE("Tool_LoadFile, runtime_error, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::bad_alloc& e)
    {
        LOGFMTE("Tool_LoadFile, bad_alloc, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (std::exception& e)
    {
        LOGFMTE("Tool_LoadFile, exception, error msg = %s, errorcode = %lu.", e.what(), GetLastError());
        return false;
    }
    catch (void*)
    {
        LOGFMTE("Tool_LoadFile,  void* exception, error code = %lu.", GetLastError());
        return false;
    }
    catch (...)
    {
        LOGFMTE("Tool_LoadFile,  unknown exception, error code = %lu.", GetLastError());
        return false;
    }
}

bool Tool_LoadCamerXml(const char* fileName, std::map<std::string, int>& myMap)
{
    //加载的XML格式如下
    //< ? xml version = "1.0" encoding = "GB2312" standalone = "yes" ? >
    //    <Camera>
    //    <IpAddress>172.18.1.2< / IpAddress>
    //    <Index>0 < / Index >
    //    < / Camera>
    //    <Camera>
    //    <IpAddress>172.18.1.3< / IpAddress>
    //    <Index>3 < / Index >
    //    < / Camera>

    TiXmlDocument cXmlDoc;
    TiXmlElement* pRootElement = NULL;

    if (!cXmlDoc.LoadFile(fileName))
    {
        LOGFMTE("Tool_LoadCamerXml::parse XML file failed \n");
        return false;
    }
    pRootElement = cXmlDoc.RootElement();
    if (NULL == pRootElement)
    {
        LOGFMTE("Tool_LoadCamerXml::no have root Element\n");
        return false;
    }
    LOGFMTD("Tool_LoadCamerXml::root element text = %s.\n", pRootElement->Value());
    const TiXmlNode* pNodeCamera = pRootElement;
    do
    {
        if (0 == strcmp(pNodeCamera->Value(), "Camera"))
        {
            const TiXmlNode* pNode = pNodeCamera->FirstChild();
            std::string strKey;
            while (pNode)
            {
                LOGFMTD("Tool_LoadCamerXml::root element value = %s.\n", pNode->Value());
                if (0 == strcmp(pNode->Value(), "IpAddress"))
                {
                    LOGFMTD("Tool_LoadCamerXml::root element FirstChild()->Value() = %s.\n", pNode->FirstChild()->Value());
                    strKey = pNode->FirstChild()->Value();
                    myMap.insert(std::make_pair(strKey, 0));
                }
                if (0 == strcmp(pNode->Value(), "Index"))
                {
                    myMap[strKey] = atoi(pNode->FirstChild()->Value());
                }
                pNode = pNode->NextSibling();
            }
        }
        pNodeCamera = pNodeCamera->NextSibling();
    } while (pNodeCamera);
    LOGFMTD("Tool_LoadCamerXm finish.");
    return true;
}

void Tool_DeleteFileByCMD(const char* chFileName)
{
    if (NULL == chFileName || strlen(chFileName) <= 0)
    {
        return;
    }
    static char szCMD[1024] = {0};
    memset(szCMD, '\0', sizeof(szCMD));
    sprintf_s(szCMD, sizeof(szCMD), "/c del /f /s %s", chFileName);

    Tool_ExcuteCMDbyCreateProcess(szCMD);
}

bool Tool_FindMapAndGetValue(std::map<std::string, int>& myMap, std::string keyName, int& value)
{
    if (myMap.size() <= 0 || keyName.size() <= 0)
    {
        return false;
    }

    for (std::map<std::string, int>::const_iterator it = myMap.begin(); it != myMap.end(); it++)
    {
        if (std::string::npos != it->first.find(keyName))
        {
            value = it->second;
            return true;
        }
    }
    return false;
}


bool Tool_IsDirExist(const char *Dir)
{
    if(NULL == Dir)
    {
        return false;
    }

    if (-1  != _access(Dir, 0))
    {
        return true;
    }
    return false;
}

//std::string Tool_GetSoftVersion(const char *exepath)
//{
//    std::string strVersionInfo;
//    if (!exepath)
//        return strVersionInfo;
//    if (_access(exepath, 0) != 0)
//        return strVersionInfo;
//
//    HMODULE hDll = NULL;
//    char szDbgHelpPath[MAX_PATH];
//    //sprintf_s( szDbgHelpPath,sizeof(szDbgHelpPath),  "Api-ms-win-core-version-l1-1-0.dll");
//	sprintf_s(szDbgHelpPath, sizeof(szDbgHelpPath), "version.dll");
//    hDll = ::LoadLibraryA( szDbgHelpPath );
//	if (hDll == NULL)
//	{
//		Tool_WriteFormatLog("load version.dll failed :: %s", Tool_GetLastErrorAsString().c_str());
//		return strVersionInfo;
//	}
//
//    typedef DWORD (WINAPI *func_GetFileVersionInfoSizeA)(LPCSTR ,LPDWORD );
//    func_GetFileVersionInfoSizeA p_GetFileVersionInfoSizeA =
//        (func_GetFileVersionInfoSizeA)::GetProcAddress( hDll, "GetFileVersionInfoSizeA" );
//	if (p_GetFileVersionInfoSizeA == NULL)
//	{
//		Tool_WriteFormatLog("load GetFileVersionInfoSizeA failed :: %s", Tool_GetLastErrorAsString().c_str());
//	}
//
//    typedef DWORD (WINAPI *func_GetFileVersionInfoA)(LPCSTR , DWORD  ,  DWORD , LPVOID );
//    func_GetFileVersionInfoA p_GetFileVersionInfoA =
//        (func_GetFileVersionInfoA)::GetProcAddress( hDll, "GetFileVersionInfoA" );
//	if (p_GetFileVersionInfoA == NULL)
//	{
//		Tool_WriteFormatLog("load GetFileVersionInfoA failed :: %s", Tool_GetLastErrorAsString().c_str());
//	}
//
//    typedef DWORD (WINAPI *func_VerQueryValueA)(LPCVOID ,  LPCSTR  , LPVOID  *,  PUINT );
//    func_VerQueryValueA p_VerQueryValueA =
//        (func_VerQueryValueA)::GetProcAddress( hDll, "VerQueryValueA" );
//	if (p_VerQueryValueA == NULL)
//	{
//		Tool_WriteFormatLog("load VerQueryValueA failed :: %s", Tool_GetLastErrorAsString().c_str());
//	}
//
//    if(p_GetFileVersionInfoSizeA == NULL
//            || p_GetFileVersionInfoA == NULL
//            || p_VerQueryValueA == NULL)
//    {
//		Tool_WriteLog("get dll api failed.");
//        if(hDll)
//        {
//            FreeLibrary(hDll);
//            hDll = NULL;
//        }
//        LOGFMTE("Tool_GetSoftVersion , GetProcAddress failed.");
//        return strVersionInfo;
//    }
//
//    UINT infoSize = p_GetFileVersionInfoSizeA(exepath, 0);
//    if (infoSize != 0) {
//        strVersionInfo.resize(infoSize, 0);
//        char *pBuf = NULL;
//        pBuf = new char[infoSize];
//        VS_FIXEDFILEINFO *pVsInfo;
//        if (p_GetFileVersionInfoA(exepath, 0, infoSize, pBuf)) {
//            if (p_VerQueryValueA(pBuf, "\\", (void **)&pVsInfo, &infoSize))
//            {
//                //sprintf(pBuf, "%d.%d.%d.%d", HIWORD(pVsInfo->dwFileVersionMS), LOWORD(pVsInfo->dwFileVersionMS), HIWORD(pVsInfo->dwFileVersionLS), LOWORD(pVsInfo->dwFileVersionLS));
//                sprintf_s(pBuf, infoSize,"%d.%d.%d.%d", HIWORD(pVsInfo->dwFileVersionMS), LOWORD(pVsInfo->dwFileVersionMS), HIWORD(pVsInfo->dwFileVersionLS), LOWORD(pVsInfo->dwFileVersionLS));
//                strVersionInfo = pBuf;
//            }
//        }
//        delete[] pBuf;
//    }
//
//    if(hDll)
//    {
//        FreeLibrary(hDll);
//    }
//    return strVersionInfo;
//}

std::string Tool_GetSoftVersion(const char *exepath)
{
	std::string strVersionInfo;
	if (!exepath)
		return strVersionInfo;
	if (_access(exepath, 0) != 0)
		return strVersionInfo;


	UINT infoSize = GetFileVersionInfoSizeA(exepath, 0);
	if (infoSize != 0) {
		strVersionInfo.resize(infoSize, 0);
		char *pBuf = NULL;
		pBuf = new char[infoSize];
		VS_FIXEDFILEINFO *pVsInfo;
		if (GetFileVersionInfoA(exepath, 0, infoSize, pBuf)) {
			if (VerQueryValueA(pBuf, "\\", (void **)&pVsInfo, &infoSize))
			{
				//sprintf(pBuf, "%d.%d.%d.%d", HIWORD(pVsInfo->dwFileVersionMS), LOWORD(pVsInfo->dwFileVersionMS), HIWORD(pVsInfo->dwFileVersionLS), LOWORD(pVsInfo->dwFileVersionLS));
				sprintf_s(pBuf, infoSize, "%d.%d.%d.%d", 
					HIWORD(pVsInfo->dwFileVersionMS),
					LOWORD(pVsInfo->dwFileVersionMS), 
					HIWORD(pVsInfo->dwFileVersionLS),
					LOWORD(pVsInfo->dwFileVersionLS));
				strVersionInfo = pBuf;
			}
		}
		delete[] pBuf;
	}

	return strVersionInfo;
}

int Tool_AnalysisPlateColorNo(const char *szPlateNo)
{
    if (szPlateNo == NULL
        || strlen(szPlateNo) <= 1
        )
    {
        return COLOR_UNKNOW;
    }
    else
    {
        if (NULL != strstr(szPlateNo, "蓝")
            || NULL != strstr(szPlateNo, "白")
            )
        {
            if (NULL != strstr(szPlateNo, "蓝")
                && NULL != strstr(szPlateNo, "白")
                )
            {
                return COLOR_BLUE_WHITE;
            }
            if (NULL != strstr(szPlateNo, "蓝"))
            {
                return COLOR_BLUE;
            }
            if (NULL != strstr(szPlateNo, "白"))
            {
                return COLOR_WHITE;
            }
            return COLOR_BLUE;
        }
        else if (NULL != strstr(szPlateNo, "黄"))
        {
            if (NULL != strstr(szPlateNo, "绿"))
            {
                return COLOR_YELLOW_GREEN;
            }
            else
            {
                return COLOR_YELLOW;
            }
        }
        else if (NULL != strstr(szPlateNo, "黑"))
        {
            return COLOR_BLACK;
        }
        else if (NULL != strstr(szPlateNo, "绿"))
        {
            char chLast = szPlateNo[strlen(szPlateNo) - 1];
            int iChar = toupper((int)chLast);
            if (iChar == 'D'
                || iChar == 'F'
                )
            {
                return COLOR_YELLOW_GREEN;
            }
            else
            {
                return COLOR_GRADIENT_CREEN;
            }
        }
        else
        {
            return COLOR_UNKNOW;
        }
    }
}

bool Tool_ProcessPlateNo(const char* pSrcPlateNum, char* destPlateNum, int bufLen)
{
    if (destPlateNum == NULL
        || bufLen<= 0)
    {
        return false;
    }
    if (pSrcPlateNum == NULL)
    {
        sprintf_s(destPlateNum, bufLen, "无车牌");
        return true;
    }

    if (NULL == strstr(pSrcPlateNum, "无"))
    {
        const char* pPlate = pSrcPlateNum;
        if (strlen(pPlate) > 0
            && strlen(pPlate) <= 11)
        {
            sprintf_s(destPlateNum, bufLen, "%s", pPlate + 2);
        }
        else if (strlen(pPlate) > 11)
        {
            sprintf_s(destPlateNum, bufLen, "%s", pPlate + 4);
        }
        else
        {
            sprintf_s(destPlateNum, bufLen, "无车牌");
        }
    }
    else
    {
        sprintf_s(destPlateNum, bufLen, "无车牌");
    }
    return true;
}

int Tool_FindSubStrCount(const char* srcText, const char* subText)
{
    if (srcText == NULL
        || subText == NULL)
    {
        return 0;
    }
    int iCount = 0;
    const char* pText = strstr(srcText, subText);;
    while (pText != NULL)
    {
        iCount++;

        pText++;
        pText = strstr(pText, subText);
    }
    return iCount;
}

void Tool_SetDllDirPath(const char* dirPath)
{
    if (dirPath != NULL
        && strlen(dirPath) < sizeof(g_chDllPath) )
    {
        memset(g_chDllPath, '\0', sizeof(g_chDllPath));
        memcpy(g_chDllPath, dirPath, strlen(dirPath));
    }
}

const char* Tool_GetDllDirPath()
{
    if (strlen(g_chDllPath) > 0)
    {
        return g_chDllPath;
    }
    return NULL;
}

std::string Tool_ReplaceStringInStd(std::string strOrigin, std::string strToReplace, std::string strNewChar)
{
    std::string str = strOrigin;
    for (std::string::size_type pos(0); pos != std::string::npos; pos += strNewChar.length())
    {
        pos = str.find(strToReplace, pos);
        if (pos != std::string::npos)
            str.replace(pos, strToReplace.length(), strNewChar);
        else
            break;
    }
    return   str;
}

int Tool_CopyStringToBuffer(char* bufer, size_t bufLen, const char * srcStr)
{
    if (NULL == bufer
        || bufLen == 0
        || NULL == srcStr
		||strlen(srcStr) <= 0)
    {
        return -1;
    }
    memset(bufer, '\0', bufLen);
    if (bufLen > strlen(srcStr))
    {
        memcpy(bufer, srcStr, strlen(srcStr));
    }
    else
    {
        memcpy(bufer, srcStr, bufLen - 1);
    }
    return 0;
}

bool Tool_CheckIfFileNameIntheList(std::list<std::string>& fileNameList, std::string fileName, CRITICAL_SECTION* pCsLock)
{
    bool bRet = false;
    EnterCriticalSection(pCsLock);
    if (!fileNameList.empty()
        && !fileName.empty())
    {
        auto FindResult = std::find(std::begin(fileNameList), std::end(fileNameList), fileName);
        if (FindResult != std::end(fileNameList))
        {
            bRet = true;
        }
    }
    LeaveCriticalSection(pCsLock);
    return bRet;
}

bool Tool_AddFileNameToTheList(std::list<std::string>& fileNameList, std::string fileName, CRITICAL_SECTION* pCsLock, int maxListSize)
{
    bool bRet = false;
    EnterCriticalSection(pCsLock);
    if (maxListSize > 0
        && !fileName.empty())
    {
        if (fileNameList.size() >= maxListSize)
        {
            fileNameList.pop_front();
            printf("list size %d is larger than maxSize %d,remove the first one", fileNameList.size(), maxListSize);
        }
        fileNameList.push_back(fileName);
        bRet = true;
    }
    LeaveCriticalSection(pCsLock);
    return bRet;
}

void Tool_MakeFileDir(const char* filePath)
{
    if (NULL == filePath
        || strlen(filePath) <= 0)
    {
        return;
    }
    std::string strFile(filePath);
    strFile = strFile.substr(0, strFile.rfind("\\"));
    Tool_MakeDir(strFile.c_str());
}

void Tool_LoadCamerInfoFromINI(const char* IniFileName, std::list<CameraInfo>& infoList)
{
    int iValue = 0;
    Tool_ReadIntValueFromConfigFile(IniFileName, "VEHICLEDETECTOR", "NUM", iValue);

    CameraInfo info;
    char chNodeName[256] = { 0 };
    char chNodeValue[256] = { 0 };
    for (int i = 0; i < iValue; i++)
    {
        memset(&info, 0, sizeof(info));
        memset(chNodeValue, '\0', sizeof(chNodeName));
        memset(chNodeValue, '\0', sizeof(chNodeValue));

        sprintf(chNodeName, "DETECTOR%d", i + 1);

        Tool_ReadIntValueFromConfigFile(IniFileName, chNodeName, "LaneID", info.iDeviceID);

        Tool_ReadIntValueFromConfigFile(IniFileName, chNodeName, "VideoChannelID", info.iVideoChannelID);

        Tool_ReadIntValueFromConfigFile(IniFileName, chNodeName, "SaveFrameChannelID", info.iSaveFrameChannelID);

        Tool_ReadIntValueFromConfigFile(IniFileName, chNodeName, "Type", info.iDeviceType);

        Tool_ReadKeyValueFromConfigFile(IniFileName, chNodeName, "IP", chNodeValue, sizeof(chNodeValue));
        if (Tool_checkIP(chNodeValue))
        {
            sprintf(info.chIP, "%s", chNodeValue);
            infoList.push_back(info);

            Tool_WriteFormatLog("get camera info %d , ip = %s, laneID = %d", i + 1, info.chIP, i + 1);
        }
        else
        {
            Tool_WriteFormatLog("get camera info %d , ip = %s, laneID = %d, is invalid , do not add to the list", i + 1, chNodeValue, i + 1);
        }


    }
}

#ifdef WIN32
int Tool_pingIp_win(const char* ipAddress)
{
    HANDLE hIcmpFile;
    unsigned long ipaddr = INADDR_NONE;
    DWORD dwRetVal = 0;
    DWORD dwError = 0;
    char SendData[] = "Data Buffer";
    LPVOID ReplyBuffer = NULL;
    DWORD ReplySize = 0;

    // Validate the parameters
    ipaddr = inet_addr(ipAddress);
    if (ipaddr == INADDR_NONE)
    {
        return 1;
    }

    hIcmpFile = IcmpCreateFile();
    if (hIcmpFile == INVALID_HANDLE_VALUE)
    {
        printf("\tUnable to open handle.\n");
        printf("IcmpCreatefile returned error: %ld\n", GetLastError());
        IcmpCloseHandle(hIcmpFile);
        return 1;
    }
    // Allocate space for at a single reply
    ReplySize = sizeof (ICMP_ECHO_REPLY)+sizeof (SendData)+8;
    ReplyBuffer = (VOID *)malloc(ReplySize);
    if (ReplyBuffer == NULL)
    {
        printf("\tUnable to allocate memory for reply buffer\n");
        IcmpCloseHandle(hIcmpFile);
        return 1;
    }

    dwRetVal = IcmpSendEcho2(hIcmpFile, NULL, NULL, NULL,
        ipaddr, SendData, sizeof (SendData), NULL,
        ReplyBuffer, ReplySize, 1000);
    if (dwRetVal != 0)
    {
        PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
        struct in_addr ReplyAddr;
        ReplyAddr.S_un.S_addr = pEchoReply->Address;
        printf("\tSent icmp message to %s\n", ipAddress);
        if (dwRetVal > 1) 
        {
            printf("\tReceived %ld icmp message responses\n", dwRetVal);
            printf("\tInformation from the first response:\n");
        }
        else 
        {
            printf("\tReceived %ld icmp message response\n", dwRetVal);
            printf("\tInformation from this response:\n");
        }
        printf("\t  Received from %s\n", inet_ntoa(ReplyAddr));
        printf("\t  Status = %ld  ", pEchoReply->Status);
        switch (pEchoReply->Status) 
        {
        case IP_DEST_HOST_UNREACHABLE:
            printf("(Destination host was unreachable)\n");
            break;
        case IP_DEST_NET_UNREACHABLE:
            printf("(Destination Network was unreachable)\n");
            break;
        case IP_REQ_TIMED_OUT:
            printf("(Request timed out)\n");
            break;
        default:
            printf("\n");
            break;
        }

        printf("\t  Roundtrip time = %ld milliseconds\n",
            pEchoReply->RoundTripTime);
        free(ReplyBuffer);
    }
    else 
    {
        printf("Call to IcmpSendEcho2 failed.\n");
        dwError = GetLastError();
        switch (dwError) {
        case IP_BUF_TOO_SMALL:
            printf("\tReplyBufferSize to small\n");
            break;
        case IP_REQ_TIMED_OUT:
            printf("\tRequest timed out\n");
            break;
        default:
            printf("\tExtended error returned: %ld\n", dwError);
            break;
        }        
        IcmpCloseHandle(hIcmpFile);
        free(ReplyBuffer);
        return 1;
    }
    IcmpCloseHandle(hIcmpFile);
    return 0;
}

int Tool_LoopDeleteSpecificFormatDirectory(const char* DirPath, int iHoldDay)
{
    if (NULL == DirPath)
    {
        printf("Tool_LoopDeleteSpecificFormatDirectory:: dir is NULL.\n");
        return -1;
    }

    WIN32_FIND_DATA fFileData;
    LARGE_INTEGER filesize;
    TCHAR szDir[MAX_PATH];
    //size_t length_of_arg;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dwError = 0;

    if (strlen(DirPath) > (MAX_PATH - 3))
    {
        _tprintf(TEXT("\nDirectory path is too long.\n"));
        return (-1);
    }
    _tprintf(TEXT("\nTarget directory is %s\n\n"), DirPath);
    StringCchCopy(szDir, MAX_PATH, DirPath);
    StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

    hFind = FindFirstFile(szDir, &fFileData);
    if (INVALID_HANDLE_VALUE == hFind)
    {
        //DisplayErrorBox(TEXT("FindFirstFile"));
        printf("Tool_LoopDeleteSpecificFormatDirectory:: FindFirstFile ==INVALID_HANDLE_VALUE.\n ");
        return dwError;
    }
    std::list<std::string> dirPathList;
    do
    {
        if (fFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            _tprintf(TEXT("  %s   <DIR>\n"), fFileData.cFileName);
            if (Tool_CheckIfDeleteDir(fFileData.cFileName, iHoldDay))
            {
                //Tool_DeleteDir(ffd.cFileName);
                dirPathList.push_back(std::string(DirPath) + fFileData.cFileName);
            }
        }
        else
        {
            filesize.LowPart = fFileData.nFileSizeLow;
            filesize.HighPart = fFileData.nFileSizeHigh;
            _tprintf(TEXT("  %s   %ld bytes\n"), fFileData.cFileName, filesize.QuadPart);
        }
    } while (FindNextFile(hFind, &fFileData) != 0);

    dwError = GetLastError();
    if (dwError != ERROR_NO_MORE_FILES)
    {
        //DisplayErrorBox(TEXT("FindFirstFile"));
    }
    FindClose(hFind);

    for (std::list<std::string>::iterator it = dirPathList.begin(); it != dirPathList.end(); it++)
    {
        Tool_DeleteDir((*it).c_str());
    }

    return dwError;
}

bool Tool_CheckIfMathExpressions(const char* targetCharSequence, const char* regularExpression)
{
    if (NULL == targetCharSequence
        || NULL == regularExpression)
    {
        printf("NULL == targetCharSequence  || NULL == regularExpression \n");
        return false;
    }
    std::string strInput(targetCharSequence);
    std::regex regularExpress(regularExpression);

    if (std::regex_match(strInput, regularExpress))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Tool_CheckIfDeleteDir(const char* dirName, int Days)
{
    if (NULL == dirName)
    {
        return false;
    }
    if (Tool_CheckIfMathExpressions(dirName, "[0-9]{4}-[0-9]{1,2}-[0-9]{1,2}"))
    {
        int iDirYear = 0;
        int iDirMonth = 0;
        int iDirDays = 0;
        sscanf_s(dirName, "%d-%d-%d", &iDirYear, &iDirMonth, &iDirDays);
        time_t  tN= { time(NULL) };        
        struct tm timeFinal = *localtime(&tN);
        timeFinal.tm_mday -= Days;
        struct tm DirTime ;
        memset(&DirTime, 0, sizeof(DirTime));
        DirTime.tm_year = iDirYear-1900;
        DirTime.tm_mon = iDirMonth-1;
        DirTime.tm_mday = iDirDays;
        double fTimeDif = difftime(mktime(&DirTime), mktime(&timeFinal));
        if (fTimeDif< 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

void Tool_DeleteDir(const char* dirName)
{
    int i = 0;

    WIN32_FIND_DATA FindFileData;

    ZeroMemory(&FindFileData, sizeof(FindFileData));

    HANDLE hFind;

    std::string dir_name(dirName);

    if (dir_name[dir_name.length() - 1] != '\\')

        dir_name += "\\";

    std::string filter = dir_name + std::string("*.*");

    hFind = FindFirstFile(filter.c_str(), &FindFileData);

    if (hFind != NULL && hFind != INVALID_HANDLE_VALUE)
    {
        while (FindNextFile(hFind, &FindFileData) != 0)
        {

            if (strcmp(FindFileData.cFileName, ".") != 0 
                && strcmp(FindFileData.cFileName, "..") != 0)
            {
                if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    Tool_DeleteDir((dir_name + std::string(FindFileData.cFileName)).c_str());
                }
                else
                {
					if (NULL != strstr(FindFileData.cFileName, ".log"))
					{
						Tool_WriteFormatLog("%s:: delete file %s", __FUNCTION__, std::string(dir_name + std::string(FindFileData.cFileName)).c_str());
						DeleteFile(std::string(dir_name + std::string(FindFileData.cFileName)).c_str());
					}
                }
            }
        }
        FindClose(hFind);
    }
	Tool_WriteFormatLog("%s:: RemoveDirectory %s", __FUNCTION__, dir_name.c_str());
    RemoveDirectory(dir_name.c_str());
}

bool Tool_GetRootPathFromFileName(const char* srcFileName, char* buf, size_t bufLen)
{
    if (srcFileName == NULL || buf == NULL || bufLen<= 0)
    {
        return false;
    }
    std::string strFullName(srcFileName);
    std::string::size_type pos = strFullName.find(":");
    if (pos != std::string::npos)
    {
        std::string strRootPath = strFullName.substr(0, pos + 1);
        strRootPath.append("\\");
        printf("%s\n", strRootPath.c_str());
        memset(buf, '\0', bufLen);
        sprintf_s(buf, bufLen, "%s", strRootPath.c_str());
        return true;
    }
    return false;
}

std::string Tool_GetLastErrorAsString()
{
    //Get the error message, if any.
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0)
        return std::string("execute success."); //No error message has been recorded

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errorMessageID, 
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&messageBuffer,
        0,
        NULL);

    std::string message(messageBuffer, size);

    //Free the buffer.
    LocalFree(messageBuffer);

    return message;
}
#endif