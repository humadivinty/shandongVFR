#include "stdafx.h"
#include "CameraResult.h"
//#include "utilityTool/log4z.h"
#include "utilityTool/ToolFunction.h"

#ifndef LOGFMTE
#define LOGFMTE printf
#endif

#ifndef LOGFMTD
#define LOGFMTD printf
#endif

typedef struct {
    INT64 iObjecSize;
}ResultInfo;

CameraIMG& CameraIMG::operator=( const CameraIMG& CaIMG )
{
	if (this != &CaIMG)
	{
		wImgWidth = CaIMG.wImgWidth;
		wImgHeight = CaIMG.wImgHeight;
		dwImgSize = CaIMG.dwImgSize;
		wImgType = CaIMG.wImgType;
		for (unsigned int i = 0; i< sizeof(chSavePath); i++)
		{
			chSavePath[i] = CaIMG.chSavePath[i];
		}
		if (NULL != CaIMG.pbImgData)
		{
            pbImgData = new BYTE[CaIMG.dwImgSize];
			for (DWORD j = 0; j< dwImgSize; j++)
			{
				pbImgData[j] = CaIMG.pbImgData[j];
			}
		}
		else
		{
			pbImgData = NULL;
		}
	}
	return *this;
}

CameraIMG::CameraIMG()
{
	wImgWidth = 0;
	wImgHeight = 0;
	pbImgData = NULL;
	dwImgSize = 0;
	for (unsigned int i = 0; i< sizeof(chSavePath); i++)
	{
		chSavePath[i] = 0;
	}
}

CameraIMG::CameraIMG( const CameraIMG& CaIMG )
{
    if (this == &CaIMG)
    {
        return;
    }
	wImgWidth = CaIMG.wImgWidth;
	wImgHeight = CaIMG.wImgHeight;
	dwImgSize = CaIMG.dwImgSize;
	wImgType = CaIMG.wImgType;
	for (unsigned int i = 0; i< sizeof(chSavePath); i++)
	{
		chSavePath[i] = CaIMG.chSavePath[i];
	}
	if (NULL != CaIMG.pbImgData)
	{
        pbImgData = new BYTE[CaIMG.dwImgSize];
		for (DWORD j = 0; j< dwImgSize; j++)
		{
			pbImgData[j] = CaIMG.pbImgData[j];
		}
	}
	else
	{
		pbImgData = NULL;
	}
}

CameraIMG::~CameraIMG()
{
	wImgWidth = 0;
	wImgHeight = 0;
	if (NULL != pbImgData)
	{
		delete[] pbImgData;
		pbImgData = NULL;
	}
	dwImgSize = 0;
	memset(chSavePath, 0, sizeof(chSavePath));
}
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

CameraResult::CameraResult()
{
	dwCarID = 0;
	iDeviceID = 0;
	iPlateColor = 0;
	iTailPlateColor = 0;
	iPlateTypeNo = 0;
	dw64TimeMS = 0;
	iSpeed = 0;
	iResultNo = 0;
	iVehTypeNo = 0;		//车型代码: 客1--1 。。。客4--4， 货1--5  。。。货4--8
    iVehSizeType = 0;
	iVehBodyColorNo = 0;	
	iVehBodyDeepNo = 0;	
    iVehLogoType = 0;
	iAreaNo = 0;
	iRoadNo = 0;
	iLaneNo = 0;
	iDirection = 0;
	iWheelCount = 0;		//轮数
	iAxletreeCount = 0;		//轴数
	iAxletreeType = 0;		//轴型
	iAxletreeGroupCount = 2;
    iReliability = 0;
	fVehLenth = 0;			//车长
	fDistanceBetweenAxles = 0.0;		//轴距
	fVehHeight = 0.0;		//车高
    fConfidenceLevel = 0.0;
	bBackUpVeh = false;		//是否倒车
    dwReceiveTime = 0;

	//pcAppendInfo = NULL;
	//for (unsigned int i = 0; i < sizeof(pcAppendInfo); i++)
	//{
	//	pcAppendInfo[i] = 0;
	//}
    
    memset(chDeviceIp, '\0', sizeof(chDeviceIp));
    memset(chServerIP, '\0', sizeof(chServerIP));
    memset(chPlateNO, '\0', sizeof(chPlateNO));
	memset(chTailPlateNO, '\0', sizeof(chTailPlateNO));
    memset(chPlateColor, '\0', sizeof(chPlateColor));
	memset(chTailPlateColor, '\0', sizeof(chTailPlateColor));
    memset(chListNo, '\0', sizeof(chListNo));
    memset(chPlateTime, '\0', sizeof(chPlateTime)); 
	memset(chTailPlateTime, '\0', sizeof(chTailPlateTime));
    memset(chSignStationID, '\0', sizeof(chSignStationID));
    memset(chSignStationName, '\0', sizeof(chSignStationName));
    memset(chSignDirection, '\0', sizeof(chSignDirection));
    memset(chDeviceID, '\0', sizeof(chDeviceID));
    memset(chLaneID, '\0', sizeof(chLaneID)); 
    memset(chCarFace, '\0', sizeof(chCarFace));
    memset(chChileLogo, '\0', sizeof(chChileLogo));
    memset(pcAppendInfo, '\0', sizeof(pcAppendInfo));
    memset(chVehTypeText, '\0', sizeof(chVehTypeText));
    memset(chSaveFileName, '\0', sizeof(chSaveFileName));
    memset(chAxleType, '\0', sizeof(chAxleType));
}

