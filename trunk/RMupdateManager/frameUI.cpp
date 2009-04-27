///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 21 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "frameUI.h"

///////////////////////////////////////////////////////////////////////////

FrameProject::FrameProject( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
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
	
	m_menubar1->Append( m_menuFile, wxT("文件") );
	
	m_menuHelp = new wxMenu();
	wxMenuItem* m_menuItemOnlineHelp;
	m_menuItemOnlineHelp = new wxMenuItem( m_menuHelp, wxID_HELP, wxString( wxT("在线文档(&H)\tCtrl+H") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuHelp->Append( m_menuItemOnlineHelp );
	
	wxMenuItem* m_menuItemAbout;
	m_menuItemAbout = new wxMenuItem( m_menuHelp, wxID_ABOUT, wxString( wxT("关于(&A)") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuHelp->Append( m_menuItemAbout );
	
	m_menubar1->Append( m_menuHelp, wxT("帮助") );
	
	this->SetMenuBar( m_menubar1 );
	
	m_toolBar1 = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY ); 
	m_toolBar1->AddTool( wxID_NEW, wxT("tool"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	m_toolBar1->AddTool( wxID_SAVE, wxT("tool"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	m_toolBar1->AddTool( wxID_OPEN, wxT("tool"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	m_toolBar1->AddTool( wxID_HELP, wxT("tool"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	m_toolBar1->Realize();
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_staticTextProjInfo = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextProjInfo->Wrap( -1 );
	bSizer1->Add( m_staticTextProjInfo, 1, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	// Connect Events
	this->Connect( m_menuItemCreateProject->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameProject::OnCreateProj ) );
	this->Connect( m_menuItemOpenProject->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameProject::OnOpenProj ) );
}

FrameProject::~FrameProject()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameProject::OnCreateProj ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameProject::OnOpenProj ) );
}

FrameConfig::FrameConfig( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("工程名："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer3->Add( m_staticText6, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textCtrlProjName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 350,-1 ), 0 );
	fgSizer3->Add( m_textCtrlProjName, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
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
	gSizer2->Add( m_buttonCheckUpdate, 0, wxALL, 5 );
	
	m_buttonRelease = new wxButton( this, wxID_RELEASE, wxT("保存并发布"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer2->Add( m_buttonRelease, 0, wxALL, 5 );
	
	bSizer9->Add( gSizer2, 1, wxEXPAND, 5 );
	
	bSizer7->Add( bSizer9, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer7 );
	this->Layout();
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	// Connect Events
	m_textCtrlProjName->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( FrameConfig::OnTextChange ), NULL, this );
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
	m_textCtrlProjName->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( FrameConfig::OnTextChange ), NULL, this );
	m_gridMapping->Disconnect( wxEVT_GRID_CELL_CHANGE, wxGridEventHandler( FrameConfig::OnGridCellChange ), NULL, this );
	m_buttonAddFile->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FrameConfig::OnAddFile ), NULL, this );
	m_buttonAddDir->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FrameConfig::OnAddDir ), NULL, this );
	m_buttonDelete->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FrameConfig::OnDelete ), NULL, this );
	m_buttonCheckUpdate->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FrameConfig::OnCheckUpdate ), NULL, this );
	m_buttonRelease->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FrameConfig::OnRelease ), NULL, this );
}
