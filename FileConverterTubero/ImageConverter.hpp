#pragma once

#include <wx/wx.h>
#include <wx/filedlg.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
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

		wxButton* btnConverterSelectFile = new wxButton(this, wxID_ANY, "Seleziona file", wxPoint(), wxSize(120, 50));

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
	wxChoice* ConvertTo;

	FREE_IMAGE_FORMAT OttieniFormatoSorgente(const wxString& estensione) {
		wxString est = estensione.Upper();

		if (est == "PNG") return FIF_PNG;
		if (est == "JPG" || est == "JPEG") return FIF_JPEG;
		if (est == "BMP") return FIF_BMP;
		if (est == "TIF" || est == "TIFF") return FIF_TIFF;
		if (est == "ICO") return FIF_ICO;
		if (est == "TGA") return FIF_TARGA;
		if (est == "WEBP") return FIF_WEBP;

		return FIF_UNKNOWN;
	}

	void ApriFile(wxCommandEvent& event) { // Apertura del file immagine da manipolare
		wxFileDialog openFileDialog(this, "Seleziona Immagine", "", "", "Immagini (*.png;*.jpg;*.jpeg;*.webp;*.ico;*.bmp;*.tif;*.tiff;*.tga)|*.bmp;*.jpg;*.jpeg;*.png;*.tif;*.tiff;*.ico;*.tga;*.webp", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

		if (openFileDialog.ShowModal() == wxID_OK) {
			wxString InputPath = openFileDialog.GetPath();

			wxString EstensioneScelta = ConvertTo->GetStringSelection();

			FREE_IMAGE_FORMAT FormatoOutput = OttieniFormatoSorgente(EstensioneScelta);

			if (FormatoOutput == FIF_UNKNOWN) {
				wxMessageBox("Formato di destinazione non supportato o non selezionato.", "Errore", wxICON_ERROR);
				return;
			}

			wxFileName PercorsoEseguibile(wxStandardPaths::Get().GetExecutablePath());
			wxString CartellaDestinazione = PercorsoEseguibile.GetPathWithSep() + "Conversioni";

			if (!wxDirExists(CartellaDestinazione)) {

				wxMkdir(CartellaDestinazione);
			}

			wxFileName GestoreInput(InputPath);
			wxString NomeOriginale = GestoreInput.GetName();

			wxFileName GestoreOutput;

			GestoreOutput.SetPath(CartellaDestinazione);

			GestoreOutput.SetName(NomeOriginale);

			GestoreOutput.SetExt(EstensioneScelta.Lower());

			wxString OutputPath = GestoreOutput.GetFullPath();

			FREE_IMAGE_FORMAT FormatoOrigine = FreeImage_GetFileType(InputPath.mb_str(), 0);

			if (FormatoOrigine == FIF_UNKNOWN) {
				FormatoOrigine = FreeImage_GetFIFFromFilename(InputPath.mb_str());
			}

			if (FormatoOrigine != FIF_UNKNOWN && FreeImage_FIFSupportsReading(FormatoOrigine)) {

				FIBITMAP* ImmagineCaricata = FreeImage_Load(FormatoOrigine, InputPath.mb_str(), 0);

				if (ImmagineCaricata) {

					if (ImmagineCaricata) {

						// Salvataggio
						bool successo = FreeImage_Save(FormatoOutput, ImmagineCaricata, OutputPath.mb_str(), 0);

						// Pulizia immediata della memoria
						FreeImage_Unload(ImmagineCaricata);

						// Avviso all'utente
						if (successo) {
							wxMessageBox("Immagine convertita e salvata con successo in:\n" + OutputPath, "Completato", wxICON_INFORMATION);
						}
						else {
							wxMessageBox("Errore durante la scrittura del file convertito.", "Errore", wxICON_ERROR);
						}

					}
					else {
						wxMessageBox("Impossibile allocare in memoria l'immagine originale.", "Errore", wxICON_ERROR);
					}

				}
				else {
					wxMessageBox("Impossibile allocare in memoria l'immagine originale.", "Errore", wxICON_ERROR);
				}
			}
			else {
				wxMessageBox("Formato del file originale non riconosciuto da FreeImage.", "Errore", wxICON_ERROR);
			}
		}
	}

};