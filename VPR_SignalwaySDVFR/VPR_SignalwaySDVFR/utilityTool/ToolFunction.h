#include "tinyxml/tinyxml.h"
#include "tinyxml/tinystr.h"
#include <string>
#include <list>
#include <map>
#include<windef.h>
#include "CameraModule/BaseCamera.h"
//#define MY_SPRINTF sprintf_s

#define SAFE_DELETE_OBJ(obj) \
if (NULL != obj)                                  \
{                                           \
    delete obj;                        \
    obj = NULL;                      \
}

#define SAFE_DELETE_ARRAY(arrayObj) \
if (NULL != arrayObj)                                  \
{                                           \
    delete[] arrayObj;                        \
    arrayObj = NULL;                      \
}

#define USE_GID_PLUS 1
#define USE_MSVC
//#define  USE_MFC

//通过节点名查找并返回相应节点
//注：XMLTYPE 为1时，InputInfo为XML路径，当为2时,InputInfo为二进制文件内容
TiXmlElement Tool_SelectElementByName(const char* InputInfo, const char* pName, int iXMLType);

TiXmlElement* Tool_ReadElememt(TiXmlElement* InputElement, const char* pName);

bool Tool_GetElementTextByName(const char* InputInfo, const char* pName, int iXMLType, char* chTextValueBuffer, size_t& bufferLength);

bool Tool_InsertElementByName(const char* InputInfo, const char* pName, int iXMLType, const char* nodeName, const char* textValue, std::string& outputString);

void Tool_ReadKeyValueFromConfigFile(const char* FileName, const char* nodeName, const char* keyName, char* keyValue, size_t bufferSize);

void Tool_ReadIntValueFromConfigFile(const char* FileName, const char* nodeName, const char* keyName, int&keyValue);

void Tool_WriteKeyValueFromConfigFile(const char* FileName, const char* nodeName, const char* keyName, char* keyValue, size_t bufferSize);

//检查IP的有效性
int Tool_checkIP(const char* p);

bool Tool_IsFileExist(const char* FilePath);

bool Tool_IsDirExist(const char* Dir);

bool Tool_MakeDir(const char* chImgPath);

long Tool_GetFileSize(const char *FileName);

bool Tool_PingIPaddress(const char* IpAddress);
#ifdef USE_GID_PLUS
bool Tool_Img_ScaleJpg(PBYTE pbSrc, int iSrcLen, PBYTE pbDst, size_t *iDstLen, int iDstWidth, int iDstHeight, int compressQuality);
#endif

#ifdef USE_GID_PLUS
int Tool_GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
#endif
void Tool_ExcuteShellCMD(char* pChCommand);

bool Tool_ExcuteCMDbyCreateProcess(const char* CmdName);

#ifdef USE_GID_PLUS
bool Tool_OverlayStringToImg(unsigned char** pImgsrc, long srcSize,
    unsigned char** pImgDest, long& DestSize,
    const wchar_t* DestString, int FontSize,
    int x, int y, int colorR, int colorG, int colorB,
    int compressQuality);
#endif

bool Tool_GetDataFromAppenedInfo(char *pszAppendInfo, std::string strItemName, char *pszRstBuf, int *piRstBufLen);
bool Tool_GetDataAttributFromAppenedInfo(char *pszAppendInfo, std::string strItemName, std::string  strAttributeName, char *pszRstBuf, int *piRstBufLen);

void Tool_ExcuteCMD(char* pChCommand);

std::wstring Tool_string2wstring(std::string strSrc);

#ifdef  USE_MFC
//删除指定文件夹
bool Tool_DeleteDirectory(const char* strDirName);

//删除指定目录中以 'yyyy-mm-dd' 方式命名的文件夹，其中限定的条件为时间早于指定天数
int Tool_CirclelaryDelete(const char* folderPath, int iBackUpDays);
#endif


int Tool_SafeCloseThread(HANDLE& threadHandle);

const char* Tool_GetCurrentPath();
#ifdef USE_MSVC
SYSTEMTIME Tool_GetCurrentTime();
#endif

bool Tool_DimCompare(const char *szSrcPlateNo, const char *szDesPlateNo);

void Tool_SetLogPath(const char* path);

void Tool_WriteLog(const char*);

void Tool_WriteFormatLog(const char* szfmt, ...);

bool Tool_SaveFileToPath(const char* szPath, void* fileData, size_t fileSize);

std::list<std::string> getFilesPath(const std::string& cate_dir, const std::string& filter);

bool Tool_LoadFile(const char* fileName, void* pBuffer, size_t& fileSize);

bool Tool_LoadCamerXml(const char* fileName, std::map<std::string, int>& myMap);

void Tool_DeleteFileByCMD(const char* chFileName);

bool Tool_FindMapAndGetValue(std::map<std::string, int>& myMap, std::string keyName, int& value);

#ifdef WIN32
std::string Tool_GetSoftVersion(const char* exepath);

//************************************
// Method:        Tool_pingIp_win
// Describe:
// FullName:      Tool_pingIp_win
// Access:          public 
// Returns:        int  0 success, 1 failed
// Returns Describe:
// Parameter:    const char * ipAddress
//************************************
int Tool_pingIp_win(const char* ipAddress);
void Tool_DeleteDir(const char* dirName);
bool Tool_CheckIfDeleteDir(const char* dirName,  int Days);
bool Tool_CheckIfMathExpressions(const char* targetCharSequence, const char* regularExpression);
int Tool_LoopDeleteSpecificFormatDirectory(const char* DirPath, int iHoldDay);
bool Tool_GetRootPathFromFileName(const char* srcFileName, char* buf, size_t bufLen);
//Returns the last Win32 error, in string format. Returns an empty string if there is no error.
std::string Tool_GetLastErrorAsString();
#endif
int Tool_AnalysisPlateColorNo(const char *szPlateNo);

bool Tool_ProcessPlateNo(const char* pSrcPlateNum, char* destPlateNum, int bufLen);

int Tool_FindSubStrCount(const char* srcText, const char* subText);

void Tool_SetDllDirPath(const char* dirPath);

const char* Tool_GetDllDirPath();

std::string Tool_ReplaceStringInStd(std::string strOrigin, std::string strToReplace, std::string strNewChar);

int Tool_CopyStringToBuffer(char* bufer, size_t bufLen, const char * srcStr);

bool Tool_CheckIfFileNameIntheList(std::list<std::string>& fileNameList, std::string  fileName, CRITICAL_SECTION* pCsLock);

bool Tool_AddFileNameToTheList(std::list<std::string>& fileNameList, std::string fileName, CRITICAL_SECTION* pCsLock, int maxListSize);

void Tool_MakeFileDir(const char* filePath);

void Tool_LoadCamerInfoFromINI(const char* iniFileName, std::list<CameraInfo>& infoList);


