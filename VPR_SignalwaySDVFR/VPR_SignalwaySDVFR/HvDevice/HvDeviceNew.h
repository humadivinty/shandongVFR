#ifndef _HVDEVICENEW_H__
#define _HVDEVICENEW_H__

#include "HvDeviceBaseType.h"
#include "HvDeviceCommDef.h"
#include <vector>
using namespace std;

#ifdef HVDEVICE_EXPORTS
#define HV_API_EX extern "C" __declspec(dllexport)
#elif HVDEVICE_LIB
#define HV_API_EX
#else
#define HV_API_EX extern "C" __declspec(dllimport)
#endif

#define HVAPI_API_VERSION_EX	"2.0"
typedef PVOID HVAPI_HANDLE_EX;         /**< HvAPI句柄类型 */


/**
* @brief			识别结果开始回调函数
* @param[out]		pUserData		由HVAPI_SetCallBack设置的传入回调函数的用户数据指针
* @param[out]		dwCarID			车辆ID
* @return			0
*/
typedef INT (CDECL* HVAPI_CALLBACK_RECORD_INFOBEGIN)(PVOID pUserData, DWORD dwCarID);

/**
* @brief			识别结果结束回调函数
* @param[out]		pUserData		由HVAPI_SetCallBack设置的传入回调函数的用户数据指针
* @param[out]		dwCarID			车辆ID
* @return			0
*/
typedef INT (CDECL* HVAPI_CALLBACK_RECORD_INFOEND)(PVOID pUserData, DWORD dwCarID);

/**
* @brief			车牌回调函数
* @param[out]		pUserData		由HVAPI_SetCallBack设置的传入回调函数的用户数据指针
* @param[out]		dwCarID			车辆ID
* @param[out]		pcPlateNo		车牌字符串
* @param[out]		pcAppendInfo	车牌附加信息
* @param[out]		dw64TimeMS		识别时间
* @return			0
*/
typedef INT (CDECL* HVAPI_CALLBACK_RECORD_PLATE)(PVOID pUserData,
												 DWORD dwCarID,
												 LPCSTR pcPlateNo,
												 LPCSTR pcAppendInfo,
												 DWORD dwRecordType,
												 DWORD64 dw64TimeMS);

/**
* @brief			识别结果大图回调函数
* @param[out]		pUserData		由HVAPI_SetCallBack设置的传入回调函数的用户数据指针
* @param[out]		dwCarID			车辆ID
* @param[out]		wImgType		大图类型
* @param[out]		wWidth			大图宽
* @param[out]		wHeight			大图高
* @param[out]		pbPicData		大图数据
* @param[out]		dwImgDataLen	大图数据长度
* @param[out]		dw64TimeMS		识别时间
* @return			0
*/
typedef INT (CDECL* HVAPI_CALLBACK_RECORD_BIGIMAGE)(PVOID pUserData,
													DWORD dwCarID, 
													WORD  wImgType,
													WORD  wWidth,
													WORD  wHeight,
													PBYTE pbPicData,
													DWORD dwImgDataLen,
													DWORD dwRecordType,
													DWORD64 dw64TimeMS);



/**
* @brief			识别结果车牌小图回调函数
* @param[out]		pUserData		由HVAPI_SetCallBack设置的传入回调函数的用户数据指针
* @param[out]		dwCarID			车辆ID
* @param[out]		wWidth			小图宽
* @param[out]		wHeight			小图高
* @param[out]		pbPicData		小图数据
* @param[out]		dwImgDataLen	小图数据长度
* @param[out]		dw64TimeMS		识别时间
* @return			0
*/
typedef INT (CDECL* HVAPI_CALLBACK_RECORD_SMALLIMAGE)(PVOID pUserData,
													  DWORD dwCarID,
													  WORD wWidth,
													  WORD wHeight,
													  PBYTE pbPicData,
													  DWORD dwImgDataLen,
													  DWORD dwRecordType,
													  DWORD64 dw64TimeMS);

/**
* @brief			识别结果车牌二值图回调函数
* @param[out]		pUserData		由HVAPI_SetCallBack设置的传入回调函数的用户数据指针
* @param[out]		dwCarID			车辆ID
* @param[out]		wWidth			二值图宽
* @param[out]		wHeight			二值图高
* @param[out]		pbPicData		二值图数据
* @param[out]		dwImgDataLen	二值图数据长度
* @param[out]		dw64TimeMS		识别时间
* @return			0
*/
typedef INT (CDECL* HVAPI_CALLBACK_RECORD_BINARYIMAGE)(PVOID pUserData,
													   DWORD dwCarID,
													   WORD wWidth,
													   WORD wHeight,
													   PBYTE pbPicData,
													   DWORD dwImgDataLen,
													   DWORD dwRecordType,
													   DWORD64 dw64TimeMS);

/**
* @brief			违法视频回调函数
* @param[out]		pUserData			由HVAPI_SetCallBack设置的传入回调函数的用户数据指针
* @param[out]		dwCarID				车辆ID
* @param[out]		dwVideoType			视频帧类型
* @param[out]		dwVideoWidth		视频的宽
* @param[out]		dwVideoHeight		视频的高
* @param[out]		dw64TimeMS			视频时间
* @param[out]		pbVideoData			视频帧数据
* @param[out]		dwVideoDataLen		视频帧数据长度
* @param[out]		szVideoExtInfo		视频帧信息
* @return			0
*/
typedef INT (CDECL* HVAPI_CALLBACK_RECORD_ILLEGALVideo)(PVOID pUserData,
													   DWORD dwCarID,													   
													   DWORD dwVideoType,
													   DWORD dwVideoWidth,
													   DWORD dwVideoHeight,
													   DWORD64 dw64TimeMS,
													   PBYTE pbVideoData,
													   DWORD dwVideoDataLen,
													   LPCSTR szVideoExtInfo);

/**
* @brief			识别器信息回调函数
* @param[out]		pUserData		由HVAPI_SetCallBack设置的传入回调函数的用户数据指针
* @param[out]		pString			信息字符串
* @param[out]		dwStrLen		信息字符串长度
* @return			0
*/
typedef INT (CDECL* HVAPI_CALLBACK_STRING)(PVOID pUserData, LPCSTR pString, DWORD dwStrLen);

/**
* @brief			雷达信息回调函数
* @param[out]		pUserData		由HVAPI_StartRecvRadarData设置的传入回调函数的用户数据指针
* @param[out]		pRadarData		雷达数据
* @param[out]		dwRadarDataLen	雷达数据长度
* @return			0
*/
typedef  INT (CDECL* HVAPI_CALLBACK_RADAR)(PVOID pUserData, PBYTE pRadarData, DWORD dwRadarDataLen);

/**
* @brief			识别器信息回调函数
* @param[out]		pUserData		由HVAPI_SetCallBack设置的传入回调函数的用户数据指针
* @param[out]		pString			信息字符串
* @param[out]		dwStrLen		信息字符串长度
* @param[out]		dwType			信息字符串类型 历史或实时
* @return			0
*/
typedef INT (CDECL* HVAPI_CALLBACK_TFD_STRING)(PVOID pUserData, LPCSTR pString, DWORD dwStrLen, DWORD dwType);


/**
* @brief			图片回调函数
* @param[out]		pUserData			由HVAPI_SetCallBack设置的传入回调函数的用户数据指针
* @param[out]		pbImageData			图片数据
* @param[out]		dwImageDataLen		图片数据长度
* @param[out]		dwImageType			图片类型
* @param[out]		szImageExtInfo		图片信息
* @return			0
*/
typedef INT (CDECL* HVAPI_CALLBACK_JPEG)(PVOID pUserData,
										 PBYTE pbImageData,
										 DWORD dwImageDataLen,
										 DWORD dwImageType,
										 LPCSTR szImageExtInfo);

/**
* @brief			视频回调函数
* @param[out]		pUserData			由HVAPI_SetCallBack设置的传入回调函数的用户数据指针
* @param[out]		pbVideoData			视频帧数据
* @param[out]		dwVideoDataLen		视频帧数据长度
* @param[out]		dwVideoType			视频帧类型
* @param[out]		szVideoExtInfo		视频帧信息
* @return			0
*/
typedef INT (CDECL* HVAPI_CALLBACK_H264)(PVOID pUserData,
										 PBYTE pbVideoData,
										 DWORD dwVideoDataLen,
										 DWORD dwVideoType,
										 LPCSTR szVideoExtInfo);

