// CGWORK0602View.cpp: CCGWORK0602View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CGWORK0602.h"
#endif

#include "CGWORK0602Doc.h"
#include "CGWORK0602View.h"
#include "ColorDialog.h"
#include"SettingsDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 在文件开头添加菜单ID定义（如果Resource.h中已定义则可省略）
#ifndef ID_DRAW_RECT
#define ID_DRAW_RECT 32771
#endif

#ifndef ID_DRAW_POLYGON
#define ID_DRAW_POLYGON 32778
#endif

// CCGWORK0602View

IMPLEMENT_DYNCREATE(CCGWORK0602View, CView)

// 在消息映射中添加圆形绘制菜单处理
BEGIN_MESSAGE_MAP(CCGWORK0602View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CCGWORK0602View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	// === 原有的消息映射 ===
	ON_COMMAND(ID_DRAW_RECT, &CCGWORK0602View::OnDrawRect)
	ON_UPDATE_COMMAND_UI(ID_DRAW_RECT, &CCGWORK0602View::OnUpdateDrawRect)
	ON_COMMAND(ID_AREA_FILL, &CCGWORK0602View::OnAreaFill)
	ON_UPDATE_COMMAND_UI(ID_AREA_FILL, &CCGWORK0602View::OnUpdateAreaFill)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	// === 多边形绘制消息映射 ===
	ON_COMMAND(ID_DRAW_POLYGON, &CCGWORK0602View::OnDrawPolygon)
	ON_UPDATE_COMMAND_UI(ID_DRAW_POLYGON, &CCGWORK0602View::OnUpdateDrawPolygon)
	ON_WM_RBUTTONDOWN()  // 新增右键按下处理
	// === 圆形绘制消息映射 ===
	ON_COMMAND(ID_DRAW_CIRCLE, &CCGWORK0602View::OnDrawCircle)
	ON_UPDATE_COMMAND_UI(ID_DRAW_CIRCLE, &CCGWORK0602View::OnUpdateDrawCircle)
	ON_COMMAND(ID_DRAW_SETCOLOR, &CCGWORK0602View::OnDrawSetcolor)
	ON_UPDATE_COMMAND_UI(ID_DRAW_SETCOLOR, &CCGWORK0602View::OnUpdateDrawSetcolor)
	ON_COMMAND(ID_AREA_FILL_SETCOLOR, &CCGWORK0602View::OnAreaFillSetcolor)
	ON_UPDATE_COMMAND_UI(ID_AREA_FILL_SETCOLOR, &CCGWORK0602View::OnUpdateAreaFillSetcolor)

	// 分别设置边框和填充颜色
	ON_COMMAND(ID_AREA_FILL_SET_BORDER_COLOR, &CCGWORK0602View::OnAreaFillSetBorderColor)
	ON_COMMAND(ID_AREA_FILL_SET_FILL_COLOR, &CCGWORK0602View::OnAreaFillSetFillColor)
	ON_UPDATE_COMMAND_UI(ID_AREA_FILL_SET_BORDER_COLOR, &CCGWORK0602View::OnUpdateAreaFillSetBorderColor)
	ON_UPDATE_COMMAND_UI(ID_AREA_FILL_SET_FILL_COLOR, &CCGWORK0602View::OnUpdateAreaFillSetFillColor)

	// === 三维变换消息映射 ===
	ON_COMMAND(ID_3D_TRANSFORM_DRAW_CUBE, &CCGWORK0602View::On3DTransformDrawCube)
	ON_UPDATE_COMMAND_UI(ID_3D_TRANSFORM_DRAW_CUBE, &CCGWORK0602View::OnUpdate3DTransformDrawCube)
	ON_COMMAND(ID_3D_ROTATE_X, &CCGWORK0602View::On3DRotateX)  
	ON_UPDATE_COMMAND_UI(ID_3D_ROTATE_X, &CCGWORK0602View::OnUpdate3DRotateX)
	ON_COMMAND(ID_3D_ROTATE_Y, &CCGWORK0602View::On3DRotateY)  
	ON_UPDATE_COMMAND_UI(ID_3D_ROTATE_Y, &CCGWORK0602View::OnUpdate3DRotateY)  
	ON_COMMAND(ID_3D_ROTATE_Z, &CCGWORK0602View::On3DRotateZ)  
	ON_UPDATE_COMMAND_UI(ID_3D_ROTATE_Z, &CCGWORK0602View::OnUpdate3DRotateZ)
	ON_COMMAND(ID_3D_TRANSLATE_X, &CCGWORK0602View::On3DTranslateX)
	ON_UPDATE_COMMAND_UI(ID_3D_TRANSLATE_X, &CCGWORK0602View::OnUpdate3DTranslateX) 
	ON_COMMAND(ID_3D_TRANSLATE_Y, &CCGWORK0602View::On3DTranslateY)  
	ON_UPDATE_COMMAND_UI(ID_3D_TRANSLATE_Y, &CCGWORK0602View::OnUpdate3DTranslateY)  
	ON_COMMAND(ID_3D_TRANSLATE_Z, &CCGWORK0602View::On3DTranslateZ)  
	ON_UPDATE_COMMAND_UI(ID_3D_TRANSLATE_Z, &CCGWORK0602View::OnUpdate3DTranslateZ) 

	// 设置数据消息映射
	ON_COMMAND(ID_SETTINGS_TRANSLATION_STEP, &CCGWORK0602View::OnSettingsTranslationStep)
	ON_COMMAND(ID_SETTINGS_ROTATION_ANGLE, &CCGWORK0602View::OnSettingsRotationAngle)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS_TRANSLATION_STEP, &CCGWORK0602View::OnUpdateSettingsTranslationStep)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS_ROTATION_ANGLE, &CCGWORK0602View::OnUpdateSettingsRotationAngle)

	// 键盘消息映射
	ON_WM_KEYDOWN()

	// === Bezier曲线消息映射 ===
	ON_COMMAND(ID_DRAW_CURVE_BEZIER, &CCGWORK0602View::OnDrawCurveBezier)
	ON_UPDATE_COMMAND_UI(ID_DRAW_CURVE_BEZIER, &CCGWORK0602View::OnUpdateDrawCurveBezier)

	// === 清屏功能消息映射 ===
	ON_COMMAND(ID_CLEAR_SCREEN, &CCGWORK0602View::OnEditClearAll)
	ON_UPDATE_COMMAND_UI(ID_CLEAR_SCREEN, &CCGWORK0602View::OnUpdateEditClearAll)

