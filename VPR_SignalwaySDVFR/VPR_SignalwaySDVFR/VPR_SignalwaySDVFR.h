// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 VPR_SIGNALWAYSDVFR_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// VPR_SIGNALWAYSDVFR_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#if defined(__cplusplus) 
#define D_EXTERN_C extern "C"
#else
#define D_EXTERN_C
#endif

#ifdef VPR_SIGNALWAYSDVFR_EXPORTS
#define VPR_SIGNALWAYSDVFR_API D_EXTERN_C __declspec(dllexport)
#else
#define VPR_SIGNALWAYSDVFR_API D_EXTERN_C __declspec(dllimport)
#endif

#ifdef __OS_WIN  //Windows环境定义
#define D_CALLTYPE __stdcall
#define D_DECL_EXPORT __declspec(dllexport)
#define D_DECL_IMPORT __declspec(dllimport)
#else //非Windows环境定义
#define D_CALLTYPE
#define D_DECL_EXPORT     __attribute__((visibility("default")))
#define D_DECL_IMPORT     __attribute__((visibility("default")))
#endif

#include "VPR_SignalwaySDVFR_Commendef.h"

//************************************
// Method:    VLPR_Init
// FullName:  VLPR_Init
// Access:    public 
// Returns:   int
//			0	操作成功
//?		- 100		设备无响应
//?		- 2000	其它错误
// Qualifier: 申请足够的内存空间，保证后续运作
//************************************
VPR_SIGNALWAYSDVFR_API int D_CALLTYPE VLPR_Init();

//************************************
// Method:    VLPR_Deinit
// FullName:  VLPR_Deinit
// Access:    public 
// Returns:   int
//			 0	操作成功
//?		- 100		设备无响应
//?		- 2000		其它错误
// Qualifier:
//************************************
VPR_SIGNALWAYSDVFR_API int D_CALLTYPE  VLPR_Deinit();

//************************************
// Method:    VLPR_Login
// FullName:  VLPR_Login
// Access:    public 
// Returns:   int
//			 >0	打开设备成功，返回值为设备句柄号
//?		- 100		设备无响应
//?		- 1000	传入参数错误
//?		- 1001	设备被占用
//?		- 1002	设备打开失败
//?		- 2000	其它错误
// Qualifier: 主机与设备建立连接
// Parameter: int nType			;连接方式：0=串口，1=网络连接
// Parameter: char * sParas		-串口：填“串口号”
//								例“COM1”
//								- 网络：填“网址，端口，用户名，密码”
//								例“192.168.0.11, 8000, admin, password”
//************************************
VPR_SIGNALWAYSDVFR_API int D_CALLTYPE  VLPR_Login(int nType,char* sParas);

//************************************
// Method:    VLPR_Logout
// FullName:  VLPR_Logout
// Access:    public 
// Returns:   int
//			0	关闭设备成功
//?		- 100		设备无响应
//?		- 1000	传入参数错误
//?		- 1001	设备被占用
// Qualifier: 主机与设备断开连接
// Parameter: int nHandle  ;设备句柄
//************************************
VPR_SIGNALWAYSDVFR_API int D_CALLTYPE  VLPR_Logout(int nHandle);

//************************************
// Method:    VLPR_SetResultCallBack
// FullName:  VLPR_SetResultCallBack
// Access:    public 
// Returns:   int
//			0	操作成功
//?		- 100		设备无响应
//?		- 1000	传入参数错误
//?		- 2000	其它错误
// Qualifier: 设置识别结果触发回调函数
// Parameter: int nHandle				;设备句柄
// Parameter: CBFun_GetFrontResult pFunc	;车头识别结果回调函数 ,NULL时，取消回调
// Parameter: CBFun_GetBackResult pFunc	;车尾识别结果回调函数,NULL时，取消回调
// Parameter: void * pUser				;用户自定义数据
//************************************
VPR_SIGNALWAYSDVFR_API int D_CALLTYPE  VLPR_SetResultCallBack(
	int nHandle,
	CBFun_GetFrontResult pFunc1,
	CBFun_GetBackResult pFunc2,
	void *pUser);

//************************************
// Method:    VLPR_SetStatusCallBack
// FullName:  VLPR_SetStatusCallBack
// Access:    public 
// Returns:   int
//			0	操作成功
//?		- 100	设备无响应
//?		- 1000	传入参数错误
//?		- 2000	其它错误
// Qualifier: 设置回调，定时汇报设备运行状态或当设备发生异常时则立即回调
// Parameter: int nHandle				;设备句柄
// Parameter: int nTimeInvl				;状态汇报时间间隔
// Parameter: CBFun_GetDevStatus pFunc	;异常消息回调函数,func = NULL时，取消回调
// Parameter: void * pUser				;用户自定义数据
//************************************
VPR_SIGNALWAYSDVFR_API int D_CALLTYPE  VLPR_SetStatusCallBack(int nHandle,int nTimeInvl,CBFun_GetDevStatus pFunc,	void* pUser);

