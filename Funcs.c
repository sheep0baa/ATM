#include "Funcs.h"
#include "lang.h"
#include "Common.h"
#include <time.h>

int g_nAccts = 0;

int g_nFlow = 0;

ACCOUNT* g_pAcct = 0;

ACCOUNT* g_pCurrent = 0;

char g_pszAdminPwd[MAX_PASSWORD + 1];

int IsNum(const char* pszStr)
{
	for (size_t i = 0; i < strlen(pszStr); i++)
	{
		if (pszStr[i] < '0' || pszStr[i] > '9')
			return 0;
	}
	return 1;
}

int Split(char* pszRaw, const char* pszDel, char** pszArray, int maxItems)
{
	int iRes = -1;
	char* pToken = NULL;
	int iCnt = 0;

	memset(pszArray, 0, maxItems * sizeof(char*));

	pToken = strtok(pszRaw, pszDel);
	while (pToken != NULL && iCnt < maxItems)
	{
		int iLen = strlen(pToken);
		if (iLen == 0)
			pszArray[iCnt] = NULL;
		else
		{
			pszArray[iCnt] = (char*)malloc(iLen + 1);
			memset((void*)pszArray[iCnt], 0, iLen + 1);
			strcpy(pszArray[iCnt], pToken);
		}

		iCnt++;
		pToken = strtok(NULL, pszDel);
	}
	return iCnt;
}

void GetTimeStamp(TS* ts, char* pszBuf, int bufSize)
{
	time_t lt = time(0);
	struct tm pt;
	localtime_s(&pt, &lt);
	ts->year = pt.tm_year + 1900;
	ts->month = pt.tm_mon + 1;
	ts->day = pt.tm_mday;
	ts->hh = pt.tm_hour;
	ts->mm = pt.tm_min;
	ts->ss = pt.tm_sec;
	sprintf(pszBuf, "%04d%02d%02d%02d%02d%02d", ts->year, ts->month, ts->day, ts->hh, ts->mm, ts->ss);
}

void DispTime(TS ts, char* pszDisp)
{
	sprintf(pszDisp, "%04d-%02d-%02d %02d:%02d:%02d", ts.year, ts.month, ts.day, ts.hh, ts.mm, ts.ss);
}

int LoadAccts(const char* pFileAcct, ACCOUNT** ppAccount, int* pnAcct, 
	const char* pFileAdminPwd, char* pszAdminPwd, int iPwdBufSize)
{
	int iRes = 0;
	(*ppAccount) = NULL;
	(*pnAcct) = 0;

	//载入普通账户信息
	FILE* fp = fopen(pFileAcct, "r");
	if (!fp)
		return -1;

	if (1 == fscanf(fp, "%d", pnAcct))
	{
		char szLine[BUFSIZ + 1];
		(*ppAccount) = (ACCOUNT*)malloc(sizeof(ACCOUNT) * (*pnAcct));
		int it = 0;
		while (!feof(fp))
		{
			char* pszArray[MAX_SECTION];
			fgets(szLine, sizeof(szLine), fp);
			for (int i = 0; i < (int)strlen(szLine); i++)
				if (szLine[i] == '\r' || szLine[i] == '\n')
					szLine[i] = 0x00;

			int iSplits = Split(szLine, DEL_BIG, pszArray, MAX_SECTION);
			if (LINEITEM != iSplits)
				continue;
			memset((unsigned char*)(*ppAccount) + it * sizeof(ACCOUNT), 0, sizeof(ACCOUNT));
			char szState[6];
			char szAmount[MAX_HINT_ITEM + 1];
			char szGen[5];
			memset(szGen, 0, sizeof(szGen));
			memset(szState, 0, sizeof(szState));
			memset(szAmount, 0, sizeof(szAmount));
			strncpy(((ACCOUNT*)(*ppAccount))[it].szCard, pszArray[0], MAX_CARDNO);
			strncpy(((ACCOUNT*)(*ppAccount))[it].szPwd, pszArray[1], MAX_PASSWORD);
			strncpy(szState, pszArray[2], sizeof(szState) - 1);
			if (strcmp(szState, "0") == 0)
				((ACCOUNT*)(*ppAccount))[it].ucState = 0x00;
			else
				((ACCOUNT*)(*ppAccount))[it].ucState = 0x01;
			strncpy(szAmount, pszArray[3], MAX_HINT_ITEM);
			((ACCOUNT*)(*ppAccount))[it].dBalance = atof(szAmount);
			strncpy(((ACCOUNT*)(*ppAccount))[it].szGender, pszArray[4], sizeof(szGen) - 1);
			strncpy(((ACCOUNT*)(*ppAccount))[it].szName, pszArray[5], MAX_NAME);
			for (int i = 0; i < iSplits; i++)
				free(pszArray[i]);
			it++;
		}
	}
	fclose(fp);

	//载入管理员密码
	fp = fopen(pFileAdminPwd, "r");
	if (!fp)
		return -1;

	char szBuf[BUFSIZ + 1];
	memset(szBuf, 0, sizeof(szBuf));
	fgets(szBuf, BUFSIZ, fp);
	memset(pszAdminPwd, 0, iPwdBufSize);
	memcpy(pszAdminPwd, szBuf, iPwdBufSize - 1);
	fclose(fp);
	return 0;
}