END_MESSAGE_MAP()



// CCGWORK0602View 构造/析构

CCGWORK0602View::CCGWORK0602View() noexcept
{
	// TODO: 在此处添加构造代码
}

CCGWORK0602View::~CCGWORK0602View()
{
}

BOOL CCGWORK0602View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CCGWORK0602View 绘图

// 修改 OnDraw 函数
void CCGWORK0602View::OnDraw(CDC* pDC)
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码

	// === 修改后的绘制代码 ===
	// 1. 首先绘制所有已完成的矩形
	int count = pDoc->m_rectArray.GetSize();
	for (int i = 0; i < count; i++)
	{
		CRect rect = pDoc->m_rectArray[i];
		if (!rect.IsRectNull())
		{
			// 使用文档中存储的颜色创建画笔
			CPen pen(PS_SOLID, 2, pDoc->m_rectColor);
			CPen* pOldPen = pDC->SelectObject(&pen);

			// 创建透明画刷
			CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

			// 绘制矩形
			pDC->Rectangle(rect);

			// 恢复原来的GDI对象
			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldBrush);
		}
	}

	// 2. 绘制所有已完成的圆形
	int circleCount = pDoc->m_circleArray.GetSize();
	for (int i = 0; i < circleCount; i++)
	{
		CRect circleRect = pDoc->m_circleArray[i];
		if (!circleRect.IsRectNull())
		{
			// 使用文档中存储的颜色创建画笔
			CPen pen(PS_SOLID, 2, pDoc->m_circleColor);
			CPen* pOldPen = pDC->SelectObject(&pen);

			// 创建透明画刷
			CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

			// 绘制圆形
			pDC->Ellipse(circleRect);

			// 恢复原来的GDI对象
			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldBrush);
		}
	}


	// 3. 如果正在绘制，显示临时矩形（蓝色虚线）
	if (pDoc->m_bDrawing && !pDoc->m_tempRect.IsRectNull())
	{
		// 创建蓝色虚线画笔用于临时显示
		CPen pen(PS_DASH, 1, RGB(0, 0, 255));
		CPen* pOldPen = pDC->SelectObject(&pen);

		// 透明画刷
		CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

		// 绘制临时矩形
		pDC->Rectangle(pDoc->m_tempRect);

		// 恢复原来的GDI对象
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
	}

	// 4. 如果正在绘制圆形，显示临时圆形（蓝色虚线）
	if (pDoc->m_bDrawing && pDoc->m_bDrawCircleMode && !pDoc->m_tempCircle.IsRectNull())
	{
		// 创建蓝色虚线画笔用于临时显示
		CPen pen(PS_DASH, 1, RGB(0, 0, 255));
		CPen* pOldPen = pDC->SelectObject(&pen);

		// 透明画刷
		CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

		// 绘制临时圆形
		pDC->Ellipse(pDoc->m_tempCircle);

		// 恢复原来的GDI对象
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
	}

	// === 新增：多边形绘制 ===
		// === 修改：多边形绘制 ===
	// 1. 首先绘制所有已完成的多边形
	for (const auto& polygon : pDoc->m_completedPolygons)
	{
		if (polygon.size() >= 3)
		{
			// 使用文档中存储的边界颜色绘制多边形边线
			CPen pen(PS_SOLID, 2, pDoc->m_polygonBorderColor);
			CPen* pOldPen = pDC->SelectObject(&pen);

			// 绘制多边形顶点之间的连线
			pDC->MoveTo(polygon[0]);
			for (size_t i = 1; i < polygon.size(); i++)
			{
				pDC->LineTo(polygon[i]);
			}
			// 闭合多边形
			pDC->LineTo(polygon[0]);

			// 设置文本颜色为填充颜色
			COLORREF oldTextColor = pDC->SetTextColor(pDoc->m_polygonFillColor);
			int oldBkMode = pDC->SetBkMode(TRANSPARENT);

			// 使用边标志算法填充多边形 - 使用新的函数名
			pDoc->EdgeFlagFillPolygon(pDC, polygon, _T("0602"));

			// 恢复原来的设置
			pDC->SetTextColor(oldTextColor);
			pDC->SetBkMode(oldBkMode);
			pDC->SelectObject(pOldPen);

			// 绘制顶点标记
			for (const auto& point : polygon)
			{
				CRect pointRect(point.x - 2, point.y - 2,
					point.x + 2, point.y + 2);
				pDC->FillSolidRect(pointRect, RGB(255, 0, 0));  // 红色小方块标记顶点
			}
		}
	}

	// 2. 然后绘制当前正在绘制的多边形（如果有）
	if (!pDoc->m_polygonPoints.empty())
	{
		// 使用文档中存储的边界颜色绘制多边形边线
		CPen pen(PS_SOLID, 2, pDoc->m_polygonBorderColor);
		CPen* pOldPen = pDC->SelectObject(&pen);

		// 绘制多边形顶点之间的连线
		for (size_t i = 0; i < pDoc->m_polygonPoints.size(); i++)
		{
			if (i == 0)
			{
				pDC->MoveTo(pDoc->m_polygonPoints[i]);
			}
			else
			{
				pDC->LineTo(pDoc->m_polygonPoints[i]);
			}
		}

		// 如果多边形绘制完成，闭合多边形并填充
		if (!pDoc->m_bPolygonDrawing && pDoc->m_polygonPoints.size() >= 3)
		{
			// 闭合多边形
			pDC->LineTo(pDoc->m_polygonPoints[0]);

			// 设置文本颜色为填充颜色
			COLORREF oldTextColor = pDC->SetTextColor(pDoc->m_polygonFillColor);
			int oldBkMode = pDC->SetBkMode(TRANSPARENT);

			// 使用边标志算法填充多边形 - 使用原有的函数名
			if (pDoc->IsInPolygonMode())
			{
				pDoc->EdgeFlagFill(pDC, _T("0602"));
			}

			// 恢复原来的设置
			pDC->SetTextColor(oldTextColor);
			pDC->SetBkMode(oldBkMode);
		}

		pDC->SelectObject(pOldPen);

		// 绘制顶点标记
		for (size_t i = 0; i < pDoc->m_polygonPoints.size(); i++)
		{
			CRect pointRect(pDoc->m_polygonPoints[i].x - 2, pDoc->m_polygonPoints[i].y - 2,
				pDoc->m_polygonPoints[i].x + 2, pDoc->m_polygonPoints[i].y + 2);
			pDC->FillSolidRect(pointRect, RGB(255, 0, 0));  // 红色小方块标记顶点
		}
	}

	// === 三维立方体绘制 ===
	if (pDoc->IsInDrawCubeMode() ||
		pDoc->IsInRotateXMode() || pDoc->IsInRotateYMode() || pDoc->IsInRotateZMode() ||
		pDoc->IsInTranslateXMode() || pDoc->IsInTranslateYMode() || pDoc->IsInTranslateZMode())
	{
		DrawCube3D(pDC, pDoc);
	}
	// === 新增：Bezier曲线绘制 ===
	// 1. 绘制所有已完成的Bezier曲线
	for (const auto& bezier : pDoc->m_completedBeziers) {
		if (bezier.controlPoints.size() == 4 && bezier.curvePoints.size() > 0) {
			// 绘制控制多边形
			CPen polyPen(PS_DOT, 1, RGB(0, 0, 255));  // 蓝色虚线
			CPen* pOldPen = pDC->SelectObject(&polyPen);

			pDC->MoveTo(bezier.controlPoints[0]);
			for (int i = 1; i < bezier.controlPoints.size(); i++) {
				pDC->LineTo(bezier.controlPoints[i]);
			}
			pDC->SelectObject(pOldPen);

			// 绘制Bezier曲线
			CPen curvePen(PS_SOLID, 2, RGB(255, 0, 0));  // 红色实线
			pDC->SelectObject(&curvePen);

			pDC->MoveTo(bezier.curvePoints[0]);
			for (int i = 1; i < bezier.curvePoints.size(); i++) {
				pDC->LineTo(bezier.curvePoints[i]);
			}
			pDC->SelectObject(pOldPen);

			// 标记控制点（黑色实心小矩形，边长5）
			CBrush blackBrush(RGB(0, 0, 0));
			CBrush* pOldBrush = pDC->SelectObject(&blackBrush);
			for (const auto& controlPoint : bezier.controlPoints) {
				CRect rect(controlPoint.x - 2, controlPoint.y - 2,
					controlPoint.x + 3, controlPoint.y + 3);  // 边长5
				pDC->Rectangle(rect);
			}
			pDC->SelectObject(pOldBrush);
		}
	}
	// 2. 绘制当前正在输入的Bezier曲线（控制点和控制多边形）
	if (pDoc->m_bBezierMode && !pDoc->m_bezierControlPoints.empty()) {
		// 绘制当前控制多边形
		CPen polyPen(PS_DOT, 1, RGB(0, 0, 255));  // 蓝色虚线
		CPen* pOldPen = pDC->SelectObject(&polyPen);

		if (pDoc->m_bezierControlPoints.size() > 1) {
			pDC->MoveTo(pDoc->m_bezierControlPoints[0]);
			for (int i = 1; i < pDoc->m_bezierControlPoints.size(); i++) {
				pDC->LineTo(pDoc->m_bezierControlPoints[i]);
			}
		}
		pDC->SelectObject(pOldPen);

		// 标记当前控制点（黑色实心小矩形，边长5）
		CBrush blackBrush(RGB(0, 0, 0));
		CBrush* pOldBrush = pDC->SelectObject(&blackBrush);
		for (const auto& controlPoint : pDoc->m_bezierControlPoints) {
			CRect rect(controlPoint.x - 2, controlPoint.y - 2,
				controlPoint.x + 3, controlPoint.y + 3);  // 边长5
			pDC->Rectangle(rect);
		}
		pDC->SelectObject(pOldBrush);

		// 如果当前有4个控制点，实时显示Bezier曲线
		if (pDoc->m_bezierControlPoints.size() == 4) {
			// 计算并绘制临时Bezier曲线
			std::vector<CPoint> tempCurvePoints;
			for (double t = 0; t <= 1.0; t += 0.01) {
				double x = pow(1 - t, 3) * pDoc->m_bezierControlPoints[0].x +
					3 * t * pow(1 - t, 2) * pDoc->m_bezierControlPoints[1].x +
					3 * pow(t, 2) * (1 - t) * pDoc->m_bezierControlPoints[2].x +
					pow(t, 3) * pDoc->m_bezierControlPoints[3].x;

				double y = pow(1 - t, 3) * pDoc->m_bezierControlPoints[0].y +
					3 * t * pow(1 - t, 2) * pDoc->m_bezierControlPoints[1].y +
					3 * pow(t, 2) * (1 - t) * pDoc->m_bezierControlPoints[2].y +
					pow(t, 3) * pDoc->m_bezierControlPoints[3].y;

				tempCurvePoints.push_back(CPoint((int)x, (int)y));
			}

			// 绘制临时Bezier曲线
			CPen tempCurvePen(PS_SOLID, 2, RGB(255, 0, 0));  // 红色实线
			pDC->SelectObject(&tempCurvePen);

			if (!tempCurvePoints.empty()) {
				pDC->MoveTo(tempCurvePoints[0]);
				for (int i = 1; i < tempCurvePoints.size(); i++) {
					pDC->LineTo(tempCurvePoints[i]);
				}
			}
			pDC->SelectObject(pOldPen);
		}
	}
}


