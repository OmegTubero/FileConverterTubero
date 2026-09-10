#include <wx/wx.h>
#include "MainMenu.hpp"

class ConvertitoreTubero : public wxApp {
	public:
		virtual bool OnInit() {

			FreeImage_Initialise();

			MainWindow* Window = new MainWindow();

			Window->Center();

			Window->Show(true);

			return true;
		}
};

wxIMPLEMENT_APP(ConvertitoreTubero);