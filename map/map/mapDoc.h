
// mapDoc.h : CmapDoc Ŭ������ �������̽�
//


#pragma once

#include "atlimage.h"
#include <vector>
using namespace std;

class CmapDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CmapDoc();
	DECLARE_DYNCREATE(CmapDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// �����Դϴ�.
public:
	virtual ~CmapDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	CImage* m_pImageMap;
	int m_nWidthGridMap;
	int m_nHeightGridMap;
	vector <COLORREF> m_pRoadColor;
	CImage* m_pImageGridMap;
	short* m_pGridMap;
	void ConstructOccupancyGridMap(void);
	bool RGBCompare(COLORREF color, vector<COLORREF> pathColor);
};
