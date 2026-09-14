#pragma once

#include <wx/wx.h>
#include <sndfile.h>
#include <wx/filedlg.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>

class AudioConverter : public wxDialog {
public:
	AudioConverter(wxWindow* MainWindow) : wxDialog(MainWindow, wxID_ANY, "Conversione audio", wxDefaultPosition, wxSize(800, 700)) { // Creazione finestra principale del wxDialog

		wxBoxSizer* ConverterSizerV = new wxBoxSizer(wxVERTICAL); // Creazione del sizer verticale

		wxFont ConverterTextFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL); // Font per il ConverterText

		wxFont ConvertToTextFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL); // Font per le etichette

		wxStaticText* ConverterText = new wxStaticText(this, wxID_ANY, "Conversione audio");

		wxStaticText* ConvertToText = new wxStaticText(this, wxID_ANY, "Converti in:");

		wxButton* btnConverterSelectFile = new wxButton(this, wxID_ANY, "Seleziona file", wxPoint(), wxSize(120, 50)); // Tasto di selezione del file da convertire

		wxButton* btnConvert = new wxButton(this, wxID_ANY, "Converti", wxPoint(), wxSize(120, 50)); // Tasto per avviare la conversione

		// Array per la drop down box con i formati
		wxArrayString ConvertToFormats;
		ConvertToFormats.Add("MP3");
		ConvertToFormats.Add("WAV");
		ConvertToFormats.Add("OGG");

		ConvertTo = new wxChoice(this, wxID_ANY, wxPoint(), wxSize(150, 30), ConvertToFormats); // Choice per la selezione del formato di output della conversione

		ConvertTo->SetSelection(0);
		// Assegnazione dei font
		ConverterText->SetFont(ConverterTextFont);

		ConvertToText->SetFont(ConvertToTextFont);

		SetSizer(ConverterSizerV); // Impostazione sizer per la finestra
		// Aggiunta dei vari elementi al sizer
		ConverterSizerV->Add(ConverterText, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

		ConverterSizerV->Add(btnConverterSelectFile, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 25);

		ConverterSizerV->Add(ConvertToText, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 30);

		ConverterSizerV->Add(ConvertTo, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 5);

		ConverterSizerV->Add(btnConvert, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 30);
		// Assegno il focus al tasto di selezione del file
		btnConverterSelectFile->SetFocus();
		// Assegnazione eventi ai bottoni
		btnConverterSelectFile->Bind(wxEVT_BUTTON, &AudioConverter::ApriFile, this);

		btnConvert->Bind(wxEVT_BUTTON, &AudioConverter::Converti, this);

		//ConvertTo->Bind(wxEVT_CHOICE, &AudioConverter::CambiaDimensione, this);
	}

