#include "lang.h"
#include "Funcs.h"

int main()
{
	g_Lang = CH;
	int iLangSel = 0;
REINPUT:
	CLEAR;
	printf("%s", LANGSEL);
	scanf("%d", &iLangSel);
	if (iLangSel == 1)
	{
		g_Lang = CH;
	}
	else if (iLangSel == 2)
	{
		g_Lang = EN;
	}
	else
		goto REINPUT;

	LoadAccts(DAT_ACCT, &g_pAcct, &g_nAccts, DAT_ADMIN_PWD, g_pszAdminPwd, sizeof(g_pszAdminPwd));
	Run();
	return 0;
}
