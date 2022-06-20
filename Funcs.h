#pragma once
#include "Common.h"
#include "lang.h"

extern int g_nAccts;
extern ACCOUNT* g_pAcct;
extern ACCOUNT* g_pCurrent;
extern char g_pszAdminPwd[MAX_PASSWORD + 1];

int ShowMainMnu();
void Run(); 
void GetTimeStamp(TS* ts, char* pszBuf, int bufSize);
int LoadAccts(const char* pFileAcct, ACCOUNT** ppAccount, int* pnAcct,
	const char* pFileAdminPwd, char* pszAdminPwd, int iPwdBufSize);
int SaveAccts(const char* pFileAcct, ACCOUNT** ppAccount, int* pnAcct,
	const char* pFileAdminPwd, char* pszAdminPwd, int iPwdBufSize);

int UserLogin();
int AdminLogin();

double _SelectAmount();
void _ShowAllAcct();
void _ShowHis();
void _AddNewAcct();
void _RemoveAcct();
void _Freeze(unsigned char ucFreeze);
void _ShowBalance(ACCOUNT* pAcct); 
void _WithDraw(ACCOUNT* pAcct);
void _Deposit(ACCOUNT* pAcct);
void _Transfer(ACCOUNT* pAcct);
void _ShowMyHis(ACCOUNT* pAcct);
void _ModifyMyPwd(ACCOUNT* pAcct);
