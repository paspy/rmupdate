
#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "RMupdateManagerAbout.h"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

DECLARE_APP(RMupdateManagerApp);

#define HEAD1(TEXT)	\
	m_richTextCredits->BeginBold();	\
	m_richTextCredits->BeginFontSize(12);	\
	m_richTextCredits->AppendText(_T(TEXT));	\
	m_richTextCredits->EndFontSize();	\
	m_richTextCredits->EndBold();	\
	m_richTextCredits->AppendText(_T("\n"));

#define PERSON(CITY, NAME, EMAIL) {	\
	m_richTextCredits->BeginTextColour(wxColor(128, 128, 128));	\
	m_richTextCredits->AppendText(_T(CITY));	\
	m_richTextCredits->AppendText(_T(" "));	\
	m_richTextCredits->EndTextColour();	\
	\
	m_richTextCredits->AppendText(_T(NAME));	\
	\
	m_richTextCredits->AppendText(_T(" <"));	\
	wxString URL = wxString("mailto:", wxConvLibc) + _T(EMAIL);	\
	m_richTextCredits->BeginURL(URL);	\
	m_richTextCredits->BeginTextColour(wxColor(64, 64, 255));	\
	m_richTextCredits->BeginUnderline();	\
	m_richTextCredits->AppendText(_T(EMAIL));	\
	m_richTextCredits->EndUnderline();	\
	m_richTextCredits->EndTextColour();	\
	m_richTextCredits->EndURL();	\
	m_richTextCredits->AppendText(_T(">\n"));	\
}

#define EMPTYLINE()	\
	m_richTextCredits->AppendText(_T("\n"));

RMupdateManagerAbout::RMupdateManagerAbout(wxFrame *frame, const wxString& title)
    : DialogAbout(frame, -1, title)
{
	HEAD1("开发");
	PERSON("广西百色", "greensea", "gs@bbxy.net");
	EMPTYLINE();

	HEAD1("测试");
	PERSON("广东广州", "包子研究员", "ipaspy@gmail.com");
	EMPTYLINE();

	HEAD1("其他贡献");
}


RMupdateManagerAbout::~RMupdateManagerAbout()
{
}

void RMupdateManagerAbout::OnQuit(wxCommandEvent &event)
{
	EndModal(wxID_OK);
}
