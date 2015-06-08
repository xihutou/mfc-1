
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
END_MESSAGE_MAP()

// CmapView 생성/소멸

CmapView::CmapView()
	: isColorGet(false)
	, isSetCrossPoint(false)
	, isConnet(false)
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

	CPen redPen, *pOldPen;

	redPen.CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
	pOldPen = pDC->SelectObject(&redPen);

	int scale = 2;
	for(int i = 0;i<m_crossPoint.size();i++){
		pDC->Ellipse(m_crossPoint.at(i).x - scale, m_crossPoint.at(i).y - scale, m_crossPoint.at(i).x + scale, m_crossPoint.at(i).y + scale);

	}
	pOldPen = pDC->SelectObject(&redPen);

	if(isConnet){
	for(int i = 0; i<m_crossPoint.size()+1; i++){	
		pDC->MoveTo(m_crossPoint[i]);
		pDC->LineTo(m_crossPoint[i+1]);
		pDC->SelectObject(&redPen);
	}
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

	CmapDoc *pDoc = GetDocument();
	if(isColorGet)
	{
		COLORREF aColor = pDoc->m_pImageMap->GetPixel(point.x,point.y);
		pDoc->m_pRoadColor.push_back(aColor);
	}
	if (isSetCrossPoint)
	{
		m_crossPoint.push_back(point);
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
	Invalidate();

	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CmapView::OnSetCrossPointEnd()
{
	isSetCrossPoint = false;
	Invalidate();
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}
