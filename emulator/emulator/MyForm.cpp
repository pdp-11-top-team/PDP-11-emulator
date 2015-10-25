#include "MyForm.h"
#include "pdp/emulator.c"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
void Main(array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	emulator::MyForm form;
	emu_init();
	Application::Run(%form);
}
