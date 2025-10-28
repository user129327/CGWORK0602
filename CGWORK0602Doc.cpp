// CGWORK0602Doc.cpp: CCGWORK0602Doc 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CGWORK0602.h"
#endif

#include "CGWORK0602Doc.h"

#include <propkey.h>
#include <algorithm>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCGWORK0602Doc

IMPLEMENT_DYNCREATE(CCGWORK0602Doc, CDocument)

BEGIN_MESSAGE_MAP(CCGWORK0602Doc, CDocument)
END_MESSAGE_MAP()


// CCGWORK0602Doc 构造/析构

CCGWORK0602Doc::CCGWORK0602Doc() noexcept
{
	// TODO: 在此添加一次性构造代码
	// 添加矩形绘制相关的初始化
	m_bDrawing = FALSE;
	m_bDrawMode = FALSE;  // 默认不处于绘制模式
	//m_rect.SetRectEmpty();
	m_bAreaFillMode = FALSE;  // 默认不在区域填充模式
	m_tempRect.SetRectEmpty();

	//=== 三维变换初始化 ===
	m_b3DMode = FALSE;
	m_bDrawCubeMode = FALSE;
	m_bRotateXMode = FALSE; 
	m_bRotateYMode = FALSE;
	m_bRotateZMode = FALSE;
	m_bTranslateXMode = FALSE;  
	m_bTranslateYMode = FALSE;  
	m_bTranslateZMode = FALSE;  

	// 参数初始化
	m_dTranslationStep = 1.0;    // 默认平移步长为1
	m_dRotationAngle = 10.0;     // 默认旋转角度为10度

	// === 圆形绘制初始化 ===
	m_bDrawCircleMode = FALSE;
	m_tempCircle.SetRectEmpty();

	// === 多边形绘制初始化 ===
	m_bDrawPolygonMode = FALSE;
	m_bPolygonDrawing = FALSE;
	m_polygonBoundingRect.SetRectEmpty();

	// 初始化颜色
	m_rectColor = RGB(0, 0, 0);   // 默认黑色
	m_circleColor = RGB(0, 0, 0); // 默认黑色

	m_polygonFillColor = RGB(0, 0, 0);     // 默认黑色
	m_polygonBorderColor = RGB(0, 0, 0);   // 默认绿色

	// 初始化立方体数据
	InitCube();

	// === Bezier曲线初始化 ===
	m_bBezierMode = FALSE;
	m_bBezierDrawing = FALSE;
}

CCGWORK0602Doc::~CCGWORK0602Doc()
{
}

BOOL CCGWORK0602Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	// 添加：清除矩形数据
	m_bDrawing = FALSE;
	//m_rect.SetRectEmpty();
	m_tempRect.SetRectEmpty();
	m_tempCircle.SetRectEmpty();  // 新增：清空临时圆形
	ClearPolygon();  // 新增：清空多边形

	// 新增：重置立方体
	ResetCube();

	// 新增：清空Bezier曲线
	ClearBezierPoints();
	m_completedBeziers.clear();

	return TRUE;
}






