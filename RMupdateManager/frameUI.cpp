///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 29 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "frameUI.h"

///////////////////////////////////////////////////////////////////////////

FrameProject::FrameProject( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	
	m_menubar1 = new wxMenuBar( 0 );
	m_menuFile = new wxMenu();
	wxMenuItem* m_menuItemCreateProject;
	m_menuItemCreateProject = new wxMenuItem( m_menuFile, wxID_NEW, wxString( wxT("新建工程(&N)...\tCtrl+N") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuItemCreateProject );
	
	wxMenuItem* m_menuItemOpenProject;
	m_menuItemOpenProject = new wxMenuItem( m_menuFile, wxID_OPEN, wxString( wxT("打开工程(&O)...\tCtrl+O") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuItemOpenProject );
	
	wxMenuItem* m_menuItemSaveProject;
	m_menuItemSaveProject = new wxMenuItem( m_menuFile, wxID_SAVE, wxString( wxT("保存工程(&S)\tCtrl+S") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuItemSaveProject );
	
	m_menuFile->AppendSeparator();
	
	wxMenuItem* m_menuItemQuit;
	m_menuItemQuit = new wxMenuItem( m_menuFile, wxID_EXIT, wxString( wxT("退出(&Q)\tAlt+F4") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuItemQuit );
	
	m_menubar1->Append( m_menuFile, wxT("文件(&F)") );
	
	m_menuHelp = new wxMenu();
	wxMenuItem* m_menuItemOnlineHelp;
	m_menuItemOnlineHelp = new wxMenuItem( m_menuHelp, wxID_HELP, wxString( wxT("在线文档(&H)\tCtrl+H") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuHelp->Append( m_menuItemOnlineHelp );
	
	wxMenuItem* m_menuItemAbout;
	m_menuItemAbout = new wxMenuItem( m_menuHelp, wxID_ABOUT, wxString( wxT("关于(&A)") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuHelp->Append( m_menuItemAbout );
	
	m_menubar1->Append( m_menuHelp, wxT("帮助(&H)") );
	
	this->SetMenuBar( m_menubar1 );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_staticTextProjInfo = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextProjInfo->Wrap( -1 );
	bSizer1->Add( m_staticTextProjInfo, 1, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( FrameProject::OnClose ) );
	this->Connect( m_menuItemCreateProject->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameProject::OnCreateProj ) );
	this->Connect( m_menuItemOpenProject->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameProject::OnOpenProj ) );
	this->Connect( m_menuItemSaveProject->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameProject::OnSaveProj ) );
	this->Connect( m_menuItemQuit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameProject::OnQuit ) );
	this->Connect( m_menuItemOnlineHelp->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameProject::OnOnlineHelp ) );
	this->Connect( m_menuItemAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameProject::OnAbout ) );
}

FrameProject::~FrameProject()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( FrameProject::OnClose ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameProject::OnCreateProj ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameProject::OnOpenProj ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameProject::OnSaveProj ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameProject::OnQuit ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameProject::OnOnlineHelp ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameProject::OnAbout ) );
}

FrameConfig::FrameConfig( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 3, 2, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("工程名："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer3->Add( m_staticText6, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	m_textCtrlProjName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 350,-1 ), 0 );
	fgSizer3->Add( m_textCtrlProjName, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("发布版本号："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer3->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	m_textCtrlVersion = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_textCtrlVersion, 0, wxALL|wxEXPAND, 5 );
	
	bSizer7->Add( fgSizer3, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );
	
	m_gridMapping = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	
	// Grid
	m_gridMapping->CreateGrid( 0, 3 );
	m_gridMapping->EnableEditing( true );
	m_gridMapping->EnableGridLines( true );
	m_gridMapping->EnableDragGridSize( false );
	m_gridMapping->SetMargins( 0, 0 );
	
	// Columns
	m_gridMapping->SetColSize( 0, 148 );
	m_gridMapping->SetColSize( 1, 169 );
	m_gridMapping->SetColSize( 2, 80 );
	m_gridMapping->AutoSizeColumns();
	m_gridMapping->EnableDragColMove( false );
	m_gridMapping->EnableDragColSize( true );
	m_gridMapping->SetColLabelSize( 30 );
	m_gridMapping->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_gridMapping->EnableDragRowSize( true );
	m_gridMapping->SetRowLabelSize( 80 );
	m_gridMapping->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	m_gridMapping->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	bSizer8->Add( m_gridMapping, 1, wxALL|wxEXPAND, 5 );
	
	bSizer7->Add( bSizer8, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	wxGridSizer* gSizer2;
	gSizer2 = new wxGridSizer( 1, 5, 0, 0 );
	
	m_buttonAddFile = new wxButton( this, wxID_ADDFILE, wxT("增加文件"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer2->Add( m_buttonAddFile, 0, wxALL, 5 );
	
	m_buttonAddDir = new wxButton( this, wxID_ADDDIR, wxT("增加目录"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer2->Add( m_buttonAddDir, 0, wxALL, 5 );
	
	m_buttonDelete = new wxButton( this, wxID_DELETE, wxT("删除"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer2->Add( m_buttonDelete, 0, wxALL, 5 );
	
	m_buttonCheckUpdate = new wxButton( this, wxID_CHECK, wxT("检查更新"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonCheckUpdate->SetDefault(); 
	gSizer2->Add( m_buttonCheckUpdate, 0, wxALL, 5 );
	
	m_buttonRelease = new wxButton( this, wxID_RELEASE, wxT("保存并发布"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonRelease->Enable( false );
	
	gSizer2->Add( m_buttonRelease, 0, wxALL, 5 );
	
	bSizer9->Add( gSizer2, 1, wxEXPAND, 5 );
	
	bSizer7->Add( bSizer9, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer7 );
	this->Layout();
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( FrameConfig::OnClose ) );
	m_textCtrlProjName->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( FrameConfig::OnTextChange ), NULL, this );
	m_textCtrlVersion->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( FrameConfig::OnTextChange ), NULL, this );
	m_gridMapping->Connect( wxEVT_GRID_CELL_CHANGE, wxGridEventHandler( FrameConfig::OnGridCellChange ), NULL, this );
	m_buttonAddFile->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FrameConfig::OnAddFile ), NULL, this );
	m_buttonAddDir->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FrameConfig::OnAddDir ), NULL, this );
	m_buttonDelete->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FrameConfig::OnDelete ), NULL, this );
	m_buttonCheckUpdate->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FrameConfig::OnCheckUpdate ), NULL, this );
	m_buttonRelease->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FrameConfig::OnRelease ), NULL, this );
}

