// CGWORK0602View.h: CCGWORK0602View 类的接口
//

#pragma once
class CMyColorDialog;

class CCGWORK0602View : public CView
{
protected:
	CCGWORK0602View() noexcept;
	DECLARE_DYNCREATE(CCGWORK0602View)


public:
	CCGWORK0602Doc* GetDocument() const;
	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual ~CCGWORK0602View();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	DECLARE_MESSAGE_MAP()

public:
	// 原有消息处理函数
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);


	// 矩形绘制消息处理函数
	afx_msg void OnDrawRect();
	afx_msg void OnUpdateDrawRect(CCmdUI* pCmdUI);  // 新增：菜单项更新处理
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	// === 圆形绘制消息处理函数 ===
	afx_msg void OnDrawCircle();
	afx_msg void OnUpdateDrawCircle(CCmdUI* pCmdUI);

	// 区域填充消息处理函数
	afx_msg void OnAreaFill();
	afx_msg void OnUpdateAreaFill(CCmdUI* pCmdUI);

	//分别设置边框和填充颜色
	afx_msg void OnAreaFillSetBorderColor();
	afx_msg void OnAreaFillSetFillColor();
	afx_msg void OnUpdateAreaFillSetBorderColor(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAreaFillSetFillColor(CCmdUI* pCmdUI);

	// === 多边形绘制消息处理函数 ===
	afx_msg void OnDrawPolygon();
	afx_msg void OnUpdateDrawPolygon(CCmdUI* pCmdUI);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);  // 新增右键处理


	// 在消息映射中添加
	afx_msg void OnDrawSetcolor();
	afx_msg void OnUpdateDrawSetcolor(CCmdUI* pCmdUI);

	// 区域填充颜色设置
		afx_msg void OnAreaFillSetcolor();
	afx_msg void OnUpdateAreaFillSetcolor(CCmdUI* pCmdUI);

	// === 三维变换消息处理函数 ===
	afx_msg void On3DTransformDrawCube();
	afx_msg void OnUpdate3DTransformDrawCube(CCmdUI* pCmdUI);
	afx_msg void On3DRotateX();  // 绕x轴旋转
	afx_msg void OnUpdate3DRotateX(CCmdUI* pCmdUI);
	afx_msg void On3DRotateY();  // 绕y轴旋转
	afx_msg void OnUpdate3DRotateY(CCmdUI* pCmdUI); 
	afx_msg void On3DRotateZ();  // 绕z轴旋转
	afx_msg void OnUpdate3DRotateZ(CCmdUI* pCmdUI);  
	afx_msg void On3DTranslateX();  // 延x轴平移
	afx_msg void OnUpdate3DTranslateX(CCmdUI* pCmdUI);  
	afx_msg void On3DTranslateY();  // 延y轴平移
	afx_msg void OnUpdate3DTranslateY(CCmdUI* pCmdUI);  
	afx_msg void On3DTranslateZ();  // 延z轴平移
	afx_msg void OnUpdate3DTranslateZ(CCmdUI* pCmdUI);  

	// 设置数据菜单处理函数
	afx_msg void OnSettingsTranslationStep();  // 设置平移步长
	afx_msg void OnSettingsRotationAngle();    // 设置旋转角度
	afx_msg void OnUpdateSettingsTranslationStep(CCmdUI* pCmdUI);  // 更新函数
	afx_msg void OnUpdateSettingsRotationAngle(CCmdUI* pCmdUI);    // 更新函数


	// 键盘消息处理函数
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	// === Bezier曲线消息处理函数 ===
	afx_msg void OnDrawCurveBezier();
	afx_msg void OnUpdateDrawCurveBezier(CCmdUI* pCmdUI);

	// 清屏功能消息处理函数
	afx_msg void OnEditClearAll();
	afx_msg void OnUpdateEditClearAll(CCmdUI* pCmdUI);

private:
	// === 私有辅助函数 ===
	void DrawCube3D(CDC* pDC, CCGWORK0602Doc* pDoc);
	//void DrawBezierCurves(CDC* pDC, CCGWORK0602Doc* pDoc);  // 绘制Bezier曲线

};

#ifndef _DEBUG
inline CCGWORK0602Doc* CCGWORK0602View::GetDocument() const
{
	return reinterpret_cast<CCGWORK0602Doc*>(m_pDocument);
}
#endif