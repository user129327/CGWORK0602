#include "pch.h"
#include "CGWORK0602.h"
#include "SettingsDialog.h"

// CSettingsDialog 对话框

IMPLEMENT_DYNAMIC(CSettingsDialog, CDialogEx)

CSettingsDialog::CSettingsDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETTINGS_DIALOG, pParent)
	, m_dValue(0.0)
	, m_strTitle(_T("设置参数"))
	, m_strPrompt(_T("请输入值："))
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

	// 设置对话框标题和提示文本
	SetWindowText(m_strTitle);
	SetDlgItemText(IDC_STATIC, m_strPrompt); // 假设静态文本ID为IDC_STATIC_PROMPT

	return TRUE;  // return TRUE unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CSettingsDialog, CDialogEx)
END_MESSAGE_MAP()