FrameConfig::~FrameConfig()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( FrameConfig::OnClose ) );
	m_textCtrlProjName->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( FrameConfig::OnTextChange ), NULL, this );
	m_textCtrlVersion->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( FrameConfig::OnTextChange ), NULL, this );
	m_gridMapping->Disconnect( wxEVT_GRID_CELL_CHANGE, wxGridEventHandler( FrameConfig::OnGridCellChange ), NULL, this );
	m_buttonAddFile->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FrameConfig::OnAddFile ), NULL, this );
	m_buttonAddDir->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FrameConfig::OnAddDir ), NULL, this );
	m_buttonDelete->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FrameConfig::OnDelete ), NULL, this );
	m_buttonCheckUpdate->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FrameConfig::OnCheckUpdate ), NULL, this );
	m_buttonRelease->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FrameConfig::OnRelease ), NULL, this );
}

DialogAbout::DialogAbout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("RMupdateManager"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	m_staticText4->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer6->Add( m_staticText4, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("一个RMXP/VX开发项目更新管理程序"), wxPoint( -1,-1 ), wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	m_staticText5->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	m_staticText5->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_GRAYTEXT ) );
	
	bSizer6->Add( m_staticText5, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	bSizer5->Add( bSizer6, 0, 0, 5 );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_notebookInfo = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panelInfo = new wxPanel( m_notebookInfo, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText7 = new wxStaticText( m_panelInfo, wxID_ANY, wxT("软件：RMupdateManager\n\n版本：舒克1（2009年5月27日）\n\n立项日期：2009年4月22日\n"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer12->Add( m_staticText7, 1, wxEXPAND|wxALL, 5 );
	
	bSizer11->Add( bSizer12, 0, wxEXPAND|wxTOP, 5 );
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText6 = new wxStaticText( m_panelInfo, wxID_ANY, wxT("开发站点："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer13->Add( m_staticText6, 0, wxALL, 5 );
	
	m_hyperlink1 = new wxHyperlinkCtrl( m_panelInfo, wxID_ANY, wxT("http://code.google.com/p/rmupdate/"), wxT("http://code.google.com/p/rmupdate"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	bSizer13->Add( m_hyperlink1, 0, wxALL, 5 );
	
	bSizer11->Add( bSizer13, 0, 0, 5 );
	
	m_panelInfo->SetSizer( bSizer11 );
	m_panelInfo->Layout();
	bSizer11->Fit( m_panelInfo );
	m_notebookInfo->AddPage( m_panelInfo, wxT("信息"), true );
	m_panelCredits = new wxPanel( m_notebookInfo, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );
	
	m_richTextCredits = new wxRichTextCtrl( m_panelCredits, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0|wxVSCROLL|wxHSCROLL|wxNO_BORDER|wxWANTS_CHARS );
	bSizer14->Add( m_richTextCredits, 1, wxEXPAND | wxALL, 5 );
	
	m_panelCredits->SetSizer( bSizer14 );
	m_panelCredits->Layout();
	bSizer14->Fit( m_panelCredits );
	m_notebookInfo->AddPage( m_panelCredits, wxT("鸣谢"), false );
	
	bSizer7->Add( m_notebookInfo, 1, wxEXPAND | wxALL, 5 );
	
	bSizer5->Add( bSizer7, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	m_buttonOK = new wxButton( this, wxID_OK, wxT("确定"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_buttonOK, 0, wxALL, 5 );
	
	bSizer5->Add( bSizer9, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	this->SetSizer( bSizer5 );
	this->Layout();
	
	// Connect Events
	m_buttonOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogAbout::OnQuit ), NULL, this );
}

DialogAbout::~DialogAbout()
{
	// Disconnect Events
	m_buttonOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogAbout::OnQuit ), NULL, this );
}
