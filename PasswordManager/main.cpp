#include "stdafx.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
void main()
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	PasswordManager::PasswordManager form;
	Application::Run(%form);
}