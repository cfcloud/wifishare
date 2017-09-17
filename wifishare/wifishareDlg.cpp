
// wifishareDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "wifishare.h"
#include "wifishareDlg.h"
#include "afxdialogex.h"
#include "wifishareKernel.h"
#include "wifishareCommonLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CwifishareDlg �Ի���



CwifishareDlg::CwifishareDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_WIFISHARE_DIALOG, pParent)

{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIcon = AfxGetApp()->LoadStandardIcon(IDI_APPLICATION);   //���������Ի����ͼ��
	m_AboutDlg.m_hIcon = AfxGetApp()->LoadStandardIcon(IDI_APPLICATION);


	m_Programatically = false;
	m_IsPasswordHidden = true;
	m_AdapterCount = 0;
	m_State_StartHostedNetworkButton = 0;
	m_State_StartSharingButton = 0;
	m_State_DisableButton = 0;
	m_SelectedDeviceName.Empty();
	m_StartSharing = false;
	m_AboutDlgShown = false;
	m_IsPasswordValid = true;
	if (!(m_hMutex_GetAllData = CreateMutex(NULL, FALSE, NULL))) {
		throw L"CreateMutex����ʧ�ܡ�";
	}


}

CwifishareDlg::~CwifishareDlg()
{

	if (m_hMutex_GetAllData) {
		CloseHandle(m_hMutex_GetAllData);
		m_hMutex_GetAllData = 0;
	}
}

void CwifishareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);


	DDX_Control(pDX, IDC_EDIT1, m_EditBox_SSID);
	DDX_Control(pDX, IDC_EDIT2, m_EditBox_Key);
	DDX_Control(pDX, IDC_EDIT14, m_EditBox_State1);
	DDX_Control(pDX, IDC_EDIT4, m_EditBox_MaxNumberOfPeers);
	DDX_Control(pDX, IDC_EDIT5, m_EditBox_IP1);
	DDX_Control(pDX, IDC_EDIT6, m_EditBox_NumberOfPeers);
	DDX_Control(pDX, IDC_EDIT7, m_EditBox_AuthAlgo);
	DDX_Control(pDX, IDC_EDIT8, m_EditBox_CipherAlgo);
	DDX_Control(pDX, IDC_COMBO1, m_ComboBox_Interface);
	DDX_Control(pDX, IDC_EDIT10, m_EditBox_Name);
	DDX_Control(pDX, IDC_EDIT11, m_EditBox_DeviceName);
	DDX_Control(pDX, IDC_EDIT15, m_EditBox_IP2);
	DDX_Control(pDX, IDC_EDIT12, m_EditBox_State2);
	DDX_Control(pDX, IDC_EDIT13, m_EditBox_SharingState);
	DDX_Control(pDX, IDC_BUTTON1, m_Button_ShowPassword);
	DDX_Control(pDX, IDC_BUTTON2, m_Button_Save);
	DDX_Control(pDX, IDC_BUTTON5, m_Button_Disable);
	DDX_Control(pDX, IDC_BUTTON3, m_Button_StartHostedNetwork);
	DDX_Control(pDX, IDC_BUTTON4, m_Button_StartSharing);
	DDX_Control(pDX, IDC_BUTTON6, m_Button_Refresh);
	DDX_Control(pDX, IDC_BUTTON8, m_Button_Close);
	DDX_Control(pDX, IDC_BUTTON7, m_Button_Exit);
	DDX_Control(pDX, IDC_BUTTON9, m_Button_About);
	DDX_Control(pDX, IDC_EDIT16, m_EditBox_IP3);
}

BEGIN_MESSAGE_MAP(CwifishareDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()


	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL, &CwifishareDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT1, &CwifishareDlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CwifishareDlg::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT4, &CwifishareDlg::OnEnChangeEdit4)
	ON_BN_CLICKED(IDC_RADIO1, &CwifishareDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CwifishareDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_BUTTON1, &CwifishareDlg::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CwifishareDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON3, &CwifishareDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CwifishareDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6, &CwifishareDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON5, &CwifishareDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON2, &CwifishareDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON8, &CwifishareDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON7, &CwifishareDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON9, &CwifishareDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CwifishareDlg::OnBnClickedButton10)