/**
* @brief			历史视频回调函数
* @param[out]		pUserData			由HVAPI_SetCallBack设置的传入回调函数的用户数据指针
* @param[out]		pbVideoData			视频帧数据
* @param[out]		dwVideoDataLen		视频帧数据长度
* @param[out]		dwVideoType			视频帧类型
* @param[out]		szVideoExtInfo		视频帧信息
* @return			成功S_OK 失败 E_FAILE
*/
typedef INT (CDECL* HVAPI_CALLBACK_HISTORY_VIDEO)(PVOID pUserData,
												  PBYTE pbVideoData,
												  DWORD dwVideoDataLen,
												  DWORD dwVideoType,
												  LPCSTR szVideoExtInfo);


//回调函数的定义

/**
* @brief               视频回调函数
* @param[out]          pUserData          由HVAPI_StartRecvH264Video设置的传入回调函数的用户数据指针
* @param[out]          dwVedioFlag        视频标记 ：有效数据标记、无效数据标记、历史结束标记
* @param[out]          dwVideoType        视频类型 ：历史视频、实时视频
* @param[out]          dwWidth            宽度
* @param[out]          dwHeight			  高度
* @param[out]          dw64TimeMS         视频时间
* @param[out]          pbVideoData        视频帧数据
* @param[out]          dwVideoDataLen     视频帧长度
* @param[out]          szVideoExtInfo     视频附加信息
* @return              成功S_OK 失败 E_FAILE
*/
typedef INT (CDECL* HVAPI_CALLBACK_H264_EX)(
												PVOID pUserData,  
												DWORD dwVedioFlag,
												DWORD dwVideoType, 
												DWORD dwWidth,
												DWORD dwHeight,
												DWORD64 dw64TimeMS,
												PBYTE pbVideoData, 
												DWORD dwVideoDataLen,
												LPCSTR szVideoExtInfo
												);

/**
* @brief               jpeg流回调函数
* @param[out]          pUserData          由HVAPI_StartRecvH264Video设置的传入回调函数的用户数据指针
* @param[out]          dwImageFlag        图片标记 ：有效图片数据、无效图片数据
* @param[out]          dwImageType        图片类型 ：调试码流、正常码流
* @param[out]          dwWidth            宽度
* @param[out]          dwHeight			  高度
* @param[out]          dw64TimeMS         JPEG时间
* @param[out]          pbImageData        图片帧数据
* @param[out]          dwImageDataLen     图片帧长度
* @param[out]          szImageExtInfo     JPEG附加信息
* @return              成功S_OK 失败 E_FAILE
*/

typedef INT (CDECL* HVAPI_CALLBACK_JPEG_EX)(
											PVOID pUserData,  
											DWORD dwImageFlag,
											DWORD dwImageType, 
											DWORD dwWidth,
											DWORD dwHeight,
											DWORD64 dw64TimeMS,
											PBYTE pbImageData, 
											DWORD dwImageDataLen,
											LPCSTR szImageExtInfo
											);

/**
* @brief               识别结果回调函数
* @param[out]          pUserData          由HVAPI_StartRecvResult设置的传入回调函数的用户数据指针
* @param[out]          dwResultFlag		  结果标志：有效结果标记、无效结果标记、接收历史结束标记
* @param[out]          dwResultType		  结果类型：实时结果、历史结果
* @param[out]          dwCarID            结果ID
* @param[out]          pcPlateNo          车牌字符串
* @param[out]          dw64TimeMS		  结果时标
* @param[out]          pPlate			  车牌小图
* @param[out]          pPlateBin          车牌二值图
* @param[out]          pBestSnapshot      最清晰大图
* @param[out]          pLastSnapshot      最后大图
* @param[out]          pBeginCapture      第一张抓拍图
* @param[out]          pBestCapture       第二张抓拍图
* @param[out]          pLastCapture       第三张抓拍图
* @return              成功S_OK 失败 E_FAILE
*/
//注意 plate 大图 小图 二维图 回传有dwCardID信息，_string 和 _tfd_string无dwCardID信息
typedef INT (CDECL* HVAPI_CALLBACK_RESULT)(
	PVOID pUserData, 
	DWORD dwResultFlag,			
	DWORD dwResultType,		
	DWORD dwCarID,
	LPCSTR pcPlateNo,
	LPCSTR pcAppendInfo,
	DWORD64 dw64TimeMS,
	CImageInfo   Plate,
	CImageInfo   PlateBin,
	CImageInfo   BestSnapshot,
	CImageInfo   LastSnapshot,
	CImageInfo   BeginCapture,
	CImageInfo   BestCapture,
	CImageInfo   LastCapture
	);

/**
* @brief               二次开发结果回调函数
* @param[out]          pUserData          由HVAPI_StartRecvReDevelopResult设置的传入回调函数的用户数据指针
* @param[out]          pbInfo        数据
* @param[out]          dwInfoLen     长度
* @param[out]          pbData        图片数据
* @param[out]          dwDataLen     图片长度
* @param[out]          nWidth     图片宽度
* @param[out]          nHeight    图片高度度
* @return              成功S_OK 失败 E_FAILE
*/
typedef INT (CDECL* HVAPI_CALLBACK_REDEVELOP_RESULT)(
	PVOID pUserData, 
	PBYTE pbInfo,
	DWORD dwInfoLen,
	PBYTE pbData,
	DWORD dwDataLen,
	INT nWidth,
	INT nHeight
	);


/**
* @brief			监听信号触发回调函数
* @param[in]		pUserData		由HVAPI_SetCallBack设置的传入回调函数的用户数据指针
* @param[in]		dwTime			触发时间
* @param[in]		dwRoadID		道路通道ID
* @param[in]		dwTrigerID		触发ID
* @return			0
*/
typedef INT (CDECL* HVAPI_CALLBACK_LISTEN_CMD)(PVOID pUserData,DWORD64 dwTime,DWORD dwRoadID,DWORD dwTrigerID);

/**
* @brief			监听信号触发回调函数扩展
* @param[in]		pUserData		由HVAPI_SetCallBack设置的传入回调函数的用户数据指针
* @param[in]		pXMLData		XML信息
* @param[in]		iLen			XML数据长度
* @return			0
*/

typedef INT (CDECL* HVAPI_CALLBACK_LISTEN_CMDEX)(PVOID pUserData,LPCSTR pXMLData,INT iLen);


/**
* @brief		搜索设备
* @param[out]	pdwDeviceCount		  设备数
* @return		成功S_OK 失败 E_FAILE
*/
HV_API_EX HRESULT HVAPI_SearchDeviceCount(DWORD32* pdwDeviceCount);

/**
* @brief		获取设备IP
* @param[in]	dwIndex			搜索时保存的数组位置
* @param[out]	dw64MacAddr		设备MAC地址
* @param[out]	dwIP			设备IP
* @param[out]	dwMask			掩码
* @param[out]	dwGateWay		网关
* @return		成功S_OK 失败 E_FAILE
*/
HV_API_EX HRESULT HVAPI_GetDeviceAddr(DWORD32 dwIndex, DWORD64* dw64MacAddr, DWORD32* dwIP,
								   DWORD32* dwMask, DWORD32* dwGateWay);

/**
* @brief		    地球新接口 注释待整理
* @param[in,out]	rgDevInfo	搜索到的设备信息结构体数组
* @param[in,out]	pnDevCount	搜索到的设备数量
* @return			成功：S_OK；失败：E_FAIL, 传入结构体数量太少：S_FALSE 
*/
HV_API_EX HRESULT CDECL HVAPI_SearchDeviceEx( CDevBasicInfo* rgDevInfo , INT* pnDevCount );

/**
* @brief		通过MAC地址设置设备IP
* @param[in]	dw64MacAddr		设备MAC地址
* @param[in]	dwIP			设备IP
* @param[in]	dwMask			掩码
* @param[in]	dwGateWay		网关
* @return		成功S_OK 失败 E_FAILE
*/
HV_API_EX HRESULT CDECL HVAPI_SetIPByMacAddr(DWORD64 dw64MacAddr, DWORD32 dwIP, DWORD32 dwMask, DWORD32 dwGateway);

/**
* @brief		从搜索到的信息里获取设备附加信息
* @param[in]	dwIndex			搜索时保存的数组位置
* @param[out]	lpExtInfo		设备类型信息缓存
* @param[in]	iBufLen			设备类型信息缓存长度
* @return		成功S_OK 失败 E_FAILE
*/
HV_API_EX HRESULT HVAPI_GetDeviceInfoEx(int iIndex, LPSTR lpExtInfo, int iBufLen);