private:
	wxChoice* ConvertTo;
	wxString InputPath; // Variabile per conservare il percorso del file letto

	void ApriFile(wxCommandEvent& event) { // Funzione di apertura del file immagine da manipolare
		// Apro il file explorer per selezionare il file
		wxFileDialog openFileDialog(this, "Seleziona Audio", "", "", "Audio (*.mp3;*.wav;*.ogg)|*.mp3;*.wav;*.ogg", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

		if (openFileDialog.ShowModal() == wxID_OK) { // Se l'utente preme Ok mette il percorso del file nella variabile InputPath
			InputPath = openFileDialog.GetPath();
		}
	}

    void Converti(wxCommandEvent& event) {
        if (InputPath.IsEmpty()) {
            wxMessageBox("Seleziona prima un file audio da convertire.", "Errore", wxOK | wxICON_ERROR, this);
            return;
        }

        wxString formatoSelezionato = ConvertTo->GetStringSelection();
        if (formatoSelezionato.IsEmpty()) {
            wxMessageBox("Seleziona un formato di destinazione.", "Errore", wxOK | wxICON_ERROR, this);
            return;
        }

        wxString estensione = formatoSelezionato.Lower();

        // 1. Estraggo il nome del file originale senza estensione
        wxFileName fnInput(InputPath);
        wxString nomeOriginale = fnInput.GetName(); // Es: da "C:\Musica\canzone.mp3" ricava "canzone"

        // 2. Percorso della cartella di destinazione "Conversioni/Audio"
        wxFileName PercorsoEseguibile(wxStandardPaths::Get().GetExecutablePath());
        wxString CartellaDestinazione = PercorsoEseguibile.GetPathWithSep() + "Conversioni" + wxFileName::GetPathSeparator() + "Audio";

        // Creazione fisica della gerarchia di cartelle annidate se non esiste
        if (!wxDirExists(CartellaDestinazione)) {
            wxFileName::Mkdir(CartellaDestinazione, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
        }

        // 3. Percorso COMPLETO con il nome originale e la nuova estensione
        wxString OutputPath = CartellaDestinazione + wxFileName::GetPathSeparator() + nomeOriginale + "." + estensione;

        // 4. Apertura del file di origine (con supporto Unicode per Windows)
        SF_INFO sfinfoIn = { 0 };

        #ifdef __WXMSW__
                SNDFILE* inFile = sf_wchar_open(InputPath.wc_str(), SFM_READ, &sfinfoIn);
        #else
                SNDFILE* inFile = sf_open(InputPath.utf8_str(), SFM_READ, &sfinfoIn);
        #endif

        if (!inFile) {
            wxMessageBox("Impossibile aprire il file di origine.", "Errore", wxOK | wxICON_ERROR, this);
            return;
        }

        // 5. Inizializzazione pulita della struttura SF_INFO per il file di destinazione
        SF_INFO sfinfoOut = { 0 };
        sfinfoOut.samplerate = sfinfoIn.samplerate;
        sfinfoOut.channels = sfinfoIn.channels;

        if (estensione == "wav") {
            sfinfoOut.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
        }
        else if (estensione == "ogg") {
            sfinfoOut.format = SF_FORMAT_OGG | SF_FORMAT_VORBIS;
        }
        else if (estensione == "mp3") {
            sfinfoOut.format = SF_FORMAT_MPEG | SF_FORMAT_MPEG_LAYER_III;
        }
        else {
            sf_close(inFile);
            wxMessageBox("Formato di output non supportato.", "Errore", wxOK | wxICON_ERROR, this);
            return;
        }

        // Controllo di validità dei parametri audio per il codec
        if (!sf_format_check(&sfinfoOut)) {
            sf_close(inFile);
            wxMessageBox("La combinazione di parametri audio non è supportata dal codec selezionato.", "Errore Codec", wxOK | wxICON_ERROR, this);
            return;
        }

        // 6. Apertura del file di output (con supporto Unicode per Windows)
        #ifdef __WXMSW__
                SNDFILE* outFile = sf_wchar_open(OutputPath.wc_str(), SFM_WRITE, &sfinfoOut);
        #else
                SNDFILE* outFile = sf_open(OutputPath.utf8_str(), SFM_WRITE, &sfinfoOut);
        #endif

        if (!outFile) {
            sf_close(inFile);
            wxString erroreNativo = wxString::FromUTF8(sf_strerror(NULL));
            wxMessageBox("Impossibile creare il file di output.\nDettaglio errore: " + erroreNativo + "\nPercorso: " + OutputPath, "Errore", wxOK | wxICON_ERROR, this);
            return;
        }

        // 7. Ciclo di lettura e scrittura dei campioni PCM
        std::vector<float> buffer(1024 * sfinfoIn.channels);
        sf_count_t framesRead = 0;

        while ((framesRead = sf_readf_float(inFile, buffer.data(), 1024)) > 0) {
            sf_writef_float(outFile, buffer.data(), framesRead);
        }

        sf_close(inFile);
        sf_close(outFile);

        wxMessageBox("Audio convertito con successo e salvato nella cartella Conversioni/Audio presente nel percorso del programma", "Completato", wxOK | wxICON_INFORMATION, this);
    }
};