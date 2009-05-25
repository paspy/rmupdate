///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 29 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "RMupdaterUI.h"

///////////////////////////////////////////////////////////////////////////

FrameUpdater::FrameUpdater( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );
	
	m_bitmapLogo = new wxStaticBitmap( this, wxID_ANY, wxBitmap( wxT("icon.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_bitmapLogo, 0, wxALL|wxALIGN_CENTER_VERTICAL, 10 );
	
	m_richTextInfo = new wxRichTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0|wxVSCROLL|wxHSCROLL|wxNO_BORDER|wxWANTS_CHARS );
	m_richTextInfo->Enable( false );
	
	bSizer9->Add( m_richTextInfo, 1, wxEXPAND|wxALL, 5 );
	
	bSizer2->Add( bSizer9, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonCheck = new wxButton( this, wxID_CHECK, wxT("检查更新"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_buttonCheck, 0, wxALL, 5 );
	
	m_buttonUpdate = new wxButton( this, wxID_APPLY, wxT("更新"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonUpdate->Enable( false );
	
	bSizer4->Add( m_buttonUpdate, 0, wxALL, 5 );
	
	m_buttonConfig = new wxButton( this, wxID_SETUP, wxT("设置"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_buttonConfig, 0, wxALL, 5 );
	
	bSizer6->Add( bSizer4, 0, 0, 5 );
	
	wxGridSizer* gSizer4;
	gSizer4 = new wxGridSizer( 1, 1, 0, 0 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonStart = new wxButton( this, wxID_START, wxT("开始游戏"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_buttonStart, 0, wxALL, 5 );
	
	gSizer4->Add( bSizer5, 0, wxALIGN_RIGHT, 5 );
	
	bSizer6->Add( gSizer4, 1, 0, 5 );
	
	bSizer2->Add( bSizer6, 0, wxEXPAND, 5 );
	
	m_staticTextCurProc = new wxStaticText( this, wxID_ANY, wxT("当前进度："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextCurProc->Wrap( -1 );
	bSizer2->Add( m_staticTextCurProc, 0, wxALL, 5 );
	
	m_gaugeCurrent = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxSize( -1,16 ), wxGA_HORIZONTAL );
	bSizer2->Add( m_gaugeCurrent, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );
	
	m_staticTextTotalProc = new wxStaticText( this, wxID_ANY, wxT("总体进度："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextTotalProc->Wrap( -1 );
	bSizer2->Add( m_staticTextTotalProc, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_gaugeTotal = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxSize( -1,16 ), wxGA_HORIZONTAL );
	m_gaugeTotal->SetValue( 0 ); 
	bSizer2->Add( m_gaugeTotal, 0, wxEXPAND|wxALL, 5 );
	
	this->SetSizer( bSizer2 );
	this->Layout();
	m_statusBarInfo = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	m_statusBarInfo->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	
	
	// Connect Events
	m_buttonCheck->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FrameUpdater::OnCheck ), NULL, this );
	m_buttonUpdate->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FrameUpdater::OnUpdate ), NULL, this );
	m_buttonConfig->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FrameUpdater::OnConfig ), NULL, this );
	m_buttonStart->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FrameUpdater::OnStart ), NULL, this );
}

FrameUpdater::~FrameUpdater()
{
	// Disconnect Events
	m_buttonCheck->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FrameUpdater::OnCheck ), NULL, this );
	m_buttonUpdate->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FrameUpdater::OnUpdate ), NULL, this );
	m_buttonConfig->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FrameUpdater::OnConfig ), NULL, this );
	m_buttonStart->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FrameUpdater::OnStart ), NULL, this );
}

DialogConfig::DialogConfig( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxString m_radioBoxUpdateIntervalChoices[] = { wxT("每周"), wxT("启动时") };
	int m_radioBoxUpdateIntervalNChoices = sizeof( m_radioBoxUpdateIntervalChoices ) / sizeof( wxString );
	m_radioBoxUpdateInterval = new wxRadioBox( this, wxID_ANY, wxT("更新检查间隔"), wxDefaultPosition, wxDefaultSize, m_radioBoxUpdateIntervalNChoices, m_radioBoxUpdateIntervalChoices, 1, wxRA_SPECIFY_COLS );
	m_radioBoxUpdateInterval->SetSelection( 0 );
	bSizer1->Add( m_radioBoxUpdateInterval, 0, wxALL|wxEXPAND, 5 );
	
	m_buttonClose = new wxButton( this, wxID_CLOSE, wxT("关闭"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_buttonClose, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( DialogConfig::OnClose ) );
	m_buttonClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogConfig::OnOK ), NULL, this );
}

DialogConfig::~DialogConfig()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( DialogConfig::OnClose ) );
	m_buttonClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogConfig::OnOK ), NULL, this );
}