/**
* @brief		获取设备类型
* @param[in]	pcIP		设备IP
* @param[out]	iDeviceType	设备类型
* @return		成功S_OK 失败 E_FAILE
*/
HV_API_EX HRESULT CDECL HVAPI_GetDevTypeEx(PSTR pcIP, int* iDeviceType);

/**
* @brief			打开设备句柄
* @param[in]		szIp			设备的IP地址
* @param[in]		szApiVer		对应设备的API版本。注：为NULL则默认HVAPI_API_VERSION
* @return			成功：设备句柄；失败：NULL
*/
HV_API_EX HVAPI_HANDLE_EX CDECL HVAPI_OpenEx(LPCSTR szIp, LPCSTR szApiVer);

/**
* @brief			关闭设备句柄
* @param[in]		hHandle			对应设备的有效句柄
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_CloseEx(HVAPI_HANDLE_EX hHandle);


/**
* @brief			获取连接设备使用的XML协议版本
* @param[in]		hHandle			对应设备的有效句柄
* @param[out]		pemProtocolVersion	该连接使用的XML协议版本
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetXmlVersionEx(HVAPI_HANDLE_EX hHandle, PROTOCOL_VERSION* pemProtocolVersion);


/**
* @brief			获取连接设备使用的协议版本
* @param[in]		hHandle			对应设备的有效句柄
* @param[out]		pemProtocolVersion	该连接使用的协议版本
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetProtocolVersionEx(HVAPI_HANDLE_EX hHandle, PROTOCOL_VERSION* pemProtocolVersion);


/**
* @brief			获取连接状态
* @param[in]		hHandle			对应设备的有效句柄
* @param[in]		nStreamType		数据流类型（即：连接类型）
* @param[out]		pdwConnStatus	该连接的当前状态
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetConnStatusEx(HVAPI_HANDLE_EX hHandle, INT nStreamType, DWORD* pdwConnStatus);


/**
* @brief			获取重连次数
* @param[in]		hHandle				对应设备的有效句柄
* @param[in]		nStreamType			数据流类型（即：连接类型）
* @param[out]		pdwReConnectTimes	该连接的当前重连次数
* @param[in]		fIsReset			获取完后是否将该连接的重连次数清零
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetReConnectTimesEx(HVAPI_HANDLE_EX hHandle, INT nStreamType, DWORD* pdwReConnectTimes, BOOL fIsReset);


/**
* @brief			执行命令
* @param[in]		hHandle			对应设备的有效句柄
* @param[in]		szCmd			用于向设备发出某条命令的字符串
* @param[in]		szRetBuf		返回的执行结果(XML形式)
* @param[in]		nBufLen			szRetBuf缓冲区的长度
* @param[out]		pnRetLen		实际返回的执行结果长度。注：为NULL则忽略该参数。
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_ExecCmdEx(HVAPI_HANDLE_EX hHandle,
								   LPCSTR szCmd,
								   LPSTR szRetBuf,
								   INT nBufLen,
								   INT* pnRetLen);

/**
* @brief			获取设备参数
* @param[in]		hHandle			对应设备的有效句柄
* @param[in]		szParamDoc		获取到的参数（XML形式）
* @param[in]		nBufLen			szParam缓冲区的长度
* @param[out]		pnRetLen		实际返回的参数长度。注：为NULL则忽略该参数。
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetParamEx(HVAPI_HANDLE_EX hHandle,
									LPSTR szParamDoc,
									INT nBufLen,
									INT* pnRetLen);

/**
* @brief			设置设备参数
* @param[in]		hHandle			对应设备的有效句柄
* @param[in]		szParamDoc		要保存到设备的参数（XML形式）
* @return			成功：S_OK；失败：E_FAIL
* @warning			更改过的参数要在设备下次启动后才会生效
*/
HV_API_EX HRESULT CDECL HVAPI_SetParamEx(HVAPI_HANDLE_EX hHandle, LPCSTR szParamDoc);

/**
* @brief			上传数控版控制板升级文件到一体机
* @param[in]		hHandle			对应设备的有效句柄
* @param[in]		szParamDoc		要保存到设备的参数（XML形式）
* @return			成功：S_OK；失败：E_FAIL
* @warning			更改过的参数要在设备下次启动后才会生效
*/
HV_API_EX HRESULT CDECL HVAPI_SendControllPannelUpdateFileEx(HVAPI_HANDLE_EX hHandle,
														PBYTE pUpdateFileBuffer,
														DWORD dwFileSize);

/**
* @brief			设置数据流接收回调函数
* @param[in]		hHandle			对应设备的有效句柄
* @param[in]		pFunc			回调函数指针。注：为NULL时表示关闭nStreamType类型的数据流。
* @param[in]		pUserData		传入回调函数的用户数据指针
* @param[in]		iVideoID		视频通道，目前只使用0
* @param[in]		nStreamType		回调数据流类型
* @param[in]		szConnCmd		数据流连接命令
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetCallBackEx(HVAPI_HANDLE_EX hHandle, PVOID pFunc, PVOID pUserData,
									   INT iVideoID, INT iCallBackType, LPCSTR szConnCmd);

/**
* @brief              雷达信息接收
* @param[in]          hHandle				 设备句柄
* @param[in]          pFunc					 回调函数指针
* @param[in]          pUserData				 用户数据
* @return             成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL  HVAPI_StartRecvRadarData
(
	HVAPI_HANDLE_EX  hHandle,
	PVOID  pFunc,
	PVOID  pUserData
 );

/**
* @brief              停止雷达信息接收
* @param[in]          hHandle			设备句柄
* @return             成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL  HVAPI_StopRecvRadarData
(
	HVAPI_HANDLE_EX  hHandle
 );

/**
* @brief              设置雷达信息
* @param[in]          hHandle				 设备句柄
* @param[in]          pRadarInfo			 要设置的雷达信息
* @param[in]          dwRadarInfoLen		 雷达信息的长度
* @return             成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL  HVAPI_SetRadarInfo(HVAPI_HANDLE_EX hHandle,PBYTE  pRadarInfo, DWORD  dwRadarInfoLen);


/**
* @brief              启动H264接收
* @param[in]          hHandle				设备句柄
* @param[in]          pFunc				    回调函数指针
* @param[in]          pUserData			    用户数据
* @param[in]          iVideoID				视频通道，目前只使用0
* @param[in]          dw64BeginTimeMS       开始时间
* @param[in]          dw64EndTimeMS         结束时间 
* @param[in]          dwRecvFlag            接收标志 :实时视频、历史视频
* @return             成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_StartRecvH264Video(
	HVAPI_HANDLE_EX hHandle,
	PVOID pFunc,
	PVOID pUserData,
	INT iVideoID ,       
	DWORD64 dw64BeginTimeMS,
	DWORD64 dw64EndTimeMS,
	DWORD dwRecvFlag
);
/**
* @brief              停止H264接收
* @param[in]          hHandle          设备句柄
* @return             成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_StopRecvH264Video(HVAPI_HANDLE_EX hHandle);


/**
* @brief              启动MJPEG接收
* @param[in]          hHandle				 设备句柄
* @param[in]          pFunc					 回调函数指针
* @param[in]          pUserData				 用户数据
* @param[in]          iVideoID				 视频通道，目前只使用0
* @param[in]          dwRecvFlag			 接收标志：调试码流、正常码流
* @return             成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_StartRecvMJPEG(
					 HVAPI_HANDLE_EX hHandle,
					 PVOID pFunc,
					 PVOID pUserData,
					 INT iVideoID ,
					 DWORD dwRecvFlag
					 );
/**
* @brief              停止MJPEG接收
* @param[in]          hHandle          设备句柄
* @return             成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL  HVAPI_StopRecvMJPEG(HVAPI_HANDLE_EX hHandle);
/**
* @brief              更改调试码流打开状态
* @param[in]          hHandle          设备句柄
* @param[in]          fEnable          是否打开调试码流
* @return             成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetDebugJpegStatus(HVAPI_HANDLE_EX hHandle, BOOL fEnable);

/**
* @brief              启动接收结果
* @param[in]          hHandle			设备句柄
* @param[in]          pFunc				回调函数指针
* @param[in]          pUserData			用户数据
* @param[in]          iVideoID          视频通道，目前只使用0
* @param[in]          dw64BeginTimeMS       开始时间
* @param[in]          dw64EndTimeMS         结束时间
* @param[in]          dwStartIndex			起始帧号
* @param[in]          dwRecvFlag			接收标志
* @return             成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_StartRecvResult(
						HVAPI_HANDLE_EX hHandle,
						PVOID pFunc,
						PVOID pUserData,
						INT iVideoID,
						DWORD64 dw64BeginTimeMS,
						DWORD64 dw64EndTimeMS,
						DWORD dwStartIndex,
						DWORD dwRecvFlag
					 );
/**
* @brief              停止结果接收
* @param[in]          hHandle          设备句柄
* @return             成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_StopRecvResult(HVAPI_HANDLE_EX hHandle);


/**
* @brief              启动状态信息接收
* @param[in]          hHandle				 设备句柄
* @param[in]          pFunc					 回调函数指针
* @param[in]          pUserData				 用户数据
* @param[in]          iVideoID				 视频通道，目前只使用
* @param[in]          dwRecvFlag			 接收标志：保留
* @return             成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_StartRecvMsg(
	HVAPI_HANDLE_EX hHandle,
	PVOID pFunc,
	PVOID pUserData,
	INT iVideoID ,
	DWORD dwRecvFlag
);

/**
* @brief		启动二次开发结果接收
* @param[in]	hHandle	        对应设备的有效句柄
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL  HVAPI_StartRecvRedevelopResult
(
	HVAPI_HANDLE_EX hHandle,
	PVOID pFunc,
	PVOID pUserData
 );

/**
* @brief		停止二次开发结果接收
* @param[in]	hHandle	        对应设备的有效句柄
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL  HVAPI_StopRecvRedevelopResult
(
	HVAPI_HANDLE_EX hHandle
);

/**
* @brief		触发信号（用于开闸）
* @param[in]	hHandle	        对应设备的有效句柄
*@param[in]   iVideo		   视频编号
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_TriggerSignal(HVAPI_HANDLE_EX hHandle, INT iVideoID);

/**
* @brief		触发报警信号， 用于控制报警设备
* @param[in]	hHandle	        对应设备的有效句柄
* @return		成功：S_OK；失败：E_FAIL
命令名为： SendAlarmTriggerOut
*/
HV_API_EX HRESULT CDECL HVAPI_TriggerAlarmSignal(HVAPI_HANDLE_EX hHandle);