//************************************
// Method:    VLPR_GetStatus
// FullName:  VLPR_GetStatus
// Access:    public 
// Returns:   int
//			0	操作成功
//?		- 100		设备无响应
//?		- 1000	传入参数错误
//?		- 2000	其它错误
// Qualifier: 获取设备运行状态
// Parameter: int nHandle			;设备句柄
// Parameter: int * pStatusCode		;设备状态错误码，0表示正常
//************************************
VPR_SIGNALWAYSDVFR_API int D_CALLTYPE  VLPR_GetStatus(int nHandle,int* pStatusCode);

//************************************
// Method:    VLPR_GetStatusMsg
// FullName:  VLPR_GetStatusMsg
// Access:    public 
// Returns:   int
//			0	操作成功
//?		- 100		设备无响应
//?		- 1000	传入参数错误
//?		- 2000	其它错误
// Qualifier: 查询错误码详细描述
// Parameter: int nStatusCode	;错误码
// Parameter: char * sStatusMsg	;错误码信息缓存地址
// Parameter: int nStatusMsgLen	;错误码信息缓存长度
//************************************
VPR_SIGNALWAYSDVFR_API int D_CALLTYPE  VLPR_GetStatusMsg(int nStatusCode,char* sStatusMsg,int nStatusMsgLen);

//************************************
// Method:    VLPR_GetDevInfo
// FullName:  VLPR_GetDevInfo
// Access:    public 
// Returns:   int
//			0	操作成功
//?		- 100		设备无响应
//?		- 1000	传入参数错误
//?		- 2000	其它错误
// Qualifier: 获取设备厂家信息
// Parameter: char * sCompany	;厂家信息缓存地址
// Parameter: int nCompanyLen	;厂家信息缓存长度
// Parameter: char * sDevMode	;设备型号信息缓存地址
// Parameter: int nModeLen		;设备型号信息缓存长度
//************************************
VPR_SIGNALWAYSDVFR_API int D_CALLTYPE  VLPR_GetDevInfo(
	char* sCompany,
	int nCompanyLen,
	char* sDevMode,
	int nModeLen
);

//************************************
// Method:    VLPR_GetVersion
// FullName:  VLPR_GetVersion
// Access:    public 
// Returns:   int
//			0	操作成功
//?		- 100		设备无响应
//?		- 1000	传入参数错误
//?		- 2000	其它错误
// Qualifier: 获取设备及接口版本信息
// Parameter: char * sDevVersion	;设备版本信息缓存地址
// Parameter: int nDevVerLen		;设备版本信息缓存长度
// Parameter: char * sAPIVersion	;接口库版本信息缓存地址
// Parameter: int nAPIVerLen		;接口库版本信息缓存长度
//************************************
VPR_SIGNALWAYSDVFR_API int D_CALLTYPE  VLPR_GetVersion(	
	char* sDevVersion,
	int nDevVerLen,
	char* sAPIVersion,
	int nAPIVerLen);

//************************************
// Method:    VLPR_SyncTime
// FullName:  VLPR_SyncTime
// Access:    public 
// Returns:   int
//			0	操作成功
//?		- 100		设备无响应
//?		- 1000	传入参数错误
//?		- 2000	其它错误
// Qualifier: 与主机同步时间,该函数只负责同步时间，不会显示时间
// Parameter: int nHandle		;设备句柄
// Parameter: char * sSysTime	;输入时间格式：yyyyMMddHHmmss
//************************************
VPR_SIGNALWAYSDVFR_API int D_CALLTYPE VLPR_SyncTime(int nHandle,char* sSysTime);

//************************************
// Method:    VLPR_StartDisplay
// FullName:  VLPR_StartDisplay
// Access:    public 
// Returns:   int
//			0	操作成功
//?		- 100	设备无响应
//?		- 1000	传入参数错误
//？    - 1003  设置显示错误
//?		- 2000	其它错误
// Qualifier: 启动显示视频
// Parameter: int nHandle		;设备句柄
// Parameter: int nWidth	    ;视频宽度
// Parameter: int nHeight		;视频高度
// Parameter: int nFHandle	    ;窗体句柄
//************************************
VPR_SIGNALWAYSDVFR_API int D_CALLTYPE VLPR_StartDisplay(int nHandle, int nWidth, int nHeight, int nFHandle);

//************************************
// Method:    VLPR_StopDisplay
// FullName:  VLPR_StopDisplay
// Access:    public 
// Returns:   int
//			0	操作成功
//?		- 100	设备无响应
//?		- 1000	传入参数错误
//？    - 1005  停止显示错误
//?		- 2000	其它错误
// Qualifier: 停止显示视频
// Parameter: int nHandle		;设备句柄
//************************************
VPR_SIGNALWAYSDVFR_API int D_CALLTYPE VLPR_StopDisplay(int nHandle);