CameraResult::CameraResult( const CameraResult& CaRESULT )
{
    if (this == &CaRESULT)
    {
        return;
    }
	dwCarID = CaRESULT.dwCarID;
	dw64TimeMS = CaRESULT.dw64TimeMS;
	iLaneNo = CaRESULT.iLaneNo;
	iPlateColor = CaRESULT.iPlateColor;
	iTailPlateColor = CaRESULT.iTailPlateColor;
	iVehTypeNo = CaRESULT.iVehTypeNo;
    iVehSizeType = CaRESULT.iVehSizeType;
	iDirection = CaRESULT.iDirection;
	iAreaNo = CaRESULT.iAreaNo;
	iVehBodyColorNo = CaRESULT.iVehBodyColorNo;
	iPlateTypeNo = CaRESULT.iPlateTypeNo;
	iVehBodyDeepNo = CaRESULT.iVehBodyDeepNo;
    iVehLogoType = CaRESULT.iVehLogoType;
	iDeviceID = CaRESULT.iDeviceID;
	iRoadNo = CaRESULT.iRoadNo;
	iSpeed = CaRESULT.iSpeed;
	iAxletreeType = CaRESULT.iAxletreeType;
	iAxletreeCount = CaRESULT.iAxletreeCount;
	iAxletreeGroupCount = CaRESULT.iAxletreeGroupCount;
	iResultNo = CaRESULT.iResultNo;
	iWheelCount = CaRESULT.iWheelCount;
    iReliability = CaRESULT.iReliability;
	fVehHeight = CaRESULT.fVehHeight;
	fVehLenth = CaRESULT.fVehLenth;
	fDistanceBetweenAxles = CaRESULT.fDistanceBetweenAxles;
    fConfidenceLevel = CaRESULT.fConfidenceLevel;
    fVehWidth = CaRESULT.fVehWidth;
	bBackUpVeh = CaRESULT.bBackUpVeh;
    dwReceiveTime = CaRESULT.dwReceiveTime;

	//pcAppendInfo = NULL;
    //strAppendInfo = CaRESULT.strAppendInfo;

    memcpy(chDeviceIp, CaRESULT.chDeviceIp, sizeof(chDeviceIp));
    memcpy(chServerIP, CaRESULT.chServerIP, sizeof(chServerIP));
    memcpy(chPlateNO, CaRESULT.chPlateNO, sizeof(chPlateNO));
	memcpy(chTailPlateNO, CaRESULT.chTailPlateNO, sizeof(chTailPlateNO));
    memcpy(chPlateColor, CaRESULT.chPlateColor, sizeof(chPlateColor));
	memcpy(chTailPlateColor, CaRESULT.chTailPlateColor, sizeof(chTailPlateColor));
    memcpy(chListNo, CaRESULT.chListNo, sizeof(chListNo));
    memcpy(chPlateTime, CaRESULT.chPlateTime, sizeof(chPlateTime));
	memcpy(chTailPlateTime, CaRESULT.chTailPlateTime, sizeof(chTailPlateTime));
    memcpy(chSignStationID, CaRESULT.chSignStationID, sizeof(chSignStationID));
    memcpy(chSignStationName, CaRESULT.chSignStationName, sizeof(chSignStationName));
    memcpy(chSignDirection, CaRESULT.chSignDirection, sizeof(chSignDirection));
    memcpy(chDeviceID, CaRESULT.chDeviceID, sizeof(chDeviceID));
    memcpy(chLaneID, CaRESULT.chLaneID, sizeof(chLaneID));
    memcpy(chCarFace, CaRESULT.chCarFace, sizeof(chCarFace));
    memcpy(chChileLogo, CaRESULT.chChileLogo, sizeof(chChileLogo));
    memcpy(pcAppendInfo, CaRESULT.pcAppendInfo, sizeof(pcAppendInfo));
    memcpy(chVehTypeText,CaRESULT.chVehTypeText, sizeof(chVehTypeText));
    memcpy(chSaveFileName,CaRESULT.chSaveFileName, sizeof(chSaveFileName));
    memcpy(chAxleType, CaRESULT.chAxleType, sizeof(chAxleType));

	CIMG_BestCapture = CaRESULT.CIMG_BestCapture;
	CIMG_BeginCapture = CaRESULT.CIMG_BeginCapture;
	CIMG_LastCapture = CaRESULT.CIMG_LastCapture;
	CIMG_BestSnapshot = CaRESULT.CIMG_BestSnapshot;
	CIMG_LastSnapshot = CaRESULT.CIMG_LastSnapshot;
	CIMG_PlateImage = CaRESULT.CIMG_PlateImage;
	CIMG_BinImage = CaRESULT.CIMG_BinImage;
}