/**
* @brief		导入黑白名单
* @param[in]	hHandle	        对应设备的有效句柄
* @param[in]	szWhileNameList	 白名单列表,各车牌(车牌-有效期)用$连接起来，如:桂A11111-2013010101$桂A2222-2013010202$桂A33333-2013020202   如果指针为NULL,表示不设置。
* @param[in]	iWhileListLen		白名单字符串，
* @param[in]	szBlackNameList		名单字符串， 各车牌(车牌-有效期)用$连接起来，如:桂A11111-2013010101$桂A2222-2013010202$桂A33333-2013020202   如果指针表示不设置
* @param[in]	iListLen		名单字符串长度 
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_InportNameList(HVAPI_HANDLE_EX hHandle, CHAR* szWhileNaneList, INT iWhileListLen, CHAR* szBlackNameList, INT iBlackListLen);


/**
* @brief		获取黑白名单
* @param[in]	hHandle	        对应设备的有效句柄 
* @param[out]	szWhileNameList		白名单字符串缓存区， 各车牌(车牌-有效期)用$连接起来，如:桂A11111-2013010101$桂A2222-2013010202$桂A33333-2013020202   
* @param[in/out]	iWhileListLen		缓存区长度指针， 用于输入和返回 
@param[out]	szBlackNameList		黑名单字符串缓存区， 各车牌(车牌-有效期)用$连接起来，如:桂A11111-2013010101$桂A2222-2013010202$桂A33333-2013020202   
* @param[in/out]	iBlackListLen		缓存区长度指针， 用于输入和返回 

* @return		成功：S_OK；失败：E_FAIL
*/

HV_API_EX HRESULT CDECL HVAPI_GetNameList(HVAPI_HANDLE_EX hHandle, CHAR* szWhileNameList, INT *iWhileListLen, CHAR* szBlackNameList, INT *iBlackListLen);


/**
* @brief              停止状态信息接收
* @param[in]          hHandle          设备句柄
* @return             成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL  HVAPI_StopRecvMsg(HVAPI_HANDLE_EX hHandle);


/**
@Brief	  获取一张抓拍图， 并保存到对应的内存位置   
@Param[in]:   hHandle					对应设备的有效句柄 
@Param[in]:   int nTimeM				nTimeM=0表示当前图片， nTimeM为前nTimeM分钟有结果的最后一张图片， 在行人卡口中有效，其他无效果
@Param[in/out]:   BYTE * pImageBuff		存储抓拍图的缓冲区
@Param[in]:   int iBuffLen				pImageBuff大小
@Param[in/out]:   int * iImageLen		实际抓拍图大小
@Param[in/out]:   DWORD64 * dwTime		抓拍图时间
@Param[in/out]:   DWORD * dwImageWidth	抓拍图宽度
@Param[in/out]:   DWORD * dwImageHeigh	抓拍图高度
@Return:  成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetCaptureImageEx(HVAPI_HANDLE_EX hHandle,int nTimeM, BYTE *pImageBuff, int iBuffLen, int *iImageLen, DWORD64 *dwTime,  DWORD *dwImageWidth, DWORD *dwImageHeigh);


/**
* @brief		获取一张抓拍图， 并保存到对应的位置
* @param[in]	hHandle	        对应设备的有效句柄 
* @param[in]	iVideoID		视频ID编号，暂时无效   
* @param[in]	strFileName		需要保存的文件名(包括路径)
* @return		成功：S_OK；失败：E_FAIL
*/

HV_API_EX HRESULT CDECL HVAPI_GetCaptureImage(HVAPI_HANDLE_EX hHandle, INT iVideoID, const CHAR *strFileName);


