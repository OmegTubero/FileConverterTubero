#pragma once

#include <wx/wx.h>
#include <wx/filedlg.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <FreeImage.h>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>

class ImageConverter : public wxDialog {
public:
	ImageConverter(wxWindow* MainWindow) : wxDialog(MainWindow, wxID_ANY, "Conversione e scalatura immagini", wxDefaultPosition, wxSize(800, 700)) { // Creazione finestra principale del wxDialog

		wxBoxSizer* ConverterSizerV = new wxBoxSizer(wxVERTICAL); // Creazione del sizer verticale

		wxFont ConverterTextFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL); // Font per il ConverterText

		wxFont ConvertToTextFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL); // Font per le etichette

		wxStaticText* ConverterText = new wxStaticText(this, wxID_ANY, "Converti o scala immagine");

		wxStaticText* ConvertToText = new wxStaticText(this, wxID_ANY, "Converti in:");

		wxButton* btnConverterSelectFile = new wxButton(this, wxID_ANY, "Seleziona file", wxPoint(), wxSize(120, 50)); // Tasto di selezione del file da convertire

		wxStaticText* Larghezza = new wxStaticText(this, wxID_ANY, "Larghezza:");

		wxStaticText* Altezza = new wxStaticText(this, wxID_ANY, "Altezza:");

		wxButton* btnConvert = new wxButton(this, wxID_ANY, "Converti", wxPoint(), wxSize(120, 50)); // Tasto per avviare la conversione
		
		// Array per la drop down box con i formati
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

		ConvertTo = new wxChoice(this, wxID_ANY, wxPoint(), wxSize(150, 30), ConvertToFormats); // Choice per la selezione del formato di output della conversione

		ConvertTo->SetSelection(0);
		// Assegnazione dei font
		ConverterText->SetFont(ConverterTextFont);

		ConvertToText->SetFont(ConvertToTextFont);

		Altezza->SetFont(ConvertToTextFont);

		Larghezza->SetFont(ConvertToTextFont);

		SetSizer(ConverterSizerV); // Impostazione sizer per la finestra
		// Aggiunta dei vari elementi al sizer
		ConverterSizerV->Add(ConverterText, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

		ConverterSizerV->Add(btnConverterSelectFile, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 25);

		ConverterSizerV->Add(ConvertToText, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 30);

		ConverterSizerV->Add(ConvertTo, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 5);

		ConverterSizerV->Add(Altezza, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 30);

		ConverterSizerV->Add(SizeW, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 5);

		ConverterSizerV->Add(Larghezza, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 30);

		ConverterSizerV->Add(SizeH, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 5);

		ConverterSizerV->Add(btnConvert, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 30);
		// Assegno il focus al tasto di selezione del file
		btnConverterSelectFile->SetFocus();
		// Assegnazione eventi ai bottoni
		btnConverterSelectFile->Bind(wxEVT_BUTTON, &ImageConverter::ApriFile, this);

		btnConvert->Bind(wxEVT_BUTTON, &ImageConverter::Converti, this);

		ConvertTo->Bind(wxEVT_CHOICE, &ImageConverter::CambiaDimensione, this);
	}