// CCGWORK0602Doc 序列化
void CCGWORK0602Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
		ar << m_bDrawing;
		// 保存矩形数组
		int count = m_rectArray.GetSize();
		ar << count;
		for (int i = 0; i < count; i++)
		{
			ar << m_rectArray[i];
		}
		// === 保存圆形数组 ===
		int circleCount = m_circleArray.GetSize();
		ar << circleCount;
		for (int i = 0; i < circleCount; i++)
		{
			ar << m_circleArray[i];
		}
		ar << m_rectColor;
		ar << m_circleColor;
		ar << m_polygonFillColor;      
		ar << m_polygonBorderColor;    
		ar << m_dTranslationStep;
		ar << m_dRotationAngle;

		// 存储Bezier曲线数据
		int bezierCount = (int)m_completedBeziers.size();
		ar << bezierCount;
		for (const auto& bezier : m_completedBeziers) {
			int pointCount = (int)bezier.controlPoints.size();
			ar << pointCount;
			for (const auto& point : bezier.controlPoints) {
				ar << point;
			}
		}
	}
	else
	{
		// TODO: 在此添加加载代码
		ar >> m_bDrawing;
		// 加载矩形数组
		int count;
		ar >> count;
		m_rectArray.SetSize(count);
		for (int i = 0; i < count; i++)
		{
			ar >> m_rectArray[i];
		}
		// === 加载圆形数组 ===
		int circleCount;
		ar >> circleCount;
		m_circleArray.SetSize(circleCount);
		for (int i = 0; i < circleCount; i++)
		{
			ar >> m_circleArray[i];
		}
		// 加载颜色
		ar >> m_rectColor;
		ar >> m_circleColor;
		ar >> m_polygonFillColor;
		ar >> m_polygonBorderColor;
		ar >> m_dTranslationStep;
		ar >> m_dRotationAngle;

		// 加载Bezier曲线数据
		int bezierCount;
		ar >> bezierCount;
		m_completedBeziers.clear();
		for (int i = 0; i < bezierCount; i++) {
			BezierCurve bezier;
			int pointCount;
			ar >> pointCount;
			for (int j = 0; j < pointCount; j++) {
				CPoint point;
				ar >> point;
				bezier.controlPoints.push_back(point);
			}
			// 重新计算曲线点
			if (bezier.controlPoints.size() == 4) {
				m_bezierControlPoints = bezier.controlPoints;
				CalculateBezierCurve();
				bezier.curvePoints = m_bezierControlPoints; // 这里应该是 curvePoints，稍后修正
				m_completedBeziers.push_back(bezier);
			}
		}
		ClearBezierPoints();
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CCGWORK0602Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CCGWORK0602Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由";"分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CCGWORK0602Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl* pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CCGWORK0602Doc 诊断

#ifdef _DEBUG
void CCGWORK0602Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCGWORK0602Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


void CCGWORK0602Doc::StartDrawing(CPoint point)
{
	m_bDrawing = TRUE;
	m_ptStart = m_ptEnd = point;
	//m_rect.SetRectEmpty();
	m_tempRect.SetRectEmpty();
	m_tempCircle.SetRectEmpty();
}

void CCGWORK0602Doc::UpdateDrawing(CPoint point)
{
	if (m_bDrawing)
	{
		m_ptEnd = point;
		if (m_bDrawMode)  // 矩形绘制模式
		{
			m_tempRect.SetRect(m_ptStart, m_ptEnd);
			m_tempRect.NormalizeRect();
		}
		else if (m_bDrawCircleMode)  // 圆形绘制模式
		{
			m_tempCircle.SetRect(m_ptStart, m_ptEnd);
			m_tempCircle.NormalizeRect();
		}
		UpdateAllViews(NULL); // 更新所有视图
	}
}

void CCGWORK0602Doc::EndDrawing(CPoint point)
{
	if (m_bDrawing)
	{
		m_ptEnd = point;
		if (m_bDrawMode)  // 矩形绘制模式
		{
			// 将完成的矩形添加到数组中
			if (!m_tempRect.IsRectEmpty())
			{
				m_rectArray.Add(m_tempRect);
			}
		}
		else if (m_bDrawCircleMode)  // 圆形绘制模式
		{
			// 将完成的圆形添加到数组中
			if (!m_tempCircle.IsRectEmpty())
			{
				m_circleArray.Add(m_tempCircle);
			}
		}

		m_bDrawing = FALSE;
		m_tempRect.SetRectEmpty();
		m_tempCircle.SetRectEmpty();
		UpdateAllViews(NULL);

		// 标记文档为已修改
		SetModifiedFlag(TRUE);
	}
}

// 切换到圆形绘制模式
void CCGWORK0602Doc::SwitchToCircleMode()
{
	// 清空其他模式
	m_bDrawMode = FALSE;
	m_bAreaFillMode = FALSE;
	m_bDrawPolygonMode = FALSE;
	m_bDrawCircleMode = TRUE;

	// 清空临时图形
	m_tempRect.SetRectEmpty();
	m_tempCircle.SetRectEmpty();
	m_bDrawing = FALSE;

	UpdateAllViews(NULL);
}

// 修改设置绘制模式函数
void CCGWORK0602Doc::SetDrawMode(BOOL bMode)
{
	m_bDrawMode = bMode;
	m_bAreaFillMode = FALSE;
	m_bDrawCircleMode = FALSE;
	m_bDrawPolygonMode = FALSE;
	if (!bMode)  // 如果退出绘制模式，清空临时矩形
	{
		m_tempRect.SetRectEmpty();
		m_tempCircle.SetRectEmpty();
		m_bDrawing = FALSE;
	}
	UpdateAllViews(NULL);
}




// 在文件末尾添加新函数
void CCGWORK0602Doc::SwitchToAreaFillMode()
{
	// 切换到区域填充模式时清空所有绘制的矩形
	m_bAreaFillMode = TRUE;
	m_bDrawMode = FALSE;
	m_bDrawPolygonMode = FALSE;
	m_bDrawCircleMode = FALSE;

	// 可以在这里添加区域填充模式的初始化代码
	UpdateAllViews(NULL);
}

void CCGWORK0602Doc::SwitchToDrawMode()
{
	m_bDrawMode = TRUE;
	m_bAreaFillMode = FALSE;
	m_bDrawPolygonMode = FALSE;
	// 注意：这里不自动进入绘制模式，需要用户手动点击"绘制矩形"
	UpdateAllViews(NULL);
}


void CCGWORK0602Doc::StartPolygonDrawing(CPoint point)
{
	m_bPolygonDrawing = TRUE;
	m_bDrawPolygonMode = TRUE;
	m_bDrawMode = FALSE;
	m_bAreaFillMode = FALSE;
	m_polygonPoints.clear();
	m_polygonPoints.push_back(point);
	m_polygonBoundingRect.SetRect(point.x, point.y, point.x, point.y);
}

void CCGWORK0602Doc::AddPolygonPoint(CPoint point)
{
	if (m_bPolygonDrawing)
	{
		m_polygonPoints.push_back(point);
		// 更新边界矩形
		m_polygonBoundingRect.left = min(m_polygonBoundingRect.left, point.x);
		m_polygonBoundingRect.top = min(m_polygonBoundingRect.top, point.y);
		m_polygonBoundingRect.right = max(m_polygonBoundingRect.right, point.x);
		m_polygonBoundingRect.bottom = max(m_polygonBoundingRect.bottom, point.y);

		UpdateAllViews(NULL);
	}
}

void CCGWORK0602Doc::EndPolygonDrawing()
{
	if (m_bPolygonDrawing)
	{
		if (m_polygonPoints.size() >= 3)
		{
			CompleteCurrentPolygon(); // 完成当前多边形并添加到已完成列表
			m_bPolygonDrawing = FALSE;
			UpdateAllViews(NULL);
			SetModifiedFlag(TRUE);
		}
		else
		{
			// 如果顶点数不足3个，提示用户
			AfxMessageBox(_T("多边形至少需要3个顶点才能完成绘制！\n请继续添加顶点或重新开始。"));
			// 不清空当前点，让用户可以继续添加顶点
		}
	}
}

void CCGWORK0602Doc::ClearPolygon()
{
	m_polygonPoints.clear();
	m_bPolygonDrawing = FALSE;
	m_polygonBoundingRect.SetRectEmpty();
	UpdateAllViews(NULL);
}


void CCGWORK0602Doc::SwitchToPolygonMode()
{
	// 清空其他模式
	m_bDrawMode = FALSE;
	m_bAreaFillMode = FALSE;
	m_bDrawPolygonMode = TRUE;
	m_bDrawCircleMode = FALSE;

	// 清空之前的多边形
	//ClearPolygon();
	m_bPolygonDrawing = FALSE;
	m_polygonPoints.clear();
	m_polygonBoundingRect.SetRectEmpty();


	UpdateAllViews(NULL);
}


// 边标志填充算法实现



// === 新增：完成当前多边形并添加到已完成列表 ===
void CCGWORK0602Doc::CompleteCurrentPolygon()
{
	if (m_polygonPoints.size() >= 3)
	{
		// 将当前多边形添加到已完成列表
		m_completedPolygons.push_back(m_polygonPoints);

		// 重置当前多边形状态
		m_polygonPoints.clear();
		m_polygonBoundingRect.SetRectEmpty();
	}
}

// === 新增：清空所有多边形 ===
void CCGWORK0602Doc::ClearAllPolygons()
{
	m_completedPolygons.clear();
	m_polygonPoints.clear();
	m_bPolygonDrawing = FALSE;
	m_polygonBoundingRect.SetRectEmpty();
	UpdateAllViews(NULL);
}

// 使用区域和位图画刷填充多边形（更高效）
void CCGWORK0602Doc::EdgeFlagFillPolygon(CDC* pDC, const std::vector<CPoint>& polygon, CString fillText)
{
	if (polygon.size() < 3) return;

	// 获取文本尺寸
	CSize textSize = pDC->GetTextExtent(fillText);
	int textWidth = textSize.cx;
	int textHeight = textSize.cy;

	if (textWidth <= 0 || textHeight <= 0) return;

	// 创建内存DC和位图
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	// 创建位图 - 大小为文本尺寸
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, textWidth, textHeight);
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	// 在位图上绘制文本
	memDC.FillSolidRect(0, 0, textWidth, textHeight, RGB(255, 255, 255)); // 白色背景
	memDC.SetTextColor(m_polygonFillColor); // 使用填充颜色
	memDC.SetBkMode(TRANSPARENT);
	memDC.TextOut(0, 0, fillText);

	// 创建位图画刷
	CBrush patternBrush;
	patternBrush.CreatePatternBrush(&bitmap);

	// 创建多边形区域
	CRgn polyRgn;
	POINT* points = new POINT[polygon.size()];
	for (size_t i = 0; i < polygon.size(); i++) {
		points[i].x = polygon[i].x;
		points[i].y = polygon[i].y;
	}
	polyRgn.CreatePolygonRgn(points, (int)polygon.size(), ALTERNATE);
	delete[] points;

	// 选择画刷到设备上下文
	CBrush* pOldBrush = pDC->SelectObject(&patternBrush);

	// 填充多边形区域
	pDC->FillRgn(&polyRgn, &patternBrush);

	// 恢复原来的GDI对象
	pDC->SelectObject(pOldBrush);
	memDC.SelectObject(pOldBitmap);

	// 清理资源
	bitmap.DeleteObject();
	patternBrush.DeleteObject();
	polyRgn.DeleteObject();
	memDC.DeleteDC();
}
// 保持原有的 EdgeFlagFill 函数不变
void CCGWORK0602Doc::EdgeFlagFill(CDC* pDC, CString fillText)
{
	EdgeFlagFillPolygon(pDC, m_polygonPoints, fillText);
}