int SaveAccts(const char* pFileAcct, ACCOUNT** ppAccount, int* pnAcct,
	const char* pFileAdminPwd, char* pszAdminPwd, int iPwdBufSize)
{
	int iRes = 0;

	FILE* fp = fopen(pFileAcct, "w");
	if (!fp)
		return -1;

	fprintf(fp, "%d\n", g_nAccts);
	for (int i = 0; i < g_nAccts; i++)
	{
		fprintf(fp, "%s|%s|%s|%lf|%s|%s\n",
			g_pAcct[i].szCard,
			g_pAcct[i].szPwd,
			(g_pAcct[i].ucState==0)?"0":"1",
			g_pAcct[i].dBalance,
			g_pAcct[i].szGender,
			g_pAcct[i].szName);
	}
	fclose(fp);

	fp = fopen(pFileAdminPwd, "w");
	if (!fp)
		return -1;
	fprintf(fp, "%s", pszAdminPwd);
	fclose(fp);
	return 0;
}

void DispLog(LOG* plog, FILE* fpout)
{
	fprintf(fpout, GetLangItem(STR_LOG_INFO), plog->szFlow, GetTransType(plog->ttType), plog->szAcctA, plog->szAcctB, plog->amount, plog->szTimestamp);
}

int FilterLog(const char* pFileLog, char* pszCard, FILE* fpOutput)
{
	FILE* fp = fopen(pFileLog, "r");
	if (!fp)
		return -1;
	while (!feof(fp))
	{
		LOG log;
		char* pszArray[MAX_SECTION];
		char szLine[BUFSIZ * 2 + 1];
		memset(szLine, 0, sizeof(szLine));
		memset(&log, 0, sizeof(LOG));

		fgets(szLine, BUFSIZ * 2, fp);
		for (int i = 0; i < (int)strlen(szLine); i++)
			if (szLine[i] == '\r' || szLine[i] == '\n')
				szLine[i] = 0x00;

		int iSplits = Split(szLine, DEL_BIG, pszArray, MAX_SECTION);
		if (LINEITEM != iSplits)
			continue;

		log.ttType = atoi(pszArray[0]);
		strcpy(log.szFlow, pszArray[1]);
		strcpy(log.szTimestamp, pszArray[2]);
		log.amount = atof(pszArray[3]);
		strcpy(log.szAcctA, pszArray[4]);
		strcpy(log.szAcctB, pszArray[5]);

		if (pszCard == NULL ||
			((log.ttType == T_WITHDRAW || log.ttType == T_DEPOSIT || log.ttType == T_TRANSFER_OUT) && strcmp(log.szAcctA, pszCard) == 0) ||
			(log.ttType == T_TRANSFER_IN == T_TRANSFER_IN && strcmp(log.szAcctB, pszCard) == 0))
		{
			DispLog(&log, stdout);
		}
	}
	fclose(fp);
	return 0;
}

