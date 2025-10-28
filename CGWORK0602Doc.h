
// CGWORK0602Doc.h: CCGWORK0602Doc 类的接口
//


#pragma once
#include <vector>    // 添加vector头文件
#include <algorithm> // 添加算法头文件

// === 3D点结构体 ===
struct CPoint3D {
	double x, y, z;
	CPoint3D() : x(0), y(0), z(0) {}
	CPoint3D(double x, double y, double z) : x(x), y(y), z(z) {}
};

// === 面结构体 ===
struct Face {
	int v1, v2, v3, v4;  // 顶点索引
	COLORREF color;       // 面的颜色
	Face() : v1(0), v2(0), v3(0), v4(0), color(RGB(0, 0, 0)) {}
	Face(int v1, int v2, int v3, int v4, COLORREF clr) :
		v1(v1), v2(v2), v3(v3), v4(v4), color(clr) {
	}
};

// === Bezier曲线相关结构体 ===
struct BezierCurve {
	std::vector<CPoint> controlPoints;  // 控制点
	std::vector<CPoint> curvePoints;    // 曲线上的点
};

class CCGWORK0602Doc : public CDocument
{
protected: // 仅从序列化创建
	CCGWORK0602Doc() noexcept;
	DECLARE_DYNCREATE(CCGWORK0602Doc)



// 特性
public:

// 操作
public:


public:
	//矩形绘制相关
	//CRect m_rect;       // 存储矩形坐标
	BOOL m_bDrawing;    // 标记是否正在绘制
	CPoint m_ptStart;   // 矩形起始点
	CPoint m_ptEnd;     // 矩形结束点
	BOOL m_bDrawMode;   // 标记是否处于绘制模式

	// 存储所有已完成的矩形
	CArray<CRect, CRect&> m_rectArray;  // 矩形数组
	CRect m_tempRect;   // 临时矩形（用于绘制过程中的预览）

	// 圆形绘制相关
	BOOL m_bDrawCircleMode;           // 标记是否处于圆形绘制模式
	CArray<CRect, CRect&> m_circleArray;  // 存储所有已完成的圆形（用外接矩形表示）
	CRect m_tempCircle;               // 临时圆形（用于绘制过程中的预览）

	// 圆形绘制模式切换
	void SwitchToCircleMode();

	// 区域填充相关
	BOOL m_bAreaFillMode;  // 标记是否处于区域填充模式
	void SwitchToAreaFillMode();  // 切换到区域填充模式
	void SwitchToDrawMode();      // 切换回绘制模式
	//BOOL IsInDrawMode() const { return m_bDrawMode && !m_bAreaFillMode; }  // 检查是否在图形绘制模式

	void StartDrawing(CPoint point);
	void UpdateDrawing(CPoint point);
	void EndDrawing(CPoint point);
	void SetDrawMode(BOOL bMode);  // 新增：设置绘制模式

	// === 多边形绘制相关 ===
	BOOL m_bDrawPolygonMode;        // 标记是否处于多边形绘制模式
	BOOL m_bPolygonDrawing;         // 标记是否正在绘制多边形
	std::vector<CPoint> m_polygonPoints;  // 存储多边形顶点
	CRect m_polygonBoundingRect;    // 多边形边界矩形

	// === 存储所有已完成的多边形 ===
	std::vector<std::vector<CPoint>> m_completedPolygons;  // 存储所有已完成的多边形
	// 修改多边形相关函数
	void CompleteCurrentPolygon();  // 完成当前多边形并添加到已完成列表
	void ClearAllPolygons();        // 清空所有多边形

	// 多边形绘制函数
	void StartPolygonDrawing(CPoint point);
	void AddPolygonPoint(CPoint point);
	void EndPolygonDrawing();
	void ClearPolygon();



	// 切换到多边形绘制模式
	void SwitchToPolygonMode();

	// === 三维变换相关 ===
	BOOL m_b3DMode;                    // 标记是否处于三维模式
	BOOL m_bDrawCubeMode;              // 标记是否处于立方体绘制模式
	BOOL m_bRotateXMode;               // 标记是否处于绕x轴旋转模式
	BOOL m_bRotateYMode;               // 标记是否处于绕y轴旋转模式
	BOOL m_bRotateZMode;               // 标记是否处于绕z轴旋转模式
	BOOL m_bTranslateXMode;            // 标记是否处于延x轴平移模式
	BOOL m_bTranslateYMode;            // 标记是否处于延y轴平移模式
	BOOL m_bTranslateZMode;            // 标记是否处于延z轴平移模式

	// 参数设置
	double m_dTranslationStep;         // 平移步长
	double m_dRotationAngle;           // 旋转角度

