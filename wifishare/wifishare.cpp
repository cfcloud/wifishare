
// wifishare.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "wifishare.h"
#include "wifishareDlg.h"
#include "wifishareKernel.h"
#include "wifishareCommonLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CwifishareApp

BEGIN_MESSAGE_MAP(CwifishareApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CwifishareApp ����

CwifishareApp::CwifishareApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CwifishareApp ����

CwifishareApp theApp;


// CwifishareApp ��ʼ��

BOOL CwifishareApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("liuqx"));

	if (GetProfileInt(L"Config", L"ShowVirtualAdapters", 0) == 0)
		WriteProfileInt(L"Config", L"ShowVirtualAdapters", 0);


	HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, EVENTNAME_START);
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		SetEvent(hEvent);
		ResetEvent(hEvent);
		return 0;
	}
	else if (!hEvent) {
		MessageBox(NULL, L"CreateEvent����ʧ�ܡ�", L"wifishare", 16);
	}

	CwifishareDlg dlg;
	INT_PTR nResponse;


	if (!lqx::IsRunasAdmin()) {
		//���û�й���ԱȨ�ޣ��������Թ���Ա�������
		SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
		wchar_t exefile[MAX_PATH];
		wcscpy_s(exefile, MAX_PATH, GetCommandLine());
		sei.lpVerb = TEXT("runas");
		sei.lpFile = lqx::trimw(exefile, L"\" ");     //ȥ�����ߵĿո��˫����
		sei.nShow = SW_SHOWNORMAL;
		if (!ShellExecuteEx(&sei)) {
			DWORD dwStatus = GetLastError();
			if (dwStatus == ERROR_CANCELLED) {
				MessageBox(NULL, L"���Թ���Ա������С�", L"wifishare", 16);
			}
			else if (dwStatus == ERROR_FILE_NOT_FOUND) {
				//printf("��Ҫִ�е��ļ�û���ҵ�\n");
			}
		}
		return 0;
	}

	//��ʼ��windock
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		MessageBox(NULL, L"WSAStartup����ʧ�ܡ�", L"wifishare", 16);
		goto finish;
	}




start: //��ʾ���Ի���

	   //m_pMainWnd = &dlg;
	try {

		nResponse = dlg.DoModal();

		if (nResponse == DIALOGRESULT_CLOSE)
		{
			// TODO: �ڴ˷��ô����ʱ��
			//  ��ȷ�������رնԻ���Ĵ���
			WaitForSingleObject(hEvent, INFINITE);
			goto start;

		}
		else if (nResponse == DIALOGRESULT_EXIT)
		{
			// TODO: �ڴ˷��ô����ʱ��
			//  ��ȡ�������رնԻ���Ĵ���
		}
		else if (nResponse == -1)
		{
			TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
			TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
		}

	}
	catch (wchar_t *Msg) {
		MessageBox(NULL, Msg, L"wifishare", 16);
	}


finish:

	WSACleanup();

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