// === 初始化立方体数据 ===
void CCGWORK0602Doc::InitCube()
{
	// 定义立方体的8个顶点（中心在原点，边长为2）
	m_cube.vertices = {
		CPoint3D(-1, -1, -1),  // 0: 左前下
		CPoint3D(1, -1, -1),   // 1: 右前下  
		CPoint3D(1, 1, -1),    // 2: 右前上
		CPoint3D(-1, 1, -1),   // 3: 左前上
		CPoint3D(-1, -1, 1),   // 4: 左后下
		CPoint3D(1, -1, 1),    // 5: 右后下
		CPoint3D(1, 1, 1),     // 6: 右后上
		CPoint3D(-1, 1, 1)     // 7: 左后上
	};

	// 定义6个面（每个面4个顶点索引）
	m_cube.faces = {
		Face(0, 1, 2, 3, RGB(255, 0, 0)),     // 前面 - 红色
		Face(1, 5, 6, 2, RGB(0, 255, 0)),     // 右面 - 绿色
		Face(5, 4, 7, 6, RGB(0, 0, 255)),     // 后面 - 蓝色
		Face(4, 0, 3, 7, RGB(255, 255, 0)),   // 左面 - 黄色
		Face(3, 2, 6, 7, RGB(255, 0, 255)),   // 上面 - 紫色
		Face(4, 5, 1, 0, RGB(0, 255, 255))    // 下面 - 青色
	};

	// 初始化变换后的顶点
	m_cube.transformedVertices = m_cube.vertices;
}

