
// wifishareDlg.h : ͷ�ļ�
//

#pragma once
#include "stdafx.h"
#include "afxwin.h"
#include "AboutDlg.h"
#include "wifishareCommonLib.h"


// CwifishareDlg �Ի���
class CwifishareDlg : public CDialogEx
{
	// ����
public:
	CwifishareDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CwifishareDlg();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WIFISHARE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
	bool m_Programatically;
	bool m_IsPasswordHidden;
	lqx::ADAPTER_INFO m_AdapterInfos[MAX_NUMBER_OF_ADAPTERS];
	size_t m_AdapterCount;
	int m_State_StartSharingButton;
	int m_State_StartHostedNetworkButton;
	int m_State_DisableButton;
	CString m_SelectedDeviceName;
	bool m_StartSharing;
	HANDLE m_hMutex_GetAllData;
	CAboutDlg m_AboutDlg;
	bool m_AboutDlgShown;
	bool m_IsPasswordValid;


	// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	void OnOK();
	void GetAllData();
	//0����ʾ�����ù���  1����ʾ����������...��  2����ʾ��ֹͣ����  3����ʾ������ֹͣ...��  4:��ťʧЧ
	void ChangeState_StartSharingButton(int state);
	//0����ʾ�������ȵ㡱  1����ʾ�����ڿ���...��  2����ʾ���ر��ȵ㡱  3����ʾ�����ڹر�...��  4:��ťʧЧ
	void ChangeState_StartHostedNetworkButton(int state);
	//0����ʾ�����á�  1����ʾ�����ڽ���...��  2����ʾ�����á�  3����ʾ����������...��  4:��ťʧЧ
	void ChangeState_DisableButton(int state);
	void InitControls();

	static UINT StartHostedNetwork(LPVOID p);
	static UINT StopHostedNetwork(LPVOID p);
	static UINT StartSharing(LPVOID p);
	static UINT StopSharing(LPVOID p);
	static UINT DisableAdapter(LPVOID p);
	static UINT EnableAdapter(LPVOID p);
	static UINT ShowAboutDlg(LPVOID p);

public:


	afx_msg void OnClose();
	afx_msg void OnBnClickedCancel();
	// SSID�༭��
	CEdit m_EditBox_SSID;
	// ������������
	CEdit m_EditBox_Key;
	// ��������״̬
	CEdit m_EditBox_State1;
	// ���������
	CEdit m_EditBox_MaxNumberOfPeers;
	// ��������IP
	CEdit m_EditBox_IP1;
	// ��������������
	CEdit m_EditBox_NumberOfPeers;
	// ����������֤��ʽ
	CEdit m_EditBox_AuthAlgo;
	// ������������㷨
	CEdit m_EditBox_CipherAlgo;
	// ѡ�����������
	CComboBox m_ComboBox_Interface;
	// ������������
	CEdit m_EditBox_Name;
	// ���������豸��
	CEdit m_EditBox_DeviceName;
	// ��������IP
	CEdit m_EditBox_IP2;
	// ��������״̬
	CEdit m_EditBox_State2;
	// ������������״̬
	CEdit m_EditBox_SharingState;
	// ��ʾ����
	CButton m_Button_ShowPassword;
	// �����޸�
	CButton m_Button_Save;
	// ����
	CButton m_Button_Disable;
	// �����ȵ�
	CButton m_Button_StartHostedNetwork;
	// ���ù���
	CButton m_Button_StartSharing;
	// ˢ������
	CButton m_Button_Refresh;
	// �رմ���
	CButton m_Button_Close;
	// �˳�����
	CButton m_Button_Exit;
	// ˵�� ��ť
	CButton m_Button_About;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton9();
	// ��������IPv6��ַ
	CEdit m_EditBox_IP3;
	afx_msg void OnBnClickedButton10();
};