// CCGWORK0602View 打印

void CCGWORK0602View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CCGWORK0602View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CCGWORK0602View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CCGWORK0602View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CCGWORK0602View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCGWORK0602View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCGWORK0602View 诊断

#ifdef _DEBUG
void CCGWORK0602View::AssertValid() const
{
	CView::AssertValid();
}

void CCGWORK0602View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCGWORK0602Doc* CCGWORK0602View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCGWORK0602Doc)));
	return (CCGWORK0602Doc*)m_pDocument;
}
#endif //_DEBUG


// CCGWORK0602View 消息处理程序

// === 新增的菜单命令处理函数 ===
void CCGWORK0602View::OnDrawRect()
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// 切换到矩形绘制模式
	pDoc->SwitchToDrawMode();
	// 可以在这里添加状态提示
	if (pDoc->m_bDrawMode)
	{
		AfxMessageBox(_T("已进入矩形绘制模式，请使用鼠标在画布上绘制矩形"));
	}
	else
	{
		AfxMessageBox(_T("已退出矩形绘制模式"));
	}
}

// === 新增的鼠标按下事件处理函数 ===
void CCGWORK0602View::OnLButtonDown(UINT nFlags, CPoint point)
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// 只有在绘制模式下才响应鼠标事件
	if (pDoc->m_bDrawMode)
	{
		pDoc->StartDrawing(point);
	}
	// 在圆形绘制模式下
	else if (pDoc->m_bDrawCircleMode)
	{
		pDoc->StartDrawing(point);
	}
	//只有在区域填充下才能生效的代码
	else if (pDoc->m_bDrawPolygonMode)
	{
		if (!pDoc->m_bPolygonDrawing)
		{
			pDoc->StartPolygonDrawing(point);
		}
		else
		{
			pDoc->AddPolygonPoint(point);
		}
	}

	CView::OnLButtonDown(nFlags, point);
}

