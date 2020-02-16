#ifndef _HVCAMARA_H__
#define _HVCAMARA_H__

#include "HvDeviceBaseType.h"
#include "HvDeviceCommDef.h"

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
* @brief		设置视频流帧率
* @param[in]	hHandle				对应设备的有效句柄
* @param[in]	nStreamId			视频流ID，0：H264,1:MJPEG
* @param[in]	nFps				帧率
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetStreamFps(HVAPI_HANDLE_EX hHandle, INT nStreamId, INT nFps);

/**
* @brief		设置H264码率控制方式
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nType			0：VBR ,1 CBR
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetH264BitRateControl(HVAPI_HANDLE_EX hHandle, INT nType );

/**
* @brief		设置H264码率
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nBitRate		范围 32768~8388608(32Kbps-8Mbps)
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetH264BitRate(HVAPI_HANDLE_EX hHandle, INT nBitRate );

/**
* @brief		设置JPEG压缩质量
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nJpegCompressRate		范围 20～80
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetJpegCompressRate(HVAPI_HANDLE_EX hHandle, INT nJpegCompressRate );

/**
* @brief		设置字符叠加开关
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nStreamId		视频流ID，0：H264,1:MJPEG
* @param[in]	fOSDEnable		0：关闭，1：打开
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetOSDEnable(HVAPI_HANDLE_EX hHandle, INT nStreamId ,BOOL fOSDEnable);

/**
* @brief		设置时间叠加开关
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nStreamId		视频流ID，0：H264,1:MJPEG
* @param[in]	fEnable			字符叠加时间叠加开关，范围：0：关闭，1：打开
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetOSDTimeEnable(HVAPI_HANDLE_EX hHandle, INT nStreamId, BOOL fEnable);

/**
* @brief		设置车牌字符叠加开关
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nStreamId		视频流ID，0：H264,1:MJPEG
* @param[in]	fOSDEnable		0：关闭，1：打开
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetOSDPlateEnable(HVAPI_HANDLE_EX hHandle, INT nStreamId ,BOOL fOSDEnable);

/**
* @brief	   设置OSD叠加时间格式
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nStreamId		视频流ID，0：H264,1:MJPEG
* @param[in]	nDateSeparator	日期分割符号 0:减号 1:斜杠 2:中文 3:点号
* @param[in]	fShowWeekDay    显示星期几
* @param[in]	fTimeNewLine	日期一行，时间另起一行
* @param[in]	fShowMicroSec	显示微秒
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetOSDTimeFormat(HVAPI_HANDLE_EX hHandle, INT nStreamId, INT nDateSeparator, INT fShowWeekDay, INT fTimeNewLine, INT fShowMicroSec);

/**
* @brief		设置单色字体叠加
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nStreamId		视频流ID，0：H264,1:MJPEG
* @param[in]	fEnable			设置单色字体叠加，范围：0：关闭，1：打开
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetOSDMonochrome(HVAPI_HANDLE_EX hHandle, INT nStreamId, BOOL fEnable);

/**
* @brief		设置字符叠加字体
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nStreamId		视频流ID，0：H264,1:MJPEG
* @param[in]	nFontSize		字体大小，范围：16~32
* @param[in]	nColorR			字体颜色R值，范围0～255
* @param[in]	nColorG			字体颜色G值，范围0～255
* @param[in]	nColorB			字体颜色B值，范围0～255
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetOSDFont(HVAPI_HANDLE_EX hHandle, INT nStreamId ,INT nFontSize ,
										 INT nColorR ,INT nColorG,INT nColorB);

/**
* @brief	    静态字符叠加位置
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nStreamId		视频流ID，0：H264,1:MJPEG
* @param[in]	nPosX			叠加位置X坐标，范围: 0~图像宽
* @param[in]	nPosY			叠加位置Y坐标，范围：0~图像高
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetOSDStaticPos(HVAPI_HANDLE_EX hHandle, INT nStreamId, INT nPosX, INT nPosY);

/**
* @brief	    动态字符叠加位置
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nStreamId		视频流ID，0：H264,1:MJPEG
* @param[in]	nPosX			叠加位置X坐标，范围: 0~图像宽
* @param[in]	nPosY			叠加位置Y坐标，范围：0~图像高
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetOSDDynamicPos(HVAPI_HANDLE_EX hHandle, INT nStreamId, INT nPosX, INT nPosY);

/**
* @brief		设置时间叠加位置
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nStreamId		视频流ID，0：H264,1:MJPEG
* @param[in]	nPosX			叠加位置X坐标，范围: 0~图像宽
* @param[in]	nPosY			叠加位置Y坐标，范围：0~图像高
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetOSDTimePos( HVAPI_HANDLE_EX hHandle,INT nStreamId, INT nPosX ,INT nPosY );

/**
* @brief		设置字符叠加位置（保持兼容）
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nStreamId		视频流ID，0：H264,1:MJPEG
* @param[in]	nPosX			叠加位置X坐标，范围: 0~图像宽
* @param[in]	nPosY			叠加位置Y坐标，范围：0~图像高
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetOSDPos( HVAPI_HANDLE_EX hHandle,INT nStreamId, INT nPosX ,INT nPosY );

/**
* @brief		设置字符叠加静态字符串
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nStreamId		视频流ID，0：H264,1:MJPEG
* @param[in]	szText			叠加字符串 长度范围：0～255
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetOSDStaticText(HVAPI_HANDLE_EX hHandle, INT nStreamId,CHAR* szText  );

/**
* @brief		设置字符叠加动态字符串
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nStreamId		视频流ID，0：H264,1:MJPEG
* @param[in]	szText			叠加字符串 长度范围：0～255
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetOSDDynamicText(HVAPI_HANDLE_EX hHandle, INT nStreamId, CHAR* szText);

/**
* @brief		设置字符叠加字符串
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nStreamId		视频流ID，0：H264,1:MJPEG
* @param[in]	szText			叠加字符串 长度范围：0～255
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetOSDText(HVAPI_HANDLE_EX hHandle, INT nStreamId,CHAR* szText  );

/**
* @brief		设置CVBS制式
* @param[in]	hHandle		对应设备的有效句柄
* @param[in]	nMode		CVBS制式，范围：0:PAL,1:NTSC
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetCVBDisPlayMode(HVAPI_HANDLE_EX hHandle, INT nMode );

/**
* @brief		设置手动快门
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nShutter		快门，范围：100~30000
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetManualShutter(HVAPI_HANDLE_EX hHandle, INT nShutter );

/**
* @brief		设置手动增益
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nGain			增益，范围：0~42
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetManualGain(HVAPI_HANDLE_EX hHandle, INT nGain );

/**
* @brief		设置AWB开关
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nEnable			AWB开关,范围：0：关闭，1：打开
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetAWBEnable(HVAPI_HANDLE_EX hHandle, INT nEnable );

/**
* @brief		设置手动RGB
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nGainR			R值，范围：0～255
* @param[in]	nGainG			G值，范围：0～255
* @param[in]	nGainB			B值，范围：0～255
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetManualRGB(HVAPI_HANDLE_EX hHandle, INT nGainR, INT nGainG, INT nGainB  );


/**
* @brief		设置AGC使能开关
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nEnable			开关，范围：0：关闭，1：打开
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetAGCEnable(HVAPI_HANDLE_EX hHandle, INT nEnable );


/**
* @brief		设置AGC基准值
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nLightBaseLine	基准值，范围：0～255
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetAGCLightBaseLine(HVAPI_HANDLE_EX hHandle, INT nLightBaseLine );


/**
* @brief		设置AGC区域
* @param[in]	hHandle		对应设备的有效句柄
* @param[in]	rgZone		AGC区域值，范围：0：区域外，1：区域内
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetAGCZone(HVAPI_HANDLE_EX hHandle, INT rgZone[16] );

/**
* @brief		获取AGC检测区域,16个值
* @param[in]	hHandle	        对应设备的有效句柄
* @param[in]	rgZone[16]	    ACG区域
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_GetAGCZone(HVAPI_HANDLE_EX hHandle, INT rgZone[16]);


/**
* @brief		设置AGC参数
* @param[in]	hHandle		对应设备的有效句柄
* @param[in]	nShutterMin ,AGC最小快门，范围： 100~30000
* @param[in]	nShutterMax ,AGC最大快门，范围： 100~30000
* @param[in]	nGainMin    ,AGC最小增益，范围： 0~42
* @param[in]	nGainMax    ,AGC最大增益，范围： 0~42
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetAGCParam(HVAPI_HANDLE_EX hHandle,INT nShutterMin , INT nShutterMax ,
										  INT nGainMin , INT nGainMax );

/**
* @brief		设置亮度LUT
* @param[in]	hHandle				对应设备的有效句柄
* @param[in]	rgLUT				LUT点
* @param[in]	nLUTPointCount		LUT点数，范围：8
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetLUT(HVAPI_HANDLE_EX hHandle, CHvPoint rgLUT[] , INT nLUTPointCount );

/**
* @brief		获取亮度LUT状态
* @param[in]	hHandle	        对应设备的有效句柄
* @param[in]	rgLUT			LUT点状态
* @param[in]	pnLUTPointCount	LUT点数量
** @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_GetLUT(HVAPI_HANDLE_EX hHandle, CHvPoint rgLUT[] , INT* pnLUTPointCount );


/**
* @brief		设置亮度值
* @param[in]	hHandle		 对应设备的有效句柄
* @param[in]	nBrightness ,亮度值，范围：0～255  
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetBrightness(HVAPI_HANDLE_EX hHandle, INT nBrightness );


/**
* @brief		设置对比度
* @param[in]	hHandle		 对应设备的有效句柄
* @param[in]	nContrast    对比度值，范围：0～255  
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetContrast(HVAPI_HANDLE_EX hHandle, INT nContrast );


/**
* @brief		设置锐度
* @param[in]	hHandle		 对应设备的有效句柄
* @param[in]	nSharpness    对比度值，范围：0～255  
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetSharpness(HVAPI_HANDLE_EX hHandle, INT nSharpness );


/**
* @brief		设置饱和度
* @param[in]	hHandle		 对应设备的有效句柄
* @param[in]	nSaturation   饱和度值，范围：0～255  
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetSaturation(HVAPI_HANDLE_EX hHandle, INT nSaturation );


/**
* @brief		设置宽动态开关
* @param[in]	hHandle	      对应设备的有效句柄
* @param[in]	fEnable		  宽动态开关，范围：0：关闭，1：打开  
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetWDREnable(HVAPI_HANDLE_EX hHandle, BOOL fEnable  );


/**
* @brief		设置宽动态等级
* @param[in]	hHandle	      对应设备的有效句柄
* @param[in]	nLevel		  宽动态等级，范围：0：弱，1：中，2：强
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetWDRLevel(HVAPI_HANDLE_EX hHandle, INT nLevel );

/**
* @brief		设置视频降噪TNF开关
* @param[in]	hHandle	      对应设备的有效句柄
* @param[in]	fEnable		  视频降噪开关，范围：0：关闭，1：打开
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetDeNoiseTNFEnable(HVAPI_HANDLE_EX hHandle, BOOL fEnable );


/**
* @brief		设置视频降噪SNF开关
* @param[in]	hHandle	      对应设备的有效句柄
* @param[in]	fEnable		  视频降噪开关，范围：0：关闭，1：打开
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetDeNoiseSNFEnable(HVAPI_HANDLE_EX hHandle, BOOL fEnable );

/**
* @brief		设置视频降噪模式
* @param[in]	hHandle	      对应设备的有效句柄
* @param[in]	nMode		  视频降噪模式，范围：0：速度 ，1：质量 ，该接口设置需复位生效
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetDeNoiseMode(HVAPI_HANDLE_EX hHandle, INT nMode );

/**
* @brief		设置视频降噪组合开关
* @param[in]	hHandle	      对应设备的有效句柄
* @param[in]	nMode		  视频降噪开关，范围：0：SNF,1:TNF,2:TNF+SNF
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetDeNoiseSwitch(HVAPI_HANDLE_EX hHandle, INT nSwitch );

/**
* @brief		设置视频降噪等级
* @param[in]	hHandle	      对应设备的有效句柄
* @param[in]	nLevel		  视频降噪等级，范围：0: 自动 1：弱，2：中，3：强
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetDeNoiseLevel(HVAPI_HANDLE_EX hHandle,INT nLevel );


/**
* @brief		设置滤光片模式
* @param[in]	hHandle	      对应设备的有效句柄
* @param[in]	nMode		  滤光片模式，范围：0: 自动，1：过滤红外 ，2：不过滤红外
										 （若采用非自动模式 晚上采用：2 白天采用：1）
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetFilterMode(HVAPI_HANDLE_EX hHandle, INT nMode );


/**
* @brief		设置滤光片使能
* @param[in]	hHandle	      对应设备的有效句柄	
* @param[in]	fEnable		  滤光片使能，范围：0：关闭，1：打开
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetCtrlCplEnable(HVAPI_HANDLE_EX hHandle, BOOL fEnable );


/**
* @brief		设置DC光圈自动控制
* @param[in]	hHandle	      对应设备的有效句柄
* @param[in]	fEnable		  DC光圈自动控制开关，范围：0：关闭，1：打开
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetDCIRIS(HVAPI_HANDLE_EX hHandle, BOOL fEnable );


/**
* @brief		设置IP信息
* @param[in]	hHandle	      对应设备的有效句柄
* @param[in]	szIP		  IP字符串
* @param[in]	szMask		  子网掩码字符串
* @param[in]	szGateWay	  网关字符串
* @param[in]	szDNS		  DNS字符串
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetIPInfo(HVAPI_HANDLE_EX hHandle, CHAR* szIP 
										,CHAR* szMask 
										,CHAR* szGateWay 
										,CHAR* szDNS  );

/**
* @brief		设置RTSP多播开关
* @param[in]	hHandle	      对应设备的有效句柄
* @param[in]	fEnable		RTSP多播开关，范围：0：关闭，1：打开
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetRTSPMulticastEnable(HVAPI_HANDLE_EX hHandle, BOOL fEnable );


/**
* @brief		设置时间
* @param[in]	hHandle	     对应设备的有效句柄
* @param[in]	nYear		年
* @param[in]	nMon		月
* @param[in]	nDay		日
* @param[in]	nHour		时
* @param[in]	nMin		分
* @param[in]	nSec		秒
* @param[in]	nMSec		毫秒
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetTime(HVAPI_HANDLE_EX hHandle, INT nYear , INT nMon , INT nDay , INT nHour ,INT nMin , INT nSec , INT nMSec );

/**
* @brief		设置时区
* @param[in]	hHandle	     对应设备的有效句柄
* @param[in]	nZoneNum	 时区编号 0~24
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetTimeZone(HVAPI_HANDLE_EX hHandle, INT nZoneNum );

/**
* @brief		设置NTP服务器IP
* @param[in]	hHandle	     对应设备的有效句柄
* @param[in]	szIP		 NTP服务器IP字符串
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetNTPServerIP(HVAPI_HANDLE_EX hHandle, CHAR* szIP );


/**
* @brief		设置NTP服务器IP
* @param[in]	hHandle	     对应设备的有效句柄
* @param[in]	dw32UpdateIntervalMS		 更新NTP服务器时间间隔
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetNTPServerUpdateInterval(HVAPI_HANDLE_EX hHandle,DWORD32 dw32UpdateIntervalMS );


/**
* @brief		设置NTP开关
* @param[in]	hHandle	     对应设备的有效句柄
* @param[in]	fEnable		 NTP服务开关 0：关闭，1：打开
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetNTPEnable(HVAPI_HANDLE_EX hHandle, BOOL fEnable );

/*
* @brief		设置OC门
* @param[in]	hHandle	     对应设备的有效句柄
* @param[in]	fEnable		 开关，范围：0：关闭，1：打开
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetOCGate(HVAPI_HANDLE_EX hHandle, BOOL fEnable );

/*
* @brief		设置色温
* @param[in]	hHandle	     对应设备的有效句柄
* @param[in]	nValue		 范围：0:（自动）; 32; 40 ;52 ;55 ; 60 ;70 
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetColor(HVAPI_HANDLE_EX hHandle, INT nValue );

/*
* @brief		设置2D降噪开关
* @param[in]	hHandle	     对应设备的有效句柄
* @param[in]	fEnable		 范围：0：关闭，1：打开
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_Set2DDeNoiseEnable(HVAPI_HANDLE_EX hHandle, BOOL fEnable );

/*
* @brief		设置2D降噪强度
* @param[in]	hHandle	     对应设备的有效句柄
* @param[in]	nValue		 范围：0～255
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_Set2DDeNoiseStrength(HVAPI_HANDLE_EX hHandle, INT nValue );

/*
* @brief		设置GAMMA开关
* @param[in]	hHandle	     对应设备的有效句柄
* @param[in]	fEnable		 范围：0： 关闭， 1：打开
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetGammaEnable(HVAPI_HANDLE_EX hHandle, BOOL fValue );

/*
* @brief		设置GAMMA强度
* @param[in]	hHandle	     对应设备的有效句柄
* @param[in]	fValue		 范围：1～5
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetGammaStrength(HVAPI_HANDLE_EX hHandle, INT nValue );

/**
* @brief		复位设备
* @param[in]	hHandle	     对应设备的有效句柄
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_ResetDevice(HVAPI_HANDLE_EX hHandle , INT nRetsetMode );

/**
* @brief		恢复默认设置
* @param[in]	hHandle	     对应设备的有效句柄
* @return		成功：S_OK；失败：E_FAIL  
*/
HV_API_EX HRESULT CDECL HVAPI_RestoreDefaultParam(HVAPI_HANDLE_EX hHandle);

