#pragma once

#include <wx/wx.h>
#include <FreeImage.h>

class ImageConverter : public wxDialog {
	public:
		ImageConverter(wxWindow* MainWindow) : wxDialog(MainWindow, wxID_ANY, "Convertitore Immagini", wxDefaultPosition, wxSize(800, 700)) {

			wxPanel* ConverterPanel = new wxPanel(this, wxID_ANY);

			wxBoxSizer* ConverterSizer = new wxBoxSizer(wxHORIZONTAL);

			ConverterPanel->SetSizer(ConverterSizer);
		}
};