// === 新增的鼠标移动事件处理函数 ===
void CCGWORK0602View::OnMouseMove(UINT nFlags, CPoint point)
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// 只有在绘制模式下且左键按下时才更新绘制
	if ((pDoc->m_bDrawMode || pDoc->m_bDrawCircleMode) && (nFlags & MK_LBUTTON) && pDoc->m_bDrawing)
	{
		pDoc->UpdateDrawing(point);
	}

	CView::OnMouseMove(nFlags, point);
}

// === 新增的鼠标释放事件处理函数 ===
void CCGWORK0602View::OnLButtonUp(UINT nFlags, CPoint point)
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// 只有在绘制模式下才结束绘制
	if ((pDoc->m_bDrawMode || pDoc->m_bDrawCircleMode) && pDoc->m_bDrawing)
	{
		pDoc->EndDrawing(point);
	}
	// Bezier曲线绘制模式
	else if (pDoc->m_bBezierMode)
	{
		pDoc->AddBezierControlPoint(point);
	}

	CView::OnLButtonUp(nFlags, point);
}

// 圆形绘制菜单处理函数
void CCGWORK0602View::OnDrawCircle()
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// 切换到圆形绘制模式
	pDoc->SwitchToCircleMode();

	// 提示信息
	AfxMessageBox(_T("已进入圆形绘制模式，请使用鼠标在画布上绘制圆形"));
}

// 新增：圆形绘制菜单更新处理函数
void CCGWORK0602View::OnUpdateDrawCircle(CCmdUI* pCmdUI)
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// 设置菜单项状态
	pCmdUI->SetCheck(pDoc->IsInCircleMode());
	pCmdUI->Enable(TRUE);  // 总是启用
}

