#pragma once
#include "Common.h"
#define MAX_HINT_ITEM 10000

#define STR_WELCOME		0
#define STR_LOGIN		1
#define STR_ADMIN		2
#define STR_SW_LANG		3
#define STR_EXIT		4
#define STR_WITHDRAW	5
#define STR_DEPOSIT		6
#define STR_Q_BAL		7
#define STR_TRANSFER	8
#define STR_BROWSE_HIS1	9
#define STR_MOD_PWD		10
#define STR_BACK_MAIN1	11
#define STR_BROWSE_ACCT	12
#define STR_BROWSE_HIS2	13
#define STR_FREEZE		14
#define STR_DEFREEZE	15
#define STR_REG			16
#define STR_REMOVE		17
#define STR_BACK_MAIN2	18
#define STR_ACCT_LIST	19
#define STR_FROZEN		20
#define STR_NORMAL		21
#define STR_PAUSE		22
#define STR_INP_ADMINPWD	23
#define STR_INP_ACCT		24
#define STR_INP_PWD			25
#define STR_WRONG_PWD		26
#define STR_PRINT_HEADER	27
#define STR_SIR				28
#define STR_MAM				29
#define STR_GREET			30
#define STR_BAL				31
#define STR_INP_AMOUNT		32
#define STR_TRANS_SUCC		33
#define STR_TRANS_FAIL		34
#define STR_INP_RECVACCT	35
#define STR_LOG_INFO		36
#define STR_INP_NEWPWD		37
#define STR_MODPASS_SUCC	38
#define STR_INP_NAME		39
#define STR_INP_GENDER		40
#define STR_SEL_AMOUNT		41
#define STR_OTHER_AMOUNT	42
#define STR_TRANS_TIME		43

#define ERR_SUCC 0
#define ERR_AMT  1
#define ERR_CARDB 2
#define ERR_INV_PWD 3
#define ERR_ACCT_FRZ 4
#define ERR_ACCT_NOTFOUND 5
#define ERR_INV_CARD 6
#define ERR_CARD_EXIST 7

#define EN 0
#define CH 1

#define LANGSEL "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n|                                                                         | \n*                       欢迎使用ATM柜员机系统                             * \n|                                                                         | \n*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n                    选择您所需要的语言，并按回车确认!                    \n          Select your desired language and press Enter to confirm!          \n|                          【1】中文                              |\n*                          【2】English                           * \n"

extern int g_Lang;
extern char g_szResEN[][MAX_HINT_ITEM];
extern char g_szResCH[][MAX_HINT_ITEM];
const char* GetTransType(TransType tt);
const char* GetLangItem(int id);
const char* GetError(int iErrorCode);
