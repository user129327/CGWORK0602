// ColorDialog.h
#pragma once

// CMyColorDialog �Ի���
class CMyColorDialog : public CDialogEx
{
    DECLARE_DYNAMIC(CMyColorDialog)

public:
    CMyColorDialog(CWnd* pParent = nullptr);
    virtual ~CMyColorDialog();

    // �Ի�������
    enum { IDD = IDD_COLOR_DIALOG };

    COLORREF GetColor() const { return m_color; }
    void SetColor(COLORREF color) { m_color = color; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

    DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnEnChangeEditRed();
    afx_msg void OnEnChangeEditGreen();
    afx_msg void OnEnChangeEditBlue();

private:
    COLORREF m_color;
    int m_red;
    int m_green;
    int m_blue;
    BOOL m_bUpdating;  // ��������־

    CEdit m_editRed;
    CEdit m_editGreen;
    CEdit m_editBlue;
    CStatic m_staticPreview;

    void UpdateColor();
    void UpdatePreview();
};