	// 立方体数据
	struct Cube3D {
		std::vector<CPoint3D> vertices;    // 立方体顶点（世界坐标）
		std::vector<CPoint3D> transformedVertices; // 变换后的顶点
		std::vector<Face> faces;           // 立方体面
	};

	Cube3D m_cube;           // 立方体对象

	// 三维变换函数
	void SwitchTo3DMode();
	void DrawCube();
	void ResetCube();        // 重置立方体到初始位置
	void InitCube();         // 初始化立方体数据
	void SwitchToRotateXMode();  // 切换到绕x轴旋转模式
	void SwitchToRotateYMode();  // 切换到绕y轴旋转模式
	void SwitchToRotateZMode();  // 切换到绕z轴旋转模式
	void SwitchToTranslateXMode(); // 切换到延x轴平移模式
	void SwitchToTranslateYMode(); // 切换到延y轴平移模式
	void SwitchToTranslateZMode(); // 切换到延z轴平移模式

	// 旋转变换函数
	void RotateCubeX(double angle);  // 绕x轴旋转
	void RotateCubeY(double angle);  // 绕y轴旋转
	void RotateCubeZ(double angle);  // 绕z轴旋转

	// 平移变换函数
	void TranslateCubeX(double distance); // 延x轴平移
	void TranslateCubeY(double distance); // 延y轴平移
	void TranslateCubeZ(double distance); // 延z轴平移

	// 参数设置函数
	void SetTranslationStep(double step);  // 设置平移步长
	void SetRotationAngle(double angle);   // 设置旋转角度

	// 投影变换函数
	CPoint PerspectiveProjection(const CPoint3D& point3D);

	// === Bezier曲线绘制相关 ===
	BOOL m_bBezierMode;                 // 标记是否处于Bezier曲线绘制模式
	BOOL m_bBezierDrawing;              // 标记是否正在绘制Bezier曲线
	std::vector<CPoint> m_bezierControlPoints;  // 当前Bezier曲线的控制点
	std::vector<BezierCurve> m_completedBeziers; // 已完成的Bezier曲线

	// Bezier曲线函数
	void SwitchToBezierMode();          // 切换到Bezier曲线绘制模式
	void AddBezierControlPoint(CPoint point);  // 添加控制点
	void CompleteBezierCurve();         // 完成当前Bezier曲线
	void ClearBezierPoints();           // 清空当前控制点
	void CalculateBezierCurve();        // 计算Bezier曲线点

	// 检查当前模式
	BOOL IsInDrawMode() const { return m_bDrawMode && !m_bAreaFillMode && !m_bDrawPolygonMode && !m_bDrawCircleMode; }
	BOOL IsInAreaFillMode() const { return m_bAreaFillMode && !m_bDrawMode && !m_bDrawPolygonMode && !m_bDrawCircleMode; }
	BOOL IsInPolygonMode() const { return m_bDrawPolygonMode && !m_bDrawMode && !m_bAreaFillMode && !m_bDrawCircleMode; }
	BOOL IsInCircleMode() const { return m_bDrawCircleMode && !m_bDrawMode && !m_bAreaFillMode && !m_bDrawPolygonMode; }
	BOOL IsIn3DMode() const { return m_b3DMode && !m_bDrawMode && !m_bAreaFillMode && !m_bDrawPolygonMode && !m_bDrawCircleMode; }
	BOOL IsInDrawCubeMode() const { return m_bDrawCubeMode && m_b3DMode; }
	BOOL IsInRotateXMode() const { return m_bRotateXMode && m_b3DMode; }  
	BOOL IsInRotateYMode() const { return m_bRotateYMode && m_b3DMode; }  
	BOOL IsInRotateZMode() const { return m_bRotateZMode && m_b3DMode; }
	BOOL IsInTranslateXMode() const { return m_bTranslateXMode && m_b3DMode; } 
	BOOL IsInTranslateYMode() const { return m_bTranslateYMode && m_b3DMode; }  
	BOOL IsInTranslateZMode() const { return m_bTranslateZMode && m_b3DMode; } 
	BOOL IsInBezierMode() const { return m_bBezierMode && !m_bDrawMode && !m_bAreaFillMode && !m_bDrawPolygonMode && !m_bDrawCircleMode && !m_b3DMode; }
	void EdgeFlagFill(CDC* pDC, CString fillText);  // 使用当前多边形点集
	void EdgeFlagFillPolygon(CDC* pDC, const std::vector<CPoint>& polygon, CString fillText);  // 使用指定多边形点集

	COLORREF m_rectColor;    // 矩形颜色
	COLORREF m_circleColor;  // 圆形颜色

	COLORREF m_polygonFillColor;   // 新增：多边形填充颜色
	COLORREF m_polygonBorderColor; // 新增：多边形边界颜色

	void ClearAll();

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CCGWORK0602Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