// === 右键按下处理函数（用于结束多边形绘制）===
void CCGWORK0602View::OnRButtonDown(UINT nFlags, CPoint point)
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// 在多边形绘制模式下，右键结束绘制
	if (pDoc->m_bDrawPolygonMode && pDoc->m_bPolygonDrawing)
	{
		// 检查顶点数是否足够
		if (pDoc->m_polygonPoints.size() >= 3)
		{
			pDoc->EndPolygonDrawing();
			AfxMessageBox(_T("多边形绘制完成，已使用边标志算法填充"));
		}
		else
		{
			// 这里不需要额外提示，因为EndPolygonDrawing中已经包含提示
			pDoc->EndPolygonDrawing(); // 仍然调用，让文档类处理提示
		}
	}

	CView::OnRButtonDown(nFlags, point);
}

// === 多边形绘制菜单处理函数 ===
void CCGWORK0602View::OnDrawPolygon()
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// 切换到多边形绘制模式
	pDoc->SwitchToPolygonMode();

	// 提示信息
	AfxMessageBox(_T("已进入多边形绘制模式\n\n使用说明：\n1. 左键点击添加顶点\n2. 右键点击结束绘制\n3. 多边形将自动使用边标志算法填充\"0602\"文本"));
}

// === 菜单项更新处理函数 ===


// 在文件末尾添加新的消息处理函数
// === 新增的区域填充菜单处理函数 ===
void CCGWORK0602View::OnAreaFill()
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// 切换到区域填充模式
	pDoc->SwitchToAreaFillMode();

	// 提示信息
	AfxMessageBox(_T("已进入区域填充模式，所有绘制的矩形已被清空"));
}



void CCGWORK0602View::OnUpdateDrawPolygon(CCmdUI* pCmdUI)
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// 设置菜单项状态
	pCmdUI->SetCheck(pDoc->IsInPolygonMode());
	pCmdUI->Enable(TRUE);//总是启用
}

// 修改原有的菜单更新函数，确保模式互斥
void CCGWORK0602View::OnUpdateDrawRect(CCmdUI* pCmdUI)
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pCmdUI->SetCheck(pDoc->IsInDrawMode());
	pCmdUI->Enable(TRUE);
}

void CCGWORK0602View::OnUpdateAreaFill(CCmdUI* pCmdUI)
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// 设置菜单项状态：如果处于区域填充模式，则显示为选中状态
	pCmdUI->SetCheck(pDoc->IsInAreaFillMode());
	pCmdUI->Enable(TRUE);
}

void CCGWORK0602View::OnDrawSetcolor()
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	


	CMyColorDialog dlg;  // 改为 CMyColorDialog
	dlg.SetColor(pDoc->m_rectColor); // 使用当前矩形颜色作为初始值

	if (dlg.DoModal() == IDOK)
	{
		COLORREF newColor = dlg.GetColor();
		pDoc->m_rectColor = newColor;
		pDoc->m_circleColor = newColor; // 同时设置矩形和圆形颜色
		pDoc->UpdateAllViews(NULL); // 更新视图显示新颜色
		pDoc->SetModifiedFlag(TRUE); // 标记文档已修改

		CString msg;
		msg.Format(_T("颜色设置成功！\nR: %d, G: %d, B: %d"),
			GetRValue(newColor), GetGValue(newColor), GetBValue(newColor));
		AfxMessageBox(msg);
	}
}

void CCGWORK0602View::OnUpdateDrawSetcolor(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

// 区域填充颜色设置处理函数
void CCGWORK0602View::OnAreaFillSetcolor()
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// 创建一个新的对话框类来处理两种颜色的设置
	// 这里我们先使用现有的CMyColorDialog，稍后需要创建新的对话框
	CMyColorDialog dlg;
	dlg.SetColor(pDoc->m_polygonFillColor); // 使用当前填充颜色作为初始值

	if (dlg.DoModal() == IDOK)
	{
		COLORREF newColor = dlg.GetColor();
		pDoc->m_polygonFillColor = newColor;
		pDoc->m_polygonBorderColor = newColor; // 暂时设置为相同颜色
		pDoc->UpdateAllViews(NULL); // 更新视图显示新颜色
		pDoc->SetModifiedFlag(TRUE); // 标记文档已修改

		CString msg;
		msg.Format(_T("多边形颜色设置成功！\n填充颜色: R: %d, G: %d, B: %d\n边界颜色: R: %d, G: %d, B: %d"),
			GetRValue(newColor), GetGValue(newColor), GetBValue(newColor),
			GetRValue(newColor), GetGValue(newColor), GetBValue(newColor));
		AfxMessageBox(msg);
	}
}

void CCGWORK0602View::OnUpdateAreaFillSetcolor(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

// 设置多边形边框颜色
void CCGWORK0602View::OnAreaFillSetBorderColor()
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CMyColorDialog dlg;
	dlg.SetColor(pDoc->m_polygonBorderColor); // 使用当前边框颜色作为初始值

	if (dlg.DoModal() == IDOK)
	{
		COLORREF newColor = dlg.GetColor();
		pDoc->m_polygonBorderColor = newColor;
		pDoc->UpdateAllViews(NULL); // 更新视图显示新颜色
		pDoc->SetModifiedFlag(TRUE); // 标记文档已修改

		CString msg;
		msg.Format(_T("多边形边框颜色设置成功！\nR: %d, G: %d, B: %d"),
			GetRValue(newColor), GetGValue(newColor), GetBValue(newColor));
		AfxMessageBox(msg);
	}
}

