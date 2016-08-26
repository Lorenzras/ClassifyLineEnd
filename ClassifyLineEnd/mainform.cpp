#include "mainform.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]//leave this as is
void main(void) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew ClassifyLineEnd::mainform);
}