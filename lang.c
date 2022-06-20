#include "lang.h"
#include "Common.h"

int g_Lang = 0;

const char* GetTransType(TransType tt)
{
	if (g_Lang == EN)
	{
		switch (tt)
		{
		case T_WITHDRAW:
			return "Withdraw";
		case T_DEPOSIT:
			return "Deposit";
		case T_TRANSFER_IN:
			return "Transfer(IN)";
		case T_TRANSFER_OUT:
			return "Transfer(OUT)";
		}
	}
	else
	{
		switch (tt)
		{
		case T_WITHDRAW:
			return "取款";
		case T_DEPOSIT:
			return "存款";
		case T_TRANSFER_IN:
			return "转入";
		case T_TRANSFER_OUT:
			return "转出";
		}
	}
	return NULL;
}

const char* GetLangItem(int id)
{
	if (g_Lang == EN)
		return g_szResEN[id];
	else
		return g_szResCH[id];
}

const char* GetError(int iErrorCode)
{
	if (g_Lang == EN)
	{
		switch (iErrorCode)
		{
		case ERR_AMT:
			return "Invailid Amount or Not Enough Balance!";
		case ERR_CARDB:
			return "Account Not Exist!";
		case ERR_INV_PWD:
			return "Invailid password, password must be 6 digits";
		case ERR_ACCT_FRZ:
			return "Account was frozen!";
		case ERR_ACCT_NOTFOUND:
			return "Account was not found!";
		case ERR_INV_CARD:
			return "Invailid Account/Card Number!";
		case ERR_CARD_EXIST:
			return "Account/Card No. already exists!";
		case ERR_SUCC:
		default:
			return "Transaction Success!";
		}
	}
	else
	{
		switch (iErrorCode)
		{
		case ERR_AMT:
			return "金额不合法或余额不足！";
		case ERR_CARDB:
			return "账号不存在！";
		case ERR_INV_PWD:
			return "密码不合法，必须是6位数字组成！";
		case ERR_ACCT_FRZ:
			return "账号已经被冻结！";
		case ERR_ACCT_NOTFOUND:
			return "账号未找到！";
		case ERR_INV_CARD:
			return "非法的卡号！";
		case ERR_CARD_EXIST:
			return "账号/卡号已经存在!";
		case ERR_SUCC:
		default:
			return "交易成功！";
		}
	}
}

char g_szResEN[][MAX_HINT_ITEM] =
{
	"*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-* \n"
	"|                                                                          | \n"
	"*                     Welcome to ATM Systems                               * \n"
	"|        Select the service you need and press Enter to confirm!           | \n"
	"*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-* \n",
	"【1】User Login",
	"【2】Administration",
	"Switch Language",
	"【3】Exit",
	"【1】Withdraw Cash",
	"【2】Deposit Cash",
	"【3】Query Balance",
	"【4】Transfer",
	"【5】Transaction History",
	"【6】Modify Password",
	"【7】Back to Main Menu",
	"【1】Browse All Accounts",
	"【2】Browse All History",
	"【3】Freeze Account",
	"【4】Defreeze Account",
	"【5】Add New Account",
	"【6】Remove Account",
	"【7】Back to Main Menu",
	"List Of Accounts:",
	"Frozen",
	"Noraml",
	"Press Any Key to Continue...",
	"Please Input Administrator Password(6 digits):",
	"Please Input Card No./Account No.(13~19 digits):",
	"Please Input Password(6 digits):",
	"Incorrect Account/Password.",
	"             Account         Name  Sex    State      Balance",
	"Sir",
	"Madam",
	"Dear %s %s, welcome here:\n",
	"Balance:\n%.02f￥\n",
	"Please Input the Amount:",
	"Transaction/Operation Success!",
	"Transaction Failed! Error Code:%s",
	"Please Input Reciever's Card No./Account No.(13~19 digits):",
	"Flow:%s, transction type:%s, Card A:%s, Card B:%s, Amount:%.02f, Timestamp(yyyymmddHHMMSS):%s\n",
	"Please Input New Password:",
	"Password has been updated!",
	"Please input the name:",
	"Please input the gender,('F' for female, 'M' for male):"
	"Please Select the Amount:",
	"Other Amount",
	"Timestamp:"
};

char g_szResCH[][MAX_HINT_ITEM] =
{
	"*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-* \n"
	"|                                                                         | \n" 
	"*                    欢迎使用ATM柜员机系统                                * \n" 
	"|                  选择您所需要的服务，并按回车确认!                      | \n" 
	"*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-* \n",
	"【1】登录",
	"【2】管理",
	"语言切换",
	"【3】退出",
	"【1】取款",
	"【2】存款",
	"【3】余额查询",
	"【4】转账",
	"【5】交易记录",
	"【6】修改密码",
	"【7】返回",
	"【1】查看账户列表",
	"【2】查看交易历史",
	"【3】冻结",
	"【4】解冻",
	"【5】开户",
	"【6】销户",
	"【7】返回",
	"账户清单:",
	"冻结",
	"正常",
	"按任意键继续...",
	"请输入管理密码(6位数字):",
	"请输入卡号/账号(13-19位数字):",
	"请输入密码(6位数字):",
	"账号/密码出错",
	"                账号         姓名 性别     状态         余额",
	"先生",
	"女士",
	"亲爱的%s%s,欢迎:\n",
	"账户余额:\n%.02f￥\n",
	"请输入金额:",
	"交易/操作成功！",
	"交易失败！错误码:%s",
	"请输入收款方卡号/账号(13-19位数字):",
	"流水号:%s, 交易类型:%s, 卡A:%s, 卡B:%s, 金额:%.02f, 时间戳(yyyymmddHHMMSS):%s\n",
	"请输入新密码:",
	"密码已修改！",
	"请输入名字:",
	"请输入性别,('F' 女, 'M' 男):",
	"请选择金额:",
	"其他金额",
	"交易时间:"
};
