// ColorDialog.cpp
#include "pch.h"
#include "framework.h"
#include "CGWORK0602.h"
#include "ColorDialog.h"

// CMyColorDialog �Ի���

IMPLEMENT_DYNAMIC(CMyColorDialog, CDialogEx)

CMyColorDialog::CMyColorDialog(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_COLOR_DIALOG, pParent)
    , m_color(RGB(0, 0, 0))
    , m_red(0)
    , m_green(0)
    , m_blue(0)
    , m_bUpdating(FALSE)  // ��ʼ����־
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

// CMyColorDialog ��Ϣ�������

BOOL CMyColorDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // ֱ�����ñ༭���ı�
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
    // ���ø��±�־����ֹ�ݹ�
    m_bUpdating = TRUE;

    CString strValue;

    // �ֶ��ӱ༭���ȡֵ
    m_editRed.GetWindowText(strValue);
    m_red = _ttoi(strValue);

    m_editGreen.GetWindowText(strValue);
    m_green = _ttoi(strValue);

    m_editBlue.GetWindowText(strValue);
    m_blue = _ttoi(strValue);

    // ��֤��Χ
    m_red = max(0, min(255, m_red));
    m_green = max(0, min(255, m_green));
    m_blue = max(0, min(255, m_blue));

    // ���ֵ�����������»ر༭��
    CString strNewValue;
    strNewValue.Format(_T("%d"), m_red);
    if (strNewValue != strValue) {
        m_editRed.SetWindowText(strNewValue);
        // ������ƶ����ı�ĩβ
        m_editRed.SetSel(strNewValue.GetLength(), strNewValue.GetLength());
    }

    strNewValue.Format(_T("%d"), m_green);
    m_editGreen.GetWindowText(strValue);
    if (strNewValue != strValue) {
        m_editGreen.SetWindowText(strNewValue);
        // ������ƶ����ı�ĩβ
        m_editGreen.SetSel(strNewValue.GetLength(), strNewValue.GetLength());
    }

    strNewValue.Format(_T("%d"), m_blue);
    m_editBlue.GetWindowText(strValue);
    if (strNewValue != strValue) {
        m_editBlue.SetWindowText(strNewValue);
        // ������ƶ����ı�ĩβ
        m_editBlue.SetSel(strNewValue.GetLength(), strNewValue.GetLength());
    }

    m_color = RGB(m_red, m_green, m_blue);
    UpdatePreview();

    // ������±�־
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

        // ���Ʊ߿�
        dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT);
    }
}