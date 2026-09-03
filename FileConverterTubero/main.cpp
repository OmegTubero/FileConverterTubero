#include <wx/wx.h>
#include "MainMenu.hpp"

class ConvertitoreTubero : public wxApp {
	public:
		virtual bool OnInit() {
			MainWindow* Window = new MainWindow();

			Window->Center();

			Window->Show(true);

			return true;
		}
};

wxIMPLEMENT_APP(ConvertitoreTubero);