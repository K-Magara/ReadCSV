// ReadCSV.cpp : DLL �p�̏����������̒�`���s���܂��B
//

#include "stdafx.h"
#include <afxdllx.h>
#include "resource.h"
#include "NCVCaddin.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static	const	char*	g_szFileExt[] = {"csv"};
static	const	char*	g_szSerialFunc = "ReadCSV";

const double PI = 3.1415926535897932384626433832795;
const double RAD = PI / 180.0;
const double DEG = 180.0 / PI;

static AFX_EXTENSION_MODULE ReadCSVDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// lpReserved ���g���ꍇ�͂������폜���Ă�������
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("READCSV.DLL Initializing!\n");
		
		// �g�� DLL ���P�񂾂����������܂��B
		if (!AfxInitExtensionModule(ReadCSVDLL, hInstance))
			return 0;

		// ���� DLL �����\�[�X �`�F�C���֑}�����܂��B
		// ����: �g�� DLL �� MFC �A�v���P�[�V�����ł͂Ȃ�
		//   MFC �W�� DLL (ActiveX �R���g���[���̂悤��)
		//   �ɈÖٓI�Ƀ����N�����ꍇ�A���̍s�� DllMain
		//   ����폜���āA���̊g�� DLL ����G�N�X�|�[�g
		//   ���ꂽ�ʂ̊֐����֒ǉ����Ă��������B  
		//   ���̊g�� DLL ���g�p����W�� DLL �͂��̊g�� DLL
		//   �����������邽�߂ɖ����I�ɂ��̊֐����Ăяo���܂��B 
		//   ����ȊO�̏ꍇ�́ACDynLinkLibrary �I�u�W�F�N�g��
		//   �W�� DLL �̃��\�[�X �`�F�C���փA�^�b�`���ꂸ�A
		//   ���̌��ʏd��Ȗ��ƂȂ�܂��B

		new CDynLinkLibrary(ReadCSVDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("READCSV.DLL Terminating!\n");
		// �f�X�g���N�^���Ăяo�����O�Ƀ��C�u�������I�����܂�
		AfxTermExtensionModule(ReadCSVDLL);
	}
	return 1;   // ok
}

/////////////////////////////////////////////////////////////////////////////
// NCVC ��޲݊֐�

NCADDIN BOOL NCVC_Initialize(NCVCINITIALIZE* nci)
{
	static	const	char	szReadMenuName[] = "CSV�ް����J��...";
	static	const	char	szReadFuncName[] = "ReadCSV_Menu";
	static	const	int		nReadMenu[] = {
		NCVCADIN_ARY_APPFILE, NCVCADIN_ARY_NCDFILE, NCVCADIN_ARY_DXFFILE
	};
/*
	static	const	char	szOptMenuName[] = "CSV�ް��̵�߼�ݐݒ�...";
	static	const	char	szOptFuncName[] = "ReadCSV_Opt";
	static	const	int		nOptMenu[] = {
		NCVCADIN_ARY_APPOPTION, NCVCADIN_ARY_NCDOPTION, NCVCADIN_ARY_DXFOPTION
	};
*/
	int		i;

	// ��޲݂̕K�v���
	nci->dwSize = sizeof(NCVCINITIALIZE);
	nci->dwType = NCVCADIN_FLG_APPFILE|NCVCADIN_FLG_NCDFILE|NCVCADIN_FLG_DXFFILE;
//		NCVCADIN_FLG_APPOPTION|NCVCADIN_FLG_NCDOPTION|NCVCADIN_FLG_DXFOPTION;
//	nci->nToolBar = 0;
	for ( i=0; i<SIZEOF(nReadMenu); i++ ) {
		nci->lpszMenuName[nReadMenu[i]] = szReadMenuName;
		nci->lpszFuncName[nReadMenu[i]] = szReadFuncName;
	}
/*
	for ( i=0; i<SIZEOF(nOptMenu); i++ ) {
		nci->lpszMenuName[nOptMenu[i]] = szOptMenuName;
		nci->lpszFuncName[nOptMenu[i]] = szOptFuncName;
	}
*/
	nci->lpszAddinName	= g_szSerialFunc;
	nci->lpszCopyright	= "MNCT Yoshiro Nose";
	nci->lpszSupport	= "http://s-gikan2.maizuru-ct.ac.jp/";

	// ���ъg���q�̓o�^�D��������Ă�����D&D�ł�̧�ق��J����
	for ( i=0; i<SIZEOF(g_szFileExt); i++ )
		NCVC_AddDXFExtensionFunc(g_szFileExt[i], g_szSerialFunc, g_szSerialFunc);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// NCVC ����Ċ֐�

NCADDIN void ReadCSV_Menu(void)
{
	CFileDialog	dlg(TRUE, g_szFileExt[0], NULL, OFN_FILEMUSTEXIST|OFN_HIDEREADONLY,
		"CSV files (*.csv)|*.csv|Text file (*.txt)|*.txt|All Files (*.*)|*.*||");
	if ( dlg.DoModal() != IDOK )
		return;
	// �V�K�޷���Ă̍쐬
	NCVC_CreateDXFDocument(dlg.GetPathName(), g_szSerialFunc);
/*
	// ��ڰ��ް�\���̂���SDK���ɏ�������
	TCHAR			szFileName[_MAX_PATH];
	::ZeroMemory(szFileName, sizeof(_MAX_PATH));
	OPENFILENAME	ofn;
	::ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize	= sizeof(OPENFILENAME);
	ofn.hwndOwner	= NCVC_GetMainWnd();
	ofn.lpstrFilter	= "CSV files (*.csv)\0*.csv\0TXT file (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0";
	ofn.lpstrFile	= szFileName;
	ofn.nMaxFile	= _MAX_PATH;
	ofn.Flags		= OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
	ofn.lpstrDefExt	= g_szFileExt[0];
	if ( !::GetOpenFileName(&ofn) )
		return;

	CString	strFile(ofn.lpstrFile);
	// �V�K�޷���Ă̍쐬
	NCVC_CreateDXFDocument(strFile, g_szSerialFunc);
*/
}
/*
NCADDIN void ReadCSV_Opt(void)
{
	AfxMessageBox("Option");
}
*/
/////////////////////////////////////////////////////////////////////////////
//	�֐��v���g�^�C�v
static BOOL GetCsvPoint(NCVCHANDLE, LPCTSTR, double[]);
static BOOL GetCsvLine(NCVCHANDLE, LPCTSTR, double[]);
static BOOL GetCsvCircle(NCVCHANDLE, LPCTSTR, int, double[]);

/////////////////////////////////////////////////////////////////////////////
//	JWC �ǂݍ���
//	NCVC_CreateDXFDocument() �̑�Q�����Ŏw��D
//	NCVC�{�̂��玩���I�ɌĂ΂��

NCADDIN BOOL ReadCSV(NCVCHANDLE hDoc, LPCTSTR pszFile)
{
//	AfxMessageBox("ReadCSV Read Start!!");

	static	LPCTSTR		szDelimiter = ", \t";

	CString	strBuf, strTmp, strLayer;
	double	nData[8];
	int		nNumOfData, nNumOfErr = 0, nAlloc = 0;
	char*	szTok;
	char*	szContext;
	char*	szBuf = NULL;
	BOOL	bResult = TRUE, bOrigin = FALSE;	// ���_ڲԏ�������

	try {
		CStdioFile	fp(pszFile, CFile::modeRead | CFile::shareDenyWrite);

		while ( fp.ReadString(strBuf) ) {				// �P�s���ǂݍ���
			// char* �ֺ�߰
			if ( strBuf.GetLength() >= nAlloc ) {
				if ( szBuf )
					delete	szBuf;
				nAlloc = strBuf.GetLength();
				szBuf = new char[nAlloc+1];
			}
			lstrcpy(szBuf, strBuf);
			nNumOfData = 0;	// �f�[�^��������
			if ( strBuf.FindOneOf(szDelimiter) >= 0 ) {				// szDelimeter ���������
				szTok = strtok_s(szBuf, szDelimiter, &szContext);		// szDelimeter ����؂�Ƃ���İ�ݎ�肾��
				while ( szTok != NULL && nNumOfData < SIZEOF(nData) ){
					nData[nNumOfData] = atof(szTok);
					szTok = strtok_s(NULL, szDelimiter, &szContext);
					nNumOfData++;
				}
			}
			else
				szTok = szBuf;							// �����łȂ���΂P�s�ۂ��� szTok ��

			if ( nNumOfData!=0 && strLayer.IsEmpty() )	// ����!=0 ���� ڲԖ�����Ȃ� while ٰ�߂̐擪��
				continue;

			switch(nNumOfData){
			case 0:		// ڲԃf�[�^
				bOrigin = NCVC_IsOriginLayer(szTok) ? TRUE : FALSE;
				if ( bOrigin || NCVC_IsCutterLayer(szTok) )
					strLayer = szTok;	// szTok �� NCVC �Őݒ肵���؍�ڲԂ����_ڲԂȂ�ڲԖ��ɑ��
				else
					strLayer.Empty();
				break;
			case 2:		// �_�f�[�^
				if (!GetCsvPoint(hDoc, strLayer, nData)) return FALSE;
				break;
			case 3:		// �~�f�[�^
				if ( bOrigin ) {
					// ���_�ް��o�^
					DPOINT	pt;
					pt.x = nData[0];
					pt.y = nData[1];
					NCVC_SetDXFCutterOrigin(hDoc, &pt, nData[2], FALSE);
				}
				else if (!GetCsvCircle(hDoc, strLayer, 0, nData)) return FALSE;
				break;
			case 4:		// ���f�[�^
				if (!GetCsvLine(hDoc, strLayer, nData)) return FALSE;
				break;
			case 5:		// �~�ʃf�[�^
				if (!GetCsvCircle(hDoc, strLayer, 1, nData)) return FALSE;
				break;
			case 7:		// �ȉ~�f�[�^
				if (!GetCsvCircle(hDoc, strLayer, 2, nData)) return FALSE;
				break;
			default:	// �����ȃf�[�^
				nNumOfErr++;
				break;
			}	// switch
		}	// while(main)
	}	// try
	catch ( CMemoryException* e ) {
		AfxMessageBox("Out of memory.", MB_OK|MB_ICONSTOP);
		e->Delete();
		bResult = FALSE;
	}
	catch (	CFileException* e ) {
		AfxMessageBox("̧�ٱ����ŗ�O���������܂���", MB_OK|MB_ICONSTOP);
		e->Delete();
		bResult = FALSE;
	}
	
	if ( szBuf )
		delete	szBuf;
	
	if ( nNumOfErr > 0 ){
		strTmp.Format("%d �̖����ȃf�[�^������܂����B", nNumOfErr);
		AfxMessageBox(strTmp);
	}

	return bResult;
}


/////////////////////////////////////////////////////////////////////////////
// �e��f�[�^��͊֐�

//// �_�f�[�^����
BOOL GetCsvPoint(NCVCHANDLE hDoc, LPCTSTR lpszLayer, double nData[]){
	DXFDATA		dxf;

	dxf.dwSize = sizeof(DXFDATA);
	dxf.enType = DXFPOINTDATA;

	lstrcpy(dxf.szLayer, lpszLayer);
	dxf.nLayer = DXFCAMLAYER;
	dxf.ptS.x = nData[0];
	dxf.ptS.y = nData[1];

	if ( !NCVC_AddDXFData(hDoc, &dxf) ){
		AfxMessageBox("�_�f�[�^�̓ǂݍ��݂Ɏ��s���܂����B");
		return FALSE;
	}

	return TRUE;
}

//// ���f�[�^����
BOOL GetCsvLine(NCVCHANDLE hDoc, LPCTSTR lpszLayer, double nData[]){
	DXFDATA		dxf;

	dxf.dwSize = sizeof(DXFDATA);
	dxf.enType = DXFLINEDATA;

	lstrcpy(dxf.szLayer, lpszLayer);
	dxf.nLayer = DXFCAMLAYER;
	dxf.ptS.x = nData[0];
	dxf.ptS.y = nData[1];
	dxf.de.ptE.x = nData[2];
	dxf.de.ptE.y = nData[3];

	if ( !NCVC_AddDXFData(hDoc, &dxf) ){
		AfxMessageBox("���f�[�^�̓ǂݍ��݂Ɏ��s���܂����B");
		return FALSE;
	}

	return TRUE;
}

//// �~�f�[�^����
BOOL GetCsvCircle(NCVCHANDLE hDoc, LPCTSTR lpszLayer, int CircleType, double nData[]){
	DXFDATA		dxf;
	double		lq;

	// �~�ް����(����)
	dxf.dwSize = sizeof(DXFDATA);
	dxf.ptS.x = nData[0];
	dxf.ptS.y = nData[1];
	
	lstrcpy(dxf.szLayer, lpszLayer);
	dxf.nLayer = DXFCAMLAYER;
	switch ( CircleType ){
	case 0:		// �~
		dxf.enType = DXFCIRCLEDATA;
		dxf.de.dR  = nData[2];
		break;
	case 1:		// �~�ʁi�p�x�P�ʂ͓x�j
		dxf.enType = DXFARCDATA;
		dxf.de.arc.r  = nData[2];
		dxf.de.arc.sq = nData[3];
		dxf.de.arc.eq = nData[4];
		break;
	case 2:		// �ȉ~�i�ʁj�i�p�x�P�ʂ͓x�j
		dxf.enType = DXFELLIPSEDATA;
		dxf.de.elli.sq = nData[3] * RAD;
		dxf.de.elli.eq = nData[4] * RAD;
		lq = nData[6] * RAD;
		dxf.de.elli.ptL.x = nData[2] * cos(lq);
		dxf.de.elli.ptL.y = nData[2] * sin(lq);
		dxf.de.elli.s = nData[5];
		break;
	default:
		break;
	}

	if ( !NCVC_AddDXFData(hDoc, &dxf) ){
		AfxMessageBox("�~(��)�܂��͑ȉ~�f�[�^�̓ǂݍ��݂Ɏ��s���܂����B");
		return FALSE;
	}

	return TRUE;
}
