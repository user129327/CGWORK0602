#include "pch.h"
#include "CGWORK0602.h"
#include "SettingsDialog.h"

// CSettingsDialog �Ի���

IMPLEMENT_DYNAMIC(CSettingsDialog, CDialogEx)

CSettingsDialog::CSettingsDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETTINGS_DIALOG, pParent)
	, m_dValue(0.0)
	, m_strTitle(_T("���ò���"))
	, m_strPrompt(_T("������ֵ��"))
{
}

CSettingsDialog::~CSettingsDialog()
{
}

void CSettingsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_VALUE, m_dValue);
}

BOOL CSettingsDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���öԻ���������ʾ�ı�
	SetWindowText(m_strTitle);
	SetDlgItemText(IDC_STATIC, m_strPrompt); // ���農̬�ı�IDΪIDC_STATIC_PROMPT

	return TRUE;  // return TRUE unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CSettingsDialog, CDialogEx)
END_MESSAGE_MAP()