int AppendLog(const char* pFileLog, LOG log)
{
	FILE* fp = fopen(pFileLog, "a");
	if (!fp)
		return -1;
	char szLog[BUFSIZ + 1];
	sprintf(szLog, "%d|%s|%s|%.02f|%s|%s\n", (int)log.ttType, 
											log.szFlow, 
											log.szTimestamp,
											log.amount,
											log.szAcctA,
											log.szAcctB);
	fprintf(fp, szLog);
	fclose(fp);
	return 0;
}

int ShowMainMnu()
{
	int iSel = -1;
	CLEAR
	printf("%s\n", GetLangItem(STR_WELCOME));
	printf("%s\n", GetLangItem(STR_LOGIN));
	printf("%s\n", GetLangItem(STR_ADMIN));
	printf("%s\n", GetLangItem(STR_EXIT));
	if (1 != scanf_s("%d", &iSel))
		return -1;
	return iSel;
}

int ShowAdminMnu()
{
	int iSel = -1;
	CLEAR
	printf("%s\n", GetLangItem(STR_WELCOME));
	printf("%s\n", GetLangItem(STR_BROWSE_ACCT));
	printf("%s\n", GetLangItem(STR_BROWSE_HIS2));
	printf("%s\n", GetLangItem(STR_FREEZE));
	printf("%s\n", GetLangItem(STR_DEFREEZE));
	printf("%s\n", GetLangItem(STR_REG));
	printf("%s\n", GetLangItem(STR_REMOVE));
	printf("%s\n", GetLangItem(STR_BACK_MAIN2));
	if (1 != scanf_s("%d", &iSel))
		return -1;
	return iSel;
}

int ShowUserMnu()
{
	int iSel = -1;
	CLEAR
	const char* dispGender = NULL;
	if (!g_pCurrent)
		return -1;
	else
	{
		if (strcmp(g_pCurrent->szGender, "F") == 0)
			dispGender = GetLangItem(STR_MAM);
		else
			dispGender = GetLangItem(STR_SIR);
	}

	printf("%s\n", GetLangItem(STR_WELCOME));
	printf(GetLangItem(STR_GREET), g_pCurrent->szName, dispGender);
	printf("%s\n", GetLangItem(STR_WITHDRAW));
	printf("%s\n", GetLangItem(STR_DEPOSIT));
	printf("%s\n", GetLangItem(STR_Q_BAL));
	printf("%s\n", GetLangItem(STR_TRANSFER));
	printf("%s\n", GetLangItem(STR_BROWSE_HIS1));
	printf("%s\n", GetLangItem(STR_MOD_PWD));
	printf("%s\n", GetLangItem(STR_BACK_MAIN1));
	if (1 != scanf_s("%d", &iSel))
		return -1;
	return iSel;
}

void RunAdmin()
{
	int iSel = -1;
	do
	{
		iSel = ShowAdminMnu();
		switch (iSel)
		{
		case 1:
			_ShowAllAcct();
			break;
		case 2:
			_ShowHis();
			break;
		case 3:
			_Freeze(1);
			break;
		case 4:
			_Freeze(0);
			break;
		case 5:
			_AddNewAcct();
			break;
		case 6:
			_RemoveAcct();
			break;
		case 7:
			return;
		}

	} while (1);
}

void RunUser()
{
	int iSel = -1;
	do
	{
		iSel = ShowUserMnu();
		switch (iSel)
		{
		case 1:
			_WithDraw(g_pCurrent);
			break;
		case 2:
			_Deposit(g_pCurrent);
			break;
		case 3:
			_ShowBalance(g_pCurrent);
			PAUSE;
			break;
		case 4:
			_Transfer(g_pCurrent);
			break;
		case 5:
			_ShowMyHis(g_pCurrent);
			break;
		case 6:
			_ModifyMyPwd(g_pCurrent);
			break;
		case 7:
			return;
		}

	} while (1);
}