// 设置多边形填充颜色
void CCGWORK0602View::OnAreaFillSetFillColor()
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CMyColorDialog dlg;
	dlg.SetColor(pDoc->m_polygonFillColor); // 使用当前填充颜色作为初始值

	if (dlg.DoModal() == IDOK)
	{
		COLORREF newColor = dlg.GetColor();
		pDoc->m_polygonFillColor = newColor;
		pDoc->UpdateAllViews(NULL); // 更新视图显示新颜色
		pDoc->SetModifiedFlag(TRUE); // 标记文档已修改

		CString msg;
		msg.Format(_T("多边形填充颜色设置成功！\nR: %d, G: %d, B: %d"),
			GetRValue(newColor), GetGValue(newColor), GetBValue(newColor));
		AfxMessageBox(msg);
	}
}

// 菜单更新处理函数
void CCGWORK0602View::OnUpdateAreaFillSetBorderColor(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CCGWORK0602View::OnUpdateAreaFillSetFillColor(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

// === 新增：绘制3D立方体函数 ===
void CCGWORK0602View::DrawCube3D(CDC* pDC, CCGWORK0602Doc* pDoc)
{
	// 保存原来的画刷和画笔
	CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
	CPen* pOldPen = (CPen*)pDC->SelectStockObject(BLACK_PEN);

	// 计算每个面的中心深度用于深度排序（简单的画家算法）
	std::vector<std::pair<double, int>> faceDepths; // <深度, 面索引>

	for (int i = 0; i < pDoc->m_cube.faces.size(); i++)
	{
		const auto& face = pDoc->m_cube.faces[i];

		// 计算面的平均深度
		double avgZ = 0;
		avgZ += pDoc->m_cube.transformedVertices[face.v1].z;
		avgZ += pDoc->m_cube.transformedVertices[face.v2].z;
		avgZ += pDoc->m_cube.transformedVertices[face.v3].z;
		avgZ += pDoc->m_cube.transformedVertices[face.v4].z;
		avgZ /= 4.0;

		faceDepths.push_back(std::make_pair(avgZ, i));
	}

	// 按深度从远到近排序（深度值小的先绘制，大的后绘制）
	std::sort(faceDepths.begin(), faceDepths.end());

	// 从远到近绘制每个面
	for (const auto& depthFace : faceDepths)
	{
		int faceIndex = depthFace.second;
		const auto& face = pDoc->m_cube.faces[faceIndex];

		// 投影4个顶点到2D
		CPoint points[4];
		points[0] = pDoc->PerspectiveProjection(pDoc->m_cube.transformedVertices[face.v1]);
		points[1] = pDoc->PerspectiveProjection(pDoc->m_cube.transformedVertices[face.v2]);
		points[2] = pDoc->PerspectiveProjection(pDoc->m_cube.transformedVertices[face.v3]);
		points[3] = pDoc->PerspectiveProjection(pDoc->m_cube.transformedVertices[face.v4]);

		// 创建面颜色的画刷
		CBrush faceBrush(face.color);
		CPen facePen(PS_SOLID, 1, RGB(0, 0, 0)); // 黑色边框

		pDC->SelectObject(&faceBrush);
		pDC->SelectObject(&facePen);

		// 绘制填充的多边形
		pDC->Polygon(points, 4);

		// 恢复透明画刷和黑笔（为下一个面准备）
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->SelectStockObject(BLACK_PEN);
	}

	// 恢复原来的GDI对象
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}

// === 新增：三维变换菜单处理函数 ===
void CCGWORK0602View::On3DTransformDrawCube()
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// 切换到三维模式并绘制立方体
	pDoc->SwitchTo3DMode();
	pDoc->DrawCube();

	AfxMessageBox(_T("已进入三维立方体绘制模式\n立方体已重置到初始位置"));
}

// === 三维变换菜单更新处理函数 ===
void CCGWORK0602View::OnUpdate3DTransformDrawCube(CCmdUI* pCmdUI)
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pCmdUI->SetCheck(pDoc->IsInDrawCubeMode());
	pCmdUI->Enable(TRUE);
}

// === 绕x轴旋转菜单处理函数 ===
void CCGWORK0602View::On3DRotateX()
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// 切换到绕x轴旋转模式
	pDoc->SwitchToRotateXMode();

	CString msg;
	msg.Format(_T("已进入绕X轴旋转模式\n\n使用说明：\n按 'A' 键：绕X轴正向旋转%.1f度\n按 'L' 键：绕X轴反向旋转%.1f度"),
		pDoc->m_dRotationAngle, pDoc->m_dRotationAngle);
	AfxMessageBox(msg);
}

// === 绕x轴旋转菜单更新处理函数 ===
void CCGWORK0602View::OnUpdate3DRotateX(CCmdUI* pCmdUI)
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pCmdUI->SetCheck(pDoc->IsInRotateXMode());
	pCmdUI->Enable(pDoc->IsIn3DMode());  // 只有在三维模式下才启用
}

// === 绕y轴旋转菜单处理函数 ===
void CCGWORK0602View::On3DRotateY()
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// 切换到绕y轴旋转模式
	pDoc->SwitchToRotateYMode();

	CString msg;
	msg.Format(_T("已进入绕Y轴旋转模式\n\n使用说明：\n按 'A' 键：绕Y轴正向旋转%.1f度\n按 'L' 键：绕Y轴反向旋转%.1f度"),
		pDoc->m_dRotationAngle, pDoc->m_dRotationAngle);
	AfxMessageBox(msg);
}

