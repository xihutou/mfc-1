
// mapDoc.cpp : CmapDoc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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


// CmapDoc ����/�Ҹ�

CmapDoc::CmapDoc()
	: m_pImageMap(NULL)
	, m_nWidthGridMap(0)
	, m_nHeightGridMap(0)
	, m_pImageGridMap(NULL)
	, m_pGridMap(NULL)
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CmapDoc::~CmapDoc()
{
}

BOOL CmapDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CmapDoc serialization

void CmapDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{

		if(m_pImageMap != NULL) 
			delete m_pImageMap;
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}

	m_pImageMap = new CImage; //���� ����
	CFile *fp = ar.GetFile(); //���� ����
	m_pImageMap->Load(fp->GetFilePath()); //���� ����

	m_nWidthGridMap = m_pImageMap->GetWidth();
	m_nHeightGridMap = m_pImageMap->GetHeight();
	m_pGridMap = new short[m_nWidthGridMap*m_nHeightGridMap];
}

#ifdef SHARED_HANDLERS

// ����� �׸��� �����մϴ�.
void CmapDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// ������ �����͸� �׸����� �� �ڵ带 �����Ͻʽÿ�.
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

// �˻� ó���⸦ �����մϴ�.
void CmapDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
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

// CmapDoc ����

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


// CmapDoc ���


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
				if(abs(colorB - pathColorB) < 10) { ///////������ 5
					RGB_flag = true;
					break;
				}
			}
		}
	}

	return RGB_flag;
}
