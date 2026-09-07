#pragma once

#include <wx/wx.h>
#include <wx/filedlg.h>
#include <FreeImage.h>

class ImageConverter : public wxDialog {
	public:
		ImageConverter(wxWindow* MainWindow) : wxDialog(MainWindow, wxID_ANY, "Convertitore Immagini", wxDefaultPosition, wxSize(800, 700)) {

			wxBoxSizer* ConverterSizerV = new wxBoxSizer(wxVERTICAL);

			wxBoxSizer* ConverterSizerH = new wxBoxSizer(wxHORIZONTAL);

			wxFont ConverterTextFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

			wxFont ConvertToTextFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

			wxStaticText* ConverterText = new wxStaticText(this, wxID_ANY, "Converti o scala immagine");

			wxStaticText* ConvertToText = new wxStaticText(this, wxID_ANY, "Converti in:");

			wxButton* btnConverterSelectFile = new wxButton(this, wxID_ANY, "Seleziona file", wxPoint(), wxSize(120,50));

			wxArrayString ConvertToFormats;
			ConvertToFormats.Add("PNG");
			ConvertToFormats.Add("JPG");
			ConvertToFormats.Add("JPEG");
			ConvertToFormats.Add("WEBP");
			ConvertToFormats.Add("ICO");
			ConvertToFormats.Add("BPM");
			ConvertToFormats.Add("TIF");
			ConvertToFormats.Add("TIFF");
			ConvertToFormats.Add("TGA");

			ConvertTo = new wxChoice(this, wxID_ANY, wxPoint(), wxSize(150, 30), ConvertToFormats);

			ConvertTo->SetSelection(0);

			ConvertTo->Bind(wxEVT_CHOICE, &ImageConverter::OnChoice, this);

			ConverterText->SetFont(ConverterTextFont);

			ConvertToText->SetFont(ConvertToTextFont);

			SetSizer(ConverterSizerV);

			ConverterSizerV->Add(ConverterText, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

			ConverterSizerV->Add(btnConverterSelectFile, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 25);

			ConverterSizerV->Add(ConvertToText, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 30);

			ConverterSizerV->Add(ConvertTo, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 5);

			ConverterSizerV->Add(ConverterSizerH, 0, wxALL, 5);

			btnConverterSelectFile->Bind(wxEVT_BUTTON, &ImageConverter::ApriFile, this);
		}

	private:

		void ApriFile(wxCommandEvent& event) { // Apertura del file immagine da manipolare
			wxFileDialog openFileDialog(this, "Seleziona Immagine", "", "", "Immagini (*.png;*.jpg;*.jpeg;*.webp;*.ico;*.bmp;*.tif;*.tiff;*.tga)|*.bmp;*.jpg;*.jpeg;*.png;*.tif;*.tiff;*.ico;*.tga;*.webp", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

			if (openFileDialog.ShowModal() == wxID_OK) {
				wxString inputPath = openFileDialog.GetPath();

				const char* pathStr = inputPath.mb_str();
			}
		}

		wxChoice* ConvertTo;

		void OnChoice(wxCommandEvent& event) {
			int indexSel = ConvertTo->GetSelection();
			wxString testoSel = ConvertTo->GetStringSelection();

			wxLogMessage("Selezionato: %s (Indice: %d)", testoSel, indexSel);
		}

};