void Run()
{
	int iSel = -1;
	do
	{
		iSel = ShowMainMnu();
		switch (iSel)
		{
		case 1:
			if (UserLogin() == 0)
				RunUser();
			break;
		case 2:
			if (AdminLogin() == 0)
				RunAdmin();
			break;
		case 3:
			return;
		default:
			break;
		}
	} while (1);
}

int UserLogin()
{
	char szBuf[BUFSIZ + 1];
	char szCard[MAX_CARDNO + 1];
	char szPwd[MAX_PASSWORD + 1];
	memset(szBuf, 0, sizeof(szBuf));
	memset(szCard, 0, sizeof(szCard));
	memset(szPwd, 0, sizeof(szPwd));

	//账户
	printf("%s\n", GetLangItem(STR_INP_ACCT));
	if (scanf("%s", szBuf) != 1)
	{
		PAUSE;
		return -1;
	}
	else
	{
		strncpy(szCard, szBuf, MAX_CARDNO);
	}

	//密码
	printf("%s\n", GetLangItem(STR_INP_PWD));
	if (scanf("%s", szBuf) != 1)
	{
		PAUSE;
		return -1;
	}
	else
	{
		strncpy(szPwd, szBuf, MAX_PASSWORD);
	}

	for (int i = 0; i < g_nAccts; i++)
	{
		if (strcmp(szCard, g_pAcct[i].szCard) == 0 &&
			strcmp(szPwd, g_pAcct[i].szPwd) == 0)
		{
			g_pCurrent = &g_pAcct[i];
			return 0;
		}
	}

	printf("%s\n", GetLangItem(STR_WRONG_PWD));
	PAUSE;
	return -1;
}

int AdminLogin()
{
	char szBuf[BUFSIZ + 1];
	memset(szBuf, 0, sizeof(szBuf));
	printf("%s\n", GetLangItem(STR_INP_ADMINPWD));
	if (scanf("%s", szBuf) != 1)
	{
		PAUSE;
		return -1;
	}

	if (strncmp(szBuf, g_pszAdminPwd, MAX_PASSWORD) == 0)
		return 0;
	else
	{
		printf("%s\n", GetLangItem(STR_WRONG_PWD));
		PAUSE;
		return -1;
	}
}

double _SelectAmount()
{
	int iSel = -1;
	printf("%s\n", GetLangItem(STR_SEL_AMOUNT));
	printf("1. 100￥\n");
	printf("2. 200￥\n");
	printf("3. 500￥\n");
	printf("4. 1000￥\n");
	printf("5. %s\n", GetLangItem(STR_OTHER_AMOUNT));
	if (scanf("%d", &iSel) == 1)
	{
		switch (iSel)
		{
		case 1:
			return 100;
		case 2:
			return 200;
		case 3:
			return 500;
		case 4:
			return 1000;
		default:
			return -1;
		}
	}
	return -1;
}

void _ShowAllAcct()
{
	printf("%s\n", GetLangItem(STR_ACCT_LIST));
	printf("%s\n", GetLangItem(STR_PRINT_HEADER));
	for (int i = 0; i < g_nAccts; i++)
	{
		//账号                 姓名         性别 状态     余额
		//Account              Name         Sex  State    Balance
		printf("%20s %12s %4s %8s %12.2f\n", g_pAcct[i].szCard, g_pAcct[i].szName, g_pAcct[i].szGender,
			g_pAcct[i].ucState == 0? (GetLangItem(STR_NORMAL)):(GetLangItem(STR_FROZEN)), g_pAcct[i].dBalance);
	}
	PAUSE;
}

void _ShowHis()
{
	FilterLog(DAT_LOG, NULL, stdout);
	PAUSE;
}