CameraResult::~CameraResult()
{
	dwCarID = 0;
	iDeviceID = 0;
	iPlateColor = 0;
	iTailPlateColor = 0;
	iPlateTypeNo = 0;
	dw64TimeMS = 0;
	iSpeed = 0;
	iResultNo = 0;
	iVehTypeNo = 0;		//车型代码: 客1--1 。。。客4--4， 货1--5  。。。货4--8
	iVehBodyColorNo = 0;	
	iVehBodyDeepNo = 0;	
    iVehLogoType = 0;
	iAreaNo = 0;
	iRoadNo = 0;
	iLaneNo = 0;
	iDirection = 0;
	iWheelCount = 0;		//轮数
	iAxletreeCount = 0;		//轴数
	iAxletreeGroupCount = 0;
	iAxletreeType = 0;		//轴型
    iReliability = 0;
	fVehLenth = 0;			//车长
	fDistanceBetweenAxles = 0.0;		//轴距
	fVehHeight = 0.0;		//车高
	bBackUpVeh = false;		//是否倒车

	memset(chDeviceIp, 0, sizeof(chDeviceIp));
	memset(chPlateNO, 0, sizeof(chPlateNO));
	memset(chTailPlateNO, 0, sizeof(chTailPlateNO));
	memset(chPlateColor, 0, sizeof(chPlateColor));
	memset(chTailPlateColor, 0, sizeof(chTailPlateColor));
	memset(chListNo, 0, sizeof(chListNo));
	memset(chPlateTime, 0, sizeof(chPlateTime));
	memset(chTailPlateTime, '\0', sizeof(chTailPlateTime));
	memset(chSignStationID, 0, sizeof(chSignStationID));
	memset(chSignStationName, 0, sizeof(chSignStationName));
	//memset(pcAppendInfo, 0, sizeof(pcAppendInfo));
	memset(chLaneID, 0, sizeof(chLaneID));
	memset(chDeviceID, 0, sizeof(chDeviceID));
    memset(chSaveFileName, 0, sizeof(chSaveFileName));
}

void CopyCImgDataToFile(CameraIMG& imageStruct, FILE* pFile)
{
    if (NULL == pFile || imageStruct.dwImgSize <= 0)
    {
        return;
    }
    if (imageStruct.dwImgSize > 0 && NULL != imageStruct.pbImgData)
    {
        fwrite(imageStruct.pbImgData, sizeof(BYTE), imageStruct.dwImgSize, pFile);
    }
}

