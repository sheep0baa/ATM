#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#define DAT_ACCT "acct.txt"
#define DAT_ADMIN_PWD "admin.txt"
#define DAT_LOG "transhis.log"

#define CLEAR system("cls");
#define PAUSE {printf("%s\n", GetLangItem(STR_PAUSE)); system("pause > nul");}

#ifndef NULL
#define NULL 0
#endif
#define MIN_CARD 13
#define MAX_CARDNO   20
#define MAX_PASSWORD 6
#define MAX_LIMIT 500000
#define MAX_NAME 32
#define MAX_SECTION 10
#define TSLEN 14

#define DEL_BIG "|"
#define NULLSTR "NULL"
#define LINEITEM 6

typedef enum _transType
{
	T_WITHDRAW = 0,
	T_DEPOSIT = 1,
	T_TRANSFER_OUT = 2,
	T_TRANSFER_IN = 3
}TransType;

typedef struct _acct
{
	char szCard[MAX_CARDNO + 1];
	char szPwd[MAX_PASSWORD + 1];
	unsigned char ucState;
	double dBalance;
	char szGender[5];
	char szName[MAX_NAME + 1];
}ACCOUNT;

typedef struct _log
{
	TransType ttType;
	char szFlow[20];
	char szTimestamp[16];
	double amount;
	char szAcctA[MAX_CARDNO + 1];
	char szAcctB[MAX_CARDNO + 1];
}LOG;

typedef struct timestamp
{
	int year;
	int month;
	int day;
	int hh;
	int mm;
	int ss;
}TS;
