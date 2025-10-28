// ColorDialog.cpp
#include "pch.h"
#include "framework.h"
#include "CGWORK0602.h"
#include "ColorDialog.h"

// CMyColorDialog 对话框

IMPLEMENT_DYNAMIC(CMyColorDialog, CDialogEx)

CMyColorDialog::CMyColorDialog(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_COLOR_DIALOG, pParent)
    , m_color(RGB(0, 0, 0))
    , m_red(0)
    , m_green(0)
    , m_blue(0)
    , m_bUpdating(FALSE)  // 初始化标志
{
}

CMyColorDialog::~CMyColorDialog()
{
}

void CMyColorDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_RED, m_editRed);
    DDX_Control(pDX, IDC_EDIT_GREEN, m_editGreen);
    DDX_Control(pDX, IDC_EDIT_BLUE, m_editBlue);
    DDX_Control(pDX, IDC_STATIC_PREVIEW, m_staticPreview);
}

BEGIN_MESSAGE_MAP(CMyColorDialog, CDialogEx)
    ON_EN_CHANGE(IDC_EDIT_RED, &CMyColorDialog::OnEnChangeEditRed)
    ON_EN_CHANGE(IDC_EDIT_GREEN, &CMyColorDialog::OnEnChangeEditGreen)
    ON_EN_CHANGE(IDC_EDIT_BLUE, &CMyColorDialog::OnEnChangeEditBlue)
END_MESSAGE_MAP()

// CMyColorDialog 消息处理程序

BOOL CMyColorDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 直接设置编辑框文本
        CString strValue;
    strValue.Format(_T("%d"), GetRValue(m_color));
    SetDlgItemText(IDC_EDIT_RED, strValue);

    strValue.Format(_T("%d"), GetGValue(m_color));
    SetDlgItemText(IDC_EDIT_GREEN, strValue);

    strValue.Format(_T("%d"), GetBValue(m_color));
    SetDlgItemText(IDC_EDIT_BLUE, strValue);
    UpdatePreview();

    return TRUE;
}

void CMyColorDialog::OnEnChangeEditRed()
{
    if (!m_bUpdating) {
        UpdateColor();
    }
}

void CMyColorDialog::OnEnChangeEditGreen()
{
    if (!m_bUpdating) {
        UpdateColor();
    }
}

void CMyColorDialog::OnEnChangeEditBlue()
{
    if (!m_bUpdating) {
        UpdateColor();
    }
}

void CMyColorDialog::UpdateColor()
{
    // 设置更新标志，防止递归
    m_bUpdating = TRUE;

    CString strValue;

    // 手动从编辑框获取值
    m_editRed.GetWindowText(strValue);
    m_red = _ttoi(strValue);

    m_editGreen.GetWindowText(strValue);
    m_green = _ttoi(strValue);

    m_editBlue.GetWindowText(strValue);
    m_blue = _ttoi(strValue);

    // 验证范围
    m_red = max(0, min(255, m_red));
    m_green = max(0, min(255, m_green));
    m_blue = max(0, min(255, m_blue));

    // 如果值被修正，更新回编辑框
    CString strNewValue;
    strNewValue.Format(_T("%d"), m_red);
    if (strNewValue != strValue) {
        m_editRed.SetWindowText(strNewValue);
        // 将光标移动到文本末尾
        m_editRed.SetSel(strNewValue.GetLength(), strNewValue.GetLength());
    }

    strNewValue.Format(_T("%d"), m_green);
    m_editGreen.GetWindowText(strValue);
    if (strNewValue != strValue) {
        m_editGreen.SetWindowText(strNewValue);
        // 将光标移动到文本末尾
        m_editGreen.SetSel(strNewValue.GetLength(), strNewValue.GetLength());
    }

    strNewValue.Format(_T("%d"), m_blue);
    m_editBlue.GetWindowText(strValue);
    if (strNewValue != strValue) {
        m_editBlue.SetWindowText(strNewValue);
        // 将光标移动到文本末尾
        m_editBlue.SetSel(strNewValue.GetLength(), strNewValue.GetLength());
    }

    m_color = RGB(m_red, m_green, m_blue);
    UpdatePreview();

    // 清除更新标志
    m_bUpdating = FALSE;
}

void CMyColorDialog::UpdatePreview()
{
    CWnd* pPreview = GetDlgItem(IDC_STATIC_PREVIEW);
    if (pPreview)
    {
        CClientDC dc(pPreview);
        CRect rect;
        pPreview->GetClientRect(&rect);

        CBrush brush(m_color);
        dc.FillRect(&rect, &brush);

        // 绘制边框
        dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT);
    }
}