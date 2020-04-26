// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� VPR_SIGNALWAYSDVFR_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// VPR_SIGNALWAYSDVFR_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
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

#ifdef __OS_WIN  //Windows��������
#define D_CALLTYPE __stdcall
#define D_DECL_EXPORT __declspec(dllexport)
#define D_DECL_IMPORT __declspec(dllimport)
#else //��Windows��������
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
//			0	�����ɹ�
//?		- 100		�豸����Ӧ
//?		- 2000	��������
// Qualifier: �����㹻���ڴ�ռ䣬��֤��������
//************************************
VPR_SIGNALWAYSDVFR_API int D_CALLTYPE VLPR_Init();

//************************************
// Method:    VLPR_Deinit
// FullName:  VLPR_Deinit
// Access:    public 
// Returns:   int
//			 0	�����ɹ�
//?		- 100		�豸����Ӧ
//?		- 2000		��������
// Qualifier:
//************************************
VPR_SIGNALWAYSDVFR_API int D_CALLTYPE  VLPR_Deinit();

//************************************
// Method:    VLPR_Login
// FullName:  VLPR_Login
// Access:    public 
// Returns:   int
//			 >0	���豸�ɹ�������ֵΪ�豸�����
//?		- 100		�豸����Ӧ
//?		- 1000	�����������
//?		- 1001	�豸��ռ��
//?		- 1002	�豸��ʧ��
//?		- 2000	��������
// Qualifier: �������豸��������
// Parameter: int nType			;���ӷ�ʽ��0=���ڣ�1=��������
// Parameter: char * sParas		-���ڣ�����ںš�
//								����COM1��
//								- ���磺���ַ���˿ڣ��û��������롱
//								����192.168.0.11, 8000, admin, password��
//************************************
VPR_SIGNALWAYSDVFR_API int D_CALLTYPE  VLPR_Login(int nType,char* sParas);

//************************************
// Method:    VLPR_Logout
// FullName:  VLPR_Logout
// Access:    public 
// Returns:   int
//			0	�ر��豸�ɹ�
//?		- 100		�豸����Ӧ
//?		- 1000	�����������
//?		- 1001	�豸��ռ��
// Qualifier: �������豸�Ͽ�����
// Parameter: int nHandle  ;�豸���
//************************************
VPR_SIGNALWAYSDVFR_API int D_CALLTYPE  VLPR_Logout(int nHandle);

//************************************
// Method:    VLPR_SetResultCallBack
// FullName:  VLPR_SetResultCallBack
// Access:    public 
// Returns:   int
//			0	�����ɹ�
//?		- 100		�豸����Ӧ
//?		- 1000	�����������
//?		- 2000	��������
// Qualifier: ����ʶ���������ص�����
// Parameter: int nHandle				;�豸���
// Parameter: CBFun_GetFrontResult pFunc	;��ͷʶ�����ص����� ,NULLʱ��ȡ���ص�
// Parameter: CBFun_GetBackResult pFunc	;��βʶ�����ص�����,NULLʱ��ȡ���ص�
// Parameter: void * pUser				;�û��Զ�������
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
//			0	�����ɹ�
//?		- 100	�豸����Ӧ
//?		- 1000	�����������
//?		- 2000	��������
// Qualifier: ���ûص�����ʱ�㱨�豸����״̬���豸�����쳣ʱ�������ص�
// Parameter: int nHandle				;�豸���
// Parameter: int nTimeInvl				;״̬�㱨ʱ����
// Parameter: CBFun_GetDevStatus pFunc	;�쳣��Ϣ�ص�����,func = NULLʱ��ȡ���ص�
// Parameter: void * pUser				;�û��Զ�������
//************************************
VPR_SIGNALWAYSDVFR_API int D_CALLTYPE  VLPR_SetStatusCallBack(int nHandle,int nTimeInvl,CBFun_GetDevStatus pFunc,	void* pUser);