END_MESSAGE_MAP()


// CwifishareDlg ��Ϣ�������

BOOL CwifishareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

									// TODO: �ڴ���Ӷ���ĳ�ʼ������

									//GetSystemMenu(FALSE)->EnableMenuItem(SC_CLOSE, MF_DISABLED);

	InitControls();

	try {
		GetAllData();
	}
	catch (wchar_t *Msg) {
		MessageBox(Msg, L"wifishare", 16);
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CwifishareDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CwifishareDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CwifishareDlg::OnOK()
{

}


void CwifishareDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//CDialogEx::OnClose();
	OnBnClickedButton8();
}


void CwifishareDlg::InitControls()
{
	m_EditBox_SSID.SetLimitText(30);
	m_EditBox_Key.SetLimitText(255);
	m_EditBox_IP2.SetWindowTextW(L" -");
	m_EditBox_IP3.SetWindowTextW(L" -");
	m_EditBox_State2.SetWindowTextW(L" -");
	m_EditBox_DeviceName.SetWindowTextW(L" -");
	m_EditBox_Name.SetWindowTextW(L" -");
	m_EditBox_SharingState.SetWindowTextW(L" -");
	m_EditBox_SSID.SetWindowTextW(L" -");
	m_EditBox_State1.SetWindowTextW(L" -");
	m_EditBox_MaxNumberOfPeers.SetWindowTextW(L" -");
	m_EditBox_IP1.SetWindowTextW(L" -");
	m_EditBox_NumberOfPeers.SetWindowTextW(L" -");
	m_EditBox_AuthAlgo.SetWindowTextW(L" -");
	m_EditBox_CipherAlgo.SetWindowTextW(L" -");
	m_ComboBox_Interface.SetWindowTextW(L" -");
	if (m_IsPasswordHidden) {
		m_Button_ShowPassword.SetWindowTextW(L"��ʾ");
		m_EditBox_Key.SetPasswordChar(9679);
	}
	else {
		m_Button_ShowPassword.SetWindowTextW(L"����");
		m_EditBox_Key.SetPasswordChar(0);
	}
	m_Button_Save.EnableWindow(FALSE);
	ChangeState_DisableButton(4);
	ChangeState_StartHostedNetworkButton(4);
	ChangeState_StartSharingButton(4);
	m_SelectedDeviceName.Empty();
}