// === 切换到三维模式 ===
void CCGWORK0602Doc::SwitchTo3DMode()
{
	// 清空其他模式
	m_bDrawMode = FALSE;
	m_bAreaFillMode = FALSE;
	m_bDrawPolygonMode = FALSE;
	m_bDrawCircleMode = FALSE;
	m_b3DMode = TRUE;

	UpdateAllViews(NULL);
}

// === 绘制立方体 ===
void CCGWORK0602Doc::DrawCube()
{
	m_bDrawCubeMode = TRUE;
	m_b3DMode = TRUE;

	// 重置立方体到初始位置
	ResetCube();

	UpdateAllViews(NULL);
}

// === 重置立方体到初始位置 ===
void CCGWORK0602Doc::ResetCube()
{
	// 将变换后的顶点重置为原始顶点
	m_cube.transformedVertices = m_cube.vertices;

	UpdateAllViews(NULL);
}

// === 新增：透视投影函数 ===
CPoint CCGWORK0602Doc::PerspectiveProjection(const CPoint3D& point3D)
{
	// 投影中心在 (0, 0, 5)，投影平面在 z=0
	double d = 8.0; // 投影中心到投影平面的距离

	// 透视投影公式: x' = (d * x) / (d + z), y' = (d * y) / (d + z)
	double z = point3D.z + 5; // 将立方体向前移动，使其在投影中心前面

	if (z <= 0) z = 0.1; // 避免除零

	double x_proj = (d * point3D.x) / z;
	double y_proj = (d * point3D.y) / z;

	// 缩放并转换到屏幕坐标（假设视图大小为 600x600，原点在中心）
	int screenX = (int)(x_proj * 120 + 300);
	int screenY = (int)(-y_proj * 120 + 300); // Y轴取反，因为屏幕坐标系Y轴向下

	return CPoint(screenX, screenY);
}