/**
* @brief			设置字符叠加
* @param[in]		hHandle					对应设备的有效句柄
* @param[in]		szCharacterDataBuff		点阵数据
* @param[in]		nBuffLen				点阵数据长度
* @param[in]		nTopLeftX				叠加字符左上角坐标X值
* @param[in]		nTopLeftY				叠加字符左上角坐标Y值
* @param[in]		nDateType				日期类型
* @param[in]		nSize					字体大小（像素）
* @param[in]		nR						RGB的R值
* @param[in]		nG						RGB的G值
* @param[in]		nB						RGB的B值
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPIUTILS_SetCharacterValue(HVAPI_HANDLE_EX hHandle, const char* szCharacterDataBuff, 
												  INT nTopLeftX, INT nTopLeftY, INT nDateType, INT nSize, INT nR, INT nG, INT nB);

/**
* @brief			结果大图红灯加红设置
* @param[in]		hHandle			对应设备的有效句柄
* @param[in]		dwEnhanceStyle	加红处理模式 0不加红 1只加红违章结果 2全加红 3只加红抓拍图
* @param[in]		iBrightness		加红后图片亮度增强处理
* @param[in]		iHubThreshold	红灯色度阀值
* @param[in]		iCompressRate	加红处理后图片压缩品质
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetEnhanceRedLightFlagEx(HVAPI_HANDLE_EX hHandle, DWORD dwEnhanceStyle, INT iBrightness, INT iHubThreshold, INT iCompressRate);

/**
* @brief			历史结果JPEG图片帧红灯加红设置
* @param[in]		hHandle			对应设备的有效句柄
* @param[in]		dwEnhanceStyle	加红处理模式 0不加红 1加红
* @param[in]		iBrightness		加红后图片亮度增强处理
* @param[in]		iHubThreshold	红灯色度阀值
* @param[in]		iCompressRate	加红处理后图片压缩品质
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetHistoryVideoEnhanceRedLightFlagEx(HVAPI_HANDLE_EX hHandle, DWORD dwEnhanceStyle, INT iBrightness, INT iHubThreshold, INT iCompressRate);

/**
* @brief			预留接口，用于设备扩展新功能后获取相关的扩展信息
* @param[in]		hHandle			对应设备的有效句柄
* @param[in]		dwType			信息类型
* @param[in]		iBrightness		保存信息的缓存地址
* @param[in/out]	iBrightness		保存信息的缓存地址长度和实际返回信息长度
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetExtensionInfoEx(HVAPI_HANDLE_EX hHandle, DWORD dwType, LPVOID pRetData, INT* iBufLen);

/**
* @brief			从结果附加信息中获取某项信息
* @param[in]		szAppened			结果附加信息
* @param[in]		szInfoName			获取项名
* @param[out]		szRetInfo			结果字串
* @param[in]		iRetInfoBufLen		结果字串保存缓存区长
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPIUTILS_GetRecordInfoFromAppenedStringEx(LPCSTR szAppened, LPCSTR szInfoName, LPSTR szRetInfo, INT iRetInfoBufLen);

/**
* @brief			从结果附加信息中提取全部信息每项目信息使用回车符区分
* @param[in]		pszXmlPlateInfo			结果附加信息
* @param[out]		pszPlateInfoBuf			结果字串缓存区
* @param[in]		iPlateInfoBufLen		结果字串缓存区长度
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPIUTILS_ParsePlateXmlStringEx(LPCSTR pszXmlPlateInfo, LPSTR pszPlateInfoBuf, INT iPlateInfoBufLen);

/**
* @brief			结果车牌小图数据转换成BMP格式
* @param[in]		pbSmallImageData		车牌小图数据
* @param[in]		nSmallImageWidth		车牌小图宽
* @param[in]		nSmallImageHeight		车牌小图高
* @param[out]		pbBitmapData			转换结果BMP数据
* @param[out]		pnBitmapDataLen			转换结果BMP数据长度
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPIUTILS_SmallImageToBitmapEx(PBYTE pbSmallImageData, INT nSmallImageWidth, INT nSmallImageHeight, PBYTE pbBitmapData, INT* pnBitmapDataLen);

/**
* @brief			将识别结果二值图转换为BMP位图
* @param[in]		pbBinImageData		识别结果二值图
* @param[in]		pbBitmapData		BMP位图数据存放缓冲区
* @param[in,out]	pnBitmapDataLen		BMP位图数据存放缓冲区长度。函数返回后则为：BMP位图数据实际长度
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPIUTILS_BinImageToBitmapEx(PBYTE pbBinImageData,
												 PBYTE pbBitmapData,
												 INT* pnBitmapDataLen);

/**
* @brief		从命令执行结果XML字符串中获取相应信息
* @param[in]	fIsNewXmlProtocol	XML协议类型
* @param[in]	pszRetXmlStr		执行命令时返回的执行结果XML字符串
* @param[in]	pszCmdName			命令名称
* @param[in]	pszInfoName			获取信息名称
* @param[out]	pszInfoValue		获取信息结果字串
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPIUTILS_GetExeCmdRetInfoEx(BOOL fIsNewXmlProtocol, LPCSTR pszRetXmlStr, LPCSTR pszCmdName, 
													  LPCSTR pszInfoName, LPSTR pszInfoValue);


HV_API_EX HRESULT CDECL HVAPI_Capture(HVAPI_HANDLE_EX hHandle, char* szValue);

/**
* @brief              向185相机发送触发命令
* @param[in]          hHandle          设备句柄
* @param[in]          dw64CaptureTime    DWORD64型，当前系统时间,为从1970-01-01 00:00:00开始至目前所经过的毫秒数。为触发抓拍时间，如果为空 (NULL)，则自动获取当前时间
* @return             成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SendCaptureCmd(HVAPI_HANDLE_EX hHandle, DWORD64 dw64CaptureTime);

/**
* @brief			开启触发信号监听服务
@param[in]			nNum	最大监听数
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_OpenListenCMDSever(int nNum);

/**
* @brief			关闭触发信号监听服务
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_CloseListenCMDSever();

/**
* @brief			设置监听回调函数
* @param[in]		hHandle			对应设备监听的有效句柄
* @param[in]		pFunc			回调函数指针
* @param[in]		pUserData		传入回调函数的用户数据指针
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetCallBackListen(HVAPI_HANDLE_EX hHandle,PVOID pFunc, PVOID pUserData);

/**
* @brief			设置监听回调函数扩展
* @param[in]		hHandle			对应设备监听的有效句柄
* @param[in]		iCallBackType	传入回调类型
* @param[in]		pFunc			回调函数指针
* @param[in]		pUserData		传入回调函数的用户数据指针
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetCallBackListenEx(HVAPI_HANDLE_EX hHandle,INT iCallBackType,PVOID pFunc, PVOID pUserData);


/**
* @brief			链接监听设备句柄
* @param[in]		szIp			设备的IP地址
* @return			成功：设备句柄；失败：NULL
*/
HV_API_EX HVAPI_HANDLE_EX CDECL HVAPI_OpenListenClientEx(LPCSTR szIp);

/**
* @brief			关闭监听设备句柄
* @param[in]		hHandle			对应设备监听的有效句柄
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_CloseListenClientEx(HVAPI_HANDLE_EX hHandle);


/**
* @brief              取行人卡口人流量(最大只能取一个月数据)
* @param[in]          hHandle          设备句柄
* @param[in]          dw64StartTime    对应获取客流量信息的起始时间,为从1970-01-01 00:00:00开始至目前所经过的毫秒数
* @param[in]          dw64EndTime      对应获取客流量信息的结束时间,为从1970-01-01 00:00:00开始至目前所经过的毫秒数
* @param[out]         szRetInfo		   对应使用者申请的内存区域，函数调用成功后客流量信息更新于此
* @param[in,out]      iLen		       szRetInfo缓冲区长度，函数返回后为字符串实际长度
* @return             成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetPCSFlow(HVAPI_HANDLE_EX hHandle, DWORD64 dw64StartTime, DWORD64 dw64EndTime, CHAR* szRetInfo, INT* iLen);



/**
* @brief              抓拍录像
* @param[in]          hHandle          设备句柄
* @param[in]          iFlag   抓拍录像开始与结束标记，0为结束抓拍，1为开始抓拍
* @return             成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_CaptureVideo(HVAPI_HANDLE_EX hHandle, INT iFlag);

/**
* @brief				补光灯开关
* @param[in]          hHandle          设备句柄
* @param[in]          iFlag   开关标记，0为关闭，1为打开
* @return             成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_OBCLightSwitch(HVAPI_HANDLE_EX hHandle, INT iFlag);



//=====================================ITTS新增接口=========================================

/**
* @brief			打开设备句柄
* @param[in]		szIp			设备的IP地址
* @param[in]		szApiVer		对应设备的API版本。注：为NULL则默认HVAPI_API_VERSION
* @param[in]		nChannelNum		通道号，范围0~6   0为不通过通道访问，  1-6代表通道1到通道6
* @return			成功：设备句柄；失败：NULL
*/
HV_API_EX HVAPI_HANDLE_EX CDECL HVAPI_OpenChannel(LPCSTR szIp, LPCSTR szApiVer, INT iChannelNum = 0);

/**
* @brief		    搜索前端一体机设备
* @param[in]		hHandle			对应设备监听的有效句柄
* @param[in,out]	pCameraInfo	    搜索到的设备信息结构体数组
* @param[in,out]	pnCameraCount	搜索到的设备数量
* @return			成功：S_OK；失败：E_FAIL, 传入结构体数量太少：S_FALSE 
*/
HV_API_EX HRESULT CDECL HVAPI_SearchCameraEx(HVAPI_HANDLE_EX hHandle, CCameraInfo* pCameraInfo,INT* pnCameraCount);