// === 绕y轴旋转菜单更新处理函数 ===
void CCGWORK0602View::OnUpdate3DRotateY(CCmdUI* pCmdUI)
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pCmdUI->SetCheck(pDoc->IsInRotateYMode());
	pCmdUI->Enable(pDoc->IsIn3DMode());  // 只有在三维模式下才启用
}

// === 绕z轴旋转菜单处理函数 ===
void CCGWORK0602View::On3DRotateZ()
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// 切换到绕z轴旋转模式
	pDoc->SwitchToRotateZMode();

	CString msg;
	msg.Format(_T("已进入绕Z轴旋转模式\n\n使用说明：\n按 'A' 键：绕Z轴正向旋转%.1f度\n按 'L' 键：绕Z轴反向旋转%.1f度"),
		pDoc->m_dRotationAngle, pDoc->m_dRotationAngle);
	AfxMessageBox(msg);
}

// === 绕z轴旋转菜单更新处理函数 ===
void CCGWORK0602View::OnUpdate3DRotateZ(CCmdUI* pCmdUI)
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pCmdUI->SetCheck(pDoc->IsInRotateZMode());
	pCmdUI->Enable(pDoc->IsIn3DMode());  // 只有在三维模式下才启用
}

// === 键盘按下事件处理函数 ===
void CCGWORK0602View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// 只有在绕x轴旋转模式下才处理键盘事件
	if (pDoc->IsInRotateXMode())
	{
		switch (nChar)
		{
		case 'A':  // 绕X轴正向
		case 'a':
			pDoc->RotateCubeX(pDoc->m_dRotationAngle);  // 正向旋转10度
			break;

		case 'L':  // 绕X轴反向
		case 'l':
			pDoc->RotateCubeX(-pDoc->m_dRotationAngle); // 反向旋转10度
			break;
		}
	}
	// 在绕y轴旋转模式下处理键盘事件
	else if (pDoc->IsInRotateYMode())
	{
		switch (nChar)
		{
		case 'A':  // 绕Y轴正向旋转
		case 'a':
			pDoc->RotateCubeY(pDoc->m_dRotationAngle);  // 使用设置的旋转角度
			break;

		case 'L':  // 绕Y轴反向旋转
		case 'l':
			pDoc->RotateCubeY(-pDoc->m_dRotationAngle); // 使用设置的旋转角度
			break;
		}
	}
	// 在绕z轴旋转模式下处理键盘事件
	else if (pDoc->IsInRotateZMode())
	{
		switch (nChar)
		{
		case 'A':  // 绕Z轴正向旋转
		case 'a':
			pDoc->RotateCubeZ(pDoc->m_dRotationAngle);  // 使用设置的旋转角度
			break;

		case 'L':  // 绕Z轴反向旋转
		case 'l':
			pDoc->RotateCubeZ(-pDoc->m_dRotationAngle); // 使用设置的旋转角度
			break;
		}
	}
	// 在延x轴平移模式下处理键盘事件
	else if (pDoc->IsInTranslateXMode())
	{
		switch (nChar)
		{
		case 'A':  // 延X轴正方向平移0.2单位
		case 'a':
			pDoc->TranslateCubeX(pDoc->m_dTranslationStep);  // 正方向平移
			break;

		case 'L':  // 延X轴负方向平移0.2单位
		case 'l':
			pDoc->TranslateCubeX(-pDoc->m_dTranslationStep); // 负方向平移
			break;
		}
	}
	// 在延y轴平移模式下处理键盘事件
	else if (pDoc->IsInTranslateYMode())
	{
		switch (nChar)
		{
		case 'A':  // 延Y轴正方向平移
		case 'a':
			pDoc->TranslateCubeY(pDoc->m_dTranslationStep);  // 使用设置的平移步长
			break;

		case 'L':  // 延Y轴负方向平移
		case 'l':
			pDoc->TranslateCubeY(-pDoc->m_dTranslationStep); // 使用设置的平移步长
			break;
		}
	}
	// 在延z轴平移模式下处理键盘事件
	else if (pDoc->IsInTranslateZMode())
	{
		switch (nChar)
		{
		case 'A':  // 延Z轴正方向平移
		case 'a':
			pDoc->TranslateCubeZ(pDoc->m_dTranslationStep);  // 使用设置的平移步长
			break;

		case 'L':  // 延Z轴负方向平移
		case 'l':
			pDoc->TranslateCubeZ(-pDoc->m_dTranslationStep); // 使用设置的平移步长
			break;
		}
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

// === 新增：延x轴平移菜单处理函数 ===
void CCGWORK0602View::On3DTranslateX()
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// 切换到延x轴平移模式
	pDoc->SwitchToTranslateXMode();

	CString msg;
	msg.Format(_T("已进入延X轴平移模式\n\n使用说明：\n按 'A' 键：延X轴正方向平移%.1f单位\n按 'L' 键：延X轴负方向平移%.1f单位"),
		pDoc->m_dTranslationStep, pDoc->m_dTranslationStep);
	AfxMessageBox(msg);
}

