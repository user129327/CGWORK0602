#pragma once
#include "afxdialogex.h"

// CSettingsDialog 对话框

class CSettingsDialog : public CDialogEx
{
    DECLARE_DYNAMIC(CSettingsDialog)

public:
    CSettingsDialog(CWnd* pParent = nullptr);   // 标准构造函数
    virtual ~CSettingsDialog();

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_SETTINGS_DIALOG };
#endif

    double GetValue() const { return m_dValue; }
    void SetValue(double value) { m_dValue = value; }
    void SetTitle(const CString& title) { m_strTitle = title; }
    void SetPrompt(const CString& prompt) { m_strPrompt = prompt; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();

    DECLARE_MESSAGE_MAP()

private:
    double m_dValue;
    CString m_strTitle;
    CString m_strPrompt;
};