/**
* @brief		    获取设备的IP地址/掩码/网关
* @param[in]		hHandle			对应设备监听的有效句柄
* @param[out]		szIP			IP地址
* @param[in,out]	szIPLen			执行后返回的实际长度
* @param[out]		szMask			掩码
* @param[in,out]	szMaskLen		掩码长度
* @param[out]		szGateWay		网关
* @param[in,out]	szGateWayLen		网关长度
* @param[out]		szMac			Mac地址
* @param[in,out]	szMacLen		Mac地址长度
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetIPInfo(HVAPI_HANDLE_EX hHandle,CHAR* szIP,INT*szIPLen,CHAR* szMask ,INT*szMaskLen ,CHAR* szGateWay,INT*szGateWayLen,CHAR* szMac,INT*szMacLen );



/**
* @brief		    查询NTP服务器是否使能
* @param[in]		hHandle			    对应设备监听的有效句柄
* @param[in,out]	fEnable				IP地址
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetNTPEnable(HVAPI_HANDLE_EX hHandle, BOOL*fEnable );



/**
* @brief		    设置NTP服务器IP/端口/时间间隔
* @param[in]		hHandle			    对应设备监听的有效句柄
* @param[in]		szIP				IP地址
* @param[in]		Port				端口
* @param[in]		UpdateIntervalMS	时间间隔
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetNTPServerIPPort(HVAPI_HANDLE_EX hHandle, CHAR* szIP,INT Port, DWORD UpdateIntervalMS);




/**
* @brief		    获取NTP服务器IP/端口/时间间隔
* @param[in]		hHandle			    对应设备监听的有效句柄
* @param[in,out]	szIP				IP地址
* @param[in,out]	szIPLen		    执行后返回的实际长度
* @param[in,out]	nPort				端口
* @param[in]		UpdateIntervalMS	时间间隔
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetNTPServerIPPort(HVAPI_HANDLE_EX hHandle,CHAR* szIP,INT*szIPLen,INT*nPort,DWORD*UpdateIntervalMS);




/**
* @brief		    获取设备时区
* @param[in]		hHandle			    对应设备监听的有效句柄
* @param[in,out]	iZoneNum			时区
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetTimeZone(HVAPI_HANDLE_EX hHandle, INT*iZoneNum);





/**
* @brief		    获取设备时间
* @param[in]		hHandle			    对应设备监听的有效句柄
* @param[in,out]	iYear				年
* @param[in,out]	iMon				月
* @param[in,out]	iDay				日
* @param[in,out]	iHour				时
* @param[in,out]	iMin				分
* @param[in,out]	iSec				秒
* @param[in,out]	iMSec				毫秒
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetTime(HVAPI_HANDLE_EX hHandle,INT*iYear,INT*iMon,INT*iDay,INT*iHour ,INT*iMin,INT*iSec,INT*iMSec);




/**
* @brief		    格式化硬盘
* @param[in]		hHandle			    对应设备监听的有效句柄
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_FormatHdd(HVAPI_HANDLE_EX hHandle);






/**
* @brief		    注销
* @param[in]		hHandle			对应设备监听的有效句柄
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_LoginOut(HVAPI_HANDLE_EX hHandle);



/**
* @brief		    恢复默认密码
* @param[in]		hHandle			对应设备监听的有效句柄
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_RestoreDefaultPass(HVAPI_HANDLE_EX hHandle);




/**
* @brief		    添加相机到通道
* @param[in]		hHandle			对应设备监听的有效句柄
* @param[in]		AddDevice		设备信息
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_AddCameraToChannel(HVAPI_HANDLE_EX hHandle,AIOAddDevice AddDevice);



/**
* @brief		    添加模拟相机到通道
* @param[in]		hHandle			对应设备监听的有效句柄
* @param[in]		AddDevice		设备信息
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_AddCameraToChannelTest(HVAPI_HANDLE_EX hHandle,AIOAddDevice AddDevice);


/**
* @brief		    删除通道
* @param[in]		hHandle			对应设备监听的有效句柄
* @param[in]		nChannel		通道	
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_RemoveCamaraFromChannel(HVAPI_HANDLE_EX hHandle,INT iChannelID);


/**
* @brief		    删除模拟通道
* @param[in]		hHandle			对应设备监听的有效句柄
* @param[in]		nChannel		通道	
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_RemoveCamaraFromChannelTest(HVAPI_HANDLE_EX hHandle,INT iChannelID);


/**
* @brief		    设置通道连接视频
* @param[in]		hHandle			对应设备监听的有效句柄
* @param[in]		nChannel		通道	
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetChannelConnectH264_Legacy(HVAPI_HANDLE_EX hHandle, INT nChannel, INT nConnectH264);


/**
* @brief		    设置合成图方式、质量、合成图大小、叠加字体格式、大小、颜色、位置
* @param[in]		hHandle			对应设备监听的有效句柄
* @param[in]		iTwoEnable		2张图合成开关	
* @param[in]		iTwoMode		2张图合成方式	
* @param[in]		iThreeMode	    3张图合成方式	
* @param[in]		iSixMode	    6张图合成方式	
* @param[in]		iQuality		合成图质量	
* @param[in]		iOutPutSize		合成图大小 一倍或多倍
* @param[in]		iFontEnable		字体叠加使能
* @param[in]		iFontSize		字体大小
* @param[in]		iFontAddPos		字体叠加位置
* @param[in]		iFontColor		字体颜色 a r g b 每个分量占一个字节
* @param[in]		iFormat			叠加格式
* @param[in]		szContent		自定义叠加文字
* @param[in]		iChannelID		通道	
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetComposeInfoByChannel(HVAPI_HANDLE_EX hHandle,INT iTwoEnable,INT iTwoMode,INT iThreeMode,INT iSixMode,INT iQuality,INT iOutPutSize,INT iFontEnable
													  ,INT iFontSize,INT iFontAddPos,INT iFontColor,INT iFormat ,CHAR*szContent,INT iChannelID);


/**
* @brief		    设置合成图方式、质量、合成图大小、叠加字体格式、大小、颜色、位置
* @param[in]		hHandle			对应设备监听的有效句柄
* @param[in]		iTwoEnable		2张图合成开关	
* @param[in]		iTwoMode		2张图合成方式	
* @param[in]		iThreeEnable	3张图合成开关	
* @param[in]		iThreeMode		3张图合成方式	
* @param[in]		iFourEnable	    4张图合成开关	
* @param[in]		iFourMode		4张图合成方式	
* @param[in]		iSixEnable	    6张图合成开关	
* @param[in]		iSixMode	    6张图合成方式	
* @param[in]		iQuality		合成图质量	
* @param[in]		iOutPutSize		合成图大小 一倍或多倍
* @param[in]		iFontEnable		字体叠加使能
* @param[in]		iFontSize		字体大小
* @param[in]		iFontAddPos		字体叠加位置
* @param[in]		iFontColor		字体颜色 a r g b 每个分量占一个字节
* @param[in]		iFormat			叠加格式
* @param[in]		szContent		自定义叠加文字
* @param[in]		iChannelID		通道	
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetComposeInfoByChannelEx(HVAPI_HANDLE_EX hHandle,INT iTwoEnable,INT iTwoMode,INT iThreeEnable,INT iThreeMode,INT iFourEnable,INT iFourMode,INT iSixEnable,INT iSixMode,INT iQuality,INT iOutPutSize,INT iFontEnable
													  ,INT iFontSize,INT iFontAddPos,INT iFontColor,INT iFormat ,CHAR*szContent,INT iChannelID);



/**
* @brief			查询合成图方式、质量、合成图大小、叠加字体格式、大小、颜色、位置
* @param[in]		hHandle			对应设备监听的有效句柄
* @param[out]		iTwoMode		2张图合成方式
* @param[out]		iTwoEnable	2张图合成方式使能
* @param[out]		iThreeMode	    3张图合成方式	
* @param[out]		iSixMode	    6张图合成方式
* @param[out]		iQuality		合成图质量	
* @param[out]		iOutPutSize		合成图大小 一倍或多倍
* @param[out]		iFontEnable		字体叠加使能
* @param[out]		iFontSize		字体大小
* @param[out]		iFontAddPos		字体叠加位置
* @param[out]		iFontColor		字体颜色 a r g b 每个分量占一个字节
* @param[out]		iFormat			叠加格式
* @param[out]		szContent		自定义叠加文字
* @param[in,out]	iContentLen    实际接收长度
* @param[in]		iChannelID		通道	
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetComposeInfoByChannel(HVAPI_HANDLE_EX hHandle,INT*iTwoMode,INT *iTwoEnable,INT*iThreeMode,INT*iSixMode,INT*iQuality,INT*iOutPutSize,INT*iFontEnable
													  ,INT*iFontSize,INT*iFontAddPos,INT*iFontColor,INT*iFormat ,CHAR*szContent,INT *iContentLen,INT iChannelID);



/**
* @brief			查询合成图方式、质量、合成图大小、叠加字体格式、大小、颜色、位置
* @param[in]		hHandle			对应设备监听的有效句柄
* @param[out]		iTwoEnable		2张图合成开关
* @param[out]		iTwoMode		2张图合成方式
* @param[out]		iThreeEnable	3张图合成开关	
* @param[out]		iThreeMode	    3张图合成方式	
* @param[out]		iFourEnable	    4张图合成开关	
* @param[out]		iFourMode		4张图合成方式	
* @param[out]		iSixEnable	    6张图合成开关	
* @param[out]		iSixMode	    6张图合成方式	
* @param[out]		iQuality		合成图质量	
* @param[out]		iOutPutSize		合成图大小 一倍或多倍
* @param[out]		iFontEnable		字体叠加使能
* @param[out]		iFontSize		字体大小
* @param[out]		iFontAddPos		字体叠加位置
* @param[out]		iFontColor		字体颜色 a r g b 每个分量占一个字节
* @param[out]		iFormat			叠加格式
* @param[out]		szContent		自定义叠加文字
* @param[in,out]	iContentLen    实际接收长度
* @param[in]		iChannelID		通道	
* @return			成功：S_OK；失败：E_FAIL
*/																
HV_API_EX HRESULT CDECL HVAPI_GetComposeInfoByChannelEx(HVAPI_HANDLE_EX hHandle,INT *iTwoEnable,INT*iTwoMode,INT *iThreeEnable,INT *iThreeMode, INT *iFourEnable,INT *iFourMode,INT *iSixEnable,INT*iSixMode,INT*iQuality,INT*iOutPutSize,INT*iFontEnable
													  ,INT*iFontSize,INT*iFontAddPos,INT*iFontColor,INT*iFormat ,CHAR*szContent,INT*iContentLen,INT iChannelID);



