#include <windows.h>
#include <stdio.h>
#define MaxMess 200
char NbCommonErrorMsg[MaxMess];

int LanaEnum(LANA_ENUM* lenum)
{
	NCB ncb;
	NbCommonErrorMsg[0] = 0;
	ZeroMemory(&ncb, sizeof(NCB));
	ncb.ncb_command = NCBENUM;
	ncb.ncb_buffer = (PUCHAR)lenum;
	ncb.ncb_length = sizeof(LANA_ENUM);
	if (Netbios(&ncb) != NRC_GOODRET)
	{
		sprintf_s(NbCommonErrorMsg, MaxMess,
			"ERROR: Netbios: NCBENUM: %d\n",
			ncb.ncb_retcode);
		return ncb.ncb_retcode;
	}
	return NRC_GOODRET;
}

int ResetAll(LANA_ENUM* lenum, UCHAR ucMaxSession,
	UCHAR ucMaxName, BOOL bFirstName)
{
	NCB ncb;
	int i;
	NbCommonErrorMsg[0] = 0;
	ZeroMemory(&ncb, sizeof(NCB));
	ncb.ncb_command = NCBRESET;
	ncb.ncb_callname[0] = ucMaxSession;
	ncb.ncb_callname[2] = ucMaxName;
	ncb.ncb_callname[3] = (UCHAR)bFirstName;
	for (i = 0; i < lenum->length; i++)
	{
		ncb.ncb_lana_num = lenum->lana[i];
		if (Netbios(&ncb) != NRC_GOODRET)
		{
			sprintf_s(NbCommonErrorMsg, MaxMess,
				"ERROR: Netbios: NCBRESET[%d]: %d\n",
				ncb.ncb_lana_num, ncb.ncb_retcode);
			return ncb.ncb_retcode;
		}
	}
	return NRC_GOODRET;
}

int AddName(int lana, char* name, int* num)
{
	NCB ncb;
	NbCommonErrorMsg[0] = 0;
	ZeroMemory(&ncb, sizeof(NCB));
	ncb.ncb_command = NCBADDNAME;
	ncb.ncb_lana_num = lana;
	memset(ncb.ncb_name, ' ', NCBNAMSZ);
	strncpy_s((char*)ncb.ncb_name, sizeof(ncb.ncb_name),
		name, strlen(name));
	if (Netbios(&ncb) != NRC_GOODRET)
	{
		sprintf_s(NbCommonErrorMsg, MaxMess,
			"ERROR: Netbios: NCBADDNAME[lana=%d;name=%s]: %d\n",
			lana, name, ncb.ncb_retcode);
		return ncb.ncb_retcode;
	}
	*num = ncb.ncb_num;
	return NRC_GOODRET;
}

int AddGroupName(int lana, char* name, int* num)
{
	NCB ncb;
	NbCommonErrorMsg[0] = 0;
	ZeroMemory(&ncb, sizeof(NCB));
	ncb.ncb_command = NCBADDGRNAME;
	ncb.ncb_lana_num = lana;
	memset(ncb.ncb_name, ' ', NCBNAMSZ);
	strncpy_s((char*)ncb.ncb_name, sizeof(ncb.ncb_name),
		name, strlen(name));
	if (Netbios(&ncb) != NRC_GOODRET)
	{
		sprintf_s(NbCommonErrorMsg, MaxMess,
			"ERROR: Netbios: NCBADDGRNAME[lana=%d;name=%s]: %d\n",
			lana, name, ncb.ncb_retcode);
		return ncb.ncb_retcode;
	}
	*num = ncb.ncb_num;
	return NRC_GOODRET;
}

int DelName(int lana, char* name)
{
	NCB ncb;
	NbCommonErrorMsg[0] = 0;
	ZeroMemory(&ncb, sizeof(NCB));
	ncb.ncb_command = NCBDELNAME;
	ncb.ncb_lana_num = lana;
	memset(ncb.ncb_name, ' ', NCBNAMSZ);
	strncpy_s((char*)ncb.ncb_name, sizeof(ncb.ncb_name),
		name, strlen(name));
	if (Netbios(&ncb) != NRC_GOODRET)
	{
		sprintf_s(NbCommonErrorMsg, MaxMess,
			"ERROR: Netbios: NCBADDNAME[lana=%d;name=%s]: %d\n",
			lana, name, ncb.ncb_retcode);
		return ncb.ncb_retcode;
	}
	return NRC_GOODRET;
}
int Send(int lana, int lsn, char* data, DWORD len)
{
	NCB ncb;
	int retcode;
	NbCommonErrorMsg[0] = 0;
	ZeroMemory(&ncb, sizeof(NCB));
	ncb.ncb_command = NCBSEND;
	ncb.ncb_buffer = (PUCHAR)data;
	ncb.ncb_length = (WORD)len;
	ncb.ncb_lana_num = (UCHAR)lana;
	ncb.ncb_lsn = lsn;
	retcode = Netbios(&ncb);
	return retcode;
}
int Recv(int lana, int lsn, char* buffer, DWORD* len)
{
	NCB ncb;
	NbCommonErrorMsg[0] = 0;
	ZeroMemory(&ncb, sizeof(NCB));
	ncb.ncb_command = NCBRECV;
	ncb.ncb_buffer = (PUCHAR)buffer;
	ncb.ncb_length = (WORD)*len;
	ncb.ncb_lana_num = (UCHAR)lana;
	ncb.ncb_lsn = lsn;
	if (Netbios(&ncb) != NRC_GOODRET)
	{
		*len = -1;
		return ncb.ncb_retcode;
	}
	*len = ncb.ncb_length;
	return NRC_GOODRET;
}

int Hangup(int lana, int lsn)
{
	NCB ncb;
	int retcode;
	NbCommonErrorMsg[0] = 0;
	ZeroMemory(&ncb, sizeof(NCB));
	ncb.ncb_command = NCBHANGUP;
	ncb.ncb_lsn = lsn;
	ncb.ncb_lana_num = lana;
	retcode = Netbios(&ncb);
	return retcode;
}
int Cancel(PNCB pncb)
{
	NCB ncb;
	NbCommonErrorMsg[0] = 0;
	ZeroMemory(&ncb, sizeof(NCB));
	ncb.ncb_command = NCBCANCEL;
	ncb.ncb_buffer = (PUCHAR)pncb;
	ncb.ncb_lana_num = pncb->ncb_lana_num;
	if (Netbios(&ncb) != NRC_GOODRET)
	{
		sprintf_s(NbCommonErrorMsg, MaxMess,
			"ERROR: NetBIOS: NCBCANCEL: %d\n", ncb.ncb_retcode);
		return ncb.ncb_retcode;
	}
	return NRC_GOODRET;
}
int FormatNetbiosName(char* nbname, char* outname)
{
	int i;
	NbCommonErrorMsg[0] = 0;
	strncpy_s(outname, NCBNAMSZ, nbname, NCBNAMSZ);
	outname[NCBNAMSZ - 1] = '\0';
	for (i = 0; i < NCBNAMSZ - 1; i++)
	{
		// Непечатные символы заменяются точками
		//`
		if (!((outname[i] >= 32) && (outname[i] <= 126)))
			outname[i] = '.';
	}
	return NRC_GOODRET;
}