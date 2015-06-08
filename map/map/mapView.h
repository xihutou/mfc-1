
// mapView.h : CmapView Ŭ������ �������̽�
//

#pragma once
#include <cmath>
using namespace std;

class CmapView : public CView
{
protected: // serialization������ ��������ϴ�.
	CmapView();
	DECLARE_DYNCREATE(CmapView)

// Ư���Դϴ�.
public:
	CmapDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CmapView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void On32771();
	bool isColorGet;
	afx_msg void OnColorGetEnd();
	

	afx_msg void OnRequestGridMap();
	afx_msg void OnSetCrossPoint();
	afx_msg void OnSetCrossPointEnd();
	bool isSetCrossPoint;
	vector <CPoint> m_crossPoint[100];
	bool isConnet;
	afx_msg void OnReTouch();
	afx_msg void OnConnet();
	int m_retouch;
	afx_msg void OnProcessReTouch();
	afx_msg void OnSetOptimalPath();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	bool isOptimalPath;
	afx_msg void OnSetStartEnd();
	bool isSetStartEnd;
	int m_retouchSSE;
	
	vector <CPoint> m_StartPoint[100];
	vector <CPoint> m_EndPoint[100];
	afx_msg void OnRetouchStartEnd();
	afx_msg void OnSetStartEndPointOver();
	int isStartEndConnet;
	afx_msg void OnStartEndConnet();
};

#ifndef _DEBUG  // mapView.cpp�� ����� ����
inline CmapDoc* CmapView::GetDocument() const
   { return reinterpret_cast<CmapDoc*>(m_pDocument); }
#endif