// === 切换到绕x轴旋转模式 ===
void CCGWORK0602Doc::SwitchToRotateXMode()
{
	// 确保处于三维模式
	m_b3DMode = TRUE;
	m_bRotateXMode = TRUE;
	m_bDrawCubeMode = TRUE;  // 确保立方体已绘制

	UpdateAllViews(NULL);
}

// === 绕x轴旋转函数 ===
void CCGWORK0602Doc::RotateCubeX(double angle)
{
	// 将角度转换为弧度
	double rad = angle * 3.14159265358979323846 / 180.0;
	double cosA = cos(rad);
	double sinA = sin(rad);

	// 对每个顶点应用绕x轴旋转矩阵
	for (auto& vertex : m_cube.transformedVertices)
	{
		double y = vertex.y;
		double z = vertex.z;

		// 绕x轴旋转矩阵:
		// [1,   0,    0]
		// [0, cosA, -sinA]
		// [0, sinA,  cosA]
		vertex.y = y * cosA - z * sinA;
		vertex.z = y * sinA + z * cosA;
	}

	UpdateAllViews(NULL);
	SetModifiedFlag(TRUE);
}

// === 切换到绕y轴旋转模式 ===
void CCGWORK0602Doc::SwitchToRotateYMode()
{
	// 确保处于三维模式
	m_b3DMode = TRUE;
	m_bRotateYMode = TRUE;
	m_bDrawCubeMode = TRUE;  // 确保立方体已绘制
	m_bRotateXMode = FALSE;  // 关闭x轴旋转模式
	m_bTranslateXMode = FALSE; // 关闭平移模式

	UpdateAllViews(NULL);
}

// === 绕y轴旋转函数 ===
void CCGWORK0602Doc::RotateCubeY(double angle)
{
	// 将角度转换为弧度
	double rad = angle * 3.14159265358979323846 / 180.0;
	double cosA = cos(rad);
	double sinA = sin(rad);

	// 对每个顶点应用绕y轴旋转矩阵
	for (auto& vertex : m_cube.transformedVertices)
	{
		double x = vertex.x;
		double z = vertex.z;

		// 绕y轴旋转矩阵:
		// [ cosA,  0, sinA]
		// [   0,   1,   0]
		// [-sinA,  0, cosA]
		vertex.x = x * cosA + z * sinA;
		vertex.z = -x * sinA + z * cosA;
	}

	UpdateAllViews(NULL);
	SetModifiedFlag(TRUE);
}

// === 新增：切换到绕z轴旋转模式 ===
void CCGWORK0602Doc::SwitchToRotateZMode()
{
	// 确保处于三维模式
	m_b3DMode = TRUE;
	m_bRotateZMode = TRUE;
	m_bDrawCubeMode = TRUE;  // 确保立方体已绘制
	m_bRotateXMode = FALSE;  // 关闭x轴旋转模式
	m_bRotateYMode = FALSE;  // 关闭y轴旋转模式
	m_bTranslateXMode = FALSE; // 关闭平移模式

	UpdateAllViews(NULL);
}

