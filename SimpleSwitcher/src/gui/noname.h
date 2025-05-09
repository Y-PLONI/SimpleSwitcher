///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/checkbox.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/hyperlink.h>
#include <wx/panel.h>
#include <wx/grid.h>
#include <wx/listbox.h>
#include <wx/notebook.h>
#include <wx/frame.h>
#include <wx/dialog.h>
#include <wx/statbmp.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class MyFrame4
///////////////////////////////////////////////////////////////////////////////
class MyFrame4 : public wxFrame
{
	private:

	protected:
		wxNotebook* m_notebook2;
		wxPanel* m_panelMain;
		wxCheckBox* m_checkBoxEnable;
		wxCheckBox* m_checkAddToAutoStart;
		wxCheckBox* m_checkBoxWorkInAdmin;
		wxCheckBox* m_checkBoxAlterantiveLayoutChange;
		wxStaticText* m_staticText5;
		wxChoice* m_choiceShowInTray;
		wxButton* m_buttonUpdateFlags;
		wxStaticText* m_staticText9;
		wxChoice* m_comboUiLang;
		wxCheckBox* m_checkBoxDisablAcc;
		wxCheckBox* m_checkBoxClearForm;
		wxCheckBox* m_checkBoxSeparateExt;
		wxCheckBox* m_checkDebuglog;
		wxButton* m_buttonReloadConfig;
		wxHyperlinkCtrl* m_hyperlink11;
		wxPanel* m_panel141;
		wxGrid* m_gridHotKeys;
		wxGrid* m_gridLayouts;
		wxPanel* m_panel5;
		wxCheckBox* m_checkcapsrem;
		wxCheckBox* m_check_scrollremap;
		wxButton* m_button7;
		wxButton* m_button8;
		wxListBox* m_listBoxRemap;
		wxPanel* m_panel61;
		wxCheckBox* m_checkBoxPrevent;
		wxCheckBox* m_checkBoxFixRAlt;
		wxButton* m_buttonAddPanel;
		wxButton* m_buttonDelAllPanels;
		wxPanel* m_panel6;
		wxStaticText* m_staticTextBuildDate;
		wxHyperlinkCtrl* m_hyperlink1;
		wxHyperlinkCtrl* m_hyperlink12;
		wxButton* m_button1;
		wxButton* m_button2;

		// Virtual event handlers, override them in your derived class
		virtual void onEnable( wxCommandEvent& event ) { event.Skip(); }
		virtual void onAutocheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void onDisableAccessebl( wxCommandEvent& event ) { event.Skip(); }
		virtual void onEnableLog( wxCommandEvent& event ) { event.Skip(); }
		virtual void onHotDClick( wxGridEvent& event ) { event.Skip(); }
		virtual void on_grid_lay_double( wxGridEvent& event ) { event.Skip(); }
		virtual void onRemapCaps( wxCommandEvent& event ) { event.Skip(); }
		virtual void onUpdateRemap( wxCommandEvent& event ) { event.Skip(); }
		virtual void onDeleteRemap( wxCommandEvent& event ) { event.Skip(); }
		virtual void onExit( wxCommandEvent& event ) { event.Skip(); }
		virtual void onCloseToTray( wxCommandEvent& event ) { event.Skip(); }


	public:

		MyFrame4( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("SimpleSwitcher"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~MyFrame4();

};

///////////////////////////////////////////////////////////////////////////////
/// Class MyDialog1
///////////////////////////////////////////////////////////////////////////////
class MyDialog1 : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticText10;
		wxChoice* m_choiceKey;
		wxButton* m_button6;
		wxCheckBox* m_checkBox12;
		wxCheckBox* m_checkBox13;
		wxCheckBox* m_checkBoxDouble;

		// Virtual event handlers, override them in your derived class
		virtual void OnChoiceSelect( wxCommandEvent& event ) { event.Skip(); }
		virtual void onclear( wxCommandEvent& event ) { event.Skip(); }
		virtual void onSetLeftRight( wxCommandEvent& event ) { event.Skip(); }
		virtual void onSetKeyup( wxCommandEvent& event ) { event.Skip(); }


	public:

		MyDialog1( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Set hotkey"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxSTAY_ON_TOP );

		~MyDialog1();

};

///////////////////////////////////////////////////////////////////////////////
/// Class FloatPanelBase
///////////////////////////////////////////////////////////////////////////////
class FloatPanelBase : public wxFrame
{
	private:

	protected:
		wxStaticBitmap* m_bitmap1;

	public:

		FloatPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 32,32 ), long style = wxSTAY_ON_TOP|wxBORDER_NONE );

		~FloatPanelBase();

};

