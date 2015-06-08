
// mapDoc.h : CmapDoc 클래스의 인터페이스
//


#pragma once

#include "atlimage.h"
#include <vector>
using namespace std;

class CmapDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CmapDoc();
	DECLARE_DYNCREATE(CmapDoc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CmapDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
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
