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

		wxStaticText* Larghezza = new wxStaticText(this, wxID_ANY, "Larghezza:");

		wxStaticText* Altezza = new wxStaticText(this, wxID_ANY, "Altezza:");

		

		wxButton* btnConvert = new wxButton(this, wxID_ANY, "Converti", wxPoint(), wxSize(120, 50));

		wxArrayString ConvertToFormats;
		ConvertToFormats.Add("PNG");
		ConvertToFormats.Add("JPG");
		ConvertToFormats.Add("JPEG");
		ConvertToFormats.Add("WEBP");
		ConvertToFormats.Add("ICO");
		ConvertToFormats.Add("BMP");
		ConvertToFormats.Add("TIF");
		ConvertToFormats.Add("TIFF");
		ConvertToFormats.Add("TGA");

		ConvertTo = new wxChoice(this, wxID_ANY, wxPoint(), wxSize(150, 30), ConvertToFormats);

		ConvertTo->SetSelection(0);

		ConverterText->SetFont(ConverterTextFont);

		ConvertToText->SetFont(ConvertToTextFont);

		Altezza->SetFont(ConvertToTextFont);

		Larghezza->SetFont(ConvertToTextFont);

		SetSizer(ConverterSizerV);

		ConverterSizerV->Add(ConverterText, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

		ConverterSizerV->Add(btnConverterSelectFile, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 25);

		ConverterSizerV->Add(ConvertToText, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 30);

		ConverterSizerV->Add(ConvertTo, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 5);

		ConverterSizerV->Add(Altezza, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 30);

		ConverterSizerV->Add(SizeW, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 5);

		ConverterSizerV->Add(Larghezza, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 30);

		ConverterSizerV->Add(SizeH, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 5);

		ConverterSizerV->Add(btnConvert, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 30);

		ConverterSizerV->Add(ConverterSizerH, 0, wxALL, 5);

		btnConverterSelectFile->Bind(wxEVT_BUTTON, &ImageConverter::ApriFile, this);

		btnConvert->Bind(wxEVT_BUTTON, &ImageConverter::Converti, this);
	}

private:
	wxChoice* ConvertTo;
	wxTextCtrl* SizeW = new wxTextCtrl(this, wxID_ANY, "", wxPoint(), wxSize(150, 25), 0, wxTextValidator(wxFILTER_NUMERIC));
	wxTextCtrl* SizeH = new wxTextCtrl(this, wxID_ANY, "", wxPoint(), wxSize(150, 25), 0, wxTextValidator(wxFILTER_NUMERIC));
	wxString InputPath;

	FREE_IMAGE_FORMAT OttieniFormatoSorgente(const wxString& estensione) {
		wxString est = estensione.Upper();

		if (est == "PNG") return FIF_PNG;
		if (est == "JPG" || est == "JPEG") return FIF_JPEG;
		if (est == "WEBP") return FIF_WEBP;
		if (est == "ICO") return FIF_ICO;
		if (est == "BMP") return FIF_BMP;
		if (est == "TIF" || est == "TIFF") return FIF_TIFF;
		if (est == "TGA") return FIF_TARGA;

		return FIF_UNKNOWN;
	}

	void ApriFile(wxCommandEvent& event) { // Apertura del file immagine da manipolare
		wxFileDialog openFileDialog(this, "Seleziona Immagine", "", "", "Immagini (*.png;*.jpg;*.jpeg;*.webp;*.ico;*.bmp;*.tif;*.tiff;*.tga)|*.bmp;*.jpg;*.jpeg;*.png;*.tif;*.tiff;*.ico;*.tga;*.webp", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

		if (openFileDialog.ShowModal() == wxID_OK) {
			InputPath = openFileDialog.GetPath();

			FIBITMAP* ImmaginePerDimensioni = FreeImage_Load(FreeImage_GetFIFFromFilename(InputPath.mb_str()), InputPath.mb_str(), 0);

			SizeW->SetValue(std::to_string(FreeImage_GetWidth(ImmaginePerDimensioni)));
			SizeH->SetValue(std::to_string(FreeImage_GetHeight(ImmaginePerDimensioni)));
		}
	}

	void Converti(wxCommandEvent& event) {
		if (InputPath.IsEmpty()) {
			wxMessageBox("Seleziona prima un file da convertire", "Attenzione", wxICON_WARNING);
			return;
		}

		int W = wxAtoi(SizeW->GetValue());

		int H = wxAtoi(SizeH->GetValue());

		if (W && H <= 0) {
			wxMessageBox("La dimensione dell'immagine non puo' essere inferiore a 1x1", "Errore", wxICON_ERROR);
			return;
		}

		wxString EstensioneScelta = ConvertTo->GetStringSelection();

		if (EstensioneScelta == "ICO" && W*H > 65536) {
			wxMessageBox("La dimensione di un file .ICO non puo' essere superiore a 256x256", "Errore", wxICON_ERROR);
			return;
		}

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

			if (FormatoOutput == FIF_WEBP) {
				unsigned bpp = FreeImage_GetBPP(ImmagineCaricata);

				if (bpp != 24 && bpp != 32) {
					if (FreeImage_IsTransparent(ImmagineCaricata)) {
						ImmagineCaricata = FreeImage_ConvertTo32Bits(ImmagineCaricata);
					}
					else {
						ImmagineCaricata = FreeImage_ConvertTo24Bits(ImmagineCaricata);
					}
				}
			}

			int Larghezza = wxAtoi(SizeW->GetValue());

			int Altezza = wxAtoi(SizeH->GetValue());

			ImmagineCaricata = RidimensionaImmagine(ImmagineCaricata, Larghezza, Altezza);

			if (ImmagineCaricata) {

				if (ImmagineCaricata) {

					bool successo = FreeImage_Save(FormatoOutput, ImmagineCaricata, OutputPath.mb_str(), 0);

					FreeImage_Unload(ImmagineCaricata);

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

	FIBITMAP* RidimensionaImmagine(FIBITMAP* sorgente, int nuovaLarghezza, int nuovaAltezza, bool mantieniProporzioni = true) {
		if (!sorgente || nuovaLarghezza <= 0 || nuovaAltezza <= 0) return nullptr;

		int larghezzaOriginale = FreeImage_GetWidth(sorgente);
		int altezzaOriginale = FreeImage_GetHeight(sorgente);

		int targetW = nuovaLarghezza;
		int targetH = nuovaAltezza;

		if (mantieniProporzioni) {
			double rapporto = static_cast<double>(altezzaOriginale) / larghezzaOriginale;
			targetH = static_cast<int>(nuovaLarghezza * rapporto);
		}

		
		if (targetW == larghezzaOriginale && targetH == altezzaOriginale) {
			return FreeImage_Clone(sorgente);
		}

		return FreeImage_Rescale(sorgente, targetW, targetH, FILTER_LANCZOS3);
	}

};