void _Freeze(unsigned char ucFreeze)
{
	char szBuf[BUFSIZ + 1];
	char szCard[MAX_CARDNO + 1];
	memset(szBuf, 0, sizeof(szBuf));
	memset(szCard, 0, sizeof(szCard));

	//账户
	printf("%s\n", GetLangItem(STR_INP_ACCT));
	if (scanf("%s", szBuf) != 1)
	{
		PAUSE;
		return;
	}
	else
	{
		strncpy(szCard, szBuf, MAX_CARDNO);
	}

	//查找并冻结、解冻
	for (int i = 0; i < g_nAccts; i++)
	{
		if (strcmp(szCard, g_pAcct[i].szCard) == 0)
		{
			g_pAcct[i].ucState = ucFreeze;
			SaveAccts(DAT_ACCT, &g_pAcct, &g_nAccts, DAT_ADMIN_PWD, g_pszAdminPwd, sizeof(g_pszAdminPwd));
			printf("%s\n", GetLangItem(STR_TRANS_SUCC));
			PAUSE;
			return;
		}
	}

	printf(GetLangItem(STR_TRANS_FAIL), GetError(ERR_ACCT_NOTFOUND));
	printf("\n");
	PAUSE;
	return;
}

void _AddNewAcct()
{
	char szBuf[BUFSIZ + 1];
	char szCard[MAX_CARDNO + 1];
	char szPwd[MAX_PASSWORD + 1];
	char szGender[4 + 1];
	char szName[MAX_NAME + 1];
	memset(szBuf, 0, sizeof(szBuf));
	memset(szCard, 0, sizeof(szCard));
	memset(szPwd, 0, sizeof(szPwd));
	memset(szGender, 0, sizeof(szGender));
	memset(szName, 0, sizeof(szName));

	//账户
	printf("%s\n", GetLangItem(STR_INP_ACCT));
	if (scanf("%s", szBuf) != 1)
	{
		PAUSE;
		return;
	}
	else
	{
		if (strlen(szBuf) < MIN_CARD ||
			strlen(szBuf) >= MAX_CARDNO ||
			IsNum(szCard) == 0)
		{
			printf(GetLangItem(STR_TRANS_FAIL), GetError(ERR_INV_CARD));
			printf("\n");
			PAUSE;
			return;
		}
		else
			strncpy(szCard, szBuf, MAX_CARDNO);
	}

	//账户是否已存在
	int isFound = 0;
	for (int i = 0; i < g_nAccts; i++)
	{
		if (strcmp(g_pAcct[i].szCard, szCard) == 0)
		{
			printf(GetLangItem(STR_TRANS_FAIL), GetError(ERR_CARD_EXIST));
			printf("\n");
			PAUSE;
			return;
		}
	}

	//密码
	printf("%s\n", GetLangItem(STR_INP_PWD));
	if (scanf("%s", szBuf) != 1)
	{
		PAUSE;
		return ;
	}
	else
	{
		if (strlen(szBuf) != MAX_PASSWORD || IsNum(szBuf) == 0)
		{
			printf(GetLangItem(STR_TRANS_FAIL), GetError(ERR_INV_PWD));
			printf("\n");
			PAUSE;
			return;
		}
		strncpy(szPwd, szBuf, MAX_PASSWORD);
	}

	//姓名
	printf("%s\n", GetLangItem(STR_INP_NAME));
	if (scanf("%s", szBuf) != 1)
	{
		PAUSE;
		return;
	}
	else
	{
		strncpy(szName, szBuf, MAX_NAME);
	}

	//性别
	printf("%s\n", GetLangItem(STR_INP_GENDER));
	if (scanf("%s", szBuf) != 1)
	{
		PAUSE;
		return;
	}
	else
	{
		if (szBuf[0] == 'F' || szBuf[0] == 'f')
			strcpy(szGender, "F");
		else
			strcpy(szGender, "M");
	}

	//添加用户
	ACCOUNT acct;
	memset(&acct, 0, sizeof(ACCOUNT));
	strncpy(acct.szCard, szCard, MAX_CARDNO);
	strncpy(acct.szPwd, szPwd, MAX_PASSWORD);
	acct.ucState = 0;
	acct.dBalance = 0;
	strncpy(acct.szName, szName, MAX_NAME);
	strncpy(acct.szGender, szGender, 4);

	ACCOUNT* pNewAccts = (ACCOUNT*)malloc(sizeof(ACCOUNT) * (g_nAccts + 1));
	memcpy(pNewAccts, g_pAcct, sizeof(ACCOUNT) * g_nAccts);
	free(g_pAcct);
	g_nAccts++;
	memcpy(&(pNewAccts[g_nAccts - 1]), &acct, sizeof(ACCOUNT));
	g_pAcct = pNewAccts;
	SaveAccts(DAT_ACCT, &g_pAcct, &g_nAccts, DAT_ADMIN_PWD, g_pszAdminPwd, sizeof(g_pszAdminPwd));
	printf("%s\n", GetLangItem(STR_TRANS_SUCC));
	PAUSE;
	return;
}