/**
* @brief		恢复出厂设置
* @param[in]	hHandle	     对应设备的有效句柄
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_RestoreFactoryParam(HVAPI_HANDLE_EX hHandle);

/**
* @brief		获取设备基本信息
* @param[in]	hHandle	     对应设备的有效句柄
* @param[in]	pbasicInfo	 设备基本信息
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetDevBasicInfo(HVAPI_HANDLE_EX hHandle, CDevBasicInfo* pbasicInfo  );

/**
* @brief		获取设备基本信息
* @param[in]	hHandle	     对应设备的有效句柄
* @param[in]	pbasicInfo	 设备基本信息
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetDevBasicInfoEx(HVAPI_HANDLE_EX hHandle, CDevBasicInfoEx* pbasicInfo  );

/**
* @brief		获取运行模式
* @param[in]	hHandle	     对应设备的有效句柄
* @param[in]	pnRunMode	运行模式指针
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetRunMode(HVAPI_HANDLE_EX hHandle, INT* pnRunMode );

/**
* @brief		获取设备状态
* @param[in]	hHandle	     对应设备的有效句柄
* @param[in]	pState	 状态结构体指针
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetDevState(HVAPI_HANDLE_EX hHandle, CDevState* pState  );

/**
* @brief		获取设备状态
* @param[in]	hHandle	     对应设备的有效句柄
* @param[in]	pReportBuf	 复位报告内存指针
* @param[in]	pnBufLen	 复位报告数据长度
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetResetReport(HVAPI_HANDLE_EX hHandle,CHAR* pReportBuf, INT* pnBufLen  );

/**
* @brief		获取设备状态
* @param[in]	hHandle	        对应设备的有效句柄
* @param[in]	pnResetCount	复位次数
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetResetCount(HVAPI_HANDLE_EX hHandle, INT* pnResetCount );

/**
* @brief		获取设备状态
* @param[in]	hHandle	        对应设备的有效句柄
* @param[in]	szLog			保存日志数据内存指针
* @param[in]	pnLogLen		保存日志数据长度
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetLog(HVAPI_HANDLE_EX hHandle, CHAR* szLog , INT* pnLogLen );

/**
* @brief		获取设备状态
* @param[in]	hHandle	        对应设备的有效句柄
* @param[in]	szLog			保存日志数据内存指针
* @param[in]	pnLogLen		保存日志数据长度
* @param[in]	iLogType        日志类型   1错误日志  2操作日志   3调试日志
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetLogEx(HVAPI_HANDLE_EX hHandle, CHAR* szLog , INT* pnLogLen ,INT iLogType);

/**
* @brief		获取设备状态
* @param[in]	hHandle	        对应设备的有效句柄
* @param[in]	pVideoState		视频状态
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetVideoState(HVAPI_HANDLE_EX hHandle, CVideoState* pVideoState );

/**
* @brief		获取相机状态
* @param[in]	hHandle	        对应设备的有效句柄
* @param[in]	pCameraState	相机状态指针
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetCameraState(HVAPI_HANDLE_EX hHandle, CCameraState* pCameraState );


/**
* @brief		获取相机基本信息
* @param[in]	hHandle	        对应设备的有效句柄
* @param[in]	pCameraBasicInfo	相机基本信息指针
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetCameraBasicInfo(HVAPI_HANDLE_EX hHandle, CCameraBasicInfo* pCameraBasicInfo );

/**
* @brief		获取硬盘检测报告
* @param[in]	hHandle	        对应设备的有效句柄
* @param[in]	szReport		检测报告
* @param[in]	piReportLen		检测报告长度
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetHddCheckReport(HVAPI_HANDLE_EX hHandle, CHAR* szReport , INT *piReportLen  );

/**
* @brief		获取字符叠加信息
* @param[in]	hHandle	        对应设备的有效句柄
* @param[in]	nStreamId		视频流ID
* @param[in]	pOSDInfo		字符叠加信息
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetOSDInfo(HVAPI_HANDLE_EX hHandle,  INT nStreamId , COSDInfo* pOSDInfo  );


/**
* @brief		获取字符叠加信息
* @param[in]	hHandle	        对应设备的有效句柄
* @param[in]	nStreamId		视频流ID
* @param[in]	pOSDInfo		字符叠加信息
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetOSDNewInfo(HVAPI_HANDLE_EX hHandle,  INT nStreamId , COSDNewInfo* pOSDInfo  );


/**
* @brief		登录
* @param[in]	hHandle	        对应设备的有效句柄
* @param[in]	szUserName		用户名字符串  长度范围：4～16字节
* @param[in]	szPassword		密码字符串  长度范围：4～16字节
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_Login(HVAPI_HANDLE_EX hHandle, CHAR* szUserName ,  CHAR* szPassword  );

/**
* @brief		增加用户
* @param[in]	hHandle	        对应设备的有效句柄
* @param[in]	szUserName		用户名字符串 长度范围：4～16字节
* @param[in]	szPassword		密码字符串  长度范围：4～16字节
* @param[in]	nAuthority		用户等级
* @return		成功：S_OK；失败：E_FAIL
* @Remark		此接口调用前，须成功调用HVAPI_Login
*/
HV_API_EX HRESULT CDECL HVAPI_AddUser( HVAPI_HANDLE_EX hHandle, CHAR* szUserName , CHAR* szPassword,  INT nAuthority );

