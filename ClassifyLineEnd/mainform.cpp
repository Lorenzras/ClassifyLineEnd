#include "mainform.h"
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")//hide console window

using namespace System;
using namespace System::Windows::Forms;

[STAThread]//leave this as is
void main(void) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew ClassifyLineEnd::mainform);
}