void _RemoveAcct()
{
	char szBuf[BUFSIZ + 1];
	char szCard[MAX_CARDNO + 1];
	char szPwd[MAX_PASSWORD + 1];
	char szGender[4 + 1];
	char szName[MAX_NAME + 1];
	memset(szBuf, 0, sizeof(szBuf));
	memset(szCard, 0, sizeof(szCard));
	memset(szPwd, 0, sizeof(szPwd));
	memset(szGender, 0, sizeof(szGender));
	memset(szName, 0, sizeof(szName));

	//账户
	printf("%s\n", GetLangItem(STR_INP_ACCT));
	if (scanf("%s", szBuf) != 1)
	{
		PAUSE;
		return;
	}
	else
	{
		if (strlen(szBuf) < MIN_CARD ||
			strlen(szBuf) >= MAX_CARDNO ||
			IsNum(szCard) == 0)
		{
			printf(GetLangItem(STR_TRANS_FAIL), GetError(ERR_INV_CARD));
			printf("\n");
			PAUSE;
			return;
		}
		else
			strncpy(szCard, szBuf, MAX_CARDNO);
	}

	//账户是否已存在
	int isFound = 0;
	for (int i = 0; i < g_nAccts; i++)
	{
		if (strcmp(g_pAcct[i].szCard, szCard) == 0)
		{
			for (int j = i + 1; j < g_nAccts; j++)
			{
				memcpy(&g_pAcct[j - 1], &g_pAcct[j], sizeof(ACCOUNT));
			}
			g_nAccts -= 1;
			SaveAccts(DAT_ACCT, &g_pAcct, &g_nAccts, DAT_ADMIN_PWD, g_pszAdminPwd, sizeof(g_pszAdminPwd));
			printf("%s\n", GetLangItem(STR_TRANS_SUCC));
			PAUSE;
			return;
		}
	}

	printf(GetLangItem(STR_TRANS_FAIL), GetError(ERR_ACCT_NOTFOUND));
	printf("\n");
	PAUSE;
	return;
}

void _ShowBalance(ACCOUNT* pAcct)
{
	if (!pAcct)
		return;
	else
		printf(GetLangItem(STR_BAL), pAcct->dBalance);
}

