
// mapView.cpp : CmapView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "map.h"
#endif

#include "mapDoc.h"
#include "mapView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CmapView

IMPLEMENT_DYNCREATE(CmapView, CView)

BEGIN_MESSAGE_MAP(CmapView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CmapView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_32771, &CmapView::On32771)
	ON_COMMAND(ID_32772, &CmapView::OnColorGetEnd)
	ON_COMMAND(ID_32773, &CmapView::OnRequestGridMap)
	ON_COMMAND(ID_32774, &CmapView::OnSetCrossPoint)
	ON_COMMAND(ID_32778, &CmapView::OnSetCrossPointEnd)
	ON_COMMAND(ID_32776, &CmapView::OnReTouch)
	ON_COMMAND(ID_32775, &CmapView::OnConnet)
	ON_COMMAND(ID_32779, &CmapView::OnProcessReTouch)
	ON_COMMAND(ID_32780, &CmapView::OnSetOptimalPath)
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_32781, &CmapView::OnSetStartEnd)
	ON_COMMAND(ID_32782, &CmapView::OnRetouchStartEnd)
	ON_COMMAND(ID_32783, &CmapView::OnSetStartEndPointOver)
	ON_COMMAND(ID_32777, &CmapView::OnStartEndConnet)
END_MESSAGE_MAP()

// CmapView 생성/소멸

CmapView::CmapView()
	: isColorGet(false)
	, isSetCrossPoint(false)
	, isConnet(false)
	, m_retouch(0)
	, isOptimalPath(false)
	, isSetStartEnd(false)
	, m_retouchSSE(0)
	, isStartEndConnet(0)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CmapView::~CmapView()
{
}

BOOL CmapView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CmapView 그리기

