
// mapDoc.cpp : CmapDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "map.h"
#endif

#include "mapDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CmapDoc

IMPLEMENT_DYNCREATE(CmapDoc, CDocument)

BEGIN_MESSAGE_MAP(CmapDoc, CDocument)
END_MESSAGE_MAP()


// CmapDoc 생성/소멸

CmapDoc::CmapDoc()
	: m_pImageMap(NULL)
	, m_nWidthGridMap(0)
	, m_nHeightGridMap(0)
	, m_pImageGridMap(NULL)
	, m_pGridMap(NULL)
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CmapDoc::~CmapDoc()
{
}

BOOL CmapDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CmapDoc serialization

void CmapDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{

		if(m_pImageMap != NULL) 
			delete m_pImageMap;
		// TODO: 여기에 로딩 코드를 추가합니다.
	}

	m_pImageMap = new CImage; //나번 문제
	CFile *fp = ar.GetFile(); //나번 문제
	m_pImageMap->Load(fp->GetFilePath()); //나번 문제

	m_nWidthGridMap = m_pImageMap->GetWidth();
	m_nHeightGridMap = m_pImageMap->GetHeight();
	m_pGridMap = new short[m_nWidthGridMap*m_nHeightGridMap];
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CmapDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CmapDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CmapDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CmapDoc 진단

#ifdef _DEBUG
void CmapDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CmapDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CmapDoc 명령


void CmapDoc::ConstructOccupancyGridMap(void)
{
	m_nWidthGridMap = m_pImageMap->GetWidth();

	m_nHeightGridMap = m_pImageMap->GetHeight();

	int nSizeOfMap = m_nWidthGridMap* m_nHeightGridMap;
	if(!m_pImageGridMap) {
		delete m_pImageGridMap;
		delete m_pGridMap;
	}
	m_pImageGridMap = new CImage();
	m_pGridMap = new short[nSizeOfMap];
	m_pImageGridMap->Create(m_nWidthGridMap,m_nHeightGridMap,24);
	int x, y;
	for(y = 0 ; y < m_nHeightGridMap ; y++) {
		for(x = 0 ; x < m_nWidthGridMap ; x++) {
			if(RGBCompare(m_pImageMap->GetPixel(x, y), m_pRoadColor)) {

				m_pImageGridMap->SetPixel(x, y, RGB(255, 255, 255));
				m_pGridMap[x + y * m_nHeightGridMap] = 1;
			}
		}
	}
}


bool CmapDoc::RGBCompare(COLORREF color, vector<COLORREF> pathColor)
{
	bool RGB_flag = false;
	int colorR = GetRValue(color);
	int colorG = GetGValue(color);
	int colorB = GetBValue(color);

	for(int i = 0 ; i < pathColor.size() ; i++) {
		int pathColorR = GetRValue(pathColor[i]);
		int pathColorG = GetGValue(pathColor[i]);
		int pathColorB = GetBValue(pathColor[i]);

		if(abs(colorR - pathColorR) < 10) {
			if(abs(colorG - pathColorG) < 10) {
				if(abs(colorB - pathColorB) < 10) { ///////원래는 5
					RGB_flag = true;
					break;
				}
			}
		}
	}

	return RGB_flag;
}