void _WithDraw(ACCOUNT* pAcct)
{
	if (!pAcct)
		return;
	else
	{
		//是否冻结
		if (pAcct->ucState)
		{
			printf(GetLangItem(STR_TRANS_FAIL), GetError(ERR_ACCT_FRZ));
			printf("\n");
			PAUSE;
			return;
		}

		//选择金额
		double dAmt = 0;
		dAmt = _SelectAmount();
		if (dAmt < 0)
		{
			printf("%s", GetLangItem(STR_INP_AMOUNT));
			if (scanf("%lf", &dAmt) == 1)
			{
				dAmt = dAmt;
			}
			else
				dAmt = 0;
		}
		
		if (dAmt > 0)
		{
			if (dAmt > 0 && dAmt <= pAcct->dBalance)
			{
				pAcct->dBalance -= dAmt;
				//UpdateData
				SaveAccts(DAT_ACCT, &g_pAcct, &g_nAccts, DAT_ADMIN_PWD, g_pszAdminPwd, sizeof(g_pszAdminPwd));

				//AppendLog
				LOG log;
				TS ts;
				memset(&log, 0, sizeof(LOG));
				log.ttType = T_WITHDRAW;
				strncpy(log.szAcctA, pAcct->szCard, MAX_CARDNO);
				strncpy(log.szAcctB, NULLSTR, MAX_CARDNO);
				GetTimeStamp(&ts, log.szTimestamp, sizeof(log.szTimestamp));
				sprintf(log.szFlow, "%s%04d", log.szTimestamp, g_nFlow++);
				log.amount = dAmt;
				AppendLog(DAT_LOG, log);

				//ShowResult OK
				printf("%s\n", GetLangItem(STR_TRANS_SUCC));
				_ShowBalance(pAcct);
				char szTs[32];
				memset(szTs, 0, sizeof(szTs));
				DispTime(ts, szTs);
				printf("%s%s\n", GetLangItem(STR_TRANS_TIME), szTs);
				PAUSE;
				return;
			}
			else
			{
				//ShowResult FAIL
				printf(GetLangItem(STR_TRANS_FAIL), GetError(ERR_AMT));
				printf("\n");
				PAUSE;
				return;
			}
		}
	}
	PAUSE;
}

void _Deposit(ACCOUNT* pAcct)
{
	if (!pAcct)
		return;
	else
	{
		//是否冻结
		if (pAcct->ucState)
		{
			printf(GetLangItem(STR_TRANS_FAIL), GetError(ERR_ACCT_FRZ));
			printf("\n");
			PAUSE;
			return;
		}

		double dAmt = 0;
		printf("%s", GetLangItem(STR_INP_AMOUNT));
		if (scanf("%lf", &dAmt) == 1)
		{
			if (dAmt > 0)
			{
				pAcct->dBalance += dAmt;

				//UpdateData
				SaveAccts(DAT_ACCT, &g_pAcct, &g_nAccts, DAT_ADMIN_PWD, g_pszAdminPwd, sizeof(g_pszAdminPwd));

				//AppendLog
				LOG log;
				TS ts;
				memset(&log, 0, sizeof(LOG));
				log.ttType = T_DEPOSIT;
				strncpy(log.szAcctA, pAcct->szCard, MAX_CARDNO);
				strncpy(log.szAcctB, NULLSTR, MAX_CARDNO);
				GetTimeStamp(&ts, log.szTimestamp, sizeof(log.szTimestamp));
				sprintf(log.szFlow, "%s%04d", log.szTimestamp, g_nFlow++);
				log.amount = dAmt;
				AppendLog(DAT_LOG, log);

				//ShowResult OK
				printf("%s\n", GetLangItem(STR_TRANS_SUCC));
				_ShowBalance(pAcct);
				char szTs[32];
				memset(szTs, 0, sizeof(szTs));
				DispTime(ts, szTs);
				printf("%s%s\n", GetLangItem(STR_TRANS_TIME), szTs);
				PAUSE;
				return;
			}
			else
			{
				//ShowResult FAIL
				printf(GetLangItem(STR_TRANS_FAIL), GetError(ERR_AMT));
				printf("\n");
				PAUSE;
				return;
			}
		}
	}
	PAUSE;
}