void CmapView::OnDraw(CDC* pDC)
{
	CmapDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	if(pDoc->m_pImageMap){
		HDC hdc = pDC->GetSafeHdc();
		pDoc->m_pImageMap->Draw(hdc,0,0); //화면에 보이기
	}
	if(pDoc->m_pImageGridMap){
		HDC m_hdc=pDC->GetSafeHdc();
		pDoc->m_pImageGridMap->Draw(m_hdc,0,0);

	}

	CPen bluePen, *pOldPen;

	bluePen.CreatePen(PS_SOLID, 5, RGB(0, 0, 255));
	pOldPen = pDC->SelectObject(&bluePen);

	int scale = 2;
	for(int i = 0;i<m_crossPoint[m_retouch].size();i++){
		pDC->Ellipse(m_crossPoint[m_retouch].at(i).x - scale, m_crossPoint[m_retouch].at(i).y - scale, m_crossPoint[m_retouch].at(i).x + scale, m_crossPoint[m_retouch].at(i).y + scale);

	}
	pOldPen = pDC->SelectObject(&bluePen);

	if(isConnet){
	for(int i = 0; i<m_crossPoint[m_retouch].size()-1; i++){	
		pDC->SelectObject(&bluePen);
		pDC->MoveTo(m_crossPoint[m_retouch][i]);
		pDC->LineTo(m_crossPoint[m_retouch][i+1]);
		
		}
	}

	if(isOptimalPath){
		HDC hdc = pDC->GetSafeHdc();
		pDoc->m_pImageMap->Draw(hdc,0,0); //화면에 보이기
	}


	CPen RedPen, *pOldPen2;

	RedPen.CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
	pOldPen2 = pDC->SelectObject(&RedPen);

	scale = 3;
	for(int i = 0;i<m_StartPoint[m_retouchSSE].size();i++){
		pDC->Ellipse(m_StartPoint[m_retouchSSE].at(i).x - scale, 
			m_StartPoint[m_retouchSSE].at(i).y - scale, 
			m_StartPoint[m_retouchSSE].at(i).x + scale, 
			m_StartPoint[m_retouchSSE].at(i).y + scale);

	}
	for(int i = 0;i<m_EndPoint[m_retouchSSE].size();i++){
		pDC->Ellipse(m_EndPoint[m_retouchSSE].at(i).x - scale, 
			m_EndPoint[m_retouchSSE].at(i).y - scale, 
			m_EndPoint[m_retouchSSE].at(i).x + scale, 
			m_EndPoint[m_retouchSSE].at(i).y + scale);

	}

	pOldPen2 = pDC->SelectObject(&RedPen);

	if(isStartEndConnet){
		pDC->SelectObject(&RedPen);
		double startX = m_StartPoint[m_retouchSSE].at(0).x;
		double startY = m_StartPoint[m_retouchSSE].at(0).y;
		double EndX = m_EndPoint[m_retouchSSE].at(0).x;
		double EndY = m_EndPoint[m_retouchSSE].at(0).y;
		double distance[200];

		for(int i = 0; i<m_crossPoint[m_retouch].size(); i++){
			double catchX = m_crossPoint[m_retouch].at(i).x;
			double catchY = m_crossPoint[m_retouch].at(i).y;
			distance[i] = sqrt(pow(startX - catchX, 2) + pow(startY - catchY, 2));
		}

		int startMin = 0;
		int startMinPos = 0;
		for (int i = 0; i <m_crossPoint[m_retouch].size() - 1 ; i++){
			
			if(startMin > distance[i]){ 
				startMin = distance[i];
				startMinPos = i;
			}
		}

		for(int i = 0; i<m_crossPoint[m_retouch].size(); i++){
			double catchX = m_crossPoint[m_retouch].at(i).x;
			double catchY = m_crossPoint[m_retouch].at(i).y;
			distance[i] = sqrt(pow(EndX - catchX, 2) + pow(EndY - catchY, 2));
		}

		int EndMin = 0;
		int EndMinPos = 0;
		for (int i = 0; i <m_crossPoint[m_retouch].size() - 1 ; i++){
			
			if(EndMin > distance[i]){ 
				EndMin = distance[i];
				EndMinPos = i;
			}
		}
		
			
		pDC->MoveTo(m_StartPoint[m_retouchSSE][0]);
		pDC->LineTo(m_crossPoint[m_retouch][startMinPos]);

		pDC->MoveTo(m_EndPoint[m_retouchSSE][0]);
		pDC->LineTo(m_crossPoint[m_retouch][EndMinPos]);
		
			
		}


	
	CDC memDC;
	
	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CmapView 인쇄


void CmapView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CmapView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CmapView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CmapView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CmapView::OnRButtonUp(UINT nFlags, CPoint point)
{
	TRACE3("%d, x:%d, y:%d\n", nFlags, point.x, point.y);
	CmapDoc *pDoc = GetDocument();
	if(isColorGet)
	{
		COLORREF aColor = pDoc->m_pImageMap->GetPixel(point.x,point.y);
		pDoc->m_pRoadColor.push_back(aColor);
	}
	if (isSetCrossPoint)
	{
		m_crossPoint[m_retouch].push_back(point);
		Invalidate();
	}

	if(isSetStartEnd){
	
		m_EndPoint[m_retouchSSE].push_back(point);
		Invalidate();	
	
	}



	CView::OnLButtonDown(nFlags, point);
	//ClientToScreen(&point);
	//OnContextMenu(this, point);
}

void CmapView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CmapView 진단

#ifdef _DEBUG
void CmapView::AssertValid() const
{
	CView::AssertValid();
}

void CmapView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CmapDoc* CmapView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CmapDoc)));
	return (CmapDoc*)m_pDocument;
}
#endif //_DEBUG


// CmapView 메시지 처리기


void CmapView::On32771()
{
	isColorGet = true;
	Invalidate();


	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CmapView::OnColorGetEnd()
{
	isColorGet = false;
	Invalidate();
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CmapView::OnRequestGridMap()
{
	CmapDoc* pDoc = GetDocument();
	pDoc->ConstructOccupancyGridMap();
	Invalidate();
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CmapView::OnSetCrossPoint()
{
	isSetCrossPoint = true;
	isConnet = false;
	Invalidate();

	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CmapView::OnSetCrossPointEnd()
{
	isSetCrossPoint = false;
	isConnet = false;
	Invalidate();
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CmapView::OnReTouch()
{
	m_retouch++;
	Invalidate();
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CmapView::OnConnet()
{
	isConnet = true;
	Invalidate();
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CmapView::OnProcessReTouch()
{
	m_retouch ++;
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CmapView::OnSetOptimalPath()
{
	isOptimalPath = true;
	Invalidate();
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CmapView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CmapDoc *pDoc = GetDocument();
	if (isSetStartEnd)
	{
		m_StartPoint[m_retouchSSE].push_back(point);
		Invalidate();
	}

	CView::OnLButtonUp(nFlags, point);
}


void CmapView::OnSetStartEnd()
{
	isSetStartEnd = true;
	Invalidate();
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CmapView::OnRetouchStartEnd()
{
	m_retouchSSE ++;
	Invalidate();
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CmapView::OnSetStartEndPointOver()
{

	isSetStartEnd = false;
	Invalidate();
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CmapView::OnStartEndConnet()
{
	isStartEndConnet = true;
	Invalidate();
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}
