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