// === 延x轴平移菜单更新处理函数 ===
void CCGWORK0602View::OnUpdate3DTranslateX(CCmdUI* pCmdUI)
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pCmdUI->SetCheck(pDoc->IsInTranslateXMode());
	pCmdUI->Enable(pDoc->IsIn3DMode());  // 只有在三维模式下才启用
}

// === 延y轴平移菜单处理函数 ===
void CCGWORK0602View::On3DTranslateY()
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// 切换到延y轴平移模式
	pDoc->SwitchToTranslateYMode();

	CString msg;
	msg.Format(_T("已进入延Y轴平移模式\n\n使用说明：\n按 'A' 键：延Y轴正方向平移%.1f单位\n按 'L' 键：延Y轴负方向平移%.1f单位"),
		pDoc->m_dTranslationStep, pDoc->m_dTranslationStep);
	AfxMessageBox(msg);
}

// === 延y轴平移菜单更新处理函数 ===
void CCGWORK0602View::OnUpdate3DTranslateY(CCmdUI* pCmdUI)
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pCmdUI->SetCheck(pDoc->IsInTranslateYMode());
	pCmdUI->Enable(pDoc->IsIn3DMode());  // 只有在三维模式下才启用
}

// === 延z轴平移菜单处理函数 ===
void CCGWORK0602View::On3DTranslateZ()
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// 切换到延z轴平移模式
	pDoc->SwitchToTranslateZMode();

	CString msg;
	msg.Format(_T("已进入延Z轴平移模式\n\n使用说明：\n按 'A' 键：延Z轴正方向平移%.1f单位\n按 'L' 键：延Z轴负方向平移%.1f单位"),
		pDoc->m_dTranslationStep, pDoc->m_dTranslationStep);
	AfxMessageBox(msg);
}

// === 延z轴平移菜单更新处理函数 ===
void CCGWORK0602View::OnUpdate3DTranslateZ(CCmdUI* pCmdUI)
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pCmdUI->SetCheck(pDoc->IsInTranslateZMode());
	pCmdUI->Enable(pDoc->IsIn3DMode());  // 只有在三维模式下才启用
}

// === 设置平移步长菜单处理函数 ===
void CCGWORK0602View::OnSettingsTranslationStep()
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CSettingsDialog dlg;
	dlg.SetTitle(_T("设置平移步长"));
	dlg.SetPrompt(_T("请输入平移步长（默认值：1.0）："));
	dlg.SetValue(pDoc->m_dTranslationStep);

	if (dlg.DoModal() == IDOK)
	{
		double newStep = dlg.GetValue();
		if (newStep > 0)
		{
			pDoc->SetTranslationStep(newStep);
			CString msg;
			msg.Format(_T("平移步长已设置为：%.2f"), newStep);
			AfxMessageBox(msg);
		}
		else
		{
			AfxMessageBox(_T("平移步长必须大于0！"));
		}
	}
}

// === 设置旋转角度菜单处理函数 ===
void CCGWORK0602View::OnSettingsRotationAngle()
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CSettingsDialog dlg;
	dlg.SetTitle(_T("设置旋转角度"));
	dlg.SetPrompt(_T("请输入旋转角度（默认值：10.0）："));
	dlg.SetValue(pDoc->m_dRotationAngle);

	if (dlg.DoModal() == IDOK)
	{
		double newAngle = dlg.GetValue();
		if (newAngle > 0 && newAngle <= 360)
		{
			pDoc->SetRotationAngle(newAngle);
			CString msg;
			msg.Format(_T("旋转角度已设置为：%.2f 度"), newAngle);
			AfxMessageBox(msg);
		}
		else
		{
			AfxMessageBox(_T("旋转角度必须在 0-360 度之间！"));
		}
	}
}

// === 设置平移步长菜单更新处理函数 ===
void CCGWORK0602View::OnUpdateSettingsTranslationStep(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);  // 总是启用
}

// === 设置旋转角度菜单更新处理函数 ===
void CCGWORK0602View::OnUpdateSettingsRotationAngle(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);  // 总是启用
}

// === Bezier曲线菜单处理函数 ===
void CCGWORK0602View::OnDrawCurveBezier()
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// 切换到Bezier曲线绘制模式
	pDoc->SwitchToBezierMode();

	// 提示信息
	AfxMessageBox(_T("已进入Bezier曲线绘制模式\n\n使用说明：\n1. 左键点击顺序输入4个控制点\n2. 自动绘制3次Bezier曲线和控制多边形\n3. 控制点用黑色实心小矩形标记\n4. 画完一条曲线后可继续画下一条，之前曲线保留"));
}

// === 新增：Bezier曲线菜单更新处理函数 ===
void CCGWORK0602View::OnUpdateDrawCurveBezier(CCmdUI* pCmdUI)
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pCmdUI->SetCheck(pDoc->IsInBezierMode());
	pCmdUI->Enable(TRUE);
}

// === 新增：清屏功能处理函数 ===
void CCGWORK0602View::OnEditClearAll()
{
	CCGWORK0602Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// 确认对话框，避免误操作
	if (AfxMessageBox(_T("确定要清除所有图形吗？此操作不可撤销！"),
		MB_YESNO | MB_ICONQUESTION) == IDYES) {
		pDoc->ClearAll();
	}
}

// === 新清屏功能菜单更新处理函数 ===
void CCGWORK0602View::OnUpdateEditClearAll(CCmdUI* pCmdUI)
{
	// 总是启用清屏功能
	pCmdUI->Enable(TRUE);
}