/**
* @brief			升级设备
* @param[in]		hHandle			对应设备监听的有效句柄
* @param[in]		UpGradeFile		升级文件名路径	
* @param[in]		UpGradeType		升级类型，1：Upgrade Kernel	2：Upgrade Rootfs 3：Upgrade the whole package
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_UpGrade(HVAPI_HANDLE_EX hHandle, CHAR* UpGradeFile,INT UpGradeType);



/**
* @brief			获取升级状态
* @param[in]		hHandle			对应设备监听的有效句柄
* @param[in]		pvOutBuf		升级状态信息接收缓存	
* @param[in,out]	BufLen			实际接收数据长度
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetUpGradeStatus(HVAPI_HANDLE_EX hHandle,CHAR*pvOutBuf,INT*BufLen);



/**
* @brief			设置设备名称
* @param[in]		hHandle		对应设备监听的有效句柄
* @param[in]		szDevName		设备名称	
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetDevName(HVAPI_HANDLE_EX hHandle,CHAR* szDevName);



/**
* @brief			查询设备名称、编号和系统版本号
* @param[in]		hHandle			对应设备监听的有效句柄
* @param[in]		szDevName			设备名称	
* @param[in]		iDevNameLen		设备名称长度	
* @param[in]		szSno				设备编号
* @param[in]		iSnoLen			设备编号长度		
* @param[in]		szBuildNo			系统版本号
* @param[in]		iBuildNoLen		系统版本号长度	
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetDevNameInfo(HVAPI_HANDLE_EX hHandle,CHAR* szDevName,INT* iDevNameLen,CHAR* szSno,INT* iSnoLen,CHAR* szBuildNo,INT* iBuildNoLen);



/**
* @brief			获取硬盘状态
* @param[in]		hHandle			对应设备监听的有效句柄
* @param[out]		iInsert			硬盘是否插入
* @param[out]		iInit			硬盘是否初始化
* @param[out]		iH264Recording	硬盘是否正在保存录像
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetHddState(HVAPI_HANDLE_EX hHandle,INT*iInsert,INT*iInit, INT* iH264Recording);






/**
* @brief			获取硬盘的存储状态
* @param[in]		hHandle						对应设备监听的有效句柄	
* @param[out]		iHddRate					硬盘使用率	
* @param[out]		iResultRate					结果使用率	
* @param[out]		iH264Rate					视频使用率	
* @param[out]		iResultCnt					结果数	
* @param[out]		iImageCnt					图片数		
* @param[out]		iHddSize					硬盘容量 单位M
* @return			成功：S_OK；失败：E_FAIL 
*/
HV_API_EX HRESULT CDECL HVAPI_GetHddUsage(HVAPI_HANDLE_EX hHandle,INT*iHddRate ,INT* iResultRate,INT* iH264Rate,INT* iResultCnt,INT*iImageCnt, INT* iHddSize);


