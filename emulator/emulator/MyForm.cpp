#include "MyForm.h"
extern "C" {
	#include "pdp\emulator.c"
}

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	emulator::MyForm form;
	emu_init();
	Application::Run(%form);

	return 0;
}