//************************************
// Method:    VLPR_GetStatus
// FullName:  VLPR_GetStatus
// Access:    public 
// Returns:   int
//			0	�����ɹ�
//?		- 100		�豸����Ӧ
//?		- 1000	�����������
//?		- 2000	��������
// Qualifier: ��ȡ�豸����״̬
// Parameter: int nHandle			;�豸���
// Parameter: int * pStatusCode		;�豸״̬�����룬0��ʾ����
//************************************
VPR_SIGNALWAYSDVFR_API int D_CALLTYPE  VLPR_GetStatus(int nHandle,int* pStatusCode);

//************************************
// Method:    VLPR_GetStatusMsg
// FullName:  VLPR_GetStatusMsg
// Access:    public 
// Returns:   int
//			0	�����ɹ�
//?		- 100		�豸����Ӧ
//?		- 1000	�����������
//?		- 2000	��������
// Qualifier: ��ѯ��������ϸ����
// Parameter: int nStatusCode	;������
// Parameter: char * sStatusMsg	;��������Ϣ�����ַ
// Parameter: int nStatusMsgLen	;��������Ϣ���泤��
//************************************
VPR_SIGNALWAYSDVFR_API int D_CALLTYPE  VLPR_GetStatusMsg(int nStatusCode,char* sStatusMsg,int nStatusMsgLen);

//************************************
// Method:    VLPR_GetDevInfo
// FullName:  VLPR_GetDevInfo
// Access:    public 
// Returns:   int
//			0	�����ɹ�
//?		- 100		�豸����Ӧ
//?		- 1000	�����������
//?		- 2000	��������
// Qualifier: ��ȡ�豸������Ϣ
// Parameter: char * sCompany	;������Ϣ�����ַ
// Parameter: int nCompanyLen	;������Ϣ���泤��
// Parameter: char * sDevMode	;�豸�ͺ���Ϣ�����ַ
// Parameter: int nModeLen		;�豸�ͺ���Ϣ���泤��
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
//			0	�����ɹ�
//?		- 100		�豸����Ӧ
//?		- 1000	�����������
//?		- 2000	��������
// Qualifier: ��ȡ�豸���ӿڰ汾��Ϣ
// Parameter: char * sDevVersion	;�豸�汾��Ϣ�����ַ
// Parameter: int nDevVerLen		;�豸�汾��Ϣ���泤��
// Parameter: char * sAPIVersion	;�ӿڿ�汾��Ϣ�����ַ
// Parameter: int nAPIVerLen		;�ӿڿ�汾��Ϣ���泤��
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
//			0	�����ɹ�
//?		- 100		�豸����Ӧ
//?		- 1000	�����������
//?		- 2000	��������
// Qualifier: ������ͬ��ʱ��,�ú���ֻ����ͬ��ʱ�䣬������ʾʱ��
// Parameter: int nHandle		;�豸���
// Parameter: char * sSysTime	;����ʱ���ʽ��yyyyMMddHHmmss
//************************************
VPR_SIGNALWAYSDVFR_API int D_CALLTYPE VLPR_SyncTime(int nHandle,char* sSysTime);

//************************************
// Method:    VLPR_StartDisplay
// FullName:  VLPR_StartDisplay
// Access:    public 
// Returns:   int
//			0	�����ɹ�
//?		- 100	�豸����Ӧ
//?		- 1000	�����������
//��    - 1003  ������ʾ����
//?		- 2000	��������
// Qualifier: ������ʾ��Ƶ
// Parameter: int nHandle		;�豸���
// Parameter: int nWidth	    ;��Ƶ���
// Parameter: int nHeight		;��Ƶ�߶�
// Parameter: int nFHandle	    ;������
//************************************
VPR_SIGNALWAYSDVFR_API int D_CALLTYPE VLPR_StartDisplay(int nHandle, int nWidth, int nHeight, int nFHandle);

//************************************
// Method:    VLPR_StopDisplay
// FullName:  VLPR_StopDisplay
// Access:    public 
// Returns:   int
//			0	�����ɹ�
//?		- 100	�豸����Ӧ
//?		- 1000	�����������
//��    - 1005  ֹͣ��ʾ����
//?		- 2000	��������
// Qualifier: ֹͣ��ʾ��Ƶ
// Parameter: int nHandle		;�豸���
//************************************
VPR_SIGNALWAYSDVFR_API int D_CALLTYPE VLPR_StopDisplay(int nHandle);