/**
* @brief		获取视频可用开始结束时间
* @param[in]	hHandle	        对应设备的有效句柄
* @param[out]	szBeginTime		开始时间
* @param[out]	szEndTime		结束时间
* @param[in]	iChannelID	    通道
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetVedioTimeList(HVAPI_HANDLE_EX hHandle, CHAR *szBeginTime ,INT *iBeginTimeLen,CHAR *szEndTime,INT *iEndTimeLen,INT iChannelID);


/**
* @brief		获取结果可用开始结束时间
* @param[in]	hHandle	        对应设备的有效句柄
* @param[out]	szBeginTime		开始时间
* @param[out]	szEndTime		结束时间
* @param[in]	iChannelID	    通道
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetResultTimeList(HVAPI_HANDLE_EX hHandle, INT iTimeRefer, CHAR *szBeginTime ,INT *iBeginTimeLen,CHAR *szEndTime,INT *iEndTimeLen,INT iChannelID);



/**
* @brief		查询通道的连接状态
* @param[in]	hHandle					对应设备的有效句柄
* @param[out]	iChannelConnStatus		通道
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetChannelConnStatus(HVAPI_HANDLE_EX hHandle,  INT* iChannelConnStatus );



/**
* @brief			获取某段时间内的可用结果列表
* @param[in]		hHandle	        对应设备的有效句柄
* @param[in]		ChannelID		通道
* @param[in]		szBeginTime		开始时间,格式2014-12-11 20:30:56
* @param[in]		szBeginTime		结束时间
* @param[in]		iPageNum		当前要查的页（默认第一次是第1页）
* @param[in]		iPageSize		每页大小
* @param[in]		szVehicleType	车辆类型
* @param[in]		iResultType		卡口 or 电子警察数据
* @param[in]		iIllegalType		违章类型
* @param[in,out]	iTotalCount		查询出的总数 1查询总数，-1不查总数
* @param[out]		iListCount		当前返回的列表数
* @param[out]		pResultList		结果列表集
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_QueryResultIndex(HVAPI_HANDLE_EX hHandle, INT iChannelID, INT iTimeRefer, CHAR* szBeginTime, CHAR* szEndTime, INT iPageNum,
	INT iPageSize, CHAR*szVehicleType, INT iResultType, INT iIllegalType, INT* iTotalCount, INT* iListCount, ResultList* pResultList);

/**
* @brief			获取视频某时间段内的可用时间分配
* @param[in]		hHandle	        对应设备的有效句柄
* @param[in]		iChannelID		通道
* @param[in]		szBeginTime		开始时间,格式2014-12-11 20:30:56
* @param[in]		szBeginTime		结束时间
* @param[in]		iPageNum		当前要查的页（默认第一次是第1页）
* @param[in]		iPageSize		每页大小
* @param[out]		iTotalCount		查询出的总数   1查询总数，-1不查总数
* @param[out]		iListCount		当前返回的列表数
* @param[out]		pVideoList		结果列表集
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_QueryVideoIndex(HVAPI_HANDLE_EX hHandle,INT iChannelID,CHAR *szBeginTime,CHAR *szEndTime,INT iPageNum,
	INT iPageSize,INT *iTotalCount,INT *iListCount,VideoList *pVideoList);

// 描述:	图片防篡改标识植入接口
// 参数:	
//			char* pcDstFileName,				植入防篡改标识后输出文件的文件名（包含完整路径）
//			char* pcSrcFileName,				待植入防篡改标识的原始文件的文件名（包含完整路径）
//			PBYTE pbPublicKey,					包含防篡改标识密钥的数据块
//			WORD wKeyLen						密钥数据块长度
// 返回值:  返回S_OK, 表示操作成功,
//          返回E_POINTER, 参数中包含有非法的指针;
//          返回E_FAIL, 表示未知的错误导致操作失败;
//注意：	本函数在成功执行后会将输入流中四张图片的拼接结果写入对应的输出流
HV_API_EX HRESULT _cdecl HV_AntiTamper_Embed(
										char* pcDstFileName,				//植入防篡改标识后输出文件的文件名（包含完整路径）
										char* pcSrcFileName,				//待植入防篡改标识的原始文件的文件名（包含完整路径）
										PBYTE pbPublicKey,					//包含防篡改标识密钥的数据块
										WORD wKeyLen						//密钥数据块长度
										);

// 描述:	图片防篡改标识校验接口
// 参数:	
//			BOOL &fIsOriginal,					校验后结果输出，TRUE为校验通过，FALSE为校验失败（被篡改）
//			char* pcSrcFileName,				待校验的原始文件的文件名（包含完整路径）
//			PBYTE pbPublicKey,					包含防篡改标识密钥的数据块
//			WORD wKeyLen						密钥数据块长度
// 返回值:  返回S_OK, 表示操作成功,
//          返回E_POINTER, 参数中包含有非法的指针;
//          返回E_FAIL, 表示未知的错误导致操作失败;
//注意：	本函数在成功执行后会将输入流中四张图片的拼接结果写入对应的输出流
HV_API_EX HRESULT _cdecl HV_AntiTamper_Examine(
											BOOL &fIsOriginal,					//校验后结果输出，TRUE为校验通过，FALSE为校验失败（被篡改）
											char* pcSrcFileName,				//待校验的原始文件的文件名（包含完整路径）
											PBYTE pbPublicKey,					//包含防篡改标识密钥的数据块
											WORD wKeyLen						//密钥数据块长度
											);

/**
* @brief			获取某段时间内的可用结果列表
* @param[in]		hHandle	        对应设备的有效句柄
* @param[in]		ChannelID		通道
* @param[in]		szBeginTime		开始时间,格式2014-12-11 20:30:56
* @param[in]		szBeginTime		结束时间
* @param[in]		iPageNum		当前要查的页（默认第一次是第1页）
* @param[in]		iPageSize		每页大小
* @param[in]		szVehicleType	车辆类型
* @param[in]		iResultType		卡口 or 电子警察数据
* @param[in]		iIllegalType		违章类型
* @param[in]		szPlate		    车牌
* @param[in]		szCarFace		车脸
* @param[in]		pExtInInfo		附加查询条件指针
* @param[in,out]	iTotalCount		查询出的总数 1查询总数，-1不查总数
* @param[out]		iListCount		当前返回的列表数
* @param[out]		pResultList		结果列表集
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_QueryResultIndexEx(HVAPI_HANDLE_EX hHandle,
												 INT iChannelID,
												 INT iTimeRefer,
												 CHAR* szBeginTime,
												 CHAR* szEndTime,
												 INT iPageNum,
												 INT iPageSize,
												 CHAR* szVehicleType,
												 INT iResultType,
												 INT iIllegalType,
												 CHAR* szPlate,
												 CHAR* szCarFace,
												 PVOID* pInExtInfo,
												 INT* iTotalCount,
												 INT* iListCount,
												 ResultListEx* pResultList);

/**
* @brief		    获取FTP时间
* @param[in]		hHandle			    对应设备监听的有效句柄
* @param[in,out]	nFtpServerNo		FTP服务器序号
* @param[in,out]	iYear				年
* @param[in,out]	iMon				月
* @param[in,out]	iDay				日
* @param[in,out]	iHour				时
* @param[in,out]	iMin				分
* @param[in,out]	iSec				秒
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetFtpSaveTime(HVAPI_HANDLE_EX hHandle, INT nFtpServerNo, INT*iYear,INT*iMon,INT*iDay,INT*iHour ,INT*iMin,INT*iSec);

/**
* @brief		    查询时间段内是否有结果
* @param[in]		hHandle			    对应设备监听的有效句柄
* @param[in]	ChannelID				通道号
* @param[in]	BeginTime				开始时间
* @param[in]	EndTime					结束时间
* @param[int,out]	pnExistResultDataCount  数据个数
* @param[int,out]	rgExistResultData       数据数组
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_ExistResultData(HVAPI_HANDLE_EX hHandle, INT iChannelID, INT iTimeRefer, CHAR *szBeginTime, CHAR *szEndTime, INT *pnExistResultDataCount, CHAR *rgExistResultData);

/**
* @brief		    查询时间段内是否有视频
* @param[in]		hHandle			    对应设备监听的有效句柄
* @param[in]	ChannelID				通道号
* @param[in]	nInterval				查询的间隔
* @param[in]	BeginTime				开始时间
* @param[in]	EndTime					结束时间
* @param[int,out]	pnExistVideoDataCount  数据个数
* @param[int,out]	rgExistVideoData       数据数组
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_ExistVideoData(HVAPI_HANDLE_EX hHandle, INT iChannelID, INT nInterval, CHAR* szBeginTime, CHAR* szEndTime, INT* pnExistVideoDataCount, CHAR* rgExistVideoData);

/**
* @brief		    获得使用的数据库版本号
* @param[in]		hHandle			    对应设备监听的有效句柄
* @param[in]	    nMode				操作模式。 0:获得当前设备中数据库的版本号  1:获得最新支持的数据库版本号
* @param[out]	    pnDbVersion				数据库版本号
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetHddDbVersion(HVAPI_HANDLE_EX hHandle, INT nMode, INT* pnDbVersion );


/**
* @brief		    查询时间段内是否有视频
* @param[in]		hHandle			    对应设备监听的有效句柄
* @param[in]	   nMode				操作模式。  0:开始升级数据库  1:停止升级数据库
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_HddUpDateDb(HVAPI_HANDLE_EX hHandle, INT nMode);

/**
* @brief		    查看数据库升级进度
* @param[in]		hHandle			    对应设备监听的有效句柄
* @param[out]	    pnUpDateDbRate		数据库升级进度。返回小数点后两位:如10.54%，返回1054
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetHddUpDateDbRate(HVAPI_HANDLE_EX hHandle, INT* pnUpDateDbRate );

/**
* @brief		    设置历史数据请求xml附加参数，再使用一次之后，自动恢复为空字符串传
* @param[in]		szExtParam			    历史数据请求xml附加参数
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL  HVAPI_SetDownloadRecordExtParam(CHAR* szExtParam);

/**
* @brief				发送一些通用的设置命令到设备
* @param[in]          hHandle          设备句柄
* @param[in]          szCmd				命令字符串
* @param[in]          nMode				设置的值
* @return             成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SendSetCommand(HVAPI_HANDLE_EX hHandle, CHAR* szCmd, INT nMode);

/**
* @brief				发送一些通用的获取参数的命令到设备
* @param[in]          hHandle          设备句柄
* @param[in]          szCmd				命令字符串
* @param[in]          pnMode			获取的值
* @return             成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SendGetCommand(HVAPI_HANDLE_EX hHandle, CHAR* szCmd, INT* pnMode);

/**
* @brief			通过命令设置设备中的INT参数，实际是执行命令SetSignalHoldTime，设置设备信号保留时间
					适用于收费站版本：3.1.200.90
* @param[in]          hHandle          设备句柄
* @param[in]          szCmd				命令字符串:SetSignalHoldTime
* @param[in]          nMode				设置的值
* @return             成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetIntParamByCmd(HVAPI_HANDLE_EX hHandle, CHAR* szCmd, INT nMode);

/**
* @brief			通过命令获取设备中的INT参数，实际是执行命令GetSignalHoldTime，获取设备信号保留时间
					适用于收费站版本：3.1.200.90
* @param[in]          hHandle          设备句柄
* @param[in]          szCmd				命令字符串:GetSignalHoldTime
* @param[in]          pnMode			获取的值
* @return             成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetIntParamByCmd(HVAPI_HANDLE_EX hHandle, CHAR* szCmd, INT* pnMode);


/**
* @brief		    测试ITTS邮箱发送功能
* @param[in]		szSMTPUrl			    smtp服务器地址
* @param[in]		nSMTPPort			    smtp服务器端口
* @param[in]		szLoginUser			    用户名
* @param[in]		szLoginPassword			密码
* @param[in]		szSenderName			发件人名称
* @param[in]		szFromEmail			    源邮箱
* @param[in]		szToEmail			    目的邮箱
* @param[out]		szMailTestResult		测试结果返回值
* @param[out]		nLen			        结果长度
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL  HVAPI_TestEmail(HVAPI_HANDLE_EX hHandle, 
										 CHAR* szSMTPUrl, 
										 INT nSMTPPort, 
										 CHAR* szLoginUser, 
										 CHAR* szLoginPassword, 
										 CHAR* szSenderName, 
										 CHAR* szFromEmail,
										 CHAR* szToEmail,
										 CHAR* szMailTestResult, 
										 INT* pnLen);

/**
* @brief		    获取3A镜头配置
* @param[out]		pnLensID			    镜头序号
* @param[out]		pnErrorControl			镜头容差
* @param[out]		pZOOMLOW			    最小焦距
* @param[out]		pZOOMHIGHT			    最大焦距
* @param[out]		pFOCUSLENGTH			最大聚焦值
* @param[out]		pIRISLENGHT			    最大光圈值

* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetLensConfig(HVAPI_HANDLE_EX hHandle, 
											INT* pnLensID, 
											INT* pnErrorControl, 
											INT* pZOOMLOW, 
											INT* pZOOMHIGHT, 
											INT* pFOCUSLENGTH, 
											INT* pIRISLENGHT);
#endif