bool CameraResult::SerializationToDisk(const char* chFilePath)
{
    if (NULL == chFilePath)
    {
        return false;
    }
    LOGFMTD("SerializationToDisk::begin , path= %s", chFilePath);
    ResultInfo info;
    info.iObjecSize = sizeof(ResultInfo)+sizeof(CameraResult)+CIMG_BestSnapshot.dwImgSize +
        CIMG_LastSnapshot.dwImgSize +
        CIMG_BeginCapture.dwImgSize +
        CIMG_BestCapture.dwImgSize +
        CIMG_LastCapture.dwImgSize +
        CIMG_PlateImage.dwImgSize +
        CIMG_BinImage.dwImgSize;

    FILE* pfile = NULL;
    errno_t errCode;
    _set_errno(0);
    errCode = fopen_s(&pfile, chFilePath, "wb");
    if (errCode != 0)
    {
        LOGFMTE("SerializationToDisk:: open file failed., error code = %d", errCode);
        return false;
    }
    if (!pfile)
    {
        LOGFMTE("SerializationToDisk:: open file failed ,pfile== NULL, error code = %d", errCode);
        return false;
    }
    fwrite(&info, sizeof(BYTE), sizeof(ResultInfo), pfile);
    fwrite(this, sizeof(BYTE), sizeof(CameraResult), pfile);

    CopyCImgDataToFile(CIMG_BestSnapshot, pfile);
    CopyCImgDataToFile(CIMG_LastSnapshot, pfile);
    CopyCImgDataToFile(CIMG_BeginCapture, pfile);
    CopyCImgDataToFile(CIMG_BestCapture, pfile);
    CopyCImgDataToFile(CIMG_LastCapture, pfile);
    CopyCImgDataToFile(CIMG_PlateImage, pfile);
    CopyCImgDataToFile(CIMG_BinImage, pfile);    

    //fflush(pfile);
    fclose(pfile);
    pfile = NULL;

    LOGFMTD("SerializationToDisk::finish, path = %s", chFilePath);
    return true;
}

void CopyCImgDataFromBuffer(CameraIMG& imageStruct, BYTE* pBuffer)
{
    if (imageStruct.dwImgSize > 0)
    {
        imageStruct.pbImgData = new BYTE[imageStruct.dwImgSize];
        memcpy(imageStruct.pbImgData, pBuffer, imageStruct.dwImgSize);
    }
    else
    {
        imageStruct.pbImgData = NULL;
    }
}

bool CameraResult::SerializationFromDisk(const char* fileName)
{
    if (NULL == fileName || strlen(fileName) <= 0)
    {
        LOGFMTE("SerializationFromDisk finish, fileName length is invalid.");
        return false;
    }
    LOGFMTD("SerializationFromDisk begin, fileName = %s", fileName);
    ///// 反序列化数据
    size_t iFileSize = Tool_GetFileSize(fileName);
    unsigned char* pFileBuffer = new unsigned char[iFileSize + 1];
    if (!Tool_LoadFile(fileName, (void*)pFileBuffer, iFileSize))
    {
        LOGFMTE("SerializationFromDisk Tool_LoadFile  failed.");

        SAFE_DELETE_ARRAY(pFileBuffer);
        return false;
    }

    BYTE* pData = pFileBuffer;
    ResultInfo info;
    memcpy(&info, pData, sizeof(ResultInfo));
    pData += sizeof(ResultInfo);

    if (info.iObjecSize == iFileSize)
    {
        //LOGFMTD("DeSerializationResult:: info.iObjecSize == iFileSize");
    }
    else
    {
        LOGFMTE("DeSerializationResult:: info.iObjecSize %I64u != iFileSize %u", info.iObjecSize, iFileSize);

        pData = NULL;
        SAFE_DELETE_ARRAY(pFileBuffer);
        DeleteFileA(fileName);
        return false;
    }
    memset(this, 0, sizeof(CameraResult));
    memcpy(this, pData, sizeof(CameraResult));
    pData += sizeof(CameraResult);

    CopyCImgDataFromBuffer(CIMG_BestSnapshot, pData);
    pData += CIMG_BestSnapshot.dwImgSize;

    CopyCImgDataFromBuffer(CIMG_LastSnapshot, pData);
    pData += CIMG_LastSnapshot.dwImgSize;

    CopyCImgDataFromBuffer(CIMG_BeginCapture, pData);
    pData += CIMG_BeginCapture.dwImgSize;

    CopyCImgDataFromBuffer(CIMG_BestCapture, pData);
    pData += CIMG_BestCapture.dwImgSize;

    CopyCImgDataFromBuffer(CIMG_LastCapture, pData);
    pData += CIMG_LastCapture.dwImgSize;

    CopyCImgDataFromBuffer(CIMG_PlateImage, pData);
    pData += CIMG_PlateImage.dwImgSize;

    CopyCImgDataFromBuffer(CIMG_BinImage, pData);
    pData += CIMG_BinImage.dwImgSize;

    DeleteFileA(fileName);
    pData = NULL;
    SAFE_DELETE_ARRAY(pFileBuffer);
    LOGFMTD("SerializationFromDisk finish, fileName = %s", fileName);
    return true;
}

