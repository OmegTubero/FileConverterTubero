#pragma once

#include <wx/wx.h>
#include <wx/hyperlink.h>
#include "FrierenCPP_xpm.hpp"

class Info : public wxDialog {
public:
	Info(wxWindow* MainWindow) : wxDialog(MainWindow, wxID_ANY, "Informazioni", wxDefaultPosition, wxSize(400, 200)) {

		wxPanel* PannelloInfo = new wxPanel(this, wxID_ANY);

		wxBoxSizer* SizerImg = new wxBoxSizer(wxHORIZONTAL);

		wxBoxSizer* SizerInfo = new wxBoxSizer(wxVERTICAL);

		wxStaticBitmap* OmegImg = new wxStaticBitmap(PannelloInfo, wxID_ANY, wxBitmap(FrierenCPP_xpm));

		wxHyperlinkCtrl* LinkSitoTubero = new wxHyperlinkCtrl(PannelloInfo, wxID_ANY, "Sito Tubero", "https://tubero.ddns.net");

		wxStaticText* TestoInfo = new wxStaticText(PannelloInfo, wxID_ANY, "Versione 1.0\n\nSviluppato da OmegTubero", wxPoint(0, 0), wxDefaultSize);

		SizerInfo->Add(TestoInfo, 0, wxALL, 5);

		SizerInfo->Add(LinkSitoTubero, 0, wxALL, 5);

		SizerImg->Add(OmegImg, 0, wxALIGN_CENTER_VERTICAL | wxALL, 15);

		SizerImg->Add(SizerInfo, 0, wxALIGN_CENTER_VERTICAL | wxALL, 15);

		PannelloInfo->SetSizer(SizerImg);
	}
};