// -*- C++ -*- generated by wxGlade 0.3.5.1 on Sun Apr 24 21:57:57 2005

#include <wx/wx.h>
#include <wx/image.h>
// begin wxGlade: ::dependencies
#include <wx/notebook.h>
// end wxGlade


#ifndef SS_GUI_H
#define SS_GUI_H


class MainFrame: public wxFrame {
public:
    // begin wxGlade: MainFrame::ids
    // end wxGlade

    MainFrame(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE);

private:
    // begin wxGlade: MainFrame::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: MainFrame::attributes
    wxMenuBar* frame_1_menubar;
    wxStatusBar* frame_1_statusbar;
    wxStaticBitmap* bitmap_ss_logo;
    wxStaticText* label_2;
    wxStaticText* label_3;
    wxStaticText* label_4;
    wxTextCtrl* text_ctrl_server_addr;
    wxTextCtrl* text_ctrl_last_cmd;
    wxTextCtrl* text_ctrl_status;
    wxPanel* panel_3;
    wxPanel* panel_4;
    wxPanel* notebook_1_pane_1;
    wxTextCtrl* text_ctrl_scan_l1;
    wxTextCtrl* text_ctrl_scan_r1;
    wxTextCtrl* text_ctrl_scan_l2;
    wxTextCtrl* text_ctrl_scan_r2;
    wxTextCtrl* text_ctrl_scan_l3;
    wxTextCtrl* text_ctrl_scan_r3;
    wxTextCtrl* text_ctrl_scan_l4;
    wxTextCtrl* text_ctrl_scan_r4;
    wxTextCtrl* text_ctrl_scan_l5;
    wxTextCtrl* text_ctrl_scan_r5;
    wxTextCtrl* text_ctrl_scan_l6;
    wxTextCtrl* text_ctrl_scan_r6;
    wxTextCtrl* text_ctrl_scan_l7;
    wxTextCtrl* text_ctrl_scan_r7;
    wxTextCtrl* text_ctrl_scan_l8;
    wxTextCtrl* text_ctrl_scan_r8;
    wxPanel* notebook_1_pane_2;
    wxTextCtrl* text_ctrl_raw_r;
    wxPanel* notebook_1_pane_4;
    wxNotebook* notebook_1;
    // end wxGlade
};


#endif // SS_GUI_H