private:
	wxTextCtrl* SizeW = new wxTextCtrl(this, wxID_ANY, "", wxPoint(), wxSize(150, 25), 0, wxTextValidator(wxFILTER_NUMERIC)); // Casella di testo per la risoluzione orrizontale
	wxTextCtrl* SizeH = new wxTextCtrl(this, wxID_ANY, "", wxPoint(), wxSize(150, 25), 0, wxTextValidator(wxFILTER_NUMERIC)); // Casella di testo per la risoluzione verticale
	wxChoice* ConvertTo; 
	wxString InputPath; // Variabile per conservare il percorso del file letto
	bool PressedAtLeastOnce = false;

	FREE_IMAGE_FORMAT OttieniFormatoSorgente(const wxString& estensione) { // Funzione per ottenere il formato del file letto
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

	void CambiaDimensione(wxCommandEvent& event) { // Controllo della dimensione per il cambio dei valori nelle text box
		if (PressedAtLeastOnce) {
			FIBITMAP* ImmaginePerDimensioni = FreeImage_Load(FreeImage_GetFIFFromFilename(InputPath.mb_str()), InputPath.mb_str(), 0); // Creazione di una variaabile FITBITMAP

			int Wi = FreeImage_GetWidth(ImmaginePerDimensioni);

			int He = FreeImage_GetHeight(ImmaginePerDimensioni);

			if (ConvertTo->GetStringSelection() == "ICO" && Wi * He > 65536) {
				SizeW->SetValue(std::to_string(256));
				SizeH->SetValue(std::to_string(256));
			}
			else {
				SizeW->SetValue(std::to_string(FreeImage_GetWidth(ImmaginePerDimensioni)));
				SizeH->SetValue(std::to_string(FreeImage_GetHeight(ImmaginePerDimensioni)));
			}
		}
	}

	void ApriFile(wxCommandEvent& event) { // Funzione di apertura del file immagine da manipolare
		// Apro il file explorer per selezionare il file
		wxFileDialog openFileDialog(this, "Seleziona Immagine", "", "", "Immagini (*.png;*.jpg;*.jpeg;*.webp;*.ico;*.bmp;*.tif;*.tiff;*.tga)|*.bmp;*.jpg;*.jpeg;*.png;*.tif;*.tiff;*.ico;*.tga;*.webp", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

		if (openFileDialog.ShowModal() == wxID_OK) { // Se l'utente preme Ok mette il percorso del file nella variabile InputPath
			InputPath = openFileDialog.GetPath();

			PressedAtLeastOnce = true;

			FIBITMAP* ImmaginePerDimensioni = FreeImage_Load(FreeImage_GetFIFFromFilename(InputPath.mb_str()), InputPath.mb_str(), 0); // Creazione di una variaabile FITBITMAP

			int Wi = FreeImage_GetWidth(ImmaginePerDimensioni);

			int He = FreeImage_GetHeight(ImmaginePerDimensioni);

			// Prendo i valori di dimensione dalla variabile appena creata per assegnarli alle caselle di testo
			if (ConvertTo->GetStringSelection() == "ICO" && Wi * He > 65536) {
				SizeW->SetValue(std::to_string(256));
				SizeH->SetValue(std::to_string(256));
			}
			else {
				SizeW->SetValue(std::to_string(FreeImage_GetWidth(ImmaginePerDimensioni)));
				SizeH->SetValue(std::to_string(FreeImage_GetHeight(ImmaginePerDimensioni)));
			}
		}
	}

	void Converti(wxCommandEvent& event) { // Funzione per la conversione del file immagine
		if (InputPath.IsEmpty()) {
			wxMessageBox("Seleziona prima un file da convertire", "Attenzione", wxICON_WARNING);
			return;
		}

		int W = wxAtoi(SizeW->GetValue()); // Effettuo la conversione da wsString a intero

		int H = wxAtoi(SizeH->GetValue());

		if (W && H <= 0) { // Controllo di validità della dimensione dell'immagine
			wxMessageBox("La dimensione dell'immagine non puo' essere inferiore a 1x1", "Errore", wxICON_ERROR);
			return;
		}

		wxString EstensioneScelta = ConvertTo->GetStringSelection(); // Prende la selezione dalla choice

		if (EstensioneScelta == "ICO" && W*H > 65536) { // Controllo se in caso si sia scelto di convertire in un file .ICO le dimensioni non siano superiori a 256x256
			wxMessageBox("La dimensione di un file .ICO non puo' essere superiore a 256x256", "Errore", wxICON_ERROR);
			return;
		}

		FREE_IMAGE_FORMAT FormatoOutput = OttieniFormatoSorgente(EstensioneScelta); // Creo una variaible di formato di FreeImage per potergli assegnare come valore l'estensione del file letto

		if (FormatoOutput == FIF_UNKNOWN) { // Se il formato non è riconosciuto blocco la conversione
			wxMessageBox("Formato di destinazione non supportato o non selezionato.", "Errore", wxICON_ERROR);
			return;
		}

		wxFileName PercorsoEseguibile(wxStandardPaths::Get().GetExecutablePath()); // Creo una variabile wxFileName che scompone i percorsi dei file e gli assegno il percorso dell'eseguibile del programma

		wxString CartellaDestinazione = PercorsoEseguibile.GetPathWithSep() + "Conversioni"; // Assegno alla variabile il percorso e aggiungendo alla fine un \ con la funzione GetPathWithSep() il nome della cartella da creare o aprire

		if (!wxDirExists(CartellaDestinazione)) { // Se la cartella non esiste viene creata

			wxMkdir(CartellaDestinazione);
		}

		wxFileName GestoreInput(InputPath);

		wxString NomeOriginale = GestoreInput.GetName();

		wxFileName GestoreOutput;

		GestoreOutput.SetPath(CartellaDestinazione); // Assegno il percorso di output alla variabile

		GestoreOutput.SetName(NomeOriginale); // Assegno il nome di output dell'immagine

		GestoreOutput.SetExt(EstensioneScelta.Lower()); // Assegno l'estensione di output scelta convertita in lowercase

		wxString OutputPath = GestoreOutput.GetFullPath(); // Assegno il percorso completo alla variabile OutputPath

		FREE_IMAGE_FORMAT FormatoOrigine = FreeImage_GetFileType(InputPath.mb_str(), 0); // Prendo il formato del file letto

		if (FormatoOrigine == FIF_UNKNOWN) {
			FormatoOrigine = FreeImage_GetFIFFromFilename(InputPath.mb_str()); // Controllo dei byte per controllare la vera estensione del file
		}

		if (FormatoOrigine != FIF_UNKNOWN && FreeImage_FIFSupportsReading(FormatoOrigine)) {

			FIBITMAP* ImmagineCaricata = FreeImage_Load(FormatoOrigine, InputPath.mb_str(), 0); // Carico l'immagine in memoria

			if (FormatoOutput == FIF_WEBP) { // Controllo se è un WEBP
				unsigned bpp = FreeImage_GetBPP(ImmagineCaricata);

				if (bpp != 24 && bpp != 32) { // Conversione della profondità a 32 o 24 bit per evitare il crash del programma
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

			ImmagineCaricata = RidimensionaImmagine(ImmagineCaricata, Larghezza, Altezza); // Ridimensionamento imamgine con funzione apposita

			if (ImmagineCaricata) {

				if (ImmagineCaricata) {

					bool successo = FreeImage_Save(FormatoOutput, ImmagineCaricata, OutputPath.mb_str(), 0);

					FreeImage_Unload(ImmagineCaricata);

					if (successo) {
						wxMessageBox("Immagine convertita con successo" + OutputPath, "Completato", wxICON_INFORMATION);
					}
					else {
						std::remove(OutputPath.mb_str());
						wxMessageBox("Errore durante la scrittura del file convertito.", "Errore", wxICON_ERROR);
					}

				}
				else {
					std::remove(OutputPath.mb_str());
					wxMessageBox("Impossibile allocare in memoria l'immagine originale.", "Errore", wxICON_ERROR);
				}

			}
			else {
				std::remove(OutputPath.mb_str());
				wxMessageBox("Impossibile allocare in memoria l'immagine originale.", "Errore", wxICON_ERROR);
			}
		}
		else {
			std::remove(OutputPath.mb_str());
			wxMessageBox("Formato del file originale non riconosciuto da FreeImage.", "Errore", wxICON_ERROR);
		}
	}

	FIBITMAP* RidimensionaImmagine(FIBITMAP* sorgente, int nuovaLarghezza, int nuovaAltezza, bool mantieniProporzioni = true) { // Funzione per il ridimensionamento dell'immagine
		if (!sorgente || nuovaLarghezza <= 0 || nuovaAltezza <= 0) return nullptr;

		int larghezzaOriginale = FreeImage_GetWidth(sorgente);
		int altezzaOriginale = FreeImage_GetHeight(sorgente);

		int targetW = nuovaLarghezza;
		int targetH = nuovaAltezza;

		if (mantieniProporzioni) { // Calcoli per mantenere le proporzioni dell'immagine
			double rapporto = static_cast<double>(altezzaOriginale) / larghezzaOriginale;
			targetH = static_cast<int>(nuovaLarghezza * rapporto);
		}

		
		if (targetW == larghezzaOriginale && targetH == altezzaOriginale) { // Se le dimensioni sono uguali si limita a effetture una copia dell'originale
			return FreeImage_Clone(sorgente);
		}

		return FreeImage_Rescale(sorgente, targetW, targetH, FILTER_LANCZOS3); // Scalamento dell'immagine con il LANCZOS3 per la qualità più alta
	}

};