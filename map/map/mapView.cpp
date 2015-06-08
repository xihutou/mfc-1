
// mapView.cpp : CmapView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
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

// CmapView ����/�Ҹ�

CmapView::CmapView()
	: isColorGet(false)
	, isSetCrossPoint(false)
	, isConnet(false)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CmapView::~CmapView()
{
}

BOOL CmapView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CmapView �׸���

void CmapView::OnDraw(CDC* pDC)
{
	CmapDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	if(pDoc->m_pImageMap){
	HDC hdc = pDC->GetSafeHdc();
	pDoc->m_pImageMap->Draw(hdc,0,0); //ȭ�鿡 ���̱�
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
	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CmapView �μ�


void CmapView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CmapView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CmapView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CmapView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
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


// CmapView ����

#ifdef _DEBUG
void CmapView::AssertValid() const
{
	CView::AssertValid();
}

void CmapView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CmapDoc* CmapView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CmapDoc)));
	return (CmapDoc*)m_pDocument;
}
#endif //_DEBUG


// CmapView �޽��� ó����


void CmapView::On32771()
{
	isColorGet = true;
	Invalidate();


	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}


void CmapView::OnColorGetEnd()
{
	isColorGet = false;
	Invalidate();
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}


void CmapView::OnRequestGridMap()
{
	CmapDoc* pDoc = GetDocument();
	pDoc->ConstructOccupancyGridMap();
	Invalidate();
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}


void CmapView::OnSetCrossPoint()
{
	isSetCrossPoint = true;
	Invalidate();

	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}


void CmapView::OnSetCrossPointEnd()
{
	isSetCrossPoint = false;
	Invalidate();
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}