// === 新增：绕z轴旋转函数 ===
void CCGWORK0602Doc::RotateCubeZ(double angle)
{
	// 将角度转换为弧度
	double rad = angle * 3.14159265358979323846 / 180.0;
	double cosA = cos(rad);
	double sinA = sin(rad);

	// 对每个顶点应用绕z轴旋转矩阵
	for (auto& vertex : m_cube.transformedVertices)
	{
		double x = vertex.x;
		double y = vertex.y;

		// 绕z轴旋转矩阵:
		// [cosA, -sinA, 0]
		// [sinA,  cosA, 0]
		// [  0,     0,  1]
		vertex.x = x * cosA - y * sinA;
		vertex.y = x * sinA + y * cosA;
	}

	UpdateAllViews(NULL);
	SetModifiedFlag(TRUE);
}

// === 切换到延x轴平移模式 ===
void CCGWORK0602Doc::SwitchToTranslateXMode()
{
	// 确保处于三维模式
	m_b3DMode = TRUE;
	m_bTranslateXMode = TRUE;
	m_bDrawCubeMode = TRUE;  // 确保立方体已绘制
	m_bRotateXMode = FALSE;  // 关闭旋转模式

	UpdateAllViews(NULL);
}

// === 切换到延y轴平移模式 ===
void CCGWORK0602Doc::SwitchToTranslateYMode()
{
	// 确保处于三维模式
	m_b3DMode = TRUE;
	m_bTranslateYMode = TRUE;
	m_bDrawCubeMode = TRUE;  // 确保立方体已绘制
	m_bRotateXMode = FALSE;  // 关闭旋转模式
	m_bRotateYMode = FALSE;
	m_bRotateZMode = FALSE;
	m_bTranslateXMode = FALSE;
	m_bTranslateZMode = FALSE;

	UpdateAllViews(NULL);
}

// === 切换到延z轴平移模式 ===
void CCGWORK0602Doc::SwitchToTranslateZMode()
{
	// 确保处于三维模式
	m_b3DMode = TRUE;
	m_bTranslateZMode = TRUE;
	m_bDrawCubeMode = TRUE;  // 确保立方体已绘制
	m_bRotateXMode = FALSE;  // 关闭旋转模式
	m_bRotateYMode = FALSE;
	m_bRotateZMode = FALSE;
	m_bTranslateXMode = FALSE;
	m_bTranslateYMode = FALSE;

	UpdateAllViews(NULL);
}

// === 延x轴平移函数 ===
void CCGWORK0602Doc::TranslateCubeX(double distance)
{
	// 对每个顶点应用x轴平移
	for (auto& vertex : m_cube.transformedVertices)
	{
		vertex.x += distance;
	}

	UpdateAllViews(NULL);
	SetModifiedFlag(TRUE);
}

// === 延y轴平移函数 ===
void CCGWORK0602Doc::TranslateCubeY(double distance)
{
	// 对每个顶点应用y轴平移
	for (auto& vertex : m_cube.transformedVertices)
	{
		vertex.y += distance;
	}

	UpdateAllViews(NULL);
	SetModifiedFlag(TRUE);
}

// === 延z轴平移函数 ===
void CCGWORK0602Doc::TranslateCubeZ(double distance)
{
	// 对每个顶点应用z轴平移
	for (auto& vertex : m_cube.transformedVertices)
	{
		vertex.z += distance;
	}

	UpdateAllViews(NULL);
	SetModifiedFlag(TRUE);
}

// === 设置平移步长 ===
void CCGWORK0602Doc::SetTranslationStep(double step)
{
	m_dTranslationStep = step;
	SetModifiedFlag(TRUE);
}

// === 设置旋转角度 ===
void CCGWORK0602Doc::SetRotationAngle(double angle)
{
	m_dRotationAngle = angle;
	SetModifiedFlag(TRUE);
}

