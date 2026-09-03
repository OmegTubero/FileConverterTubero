#pragma once

#include <wx/wx.h>
#include "Info.hpp"
#include "ImageConverter.hpp"
#include "ImgConverter_xpm.hpp"

class MainWindow : public wxFrame {
	public:
		MainWindow() : wxFrame(NULL, wxID_ANY, "Convertitore Tubero",wxDefaultPosition, wxSize(900, 800), (wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))) {
			wxPanel* MainWindowPanel = new wxPanel(this, wxID_ANY);

			wxBoxSizer* MainWindowSizer = new wxBoxSizer(wxVERTICAL);

			wxFont WelcomeTextFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

			wxFont ImgConverterFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

			wxButton* btnInfo = new wxButton(MainWindowPanel, wxID_ANY, "?", wxPoint(860, 735), wxSize(20, 20));

			wxStaticText* WelcomeText = new wxStaticText(MainWindowPanel, wxID_ANY, "Benvenuto nel convertitore tubero!");

			wxButton* btnImgConverter = new wxButton(MainWindowPanel, wxID_ANY, "Converti o scala immagine", wxPoint(0, 50), wxSize(850, 80));

			//wxStaticBitmap* ImgConverter = new wxStaticBitmap(MainWindowPanel, wxID_ANY, wxBitmap(FrierenCPP_xpm));

			wxBitmap ImgConverterBitmap(ImgConverter_xpm);

			btnImgConverter->SetBitmap(ImgConverterBitmap);

			btnImgConverter->SetBitmapPosition(wxLEFT);

			btnImgConverter->SetFont(ImgConverterFont);
			
			WelcomeText->SetFont(WelcomeTextFont);

			MainWindowSizer->Add(WelcomeText, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

			MainWindowSizer->Add(btnImgConverter, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

			btnInfo->Bind(wxEVT_BUTTON, &MainWindow::ShowInfo, this);

			btnImgConverter->Bind(wxEVT_BUTTON, &MainWindow::OpenImgConverter, this);

			MainWindowPanel->SetSizer(MainWindowSizer);
		}
	private:
		void ShowInfo(wxCommandEvent& event) {
			Info dlg(this);

			dlg.ShowModal();
		}

		void OpenImgConverter(wxCommandEvent& event) {
			ImageConverter dlg(this);

			dlg.ShowModal();
		}
};