void _Transfer(ACCOUNT* pAcct)
{
	if (!pAcct)
		return;
	else
	{
		//是否冻结
		if (pAcct->ucState)
		{
			printf(GetLangItem(STR_TRANS_FAIL), GetError(ERR_ACCT_FRZ));
			printf("\n");
			PAUSE;
			return;
		}

		char szCard[MAX_CARDNO + 1];
		ACCOUNT* pAcctB = NULL;
		
		//输入转入卡号
		printf("%s", GetLangItem(STR_INP_RECVACCT));
		if (scanf("%s", szCard) == 1)
		{
			//检查收款方是否存在
			for (int i = 0; i < g_nAccts; i++)
			{
				if (strcmp(szCard, g_pAcct[i].szCard) == 0)
				{
					pAcctB = &g_pAcct[i];
					break;
				}
			}

			//卡B不存在
			if (!pAcctB)
			{
				printf(GetLangItem(STR_TRANS_FAIL), GetError(ERR_CARDB));
				printf("\n");
				PAUSE;
				return;
			}
		}

		//输入金额
		double dAmt = -1;
		printf("%s", GetLangItem(STR_INP_AMOUNT));
		if (scanf("%lf", &dAmt) == 1)
		{
			if (dAmt > 0 && dAmt <= pAcct->dBalance)
			{
				pAcct->dBalance -= dAmt;
				pAcctB->dBalance += dAmt;

				//UpdateData
				SaveAccts(DAT_ACCT, &g_pAcct, &g_nAccts, DAT_ADMIN_PWD, g_pszAdminPwd, sizeof(g_pszAdminPwd));

				//AppendLog 转出日志
				LOG logOut;
				TS ts;
				memset(&logOut, 0, sizeof(LOG));
				logOut.ttType = T_TRANSFER_OUT;
				strncpy(logOut.szAcctA, pAcct->szCard, MAX_CARDNO);
				strncpy(logOut.szAcctB, pAcctB->szCard, MAX_CARDNO);
				GetTimeStamp(&ts, logOut.szTimestamp, sizeof(logOut.szTimestamp));
				sprintf(logOut.szFlow, "%s%04d", logOut.szTimestamp, g_nFlow++);
				logOut.amount = dAmt;
				AppendLog(DAT_LOG, logOut);

				//AppendLog 转入日志
				LOG logIn;
				memset(&logIn, 0, sizeof(LOG));
				logIn.ttType = T_TRANSFER_IN;
				strncpy(logIn.szAcctA, pAcct->szCard, MAX_CARDNO);
				strncpy(logIn.szAcctB, pAcctB->szCard, MAX_CARDNO);
				memcpy(logIn.szTimestamp, logOut.szTimestamp, sizeof(logIn.szTimestamp));
				sprintf(logIn.szFlow, "%s%04d", logIn.szTimestamp, g_nFlow++);
				logIn.amount = dAmt;
				AppendLog(DAT_LOG, logIn);

				//ShowResult OK
				printf("%s\n", GetLangItem(STR_TRANS_SUCC));
				_ShowBalance(pAcct);
				char szTs[32];
				memset(szTs, 0, sizeof(szTs));
				DispTime(ts, szTs);
				printf("%s%s\n", GetLangItem(STR_TRANS_TIME), szTs);
				PAUSE;
				return;
			}
			else
			{
				//ShowResult FAIL
				printf(GetLangItem(STR_TRANS_FAIL), GetError(ERR_AMT));
				printf("\n");
				PAUSE;
				return;
			}
		}

		PAUSE;
	}
}

void _ShowMyHis(ACCOUNT* pAcct)
{

	if (!pAcct)
		return;
	else
	{
		FilterLog(DAT_LOG, pAcct->szCard, stdout);
		PAUSE;
	}
}

void _ModifyMyPwd(ACCOUNT* pAcct)
{
	if (!pAcct)
		return;
	else
	{
		char szBuf[BUFSIZ + 1];
		memset(szBuf, 0, sizeof(szBuf));
		printf("%s", GetLangItem(STR_INP_NEWPWD));
		if (scanf("%s", szBuf) == 1)
		{
			if (strlen(szBuf) != MAX_PASSWORD || IsNum(szBuf) == 0)
			{
				printf(GetLangItem(STR_TRANS_FAIL), GetError(ERR_INV_PWD));
				printf("\n");
				PAUSE;
				return;
			}
			else
			{
				strncpy(pAcct->szPwd, szBuf, MAX_PASSWORD);
				SaveAccts(DAT_ACCT, &g_pAcct, &g_nAccts, DAT_ADMIN_PWD, g_pszAdminPwd, sizeof(g_pszAdminPwd));
				printf("%s\n", GetLangItem(STR_MODPASS_SUCC));
				PAUSE;
				return;
			}
		}
	}
}