// === 新增：切换到Bezier曲线绘制模式 ===
void CCGWORK0602Doc::SwitchToBezierMode()
{
	// 清空其他模式
	m_bDrawMode = FALSE;
	m_bAreaFillMode = FALSE;
	m_bDrawPolygonMode = FALSE;
	m_bDrawCircleMode = FALSE;
	m_b3DMode = FALSE;
	m_bBezierMode = TRUE;

	// 清空当前控制点
	ClearBezierPoints();
	m_bBezierDrawing = FALSE;

	UpdateAllViews(NULL);
}

// === 新增：添加Bezier控制点 ===
void CCGWORK0602Doc::AddBezierControlPoint(CPoint point)
{
	if (m_bBezierMode)
	{
		if (!m_bBezierDrawing)
		{
			m_bBezierDrawing = TRUE;
		}

		m_bezierControlPoints.push_back(point);

		// 当控制点达到4个时，自动计算曲线
		if (m_bezierControlPoints.size() == 4)
		{
			CalculateBezierCurve();
			CompleteBezierCurve();
		}

		UpdateAllViews(NULL);
	}
}

// === 新增：计算Bezier曲线 ===
void CCGWORK0602Doc::CalculateBezierCurve()
{
	if (m_bezierControlPoints.size() != 4)
		return;

	// 清空之前的曲线点
	// 注意：这里我们不在文档中存储临时曲线点，而是在绘制时实时计算

	UpdateAllViews(NULL);
}

// === 新增：完成Bezier曲线 ===
void CCGWORK0602Doc::CompleteBezierCurve()
{
	if (m_bezierControlPoints.size() == 4)
	{
		BezierCurve bezier;
		bezier.controlPoints = m_bezierControlPoints;

		// 计算曲线点
		std::vector<CPoint> curvePoints;
		for (double t = 0; t <= 1.0; t += 0.01) {
			double x = pow(1 - t, 3) * m_bezierControlPoints[0].x +
				3 * t * pow(1 - t, 2) * m_bezierControlPoints[1].x +
				3 * pow(t, 2) * (1 - t) * m_bezierControlPoints[2].x +
				pow(t, 3) * m_bezierControlPoints[3].x;

			double y = pow(1 - t, 3) * m_bezierControlPoints[0].y +
				3 * t * pow(1 - t, 2) * m_bezierControlPoints[1].y +
				3 * pow(t, 2) * (1 - t) * m_bezierControlPoints[2].y +
				pow(t, 3) * m_bezierControlPoints[3].y;

			curvePoints.push_back(CPoint((int)x, (int)y));
		}
		bezier.curvePoints = curvePoints;

		m_completedBeziers.push_back(bezier);
		ClearBezierPoints();

		UpdateAllViews(NULL);
		SetModifiedFlag(TRUE);
	}
}

// === 新增：清空Bezier控制点 ===
void CCGWORK0602Doc::ClearBezierPoints()
{
	m_bezierControlPoints.clear();
	m_bBezierDrawing = FALSE;
	UpdateAllViews(NULL);
}

// 在 CCGWORK0602Doc.cpp 中添加清屏函数实现
void CCGWORK0602Doc::ClearAll()
{
	// 清空所有矩形
	m_rectArray.RemoveAll();

	// 清空所有圆形
	m_circleArray.RemoveAll();

	// 清空所有多边形
	ClearAllPolygons();

	// 清空所有Bezier曲线
	m_completedBeziers.clear();
	ClearBezierPoints();

	// 重置三维立方体
	if (m_b3DMode) {
		ResetCube();
	}

	// 清空临时图形
	m_tempRect.SetRectEmpty();
	m_tempCircle.SetRectEmpty();
	m_bDrawing = FALSE;

	// 清空当前多边形点（如果正在绘制）
	if (m_bPolygonDrawing) {
		m_polygonPoints.clear();
		m_bPolygonDrawing = FALSE;
		m_polygonBoundingRect.SetRectEmpty();
	}

	// 清空当前Bezier控制点（如果正在绘制）
	if (m_bBezierDrawing) {
		ClearBezierPoints();
	}

	// 更新所有视图
	UpdateAllViews(NULL);

	// 标记文档为已修改
	SetModifiedFlag(TRUE);

	// 可选：显示清屏完成消息
	AfxMessageBox(_T("已清除所有图形！"));
}