void CwifishareDlg::GetAllData()
{

	WaitForSingleObject(m_hMutex_GetAllData, INFINITE);

	m_Programatically = true;

	wchar_t tmp[256];
	bool found;

	m_AdapterCount = lqx::GetAllAdaptersInfo(m_AdapterInfos, MAX_NUMBER_OF_ADAPTERS);

	lqx::_HostedNetworkInfo HostedNetworkInfo;
	lqx::QueryHostedNetwork(&HostedNetworkInfo);



	MultiByteToWideChar(CP_ACP, 0, HostedNetworkInfo.SSID, -1, tmp, 256);
	m_EditBox_SSID.SetWindowTextW(tmp);

	if (HostedNetworkInfo.IsPassPhase) {
		((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
		((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
		MultiByteToWideChar(CP_ACP, 0, HostedNetworkInfo.Key, -1, tmp, 256);
		m_EditBox_Key.SetWindowTextW(tmp);
		if (HostedNetworkInfo.Key[0] == '\0') m_IsPasswordValid = false;
	}
	else {
		((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(TRUE);
		((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
		lqx::BytesToHexStringW((const unsigned char *)HostedNetworkInfo.Key, 32, tmp, 256);
		m_EditBox_Key.SetWindowTextW(tmp);
	}

	if (HostedNetworkInfo.Active) {
		m_EditBox_State1.SetWindowTextW(L"�ѿ���");
		ChangeState_StartHostedNetworkButton(2);
	}
	else {
		m_EditBox_State1.SetWindowTextW(L"δ����");
		ChangeState_StartHostedNetworkButton(0);
	}

	_itow_s(HostedNetworkInfo.MaxNumberOfPeers, tmp, 256, 10);
	m_EditBox_MaxNumberOfPeers.SetWindowTextW(tmp);

	found = false;
	for (size_t i = 0; i < m_AdapterCount; i++) {
		if (!m_AdapterInfos[i].DeviceName.Compare(HOSTEDNETWORK_DEVICENAME)) {
			m_EditBox_IP1.SetWindowTextW(m_AdapterInfos[i].IPv4);
			found = true;
			break;
		}
	}
	if (!found) {
		m_EditBox_IP1.SetWindowTextW(L"������");
	}

	_itow_s(HostedNetworkInfo.NumberOfPeers, tmp, 256, 10);
	m_EditBox_NumberOfPeers.SetWindowTextW(tmp);

	switch (HostedNetworkInfo.dot11AuthAlgo) {
	case DOT11_AUTH_ALGO_80211_OPEN:
		m_EditBox_AuthAlgo.SetWindowTextW(L"802.11_OPEN");
		break;
	case DOT11_AUTH_ALGO_80211_SHARED_KEY:
		m_EditBox_AuthAlgo.SetWindowTextW(L"802.11_SHARED_KEY");
		break;
	case DOT11_AUTH_ALGO_WPA:
		m_EditBox_AuthAlgo.SetWindowTextW(L"WPA");
		break;
	case DOT11_AUTH_ALGO_WPA_PSK:
		m_EditBox_AuthAlgo.SetWindowTextW(L"WPA_PSK");
		break;
	case DOT11_AUTH_ALGO_WPA_NONE:
		m_EditBox_AuthAlgo.SetWindowTextW(L"WPA_NONE");
		break;
	case DOT11_AUTH_ALGO_RSNA:
		m_EditBox_AuthAlgo.SetWindowTextW(L"RSNA");
		break;
	case DOT11_AUTH_ALGO_RSNA_PSK:
		m_EditBox_AuthAlgo.SetWindowTextW(L"RSNA_PSK");
		break;
	default:
		m_EditBox_AuthAlgo.SetWindowTextW(L"proprietary authentication algorithms");
		break;
	}

	switch (HostedNetworkInfo.dot11CipherAlgo)
	{
	case DOT11_CIPHER_ALGO_NONE:
		m_EditBox_CipherAlgo.SetWindowTextW(L"NONE");
		break;
	case DOT11_CIPHER_ALGO_WEP40:
		m_EditBox_CipherAlgo.SetWindowTextW(L"WEP40");
		break;
	case 0x02: //DOT11_CIPHER_ALGO_ALGO_TKIP
		m_EditBox_CipherAlgo.SetWindowTextW(L"TKIP");
		break;
	case 0x04: //DOT11_CIPHER_ALGO_ALGO_CCMP
		m_EditBox_CipherAlgo.SetWindowTextW(L"CCMP");
		break;
	case 0x05: //DOT11_CIPHER_ALGO_ALGO_WEP104
		m_EditBox_CipherAlgo.SetWindowTextW(L"WEP104");
		break;
	case DOT11_CIPHER_ALGO_WPA_USE_GROUP: //or DOT11_CIPHER_ALGO_RSN_USE_GROUP
		m_EditBox_CipherAlgo.SetWindowTextW(L"WPA/RSN_USE_GROUP");
		break;
	case DOT11_CIPHER_ALGO_WEP:
		m_EditBox_CipherAlgo.SetWindowTextW(L"WEP");
		break;
	default:
		m_EditBox_CipherAlgo.SetWindowTextW(L"proprietary cipher algorithms");
		break;
	}

	while (m_ComboBox_Interface.GetCount()) m_ComboBox_Interface.DeleteString(0);
	CString LowerString;
	for (size_t i = 0; i < m_AdapterCount; i++) {
		if (m_AdapterInfos[i].DeviceName.Compare(HOSTEDNETWORK_DEVICENAME)) {
			LowerString.SetString(m_AdapterInfos[i].DeviceName);
			LowerString.MakeLower();
			if (AfxGetApp()->GetProfileIntW(L"Config", L"ShowVirtualAdapters", 0) == 0 && LowerString.Find(L"virtual") != -1) continue;
			m_ComboBox_Interface.AddString(m_AdapterInfos[i].DeviceName);
		}
	}

	m_ComboBox_Interface.SelectString(0, AfxGetApp()->GetProfileStringW(L"Data", L"DefaultDevice"));
	OnCbnSelchangeCombo1();


	m_Programatically = false;

	ReleaseMutex(m_hMutex_GetAllData);


}

//0����ʾ�����ù���  1����ʾ����������...��  2����ʾ��ֹͣ����  3����ʾ������ֹͣ...��  4:��ťʧЧ
void CwifishareDlg::ChangeState_StartSharingButton(int state)
{
	if (m_State_StartSharingButton == 1 || m_State_StartSharingButton == 3) {
		return;
	}
	switch (state)
	{
	case 0:
		m_Button_StartSharing.SetWindowTextW(L"���ù���");
		m_Button_StartSharing.EnableWindow(TRUE);
		m_State_StartSharingButton = state;
		break;
	case 1:
		m_Button_StartSharing.SetWindowTextW(L"��������...");
		m_Button_StartSharing.EnableWindow(FALSE);
		m_State_StartSharingButton = state;
		break;
	case 2:
		m_Button_StartSharing.SetWindowTextW(L"ֹͣ����");
		m_Button_StartSharing.EnableWindow(TRUE);
		m_State_StartSharingButton = state;
		break;
	case 3:
		m_Button_StartSharing.SetWindowTextW(L"����ֹͣ...");
		m_Button_StartSharing.EnableWindow(FALSE);
		m_State_StartSharingButton = state;
		break;
	case 4:
		m_Button_StartSharing.SetWindowTextW(L"���ù���");
		m_Button_StartSharing.EnableWindow(FALSE);
		m_State_StartSharingButton = state;
		break;
	default:
		break;
	}
}

//0����ʾ�������ȵ㡱  1����ʾ�����ڿ���...��  2����ʾ���ر��ȵ㡱  3����ʾ�����ڹر�...��  4:��ťʧЧ
void CwifishareDlg::ChangeState_StartHostedNetworkButton(int state)
{
	if (m_State_StartHostedNetworkButton == 1 || m_State_StartHostedNetworkButton == 3) {
		return;
	}
	switch (state)
	{
	case 0:
		m_Button_StartHostedNetwork.SetWindowTextW(L"�����ȵ�");
		m_Button_StartHostedNetwork.EnableWindow(TRUE);
		m_State_StartHostedNetworkButton = state;
		break;
	case 1:
		m_Button_StartHostedNetwork.SetWindowTextW(L"���ڿ���...");
		m_Button_StartHostedNetwork.EnableWindow(FALSE);
		m_State_StartHostedNetworkButton = state;
		break;
	case 2:
		m_Button_StartHostedNetwork.SetWindowTextW(L"�ر��ȵ�");
		m_Button_StartHostedNetwork.EnableWindow(TRUE);
		m_State_StartHostedNetworkButton = state;
		break;
	case 3:
		m_Button_StartHostedNetwork.SetWindowTextW(L"���ڹر�...");
		m_Button_StartHostedNetwork.EnableWindow(FALSE);
		m_State_StartHostedNetworkButton = state;
		break;
	case 4:
		m_Button_StartHostedNetwork.SetWindowTextW(L"�����ȵ�");
		m_Button_StartHostedNetwork.EnableWindow(FALSE);
		m_State_StartHostedNetworkButton = state;
		break;
	default:
		break;
	}
}


//0����ʾ�����á�  1����ʾ�����ڽ���...��  2����ʾ�����á�  3����ʾ����������...��  4:��ťʧЧ
void CwifishareDlg::ChangeState_DisableButton(int state)
{
	if (m_State_DisableButton == 1 || m_State_DisableButton == 3) {
		return;
	}
	switch (state)
	{
	case 0:
		m_Button_Disable.SetWindowTextW(L"����");
		m_Button_Disable.EnableWindow(TRUE);
		m_State_DisableButton = state;
		break;
	case 1:
		m_Button_Disable.SetWindowTextW(L"���ڽ���...");
		m_Button_Disable.EnableWindow(FALSE);
		m_State_DisableButton = state;
		break;
	case 2:
		m_Button_Disable.SetWindowTextW(L"����");
		m_Button_Disable.EnableWindow(TRUE);
		m_State_DisableButton = state;
		break;
	case 3:
		m_Button_Disable.SetWindowTextW(L"��������...");
		m_Button_Disable.EnableWindow(FALSE);
		m_State_DisableButton = state;
		break;
	case 4:
		m_Button_Disable.SetWindowTextW(L"����");
		m_Button_Disable.EnableWindow(FALSE);
		m_State_DisableButton = state;
		break;
	default:
		break;
	}
}

void CwifishareDlg::OnEnChangeEdit1()   //SSID
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	if (!m_Programatically) m_Button_Save.EnableWindow(TRUE);

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CwifishareDlg::OnEnChangeEdit2()  //����
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	if (!m_Programatically) m_Button_Save.EnableWindow(TRUE);

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CwifishareDlg::OnEnChangeEdit4()   //���������
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	if (!m_Programatically) m_Button_Save.EnableWindow(TRUE);

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}




void CwifishareDlg::OnBnClickedRadio1()   //�ַ���
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (!m_Programatically) m_Button_Save.EnableWindow(TRUE);

}


void CwifishareDlg::OnBnClickedRadio2()   //16��������
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (!m_Programatically) m_Button_Save.EnableWindow(TRUE);

}


void CwifishareDlg::OnBnClickedButton1()   //��ʾ����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_Programatically = true;
	if (m_IsPasswordHidden) {
		m_EditBox_Key.SetPasswordChar(0);
		m_Button_ShowPassword.SetWindowTextW(L"����");
		m_EditBox_Key.Invalidate();
		m_IsPasswordHidden = false;
	}
	else {
		m_EditBox_Key.SetPasswordChar(9679);
		m_Button_ShowPassword.SetWindowTextW(L"��ʾ");
		m_EditBox_Key.Invalidate();
		m_IsPasswordHidden = true;
	}
	m_Programatically = false;
}


void CwifishareDlg::OnCbnSelchangeCombo1()   //����ѡ��
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int index = m_ComboBox_Interface.GetCurSel();

	if (index == -1) return;

	m_ComboBox_Interface.GetLBText(index, m_SelectedDeviceName);

	AfxGetApp()->WriteProfileStringW(L"Data", L"DefaultDevice", m_SelectedDeviceName);

	for (size_t i = 0; i < m_AdapterCount; i++) {
		if (!m_AdapterInfos[i].DeviceName.Compare(m_SelectedDeviceName)) {
			m_EditBox_IP2.SetWindowTextW(m_AdapterInfos[i].IPv4);
			m_EditBox_IP3.SetWindowTextW(m_AdapterInfos[i].IPv6);
			m_EditBox_Name.SetWindowTextW(m_AdapterInfos[i].Name);
			m_EditBox_DeviceName.SetWindowTextW(m_AdapterInfos[i].DeviceName);

			if (m_AdapterInfos[i].SharingType == lqx::SharingType_Public) {
				m_EditBox_SharingState.SetWindowTextW(L"�ѿ���");
				ChangeState_StartSharingButton(2);
			}
			else {
				m_EditBox_SharingState.SetWindowTextW(L"δ����");
				ChangeState_StartSharingButton(0);
			}

			switch (m_AdapterInfos[i].Status)
			{
			case NCS_DISCONNECTED:
				m_EditBox_State2.SetWindowTextW(L"�ѽ���");
				ChangeState_DisableButton(2);
				break;
			case NCS_CONNECTING:
				m_EditBox_State2.SetWindowTextW(L"��������...");
				ChangeState_DisableButton(4);
				break;
			case NCS_CONNECTED:
				m_EditBox_State2.SetWindowTextW(L"������");
				ChangeState_DisableButton(0);
				break;
			case NCS_DISCONNECTING:
				m_EditBox_State2.SetWindowTextW(L"���ڽ���...");
				ChangeState_DisableButton(4);
				break;
			case NCS_HARDWARE_NOT_PRESENT:
				m_EditBox_State2.SetWindowTextW(L"Ӳ��������");
				ChangeState_DisableButton(4);
				break;
			case NCS_HARDWARE_DISABLED:
				m_EditBox_State2.SetWindowTextW(L"Ӳ���ѽ���");
				ChangeState_DisableButton(4);
				break;
			case NCS_HARDWARE_MALFUNCTION:
				m_EditBox_State2.SetWindowTextW(L"Ӳ������");
				ChangeState_DisableButton(4);
				break;
			case NCS_MEDIA_DISCONNECTED:
				m_EditBox_State2.SetWindowTextW(L"δ����");
				ChangeState_DisableButton(0);
				break;
			case NCS_AUTHENTICATING:
				m_EditBox_State2.SetWindowTextW(L"������֤...");
				ChangeState_DisableButton(0);
				break;
			case NCS_AUTHENTICATION_SUCCEEDED:
				m_EditBox_State2.SetWindowTextW(L"��֤�ɹ�");
				ChangeState_DisableButton(0);
				break;
			case NCS_AUTHENTICATION_FAILED:
				m_EditBox_State2.SetWindowTextW(L"��֤ʧ��");
				ChangeState_DisableButton(0);
				break;
			case NCS_INVALID_ADDRESS:
				m_EditBox_State2.SetWindowTextW(L"��Ч��ַ");
				ChangeState_DisableButton(0);
				break;
			case NCS_CREDENTIALS_REQUIRED:
				m_EditBox_State2.SetWindowTextW(L"��Ҫ����");
				ChangeState_DisableButton(0);
				break;
			default:
				m_EditBox_State2.SetWindowTextW(L"����");
				ChangeState_DisableButton(0);
				break;
			}

		}
	}



}


UINT CwifishareDlg::StartHostedNetwork(LPVOID p)
{
	CwifishareDlg *dlg = (CwifishareDlg*)p;

	try {
		lqx::SetWlanPowerState(true);
		lqx::StartHostedNetwork();
		dlg->m_State_StartHostedNetworkButton = 2;
		dlg->GetAllData();
		//dlg->ChangeState_StartHostedNetworkButton(2);
		if (dlg->m_StartSharing) {
			dlg->m_StartSharing = false;
			dlg->m_State_StartSharingButton = 0;
			dlg->OnBnClickedButton4();
		}
	}
	catch (wchar_t *Msg) {
		if (dlg->m_StartSharing) {
			dlg->m_StartSharing = false;
			dlg->m_State_StartSharingButton = 0;
			dlg->ChangeState_StartSharingButton(0);
		}
		
		dlg->m_State_StartHostedNetworkButton = 0;
		dlg->ChangeState_StartHostedNetworkButton(0);

		AfxMessageBox(Msg, 16);
	}


	return 0;
}

UINT CwifishareDlg::StopHostedNetwork(LPVOID p)
{
	CwifishareDlg *dlg = (CwifishareDlg*)p;

	try {
		lqx::StopHostedNetwork();
		dlg->m_State_StartHostedNetworkButton = 0;
		dlg->GetAllData();
		//dlg->ChangeState_StartHostedNetworkButton(0);

	}
	catch (wchar_t *Msg) {
		dlg->m_State_StartHostedNetworkButton = 2;
		dlg->ChangeState_StartHostedNetworkButton(2);
		AfxMessageBox(Msg, 16);
	}

	return 0;
}


void CwifishareDlg::OnBnClickedButton3()  //�ȵ㿪��
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	switch (m_State_StartHostedNetworkButton)
	{
	case 0:
		if (!m_IsPasswordValid) {
			MessageBox(L"���벻��Ϊ�ա�", L"wifishare", 16);
			return;
		}
		ChangeState_StartHostedNetworkButton(1);
		AfxBeginThread(&StartHostedNetwork, this);
		break;
	case 2:
		ChangeState_StartHostedNetworkButton(3);
		AfxBeginThread(&StopHostedNetwork, this);
		break;
	default:
		break;
	}

}


UINT CwifishareDlg::StartSharing(LPVOID p)
{
	CwifishareDlg *dlg = (CwifishareDlg*)p;

	try {
		lqx::StartSharing(dlg->m_SelectedDeviceName);
		dlg->m_State_StartSharingButton = 2;
		dlg->GetAllData();
		//dlg->ChangeState_StartSharingButton(2);

	}
	catch (wchar_t *Msg) {
		dlg->m_State_StartSharingButton = 0;
		dlg->ChangeState_StartSharingButton(0);
		AfxMessageBox(Msg, 16);
	}

	return 0;
}

UINT CwifishareDlg::StopSharing(LPVOID p)
{
	CwifishareDlg *dlg = (CwifishareDlg*)p;

	try {
		lqx::StopSharing();
		dlg->m_State_StartSharingButton = 0;
		dlg->GetAllData();
		//dlg->ChangeState_StartSharingButton(0);

	}
	catch (wchar_t *Msg) {
		dlg->m_State_StartSharingButton = 2;
		dlg->ChangeState_StartSharingButton(2);
		AfxMessageBox(Msg, 16);
	}

	return 0;
}


void CwifishareDlg::OnBnClickedButton4()  //������
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	switch (m_State_StartSharingButton)
	{
	case 0:
		if (!m_IsPasswordValid) {
			MessageBox(L"���벻��Ϊ�ա�", L"wifishare", 16);
			return;
		}
		ChangeState_StartSharingButton(1);
		if (m_State_StartHostedNetworkButton == 0) {
			m_StartSharing = true;
			OnBnClickedButton3();
		}
		else {
			AfxBeginThread(&StartSharing, this);

		}

		break;
	case 2:
		ChangeState_StartSharingButton(3);
		AfxBeginThread(&StopSharing, this);

		break;
	default:
		break;
	}
}


void CwifishareDlg::OnBnClickedButton6()  //ˢ������
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	InitControls();
	try {
		GetAllData();
	}
	catch (wchar_t *Msg) {
		MessageBox(Msg, L"wifishare", 16);
	}
}