CameraResult& CameraResult::operator=(const CameraResult& CaRESULT)
{
    if (this == &CaRESULT)
    {
        return *this;
    }
	dwCarID = CaRESULT.dwCarID;
	dw64TimeMS = CaRESULT.dw64TimeMS;
	iLaneNo = CaRESULT.iLaneNo;
	iPlateColor = CaRESULT.iPlateColor;
	iTailPlateColor = CaRESULT.iTailPlateColor;
	iVehTypeNo = CaRESULT.iVehTypeNo;
    iVehSizeType = CaRESULT.iVehSizeType;
	iDirection = CaRESULT.iDirection;
	iAreaNo = CaRESULT.iAreaNo;
	iVehBodyColorNo = CaRESULT.iVehBodyColorNo;
	iPlateTypeNo = CaRESULT.iPlateTypeNo;
	iVehBodyDeepNo = CaRESULT.iVehBodyDeepNo;
    iVehLogoType = CaRESULT.iVehLogoType;
	iDeviceID = CaRESULT.iDeviceID;
	iRoadNo = CaRESULT.iRoadNo;
	iSpeed = CaRESULT.iSpeed;
	iAxletreeType = CaRESULT.iAxletreeType;
	iAxletreeCount = CaRESULT.iAxletreeCount;
	iAxletreeGroupCount = CaRESULT.iAxletreeGroupCount;
	iResultNo = CaRESULT.iResultNo;
	iWheelCount = CaRESULT.iWheelCount;
    iReliability = CaRESULT.iReliability;
	fVehHeight = CaRESULT.fVehHeight;
	fVehLenth = CaRESULT.fVehLenth;
	fDistanceBetweenAxles = CaRESULT.fDistanceBetweenAxles;
    fConfidenceLevel = CaRESULT.fConfidenceLevel;
    fVehWidth = CaRESULT.fVehWidth;
	bBackUpVeh = CaRESULT.bBackUpVeh;
    dwReceiveTime = CaRESULT.dwReceiveTime;

	//pcAppendInfo = NULL;
    //strAppendInfo = CaRESULT.strAppendInfo;

    memcpy(chDeviceIp, CaRESULT.chDeviceIp, sizeof(chDeviceIp));
    memcpy(chServerIP, CaRESULT.chServerIP, sizeof(chServerIP));
    memcpy(chPlateNO, CaRESULT.chPlateNO, sizeof(chPlateNO));
	memcpy(chTailPlateNO, CaRESULT.chTailPlateNO, sizeof(chTailPlateNO));
    memcpy(chPlateColor, CaRESULT.chPlateColor, sizeof(chPlateColor));
	memcpy(chTailPlateColor, CaRESULT.chTailPlateColor, sizeof(chTailPlateColor));
    memcpy(chListNo, CaRESULT.chListNo, sizeof(chListNo));
    memcpy(chPlateTime, CaRESULT.chPlateTime, sizeof(chPlateTime));
	memcpy(chTailPlateTime, CaRESULT.chTailPlateTime, sizeof(chTailPlateTime));
    memcpy(chSignStationID, CaRESULT.chSignStationID, sizeof(chSignStationID));
    memcpy(chSignStationName, CaRESULT.chSignStationName, sizeof(chSignStationName));
    memcpy(chSignDirection, CaRESULT.chSignDirection, sizeof(chSignDirection));
    memcpy(chDeviceID, CaRESULT.chDeviceID, sizeof(chDeviceID));
    memcpy(chLaneID, CaRESULT.chLaneID, sizeof(chLaneID));
    memcpy(chCarFace, CaRESULT.chCarFace, sizeof(chCarFace));
    memcpy(chChileLogo, CaRESULT.chChileLogo, sizeof(chChileLogo));
    memcpy(pcAppendInfo, CaRESULT.pcAppendInfo, sizeof(pcAppendInfo));
    memcpy(chVehTypeText,CaRESULT.chVehTypeText, sizeof(chVehTypeText));
    memcpy(chSaveFileName,CaRESULT.chSaveFileName, sizeof(chSaveFileName));
    memcpy(chAxleType, CaRESULT.chAxleType, sizeof(chAxleType));

	CIMG_BestCapture = CaRESULT.CIMG_BestCapture;
	CIMG_BeginCapture = CaRESULT.CIMG_BeginCapture;
	CIMG_LastCapture = CaRESULT.CIMG_LastCapture;
	CIMG_BestSnapshot = CaRESULT.CIMG_BestSnapshot;
	CIMG_LastSnapshot = CaRESULT.CIMG_LastSnapshot;
	CIMG_PlateImage = CaRESULT.CIMG_PlateImage;
	CIMG_BinImage = CaRESULT.CIMG_BinImage;

	return *this;
};