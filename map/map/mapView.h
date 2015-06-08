
// mapView.h : CmapView Ŭ������ �������̽�
//

#pragma once


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
	vector <CPoint> m_crossPoint;
	bool isConnet;
};

#ifndef _DEBUG  // mapView.cpp�� ����� ����
inline CmapDoc* CmapView::GetDocument() const
   { return reinterpret_cast<CmapDoc*>(m_pDocument); }
#endif