UINT CwifishareDlg::DisableAdapter(LPVOID p)
{
	CwifishareDlg *dlg = (CwifishareDlg*)p;

	try {
		lqx::ChangeAdapterSdate(dlg->m_SelectedDeviceName, false);
		dlg->m_State_DisableButton = 2;
		dlg->GetAllData();
		//dlg->ChangeState_DisableButton(2);

	}
	catch (wchar_t *Msg) {
		dlg->m_State_DisableButton = 0;
		dlg->ChangeState_DisableButton(0);
		AfxMessageBox(Msg, 16);
	}

	return 0;
}

UINT CwifishareDlg::EnableAdapter(LPVOID p)
{
	CwifishareDlg *dlg = (CwifishareDlg*)p;

	try {
		lqx::ChangeAdapterSdate(dlg->m_SelectedDeviceName, true);
		dlg->m_State_DisableButton = 0;
		dlg->GetAllData();
		//dlg->ChangeState_DisableButton(0);
	}
	catch (wchar_t *Msg) {
		dlg->m_State_DisableButton = 2;
		dlg->ChangeState_DisableButton(2);
		AfxMessageBox(Msg, 16);
	}

	return 0;
}

void CwifishareDlg::OnBnClickedButton5()  //����/����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	switch (m_State_DisableButton)
	{
	case 0:
		ChangeState_DisableButton(1);
		AfxBeginThread(&DisableAdapter, this);
		break;
	case 2:
		ChangeState_DisableButton(3);
		AfxBeginThread(&EnableAdapter, this);
		break;
	default:
		break;
	}
}


