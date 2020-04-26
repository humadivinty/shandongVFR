#ifndef MINIDUMPER_H
#define MINIDUMPER_H

#include <windows.h>
#include "StackWalker.h"

class CMiniDumper : public StackWalker
{
public:

    CMiniDumper(bool bPromptUserForMiniDump);
    ~CMiniDumper(void);

private:

    static LONG WINAPI unhandledExceptionHandler(struct _EXCEPTION_POINTERS *pExceptionInfo);
	// 此函数一旦成功调用，之后对 SetUnhandledExceptionFilter 的调用将无效  
	void DisableSetUnhandledExceptionFilter();

    void setMiniDumpFileName(void);
    bool getImpersonationToken(HANDLE* phToken);
    BOOL enablePrivilege(LPCTSTR pszPriv, HANDLE hToken, TOKEN_PRIVILEGES* ptpOld);
    BOOL restorePrivilege(HANDLE hToken, TOKEN_PRIVILEGES* ptpOld);
    LONG writeMiniDump(_EXCEPTION_POINTERS *pExceptionInfo );

    _EXCEPTION_POINTERS *m_pExceptionInfo;
    TCHAR m_szMiniDumpPath[MAX_PATH];
    TCHAR m_szAppPath[MAX_PATH];
    TCHAR m_szAppBaseName[MAX_PATH];
    bool m_bPromptUserForMiniDump;

    static CMiniDumper* s_pMiniDumper;
    static LPCRITICAL_SECTION s_pCriticalSection;

protected:
	//virtual void OnOutput(LPCSTR szText) { printf("hahahahahahahaha ::::::%s", szText); }
	virtual void OnOutput(LPCSTR szText);
};

#endif // MINIDUMPER_H