/**
* @brief		删除用户
* @param[in]	hHandle	        对应设备的有效句柄
* @param[in]	szUserName		用户名字符串  长度范围：4～16字节
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
* @Remark		此接口调用前，须成功调用HVAPI_Login
*/
HV_API_EX HRESULT CDECL HVAPI_DelUser(HVAPI_HANDLE_EX hHandle, CHAR* szUserName );

/**
* @brief		获取用户列表
* @param[in]	hHandle	        对应设备的有效句柄
* @param[in]	rgUserInfo		用户信息列表
* @param[in]	pnUserCount		用户数
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
* @Remark		此接口调用前，须成功调用HVAPI_Login
*/
HV_API_EX HRESULT CDECL HVAPI_GetUsersList(HVAPI_HANDLE_EX hHandle, CUserInfo rgUserInfo[] , INT* pnUserCount );

/**
* @brief		修改用户信息
* @param[in]	hHandle	        对应设备的有效句柄
* @param[in]	szUserName		用户名字符串  长度范围：4～16字节
* @param[in]	szOldPassword	密码字符串	  长度范围：4～16字节
* @param[in]	szNewPassword	密码字符串	  长度范围：4～16字节
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
* @Remark		此接口调用前，须成功调用HVAPI_Login
*/
HV_API_EX HRESULT CDECL HVAPI_ModUser(HVAPI_HANDLE_EX hHandle,CHAR* szUserName ,CHAR* szOldPassword, CHAR* szNewPassword );
/**
* @brief		触发图片
* @param[in]	hHandle	        对应设备的有效句柄
* @param[in]	pCapImage		抓拍图内存指针
* @param[in]	pnCapImageSize	抓拍图内存大小
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_TriggerImage(HVAPI_HANDLE_EX hHandle, PBYTE pCapImage , INT* pnCapImageSize );


/**
* @brief		启动设备串口检测功能
* @param[in]	hHandle	        对应设备的有效句柄
* @param[in]	nCOMNum			调试码流开关
* @param[in]	fEnable	        开关标识
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_StartCOMCheck(HVAPI_HANDLE_EX hHandle, INT nCOMNum, BOOL fEnable );


/**
* @brief		设置锐度使能
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetSharpnessEnable(HVAPI_HANDLE_EX hHandle, BOOL fEnable);

/**
* @brief		设置相机光圈放大 
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_ZoomDCIRIS(HVAPI_HANDLE_EX hHandle); 

/**
* @brief		设置相机光圈缩小 
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_ShrinkDCIRIS(HVAPI_HANDLE_EX hHandle); 



/**
* @brief		黑白图模式
* @param[in]	hHandle	        对应设备的有效句柄
* @param[in]	fEnable	        启动标志
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetGrayImageEnable(HVAPI_HANDLE_EX hHandle, BOOL fEnable); 


/**
* @brief		电网同步
* @param[in]	hHandle	        对应设备的有效句柄
* @param[in]	nLevel	        电网同步模式
* @param[in]	dwDelayMS	    同步延时
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetACSync(HVAPI_HANDLE_EX hHandle, INT nLevel, DWORD32 dw32DelayMS); 

/**
* @brief		设置调试码流开关
* @param[in]	hHandle	        对应设备的有效句柄
* @param[in]	fEnable			调试码流开关
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetDebugJpegStatus(HVAPI_HANDLE_EX hHandle, BOOL fEnable);  

/**
* @brief		启动自动测试相机功能
* @param[in]	hHandle	        对应设备的有效句柄
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_StartCameraTest(HVAPI_HANDLE_EX hHandle);  

/**
* @brief		设置jpeg自动压缩使能
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetAutoJpegCompressEnable(HVAPI_HANDLE_EX hHandle,  BOOL fEnable);

/**
* @brief		获取调试等级
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetTraceRank(HVAPI_HANDLE_EX hHandle, INT nlevel);

/**
* @brief		设置jpeg自动压缩参数
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetAutoJpegCompressParam (HVAPI_HANDLE_EX hHandle, INT nMaxCompressRate, INT nMinCompressRate, INT nImageSize);

/**
* @brief		设置图像增强使能
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetImageEnhancementEnable(HVAPI_HANDLE_EX hHandle, BOOL fEnable);

/**
* @brief		设置红灯加红阈值
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetEnRedLightThreshold(HVAPI_HANDLE_EX hHandle, INT nValue);

/**
* @brief		设置增加边缘增强
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetEdgeEnhance(HVAPI_HANDLE_EX hHandle, INT nValue);

/**
* @brief		获取视频采集数据信息
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetRunStatusString(HVAPI_HANDLE_EX hHandle, CHAR* szRunStatusString, INT* pnRunStatusStringLen  );

/**
* @brief		获取增加摄像机工作状态
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetCameraWorkState(HVAPI_HANDLE_EX hHandle, CHAR* szStatus, INT* pnStatusStringLen  );

/**
* @brief		获取硬盘状态
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetHDDStatus(HVAPI_HANDLE_EX hHandle, CHAR* szStatus, INT* pnStatusStringLen  );


/**
* @brief		设置触发抓拍输入IO口触发类型
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetTGIO(HVAPI_HANDLE_EX hHandle, INT nValue  );

/**
* @brief		获取触发抓拍输入IO口触发类型
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetTGIO(HVAPI_HANDLE_EX hHandle, INT* pnValue  );

/**
* @brief		设置触发输出IO口状态 
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetF1IO(HVAPI_HANDLE_EX hHandle, INT nPolarity, INT nType);

/**
* @brief		获取触发输出IO口状态
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetF1IO(HVAPI_HANDLE_EX hHandle, INT* pnPolarity, INT* pnType);


/**
* @brief		设置补光灯IO口状态 
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetEXPIO(HVAPI_HANDLE_EX hHandle, INT nPolarity, INT nType);


/**
* @brief		获取补光灯IO口状态 
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetEXPIO(HVAPI_HANDLE_EX hHandle, INT* pnPolarity, INT* pnType);


/**
* @brief		设置报警IO口状态 
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetALMIO(HVAPI_HANDLE_EX hHandle, INT nPolarity, INT nType);


/**
* @brief		获取报警IO口状态 
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetALMIO(HVAPI_HANDLE_EX hHandle, INT* pnPolarity, INT* pnType);

/**
* @brief		设置MJPEG区域
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetMJPEGRect(HVAPI_HANDLE_EX hHandle, CHvPoint PosTopLeft, CHvPoint PosLowerRight);


/**
* @brief		设置色阶 
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetColorGradation(HVAPI_HANDLE_EX hHandle, INT nValue);

/**
* @brief		设置视频请求控制 
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetVedioRequestControl(HVAPI_HANDLE_EX hHandle, BOOL fEnable,  DWORD32 dwIP, INT nPort);

/**
* @brief		设置AE场景 
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetAEScene(HVAPI_HANDLE_EX hHandle, INT nMode);

/**
* @brief		获取AE场景 
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetAEScene(HVAPI_HANDLE_EX hHandle, INT* pnMode);


/**
* @brief		获取设备名 
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetCustomizedDevName(HVAPI_HANDLE_EX hHandle, CHAR* pszDevName, INT* pnDevNameLen);


/**
* @brief		设置设备名 
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetCustomizedDevName(HVAPI_HANDLE_EX hHandle, CHAR* pszDevName);


/**
* @brief		设置H264码率
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nBitRate		
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetH264SecondBitRate(HVAPI_HANDLE_EX hHandle, INT nBitRate );


/**
* @brief		获取H264码率
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	pnBitRate		
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_GetH264SecondBitRate(HVAPI_HANDLE_EX hHandle, INT* pnBitRate );

/**
* @brief		软触发抓拍
* @param[in]	hHandle			对应设备的有效句柄	
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SoftTriggerCapture(HVAPI_HANDLE_EX hHandle);

/**
* @brief		设置抓拍快门
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nEnable			使能开关
* @param[in]	nValue			抓拍快门值
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetManualCaptureShutter(HVAPI_HANDLE_EX hHandle, INT nEnable, INT nValue);

/**
* @brief		设置白天抓拍快门上限
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nValue			抓拍快门上限值
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetDayCaptureShutterH(HVAPI_HANDLE_EX hHandle,INT nValue);

/**
* @brief		设置抓拍增益
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nEnable			使能开关
* @param[in]	nValue			抓拍增益值
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetManualCaptureGain(HVAPI_HANDLE_EX hHandle, INT nEnable, INT nValue);

/**
* @brief		设置白天抓拍增益上限
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nValue			抓拍增益值
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetDayCaptureGainH(HVAPI_HANDLE_EX hHandle, INT nValue);

/**
* @brief		设置抓拍RGB增益
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nEnable			使能开关
* @param[in]	nValueR			抓拍R增益
* @param[in]	nValueG			抓拍G增益
* @param[in]	nValueB			抓拍B增益
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetManualCaptureRGB(HVAPI_HANDLE_EX hHandle, INT nEnable, INT nValueR, INT nValueG, INT nValueB);

/**
* @brief		设置抓拍锐度
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nEnable			使能开关
* @param[in]	nValue			抓拍锐度值
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetManualCaptureSharpen(HVAPI_HANDLE_EX hHandle, INT nEnable, INT nSharpen);

/**
* @brief		设置LED脉宽
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nValue			脉宽值
* @return		成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetExpPluseWidth(HVAPI_HANDLE_EX hHandle, INT nValue);

/**
* @brief			设置人脸库
* @param[in]		hHandle			对应设备的有效句柄
* @param[in]		nNum			要设置的序号   0标准库  1增强库    2 人脸信息库
* @param[in]		szUploadDate		要保存到设备的数据
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetFaceDataAdv(HVAPI_HANDLE_EX hHandle, INT nNum, CHAR* szUploadDate, DWORD32 dwDataLen);

/**
* @brief			设置DSP参数
* @param[in]		hHandle			对应设备的有效句柄
* @param[in]		szUploadDate		要保存到设备的数据
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetDSPParam(HVAPI_HANDLE_EX hHandle, CHAR* szUploadDate, DWORD32 dwDataLen);

/**
* @brief                设置AE模式
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]	nValue 		AE模式，范围：0:自动;1:手动;2:光圈优先;3:快门优先
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetAEMode(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                获取AE模式
* @param[in]    hHandle                 对应设备的有效句柄    
* @param[out]	pnValue	AE模式   
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_GetAEMode(HVAPI_HANDLE_EX hHandle, INT* pnValue );


/**
* @brief                设置白平衡模式
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue 	白平衡模式，范围0:自动;1:ATW(Auto Tracing White balance);2:室内;3:室外;4:触发一次调节(用白板调时可用到，断电后调节的值才会丢失);5:手动;6:自动户外;7:自动纳灯;8:纳灯
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetManualAWBMode(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                获取白平衡模式
* @param[in]    hHandle                 对应设备的有效句柄    
* @param[out]	pnValue  白平衡模式   
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_GetManualAWBMode(HVAPI_HANDLE_EX hHandle, INT* pnValue );


/**
* @brief                设置AF使能
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue  自动聚焦使能，范围0:自动聚焦;1:手动聚焦;
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetAFEnable(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                获取AF使能
* @param[in]    hHandle                 对应设备的有效句柄    
* @param[out]    pnValue    自动聚焦使能
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_GetAFEnable(HVAPI_HANDLE_EX hHandle, INT* pnValue );


/**
* @brief                设置光圈值
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue  光圈值，范围：13:F1.6;12:F2;11:F2.4;10:F2.8;9:F3.4;8:F4;7:F4.8;6:F5.6;5:F6.8;4:F8;3:F9.6;2:F11;1:F14;0:CLOSE
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetIRIS(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                获取光圈值
* @param[in]    hHandle                 对应设备的有效句柄    
* @param[out]    pnValue   光圈值 
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_GetIRIS(HVAPI_HANDLE_EX hHandle, INT* pnValue );


/**
* @brief                设置聚焦
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue  聚焦值，范围：0x1000 ~ 0xd000
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetFocusing(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                获取聚焦
* @param[in]    hHandle                 对应设备的有效句柄    
* @param[out]    pnValue    聚焦值
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_GetFocusing(HVAPI_HANDLE_EX hHandle, INT* pnValue );


/**
* @brief                设置焦距（设置放大图像）
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue  焦距，范围：100~3000
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetZoom(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                获取焦距（获取放大倍数）
* @param[in]     hHandle                 对应设备的有效句柄    
* @param[out]    pnValue    焦距
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_GetZoom(HVAPI_HANDLE_EX hHandle, INT* pnValue );


/**
* @brief                设置辅助聚焦
* @param[in]    hHandle                 对应设备的有效句柄      
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetAssistantFocus(HVAPI_HANDLE_EX hHandle);

/**
* @brief                微调光圈
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue  调节标识  范围：0 调小，  1调大
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_TuneIris(HVAPI_HANDLE_EX hHandle, INT nValue );
/**
* @brief                微调聚焦
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue  范围：0 调小，  1调大
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_TuneFocus(HVAPI_HANDLE_EX hHandle, INT nValue );

/**
* @brief                开始持续缩放
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue   缩放标识 范围：0放大  1缩小
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_StartZoom(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                停止持续缩放
* @param[in]    hHandle                 对应设备的有效句柄      
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_StopZoom(HVAPI_HANDLE_EX hHandle);

/**
* @brief                设置红外LED补光灯模式
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue  LED模式，范围:0-关闭 1-手动 2-自动控制（设备根据光敏调节亮度） 
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetLEDMode(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                获取红外LED补光灯状态
* @param[in]    hHandle                 对应设备的有效句柄    
* @param[out]    pnValue    LED模式
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_GetLEDMode(HVAPI_HANDLE_EX hHandle, INT* pnValue);;

/**
* @brief                设置红外LED补光灯功率
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue  红外LED补光灯功率，范围：100~3000
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetLEDPower(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                获取红外LED补光灯功率
* @param[in]    hHandle                 对应设备的有效句柄    
* @param[out]    pnValue    红外LED补光灯功率
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_GetLEDPower(HVAPI_HANDLE_EX hHandle, INT* pnValue );

/**
* @brief                开始雨刷工作
* @param[in]    hHandle                 对应设备的有效句柄      
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_StartWiper(HVAPI_HANDLE_EX hHandle);


/**
* @brief                设置预置位
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nNum    预置位序号	范围：0~255
* @param[in]    szName 	预置位名字符串  长度范围：1～31字节
* @param[in]    nValue  检测违章标志 范围： 0：关闭；  1:开启
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetPreset(HVAPI_HANDLE_EX hHandle,  INT nNum, CHAR* szName,  BOOL nValue );


/**
* @brief                获取预置位
* @param[in]    hHandle                 对应设备的有效句柄    
* @param[in]    nNum    预置位序号	范围：0~255
* @param[out]    szName 	预置位名字符串
* @param[out]    pnNameLen 	预置位名字符串长度
* @param[out]    pfValue 	检测违章标志
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_GetPreset(HVAPI_HANDLE_EX hHandle,   INT nNum, CHAR* szName,INT* pnNameLen, BOOL* pfValue );


/**
* @brief                调用预置位
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue  预置位序号	范围：0~255
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_CallPreset(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                清除预置位
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue  预置位序号	范围：0~255
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_ClearPreset(HVAPI_HANDLE_EX hHandle, INT nValue );



/**
* @brief                获取预置位名称列表
* @param[in]    hHandle                 对应设备的有效句柄    
* @param[out]    szNameList  预置位名称和检测违章标志列表(各数据以;号分隔开，名称和检测违章标志用:隔开，如"预置位1:0;预置位2:1;")     
* @param[out]    pnNameListLen    预置位名称列表长度   
* @return               成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetPresetNameList(HVAPI_HANDLE_EX hHandle,  CHAR szNameList[], INT* pnNameListLen );


/**
* @brief                设置画面冻结标志
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    fEnable 画面冻结标志  范围： 0：不冻结 1：冻结    
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetFreezeFlag(HVAPI_HANDLE_EX hHandle, BOOL fEnable);


/**
* @brief                获取画面冻结标志
* @param[in]    hHandle                 对应设备的有效句柄 
* @param[out]    pfEnable   画面冻结标志   
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_GetFreezeFlag(HVAPI_HANDLE_EX hHandle, BOOL* pfEnable);

/**
* @brief                设置水平扫描参数（名称、速度、方向）
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nNum	 水平扫描序号 范围： 0~7
* @param[in]    szScanName   水平扫描名称  长度范围：1～31字节 
* @param[in]    nSpeed   水平扫描速度等级   范围：1~30   
* @param[in]    nDirection   水平扫描方向   范围：0：顺时针 1：逆时针   
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetHScanParam(HVAPI_HANDLE_EX hHandle,  INT nNum, CHAR* szScanName, INT nSpeed, INT nDirection );


/**
* @brief                获取水平扫描参数（名称、速度、方向）
* @param[in]    hHandle                 对应设备的有效句柄    
* @param[in]    nNum	 水平扫描序号 范围： 0~7
* @param[out]    szScanName   水平扫描名称 
* @param[out]    pnNameLen 	水平扫描名字符串长度
* @param[out]    pnSpeed   水平扫描速度等级  
* @param[out]    pnDirection   水平扫描方向     
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_GetHScanParam(HVAPI_HANDLE_EX hHandle,  INT nNum, CHAR* szScanName, INT* pnNameLen, INT* pnSpeed, INT* pnDirection );

/**
* @brief                设置水平扫描左限位
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nNum  水平扫描序号 范围： 0~7
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetHScanLeftPos(HVAPI_HANDLE_EX hHandle, INT nNum );



/**
* @brief                设置水平扫描右限位
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nNum  水平扫描序号 范围： 0~7
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetHScanRightPos(HVAPI_HANDLE_EX hHandle, INT nNum );


/**
* @brief                开始水平扫描
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nNum  水平扫描序号 范围： 0~7
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_StartHScan(HVAPI_HANDLE_EX hHandle, INT nNum );


/**
* @brief                停止水平扫描
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nNum  水平扫描序号 范围： 0~7
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_StopHScan(HVAPI_HANDLE_EX hHandle, INT nNum );

/**
* @brief                清除水平扫描
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nNum  水平扫描序号 范围： 0~7
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_ClearHScan(HVAPI_HANDLE_EX hHandle, INT nNum );



/**
* @brief                开始花样扫描路径录制
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue  花样扫描序号  范围：0~3
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_StartFigureScanRecord(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                停止花样扫描路径录制
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue   花样扫描序号  范围：0~3
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_StopFigureScanRecord(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                调用花样扫描路径
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue   花样扫描序号  范围：0~3
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_CallFigureScan(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                停止花样扫描路径
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue   花样扫描序号  范围：0~3
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_StopFigureScan (HVAPI_HANDLE_EX hHandle, INT nValue );

/**
* @brief                清除花样扫描路径
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue   花样扫描序号  范围：0~3
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_ClearFigureScan(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                设置花样扫描路径参数
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nNum    花样扫描序号	范围：0~255
* @param[in]    szName 	花样扫描名字符串  长度范围：1～31字节
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetFigureScanName(HVAPI_HANDLE_EX hHandle,  INT nNum, CHAR* szName );


/**
* @brief                获取花样扫描路径参数
* @param[in]    hHandle                 对应设备的有效句柄    
* @param[in]    nNum    花样扫描序号	范围：0~255
* @param[out]    szName 	花样扫描名字符串  长度范围：1～31字节
* @param[out]    pnNameLen 	花样扫描名字符串长度
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_GetFigureScanName(HVAPI_HANDLE_EX hHandle,  INT nNum, CHAR* szName, INT* pnNameLen );


/**
* @brief                获取花样扫描名称列表
* @param[in]    hHandle                 对应设备的有效句柄    
* @param[out]    szNameList     花样扫描名称列表(各名称以;号分隔开)         
* @param[out]    pnNameListLen 	花样扫描名称列表长度   
* @return               成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetFigureScanNameList(HVAPI_HANDLE_EX hHandle,  CHAR szNameList[], INT* pnNameListLen );


/**
* @brief                设置巡航路径
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nNum   巡航序号 范围：0~7 
* @param[in]    szName 	巡航名字符串  长度范围：1～31字节 
* @param[in]    nActCount 动作个数 范围： 0~14
* @param[in]    ActType  动作类型数组 范围：动作类型 0~2（0预置位 1水平扫描 2花样扫描 ）  
* @param[in]    ActNum   动作序号数组  范围：（预置位0~255  水平扫描0~7 花样扫描0~3）  
* @param[in]    Time  驻留时间  范围： 1~3600秒
* @param[in]    fCheckPeccancy  保留参数 
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetCruise(HVAPI_HANDLE_EX hHandle,  INT nNum, CHAR* szName, INT nActCount, INT ActType[], INT ActNum[], INT Time[], BOOL fCheckPeccancy );


/**
* @brief                获取巡航路径
* @param[in]    hHandle                 对应设备的有效句柄    
* @param[in]     nNum   巡航序号 范围：0~7 
* @param[out]    szName 	巡航名字符串 
* @param[out]    pNameLen 	巡航名字符串长度 
* @param[out]    pnActCount 动作个数 
* @param[out]    ActType  动作类型数组  
* @param[out]    ActNum   动作序号数组    
* @param[out]    Time  驻留时间  
* @param[out]    pfCheckPeccancy  保留参数  
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_GetCruise(HVAPI_HANDLE_EX hHandle,  INT nNum, CHAR* szName, INT* pNameLen, INT* pnActCount, INT ActType[], INT ActNum[],  INT Time[], BOOL* pfCheckPeccancy );


/**
* @brief                调用巡航路径
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue  巡航序号 范围：0~7 
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_CallCruise(HVAPI_HANDLE_EX hHandle, INT nValue );

/**
* @brief                停止巡航路径
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue  巡航序号 范围：0~7 
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_StopCruise(HVAPI_HANDLE_EX hHandle, INT nValue );

/**
* @brief                清除巡航路径
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue  巡航序号 范围：0~7 
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_ClearCruise(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                设置守望
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    ActType  动作类型   范围：动作类型 0~3（0预制位 1水平扫描 2花样扫描 3巡航 ）  
* @param[in]    ActNum   动作序号  范围：预制位0~255  水平扫描0~7 花样扫描0~3 巡航0~7
* @param[in]    Time     守望等待时间  范围 5~3600秒
* @param[in]    fEnable  是否启用标志  范围： 0~1
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetWatch(HVAPI_HANDLE_EX hHandle,  INT ActType, INT ActNum, INT Time, BOOL fEnable );


/**
* @brief                获取守望
* @param[in]    hHandle                 对应设备的有效句柄    
* @param[out]    ActType  动作类型    
* @param[out]    ActNum   动作序号  
* @param[out]    Time     守望等待时间   
* @param[out]    pfEnable  是否启用标志    
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_GetWatch(HVAPI_HANDLE_EX hHandle,  INT* ActType, INT* ActNum, INT* Time, BOOL* pfEnable );

/**
* @brief                开始守望
* @param[in]    hHandle                 对应设备的有效句柄      
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_StartWatchKeeping(HVAPI_HANDLE_EX hHandle);



/**
* @brief                设置隐私区域
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nNum   隐私区域序号  范围：0~15
* @param[in]    szName  隐私区域名称名字符串  长度范围：1～31字节
* @param[in]    nPoxX  横坐标  范围： 0~1920像素
* @param[in]    nPoxY  纵坐标  范围： 0~1080像素
* @param[in]    nwidth 宽  范围： 0~1920像素
* @param[in]    nheight 高  范围： 0~1080像素
* @param[in]    nColor  隐私区域颜色，范围：0x00~0x0D 0x00:Black   0x01:Gray1   0x02:Gray2   0x03:Gray3   0x04:Gray4   0x05:Gray5   0x06:Gray6   0x07:White   0x08:Red:  0x09:Green   0x0A:Blue: 0x0B:Cyan: 0x0C:Yellow  0x0D:Magenta   
* @param[in]    nPreSet 预置位序号  范围： 0~255
* @param[in]    nFlag  是否启用隐私区域 范围0~1
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetMask(HVAPI_HANDLE_EX hHandle,  INT nNum, CHAR* szName, INT nPoxX,INT nPoxY, INT nwidth, INT nheight, INT nColor, INT nPreSet, INT nFlag );


/**
* @brief                获取隐私区域
* @param[in]    hHandle                 对应设备的有效句柄    
* @param[in]    nNum   隐私区域序号  范围：0~15
* @param[out]    szName  隐私区域名称名字符串 
* @param[out]    pnNameLen 	隐私区域名称字符串长度
* @param[out]    nPoxX  横坐标   
* @param[out]    nPoxY  纵坐标   
* @param[out]    nwidth 宽   
* @param[out]    nheight 高   
* @param[out]    nColor  隐私区域颜色 
* @param[out]    nPreSet 预置位序号   
* @param[out]    pnFlag  是否启用隐私区域  
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_GetMask(HVAPI_HANDLE_EX hHandle,  INT nNum, CHAR* szName,INT* pnNameLen, INT* nPoxX,  INT* nPoxY,  INT* nwidth, INT* nheight,  INT* nColor, INT* nPreSet, INT* pnFlag );

/**
* @brief                清除隐私区域
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue  隐私区域序号  范围：0~15
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_ClearMask(HVAPI_HANDLE_EX hHandle, INT nValue );



/**
* @brief                开始水平运动
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue  水平运动方向  范围： 0：左；1：右
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_StartPan(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                停止水平运动
* @param[in]    hHandle                 对应设备的有效句柄      
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_StopPan(HVAPI_HANDLE_EX hHandle);


/**
* @brief                设置水平运动速度
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue  水平运动速度  范围：1~30
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetPanSpeed(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                获取水平运动速度
* @param[in]    hHandle                 对应设备的有效句柄    
* @param[out]    pnValue	水平运动速度
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_GetPanSpeed(HVAPI_HANDLE_EX hHandle, INT* pnValue );


/**
* @brief                开始垂直运动
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue  垂直运动方向  范围： 0：下 ；  1：上
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_StartTilt(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                停止垂直运动
* @param[in]    hHandle                 对应设备的有效句柄      
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_StopTilt(HVAPI_HANDLE_EX hHandle);


/**
* @brief                开始垂直翻转
* @param[in]    hHandle                 对应设备的有效句柄      
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_StartVturnOver(HVAPI_HANDLE_EX hHandle);


/**
* @brief                停止垂直翻转
* @param[in]    hHandle                 对应设备的有效句柄      
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_StopVturnOver(HVAPI_HANDLE_EX hHandle);


/**
* @brief                设置垂直运动速度
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue  垂直运动速度  范围：1~30
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetTiltSpeed(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                获取垂直运动速度
* @param[in]    hHandle                 对应设备的有效句柄    
* @param[out]    pnValue    垂直运动速度
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_GetTiltSpeed(HVAPI_HANDLE_EX hHandle, INT* pnValue );


/**
* @brief                设置定时功能
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nNum    定时器序号  范围：0~11
* @param[in]    szName   定时器名字符串  长度范围：1～31字节 
* @param[in]    bitArrDays    星期比特向量   范围：0 ~ （2^7-1）   1为周一 2为周二  4周三 8为周四 16为周五 32为周六 64为周日，根据星期分配，进行或操作。
* @param[in]    BeginMinu    开始时间 范围：0~24*60-1 分钟
* @param[in]    EndMinu      结束时间 范围：0~24*60-1 分钟
* @param[in]    ActType    动作类型  范围： 0~3（0预制位 1水平扫描 2花样扫描 3巡航 ）  
* @param[in]    ActNum    动作序号   范围：预制位0~255  水平扫描0~7 花样扫描0~3 巡航0~7 
* @param[in]    fValue    是否启用  范围0~1
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetTimingParam(HVAPI_HANDLE_EX hHandle, INT nNum, CHAR* szName, INT bitArrDays, INT BeginMinu, INT EndMinu, INT ActType, INT ActNum , BOOL fValue );


/**
* @brief                获取定时功能
* @param[in]    hHandle                 对应设备的有效句柄    
* @param[in]    nNum    定时器序号  范围：0~11
* @param[out]    szName   定时器名字符串  长度范围：1～31字节 
* @param[out]    pnNameLen 	定时器名字符串长度
* @param[out]    bitArrDays    星期比特向量   
* @param[out]    BeginMintu    开始时间  
* @param[out]    EndMinu      结束时间 
* @param[out]    ActType    动作类型  
* @param[out]    ActNum    动作序号   
* @param[out]    pfValue    是否启用   
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_GetTimingParam(HVAPI_HANDLE_EX hHandle,INT nNum, CHAR* szName, INT* pnNameLen,INT *bitArrDays, INT* BeginMintu, INT* EndMinu, INT* ActType, INT*ActNum, BOOL* pfValue ) ;


/**
* @brief                设置Tilt运动最大速度
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue  最大垂直运动速度  范围：1~30
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetMaxTiltSpeed(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                获取Tilt运动最大速度
* @param[in]    hHandle                 对应设备的有效句柄    
* @param[out]    pnValue	Tilt运动最大速度
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_GetMaxTiltSpeed(HVAPI_HANDLE_EX hHandle, INT* pnValue );


/**
* @brief                设置Pan运动最大速度
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue  最大水平运动速度  范围：1~30
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetMaxPanSpeed(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                获取Pan运动最大速度
* @param[in]    hHandle                 对应设备的有效句柄    
* @param[out]    pnValue  Pan运动最大速度  
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_GetMaxPanSpeed(HVAPI_HANDLE_EX hHandle, INT* pnValue );


/**
* @brief                设置水平坐标
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue  水平坐标 范围：0~3599
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetPanCoordinate(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                获取水平坐标
* @param[in]    hHandle                 对应设备的有效句柄    
* @param[out]    pnValue    水平坐标
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_GetPanCoordinate(HVAPI_HANDLE_EX hHandle, INT* pnValue );


/**
* @brief                设置垂直坐标
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue  垂直坐标  范围：0~900
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetTiltCoordinate(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                获取垂直坐标
* @param[in]    hHandle                 对应设备的有效句柄    
* @param[out]    pnValue 垂直坐标
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_GetTiltCoordinate(HVAPI_HANDLE_EX hHandle, INT* pnValue );


/**
* @brief                开始水平垂直运动
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue  水平垂直运动方向 范围：0：左下；1左上；2：右下；3：右上
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_StartPanAndTilt(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                停止水平垂直运动
* @param[in]    hHandle                 对应设备的有效句柄      
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_StopPanAndTilt(HVAPI_HANDLE_EX hHandle);


/**
* @brief                点移动到中心
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nPoxX   横坐标 范围：0~1920 像素
* @param[in]    nPoxY	纵坐标 范围：0~1080 像素
* @param[in]    fZoomOut 是否缩小标识  范围：0：不缩小  1：缩小 
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_MovePointToCenter(HVAPI_HANDLE_EX hHandle, INT nPoxX, INT nPoxY, BOOL fZoomOut );


/**
* @brief                区域移动到中心
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nPoxX   横坐标 范围：0~1920 像素
* @param[in]    nPoxY	纵坐标 范围：0~1080 像素
* @param[in]    nWidth   宽   范围：0~1920 像素
* @param[in]    nHeight	高   范围：0~1080 像素
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_MoveBlockToCenter(HVAPI_HANDLE_EX hHandle, INT nPoxX, INT nPoxY, INT nWidth, INT nHeight );

/**
* @brief                复位球机位置到初始点
* @param[in]    hHandle                 对应设备的有效句柄      
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_ResetDevicePosition(HVAPI_HANDLE_EX hHandle);


/**
* @brief			设置球机信息XML
* @param[in]		hHandle			对应设备的有效句柄
* @param[in]		szUploadDate		要保存到设备的数据
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetSpeedDomeParam(HVAPI_HANDLE_EX hHandle,  CHAR* szUploadDate, DWORD32 dwDataLen);

/**
* @brief			下载球机信息XML
* @param[in]		hHandle			对应设备的有效句柄
* @param[in]		szDownloadDate		要保存到设备的数据
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetSpeedDomeParam(HVAPI_HANDLE_EX hHandle, CHAR* szDownloadDate, DWORD32* dwDataLen);

/**
* @brief			上传算法参数
* @param[in]		hHandle			对应设备的有效句柄
* @param[in]		szUploadDate		上传算法参数
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetAlgorithmParameterData(HVAPI_HANDLE_EX hHandle, CHAR* szUploadDate, DWORD32 dwDataLen); 

/**
* @brief			上传算法模型
* @param[in]		hHandle			对应设备的有效句柄
* @param[in]		szUploadDate		上传算法模型
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetAlgorithmModelData(HVAPI_HANDLE_EX hHandle, CHAR* szUploadDate, DWORD32 dwDataLen);

/**
* @brief			透明串口传输
* @param[in]		hHandle			对应设备的有效句柄
* @param[in]		szUploadDate		透明串口传输
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetTransparentSerialData(HVAPI_HANDLE_EX hHandle, CHAR* szUploadDate, DWORD32 dwDataLen);

/**
* @brief			透明串口传输
* @param[in]		hHandle			对应设备的有效句柄
* @param[in]		szUploadDate		透明串口传输
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetLastEor(INT* pEorCode);

/**
* @brief                获取当前执行任务
* @param[in]    hHandle                 对应设备的有效句柄    
* @param[out]    szTaskName     当前执行任务名  范围：花样扫描、水平、巡航、空闲、花样扫描录制、无限水平扫描、复位球机，分别对应Fscan、Hscan、Cruise、idle、FScanRecord、InfinityPan、Reset
* @param[out]    pnNameLen   执行任务名长度     
* @param[out]    pTaskID     当前执行任务ID 范围花样扫描、水平、巡航、空闲、花样扫描录制、无限水平扫描、复位球机，分别对应Fscan（0~3）  Hscan（0~7） Cruise（0~7） idle（ 65535） FScanRecordFScanRecord(0~3) InfinityPan（65535） Reset（65535）
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_GetCurrentTask(HVAPI_HANDLE_EX hHandle, CHAR* szTaskName, INT* pnNameLen, INT* pTaskID );

/**
* @brief                开始无限旋转
* @param[in]    hHandle                 对应设备的有效句柄      
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_StartInfinityPan(HVAPI_HANDLE_EX hHandle);


/**
* @brief                结束无限旋转
* @param[in]    hHandle                 对应设备的有效句柄      
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_StopInfinityPan(HVAPI_HANDLE_EX hHandle);


/**
* @brief                除雾
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue  除雾开关标识 范围：0关闭；1开启
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetDefog(HVAPI_HANDLE_EX hHandle, INT nValue );

/**
* @brief                设置I帧间隔
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue 
* @return               成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetH264IFrameInterval(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                获取I帧间隔
* @param[in]    hHandle                 对应设备的有效句柄    
* @param[out]    pnValue    
* @return               成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetH264IFrameInterval(HVAPI_HANDLE_EX hHandle, INT* pValue );


/**
* @brief                设置VBR的场景变化时间
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue 
* @return               成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetH264VbrDuration(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                设置VBR场景变化的敏感度
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue 
* @return               成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetH264VbrSensitivity(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                设置第二路I帧间隔
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue 
* @return               成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetH264SecondIFrameInterval(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                获取第二路I帧间隔
* @param[in]    hHandle                 对应设备的有效句柄    
* @param[out]    pnValue    
* @return               成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetH264SecondIFrameInterval(HVAPI_HANDLE_EX hHandle, INT* pValue );


/**
* @brief                设置第二路码率控制方式0：VBR  1：CBR
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue 
* @return               成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetH264SecondRateControl(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                设置第二路VBR的场景变化时间
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue 
* @return               成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetH264SecondVbrDuration(HVAPI_HANDLE_EX hHandle, INT nValue );


/**
* @brief                设置第二路VBR场景变化的敏感度
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue 
* @return               成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetH264SecondVbrSensitivity(HVAPI_HANDLE_EX hHandle, INT nValue );

/**
* @brief                设置停止MJPEG字符叠加
* @param[in]    hHandle                 对应设备的有效句柄      
* @param[in]    nValue 
* @return               成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetMJPEGOverlay(HVAPI_HANDLE_EX hHandle, INT nValue );



/**
* @brief			设置JPEG输出类型
* @param[in]		hHandle		对应设备的有效句柄
* @param[in]		nMode		要保存到设备的数据
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetJpegOutType(HVAPI_HANDLE_EX hHandle,  INT nMode);

/**
* @brief			获取JPEG输出类型
* @param[in]		hHandle		对应设备的有效句柄
* @param[out]		pnMode		获取到的类型值
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetJpegOutType(HVAPI_HANDLE_EX hHandle,  INT* pnMode);

/**
* @brief			设置视频触发结果输出选择
* @param[in]		hHandle		对应设备的有效句柄
* @param[in]		nMode		要保存到设备的数据
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetCoilEnable(HVAPI_HANDLE_EX hHandle,  INT nMode);

/**
* @brief			获取视频触发结果输出选择
* @param[in]		hHandle		对应设备的有效句柄
* @param[out]		pnMode		获取到的状态值
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetCoilEnable(HVAPI_HANDLE_EX hHandle,  INT* pnMode);


/**
* @brief			获取AWB工作模式
* @param[in]		hHandle		对应设备的有效句柄
* @param[out]		pfEnable		获取到的类型值
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetAWBWorkMode(HVAPI_HANDLE_EX hHandle,  BOOL* pfEnable);

/**
* @brief			设置AWB工作模式
* @param[in]		hHandle		对应设备的有效句柄
* @param[in]		nEnable		设置到设备的模式值
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetAWBWorkMode(HVAPI_HANDLE_EX hHandle, INT nEnable );

/**
* @brief			获取相机全控制
* @param[in]		hHandle		对应设备的有效句柄
* @param[out]		pfEnable		获取到的状态值
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetAutoControlCammeraEnable(HVAPI_HANDLE_EX hHandle,  BOOL* pfEnable);

/**
* @brief			设置相机全控制
* @param[in]		hHandle		对应设备的有效句柄
* @param[in]		nEnable		设置到设备的模式值
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetAutoControlCammeraEnable(HVAPI_HANDLE_EX hHandle, INT nEnable );

/**
* @brief			获取H264分辨率
* @param[in]		hHandle		对应设备的有效句柄
* @param[out]		piValue		获取到的值
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetH264Resolution(HVAPI_HANDLE_EX hHandle,  INT* piValue);

/**
* @brief			设置H264分辨率
* @param[in]		hHandle		对应设备的有效句柄
* @param[in]		nEnable		设置到设备的值
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetH264Resolution(HVAPI_HANDLE_EX hHandle, INT nEnable );

/**
* @brief                获取自动聚焦状态，判断聚焦成功，必须完成从0到 （发自动聚焦命令）-1 再到 0这个状态的状态序列。
* @param[in]    hHandle                 对应设备的有效句柄    
* @param[out]    pnValue       0：当前未进行自动聚焦。如果是发送自动聚焦命令后，表示自动聚焦已成功；  -1：未完成，自动聚焦中。
* @return               成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetAutoFocusStatus(HVAPI_HANDLE_EX hHandle, INT* pValue );

/**
* @brief                设置F1抓拍输出
* @param[in]    hHandle                 对应设备的有效句柄     
* @param[in]    nEnable    1 使能 0不使能
* @param[in]    nCoupling    1 耦合 0不耦合
* @param[in]    nOutputType    输出类型  1OC（开关）  0上拉（电平）
* @param[in]    nPolarity    1 正极性 0负极性
* @param[in]    nPulseWidth    脉宽 0~100
* @return               成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetF1TriggerOut(HVAPI_HANDLE_EX hHandle, INT nEnable, INT  nCoupling, INT  nOutputType, INT  nPolarity, INT  nPulseWidth );


/**
* @brief                获取F1抓拍输出
* @param[in]    hHandle                 对应设备的有效句柄   
* @return               成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetF1TriggerOut(HVAPI_HANDLE_EX hHandle, INT* pnEnable, INT*  pnCoupling, INT*  pnOutputType, INT*  pnPolarity, INT*  pnPulseWidth );


/**
* @brief                设置ALM口抓拍输出
* @param[in]    hHandle                 对应设备的有效句柄   
* @param[in]    nEnable    1 使能 0不使能
* @param[in]    nCoupling    1 耦合 0不耦合
* @param[in]    nOutputType    输出类型  1OC（开关）  0上拉（电平）
* @param[in]    nPolarity    1 正极性 0负极性
* @param[in]    nPulseWidth    脉宽 0~100
* @return               成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetAlarmTriggerOut(HVAPI_HANDLE_EX hHandle, INT nEnable, INT  nCoupling, INT  nOutputType, INT  nPolarity, INT  nPulseWidth );


/**
* @brief                获取ALM口抓拍输出
* @param[in]    hHandle                 对应设备的有效句柄   
* @return               成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetAlarmTriggerOut(HVAPI_HANDLE_EX hHandle, INT* pnEnable, INT*  pnCoupling, INT*  pnOutputType, INT*  pnPolarity, INT*  pnPulseWidth );

/**
* @brief		设置抓拍参数自动调节使能
* @param[in]	hHandle			对应设备的有效句柄
* @param[in]	nEnable			开关,范围：0：关闭，1：打开
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetCaptureAutoParamEnable(HVAPI_HANDLE_EX hHandle, INT nEnable );

/**
* @brief		获取抓拍参数自动调节使能
* @param[in]	hHandle			对应设备的有效句柄
* @param[out]	nEnable			开关,范围：0：关闭，1：打开
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_GetCaptureAutoParamEnable(HVAPI_HANDLE_EX hHandle, INT* pnEnable );

/**
* @brief		触发辅助安装
* @param[in]	hHandle			对应设备的有效句柄
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_TriggerAssistantInstallation(HVAPI_HANDLE_EX hHandle );

/**
* @brief		获取辅助安装状态
* @param[in]	hHandle			对应设备的有效句柄
* @param[out]	pnValue		状态,范围：0～100， 0未开始  100完成
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_GetAssistantInstallationStatus(HVAPI_HANDLE_EX hHandle, INT* pnValue );

/**
* @brief		设置IP信息
* @param[in]	hHandle	      对应设备的有效句柄
* @param[in]	nEthNO		  网卡号   0表示修改eth0网卡
* @param[in]	szIP		  IP字符串
* @param[in]	szMask		  子网掩码字符串
* @param[in]	szGateWay	  网关字符串
* @param[in]	szDNS		  DNS字符串
* @return		成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetIPInfoEx(HVAPI_HANDLE_EX hHandle, 
										INT  nEthNO,
										CHAR* szIP,
										CHAR* szMask,
										CHAR* szGateWay,
										CHAR* szDNS   );

/**
* @brief		    获取设备的IP地址/掩码/网关
* @param[in]		hHandle			对应设备监听的有效句柄
* @param[out]		pnEthNO			网卡号
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
HV_API_EX HRESULT CDECL HVAPI_GetIPInfoEx(HVAPI_HANDLE_EX hHandle, 
										  INT  nEthNO,
										  CHAR* szIP,
										  INT*szIPLen,
										  CHAR* szMask ,
										  INT*szMaskLen ,
										  CHAR* szGateWay,
										  INT*szGateWayLen,
										  CHAR* szMac,
										  INT*szMacLen  );


/**
* @brief			开始恢复数据库
* @param[in]		hHandle		对应设备监听的有效句柄
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_StartHddRecoverDb(HVAPI_HANDLE_EX hHandle);

/**
* @brief			停止恢复数据库
* @param[in]		hHandle		对应设备监听的有效句柄
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_StopHddRecoverDb(HVAPI_HANDLE_EX hHandle);

/**
* @brief			获得数据库恢复时已经扫描的硬盘大小
* @param[in]		hHandle		对应设备监听的有效句柄
* @param[out]		pnHddSize	已经扫描的硬盘大小（单位GB）
* @return			成功：S_OK；失败：E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetHddRecoverDbSize(HVAPI_HANDLE_EX hHandle, INT* pnHddSize);


/**
* @brief        设置Onvif绑定的网卡
* @param[in]    hHandle 对应设备的有效句柄      
* @param[in]    iEthernet  0或1
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_BindOnvifEthernet(HVAPI_HANDLE_EX hHandle, INT iEthernet );

/**
* @brief        网关拓展设置选项
* @param[in]    hHandle 对应设备的有效句柄      
* @param[in]    iFlag   0主副网关生效,1主网关生效,2副网关生效
* @return               成功：S_OK；失败：E_FAIL  传入参数异常：S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetGatewayEx(HVAPI_HANDLE_EX hHandle, INT iFlag );
#endif