void CwifishareDlg::OnBnClickedButton2()   //�����޸�
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char tmp1[512];
	wchar_t tmp2[256];
	char ch = '?';
	int n;

	lqx::_HostedNetworkInfo HostedNetworkInfo;
	lqx::QueryHostedNetwork(&HostedNetworkInfo);

	n = m_EditBox_Key.GetWindowTextW(tmp2, 256);
	if (((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck()) {
		//���Ϊ�ַ���
		if (n < 8 || n>63) {
			MessageBox(L"���볤�ȱ�����8~63֮�䡣", L"wifishare", 16);
			return;
		}
		for (int i = 0; tmp2[i]; i++) {
			if ((unsigned int)tmp2[i] < 0x20U || (unsigned int)tmp2[i]>0x7EU) {
				MessageBox(L"���������Ч�ַ���", L"wifishare", 16);
				return;
			}
		}

		//ȷ�������ʽ��ȷ��

		HostedNetworkInfo.IsPassPhase = true;
		WideCharToMultiByte(CP_ACP, 0, tmp2, -1, tmp1, 512, &ch, NULL);
		strcpy_s(HostedNetworkInfo.Key, 64, tmp1);
		m_IsPasswordValid = true;

	}
	else {
		//���Ϊ16��������
		try {
			n = lqx::HexStringToBytesW(tmp2, (unsigned char *)tmp1, 512);
			if (n != 32) {
				MessageBox(L"16���Ƹ�ʽ�����������64�� 0~9 A~F a~f ���ַ���ɡ��������ַ������Բ��ƣ�", L"wifishare", 16);
				return;
			}
		}
		catch (wchar_t *Msg) {
			MessageBox(L"16���Ƹ�ʽ�����������64�� 0~9 A~F a~f ���ַ���ɡ��������ַ������Բ��ƣ�", L"wifishare", 16);
			return;
		}

		//ȷ�������ʽ��ȷ��

		HostedNetworkInfo.IsPassPhase = false;
		memcpy_s(HostedNetworkInfo.Key, 32, tmp1, 32);
		m_IsPasswordValid = true;

	}


	m_EditBox_SSID.GetWindowTextW(tmp2, 256);
	WideCharToMultiByte(CP_ACP, 0, tmp2, -1, tmp1, 512, &ch, NULL);
	tmp1[30] = '\0';            //SSID���ַ�����󳤶ȣ�����ĩβ��\0����31�ֽ�
	strcpy_s(HostedNetworkInfo.SSID, 32, tmp1);

	m_EditBox_MaxNumberOfPeers.GetWindowTextW(tmp2, 256);
	n = _wtoi(tmp2);
	if (n <= 0) {
		MessageBox(L"���������������Ч��", L"wifishare", 16);
		return;
	}
	HostedNetworkInfo.MaxNumberOfPeers = n;

	try {
		lqx::SetHostedNetwork(&HostedNetworkInfo);
	}
	catch (wchar_t *Msg) {
		MessageBox(Msg, L"wifishare", 16);
		return;
	}

	MessageBox(L"�ɹ����棡");
	m_Button_Save.EnableWindow(FALSE);

	GetAllData();

}


void CwifishareDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnCancel();
	OnBnClickedButton8();
}

void CwifishareDlg::OnBnClickedButton8()    //�رմ���
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	EndDialog(DIALOGRESULT_CLOSE);
}


void CwifishareDlg::OnBnClickedButton7()   //�˳�����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	EndDialog(DIALOGRESULT_EXIT);
}


UINT CwifishareDlg::ShowAboutDlg(LPVOID p)
{
	CwifishareDlg *dlg = (CwifishareDlg*)p;
	if (!dlg->m_AboutDlgShown) {
		dlg->m_AboutDlgShown = true;
		dlg->m_AboutDlg.DoModal();
		dlg->m_AboutDlgShown = false;
	}
	else {
		//����˵��������ǰ��
		SwitchToThisWindow(dlg->m_AboutDlg.m_hWnd, TRUE);
	}

	return 0;
}

void CwifishareDlg::OnBnClickedButton9()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	AfxBeginThread(&ShowAboutDlg, this);

}


void CwifishareDlg::OnBnClickedButton10()
{
	// TODO: Add your control notification handler code here
	WinExec("control netconnections", SW_SHOW);
}
