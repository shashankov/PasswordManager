#pragma once
#include "stdafx.h"
#include <string>
#include <time.h>
#include <vector>
#include <stdlib.h>

namespace PasswordManager {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace std;

	/// <summary>
	/// Summary for PasswordManager
	/// </summary>
	public ref class PasswordManager : public System::Windows::Forms::Form
	{
	private:
		bool newUser = false;
		unsigned char* passwordSHA = NULL;
		unsigned char* usernameSHA = NULL;
		int *searchIndices = NULL;
		int currentUserIndex = -1;

	private: System::Windows::Forms::ListBox^  listBoxRules;
	private: System::Windows::Forms::Label^  labelAddMessage;
	private: System::Windows::Forms::Panel^  panel3;
	private: System::Windows::Forms::TextBox^  textBoxShowUsername;
	private: System::Windows::Forms::Button^  buttonCopyPassword;

	private: System::Windows::Forms::Button^  buttonCopyUsername;

	private: System::Windows::Forms::TextBox^  textBoxShowPassword;
	private: System::Windows::Forms::Button^  buttonShowClose;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Button^  buttonShowShowPassword;

	public:
		PasswordManager(void)
		{
			srand(time(NULL));
			InitializeComponent();
			this->listBoxRules->SetSelected(0, true);
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~PasswordManager()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  buttonSign;
	private: System::Windows::Forms::CheckBox^  checkBoxNewUser;
	protected:

	private: System::Windows::Forms::Label^  labelUserID;
	private: System::Windows::Forms::Label^  labelPassword;
	private: System::Windows::Forms::Label^  labelMessage;
	private: System::Windows::Forms::TextBox^  textBoxUserID;

	private: System::Windows::Forms::TextBox^  textBoxPassword;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  buttonSignOut;
	private: System::Windows::Forms::ToolTip^  toolTip1;
	private: System::Windows::Forms::DataGridView^  dataGridView;



	private: System::Windows::Forms::TextBox^  textBoxSearch;




	private: System::Windows::Forms::TextBox^  textBoxAddUsername;

	private: System::Windows::Forms::TextBox^  textBoxAddDescription;
	private: System::Windows::Forms::TextBox^  textBoxAddPassword;
	private: System::Windows::Forms::Button^  buttonAdd;




	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;


	private: System::Windows::Forms::Button^  buttonShowNewPassword;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::MaskedTextBox^  maskedTextBoxRandomLength;

	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label6;

	private: System::Windows::Forms::Button^  buttonRandomGenerate;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::GroupBox^  groupBox3;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColUserName;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  colDescription;
	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::Button^  buttonSearch;
	private: System::Windows::Forms::Button^  buttonShow;
	private: System::Windows::Forms::Button^  buttonDelete;
	private: System::Windows::Forms::Button^  buttonSave;



	private: System::ComponentModel::IContainer^  components;



	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(PasswordManager::typeid));
			this->buttonSign = (gcnew System::Windows::Forms::Button());
			this->checkBoxNewUser = (gcnew System::Windows::Forms::CheckBox());
			this->labelUserID = (gcnew System::Windows::Forms::Label());
			this->labelPassword = (gcnew System::Windows::Forms::Label());
			this->labelMessage = (gcnew System::Windows::Forms::Label());
			this->textBoxUserID = (gcnew System::Windows::Forms::TextBox());
			this->textBoxPassword = (gcnew System::Windows::Forms::TextBox());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->buttonSignOut = (gcnew System::Windows::Forms::Button());
			this->toolTip1 = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->buttonShowNewPassword = (gcnew System::Windows::Forms::Button());
			this->dataGridView = (gcnew System::Windows::Forms::DataGridView());
			this->ColUserName = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->colDescription = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->textBoxSearch = (gcnew System::Windows::Forms::TextBox());
			this->textBoxAddUsername = (gcnew System::Windows::Forms::TextBox());
			this->textBoxAddDescription = (gcnew System::Windows::Forms::TextBox());
			this->textBoxAddPassword = (gcnew System::Windows::Forms::TextBox());
			this->buttonAdd = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->listBoxRules = (gcnew System::Windows::Forms::ListBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->buttonRandomGenerate = (gcnew System::Windows::Forms::Button());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->maskedTextBoxRandomLength = (gcnew System::Windows::Forms::MaskedTextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->labelAddMessage = (gcnew System::Windows::Forms::Label());
			this->buttonSearch = (gcnew System::Windows::Forms::Button());
			this->buttonDelete = (gcnew System::Windows::Forms::Button());
			this->buttonShow = (gcnew System::Windows::Forms::Button());
			this->buttonSave = (gcnew System::Windows::Forms::Button());
			this->panel3 = (gcnew System::Windows::Forms::Panel());
			this->buttonShowShowPassword = (gcnew System::Windows::Forms::Button());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->buttonShowClose = (gcnew System::Windows::Forms::Button());
			this->buttonCopyPassword = (gcnew System::Windows::Forms::Button());
			this->buttonCopyUsername = (gcnew System::Windows::Forms::Button());
			this->textBoxShowPassword = (gcnew System::Windows::Forms::TextBox());
			this->textBoxShowUsername = (gcnew System::Windows::Forms::TextBox());
			this->panel1->SuspendLayout();
			this->groupBox3->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView))->BeginInit();
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->panel2->SuspendLayout();
			this->panel3->SuspendLayout();
			this->SuspendLayout();
			// 
			// buttonSign
			// 
			this->buttonSign->Location = System::Drawing::Point(165, 150);
			this->buttonSign->Margin = System::Windows::Forms::Padding(4);
			this->buttonSign->Name = L"buttonSign";
			this->buttonSign->Size = System::Drawing::Size(100, 28);
			this->buttonSign->TabIndex = 0;
			this->buttonSign->Text = L"Sign In";
			this->buttonSign->UseVisualStyleBackColor = true;
			this->buttonSign->Click += gcnew System::EventHandler(this, &PasswordManager::buttonSign_Click);
			// 
			// checkBoxNewUser
			// 
			this->checkBoxNewUser->AutoSize = true;
			this->checkBoxNewUser->Location = System::Drawing::Point(43, 155);
			this->checkBoxNewUser->Margin = System::Windows::Forms::Padding(4);
			this->checkBoxNewUser->Name = L"checkBoxNewUser";
			this->checkBoxNewUser->Size = System::Drawing::Size(91, 21);
			this->checkBoxNewUser->TabIndex = 1;
			this->checkBoxNewUser->Text = L"New User";
			this->checkBoxNewUser->UseVisualStyleBackColor = true;
			this->checkBoxNewUser->CheckedChanged += gcnew System::EventHandler(this, &PasswordManager::checkBoxNewUser_CheckedChanged);
			// 
			// labelUserID
			// 
			this->labelUserID->AutoSize = true;
			this->labelUserID->Location = System::Drawing::Point(44, 70);
			this->labelUserID->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->labelUserID->Name = L"labelUserID";
			this->labelUserID->Size = System::Drawing::Size(55, 17);
			this->labelUserID->TabIndex = 2;
			this->labelUserID->Text = L"User ID";
			// 
			// labelPassword
			// 
			this->labelPassword->AutoSize = true;
			this->labelPassword->Location = System::Drawing::Point(39, 111);
			this->labelPassword->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->labelPassword->Name = L"labelPassword";
			this->labelPassword->Size = System::Drawing::Size(69, 17);
			this->labelPassword->TabIndex = 3;
			this->labelPassword->Text = L"Password";
			// 
			// labelMessage
			// 
			this->labelMessage->AutoSize = true;
			this->labelMessage->ForeColor = System::Drawing::Color::Red;
			this->labelMessage->Location = System::Drawing::Point(44, 182);
			this->labelMessage->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->labelMessage->MaximumSize = System::Drawing::Size(256, 0);
			this->labelMessage->MinimumSize = System::Drawing::Size(256, 0);
			this->labelMessage->Name = L"labelMessage";
			this->labelMessage->Size = System::Drawing::Size(256, 17);
			this->labelMessage->TabIndex = 4;
			this->labelMessage->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// textBoxUserID
			// 
			this->textBoxUserID->Location = System::Drawing::Point(133, 66);
			this->textBoxUserID->Margin = System::Windows::Forms::Padding(4);
			this->textBoxUserID->Name = L"textBoxUserID";
			this->textBoxUserID->Size = System::Drawing::Size(132, 22);
			this->textBoxUserID->TabIndex = 5;
			// 
			// textBoxPassword
			// 
			this->textBoxPassword->Location = System::Drawing::Point(133, 110);
			this->textBoxPassword->Margin = System::Windows::Forms::Padding(4);
			this->textBoxPassword->Name = L"textBoxPassword";
			this->textBoxPassword->PasswordChar = '*';
			this->textBoxPassword->Size = System::Drawing::Size(132, 22);
			this->textBoxPassword->TabIndex = 6;
			this->textBoxPassword->UseSystemPasswordChar = true;
			this->textBoxPassword->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &PasswordManager::textBoxPassword_KeyPress);
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->groupBox3);
			this->panel1->Location = System::Drawing::Point(0, 0);
			this->panel1->Margin = System::Windows::Forms::Padding(4);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(724, 442);
			this->panel1->TabIndex = 7;
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->labelMessage);
			this->groupBox3->Controls->Add(this->button1);
			this->groupBox3->Controls->Add(this->checkBoxNewUser);
			this->groupBox3->Controls->Add(this->textBoxUserID);
			this->groupBox3->Controls->Add(this->buttonSign);
			this->groupBox3->Controls->Add(this->labelUserID);
			this->groupBox3->Controls->Add(this->textBoxPassword);
			this->groupBox3->Controls->Add(this->labelPassword);
			this->groupBox3->Location = System::Drawing::Point(195, 84);
			this->groupBox3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->groupBox3->Size = System::Drawing::Size(329, 236);
			this->groupBox3->TabIndex = 9;
			this->groupBox3->TabStop = false;
			// 
			// button1
			// 
			this->button1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"button1.Image")));
			this->button1->Location = System::Drawing::Point(273, 110);
			this->button1->Margin = System::Windows::Forms::Padding(4);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(27, 25);
			this->button1->TabIndex = 8;
			this->toolTip1->SetToolTip(this->button1, L"Show Password");
			this->button1->UseVisualStyleBackColor = true;
			this->button1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &PasswordManager::button1_MouseDown);
			this->button1->MouseLeave += gcnew System::EventHandler(this, &PasswordManager::button1_MouseLeave);
			this->button1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &PasswordManager::button1_MouseUp);
			// 
			// buttonSignOut
			// 
			this->buttonSignOut->Location = System::Drawing::Point(597, 201);
			this->buttonSignOut->Margin = System::Windows::Forms::Padding(4);
			this->buttonSignOut->Name = L"buttonSignOut";
			this->buttonSignOut->Size = System::Drawing::Size(100, 28);
			this->buttonSignOut->TabIndex = 8;
			this->buttonSignOut->Text = L"Sign Out";
			this->buttonSignOut->UseVisualStyleBackColor = true;
			this->buttonSignOut->Click += gcnew System::EventHandler(this, &PasswordManager::buttonSignOut_Click);
			// 
			// buttonShowNewPassword
			// 
			this->buttonShowNewPassword->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"buttonShowNewPassword.Image")));
			this->buttonShowNewPassword->Location = System::Drawing::Point(476, 36);
			this->buttonShowNewPassword->Margin = System::Windows::Forms::Padding(4);
			this->buttonShowNewPassword->Name = L"buttonShowNewPassword";
			this->buttonShowNewPassword->Size = System::Drawing::Size(27, 25);
			this->buttonShowNewPassword->TabIndex = 19;
			this->toolTip1->SetToolTip(this->buttonShowNewPassword, L"Show Password");
			this->buttonShowNewPassword->UseVisualStyleBackColor = true;
			this->buttonShowNewPassword->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &PasswordManager::buttonShowNewPassword_MouseDown);
			this->buttonShowNewPassword->MouseLeave += gcnew System::EventHandler(this, &PasswordManager::buttonShowNewPassword_MouseLeave);
			this->buttonShowNewPassword->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &PasswordManager::buttonShowNewPassword_MouseUp);
			// 
			// dataGridView
			// 
			this->dataGridView->AllowUserToAddRows = false;
			this->dataGridView->AllowUserToDeleteRows = false;
			this->dataGridView->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(2) {
				this->ColUserName,
					this->colDescription
			});
			this->dataGridView->Location = System::Drawing::Point(5, 44);
			this->dataGridView->Margin = System::Windows::Forms::Padding(4);
			this->dataGridView->Name = L"dataGridView";
			this->dataGridView->ReadOnly = true;
			this->dataGridView->ShowEditingIcon = false;
			this->dataGridView->Size = System::Drawing::Size(543, 185);
			this->dataGridView->TabIndex = 9;
			// 
			// ColUserName
			// 
			this->ColUserName->HeaderText = L"User Name";
			this->ColUserName->Name = L"ColUserName";
			this->ColUserName->ReadOnly = true;
			this->ColUserName->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->ColUserName->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			this->ColUserName->Width = 200;
			// 
			// colDescription
			// 
			this->colDescription->HeaderText = L"Description";
			this->colDescription->Name = L"colDescription";
			this->colDescription->ReadOnly = true;
			this->colDescription->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->colDescription->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			this->colDescription->Width = 300;
			// 
			// textBoxSearch
			// 
			this->textBoxSearch->Location = System::Drawing::Point(5, 10);
			this->textBoxSearch->Margin = System::Windows::Forms::Padding(4);
			this->textBoxSearch->Name = L"textBoxSearch";
			this->textBoxSearch->Size = System::Drawing::Size(431, 22);
			this->textBoxSearch->TabIndex = 10;
			this->textBoxSearch->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &PasswordManager::textBoxSearch_KeyPress);
			// 
			// textBoxAddUsername
			// 
			this->textBoxAddUsername->Location = System::Drawing::Point(25, 41);
			this->textBoxAddUsername->Margin = System::Windows::Forms::Padding(4);
			this->textBoxAddUsername->Name = L"textBoxAddUsername";
			this->textBoxAddUsername->Size = System::Drawing::Size(132, 22);
			this->textBoxAddUsername->TabIndex = 11;
			// 
			// textBoxAddDescription
			// 
			this->textBoxAddDescription->Location = System::Drawing::Point(179, 39);
			this->textBoxAddDescription->Margin = System::Windows::Forms::Padding(4);
			this->textBoxAddDescription->Name = L"textBoxAddDescription";
			this->textBoxAddDescription->Size = System::Drawing::Size(132, 22);
			this->textBoxAddDescription->TabIndex = 12;
			// 
			// textBoxAddPassword
			// 
			this->textBoxAddPassword->Location = System::Drawing::Point(336, 39);
			this->textBoxAddPassword->Margin = System::Windows::Forms::Padding(4);
			this->textBoxAddPassword->Name = L"textBoxAddPassword";
			this->textBoxAddPassword->PasswordChar = '*';
			this->textBoxAddPassword->Size = System::Drawing::Size(132, 22);
			this->textBoxAddPassword->TabIndex = 13;
			this->textBoxAddPassword->UseSystemPasswordChar = true;
			// 
			// buttonAdd
			// 
			this->buttonAdd->Location = System::Drawing::Point(525, 36);
			this->buttonAdd->Margin = System::Windows::Forms::Padding(4);
			this->buttonAdd->Name = L"buttonAdd";
			this->buttonAdd->Size = System::Drawing::Size(141, 28);
			this->buttonAdd->TabIndex = 14;
			this->buttonAdd->Text = L"Add";
			this->buttonAdd->UseVisualStyleBackColor = true;
			this->buttonAdd->Click += gcnew System::EventHandler(this, &PasswordManager::buttonAdd_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(65, 21);
			this->label1->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(55, 17);
			this->label1->TabIndex = 15;
			this->label1->Text = L"User ID";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(209, 21);
			this->label2->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(79, 17);
			this->label2->TabIndex = 16;
			this->label2->Text = L"Description";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(373, 21);
			this->label3->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(69, 17);
			this->label3->TabIndex = 17;
			this->label3->Text = L"Password";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->buttonShowNewPassword);
			this->groupBox1->Controls->Add(this->buttonAdd);
			this->groupBox1->Controls->Add(this->textBoxAddUsername);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->textBoxAddDescription);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->textBoxAddPassword);
			this->groupBox1->Controls->Add(this->groupBox2);
			this->groupBox1->Location = System::Drawing::Point(5, 238);
			this->groupBox1->Margin = System::Windows::Forms::Padding(4);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Padding = System::Windows::Forms::Padding(4);
			this->groupBox1->Size = System::Drawing::Size(703, 223);
			this->groupBox1->TabIndex = 20;
			this->groupBox1->TabStop = false;
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->listBoxRules);
			this->groupBox2->Controls->Add(this->label6);
			this->groupBox2->Controls->Add(this->buttonRandomGenerate);
			this->groupBox2->Controls->Add(this->label4);
			this->groupBox2->Controls->Add(this->maskedTextBoxRandomLength);
			this->groupBox2->Controls->Add(this->label5);
			this->groupBox2->Location = System::Drawing::Point(25, 68);
			this->groupBox2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->groupBox2->Size = System::Drawing::Size(643, 153);
			this->groupBox2->TabIndex = 26;
			this->groupBox2->TabStop = false;
			// 
			// listBoxRules
			// 
			this->listBoxRules->FormattingEnabled = true;
			this->listBoxRules->IntegralHeight = false;
			this->listBoxRules->ItemHeight = 16;
			this->listBoxRules->Items->AddRange(gcnew cli::array< System::Object^  >(5) {
				L"Alphanumeric Characters with Special Characters",
					L"Alphanumeric Characters", L"Case-Sensistive Alphabets", L"Case-Insenstitve Alphabets", L"Numeric [Pin]"
			});
			this->listBoxRules->Location = System::Drawing::Point(153, 66);
			this->listBoxRules->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->listBoxRules->Name = L"listBoxRules";
			this->listBoxRules->Size = System::Drawing::Size(325, 36);
			this->listBoxRules->TabIndex = 26;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(293, 47);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(44, 17);
			this->label6->TabIndex = 24;
			this->label6->Text = L"Rules";
			// 
			// buttonRandomGenerate
			// 
			this->buttonRandomGenerate->Location = System::Drawing::Point(511, 68);
			this->buttonRandomGenerate->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->buttonRandomGenerate->Name = L"buttonRandomGenerate";
			this->buttonRandomGenerate->Size = System::Drawing::Size(93, 23);
			this->buttonRandomGenerate->TabIndex = 25;
			this->buttonRandomGenerate->Text = L"Generate";
			this->buttonRandomGenerate->UseVisualStyleBackColor = true;
			this->buttonRandomGenerate->Click += gcnew System::EventHandler(this, &PasswordManager::buttonRandomGenerate_Click);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(231, 18);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(190, 17);
			this->label4->TabIndex = 20;
			this->label4->Text = L"Generate Random Password";
			// 
			// maskedTextBoxRandomLength
			// 
			this->maskedTextBoxRandomLength->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom));
			this->maskedTextBoxRandomLength->Location = System::Drawing::Point(19, 66);
			this->maskedTextBoxRandomLength->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->maskedTextBoxRandomLength->Mask = L"000";
			this->maskedTextBoxRandomLength->Name = L"maskedTextBoxRandomLength";
			this->maskedTextBoxRandomLength->Size = System::Drawing::Size(100, 22);
			this->maskedTextBoxRandomLength->TabIndex = 22;
			this->maskedTextBoxRandomLength->Text = L"015";
			this->maskedTextBoxRandomLength->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// label5
			// 
			this->label5->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(43, 46);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(52, 17);
			this->label5->TabIndex = 21;
			this->label5->Text = L"Length";
			// 
			// panel2
			// 
			this->panel2->Controls->Add(this->labelAddMessage);
			this->panel2->Controls->Add(this->buttonSearch);
			this->panel2->Controls->Add(this->dataGridView);
			this->panel2->Controls->Add(this->groupBox1);
			this->panel2->Controls->Add(this->textBoxSearch);
			this->panel2->Controls->Add(this->buttonDelete);
			this->panel2->Controls->Add(this->buttonShow);
			this->panel2->Controls->Add(this->buttonSave);
			this->panel2->Controls->Add(this->buttonSignOut);
			this->panel2->Location = System::Drawing::Point(12, 16);
			this->panel2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(724, 462);
			this->panel2->TabIndex = 21;
			this->panel2->Visible = false;
			// 
			// labelAddMessage
			// 
			this->labelAddMessage->AutoSize = true;
			this->labelAddMessage->ForeColor = System::Drawing::Color::Red;
			this->labelAddMessage->Location = System::Drawing::Point(105, 420);
			this->labelAddMessage->MinimumSize = System::Drawing::Size(500, 0);
			this->labelAddMessage->Name = L"labelAddMessage";
			this->labelAddMessage->Size = System::Drawing::Size(500, 17);
			this->labelAddMessage->TabIndex = 27;
			this->labelAddMessage->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// buttonSearch
			// 
			this->buttonSearch->Location = System::Drawing::Point(457, 9);
			this->buttonSearch->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->buttonSearch->Name = L"buttonSearch";
			this->buttonSearch->Size = System::Drawing::Size(91, 28);
			this->buttonSearch->TabIndex = 23;
			this->buttonSearch->Text = L"Search";
			this->buttonSearch->UseVisualStyleBackColor = true;
			this->buttonSearch->Click += gcnew System::EventHandler(this, &PasswordManager::buttonSearch_Click);
			// 
			// buttonDelete
			// 
			this->buttonDelete->Location = System::Drawing::Point(597, 95);
			this->buttonDelete->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->buttonDelete->Name = L"buttonDelete";
			this->buttonDelete->Size = System::Drawing::Size(100, 27);
			this->buttonDelete->TabIndex = 21;
			this->buttonDelete->Text = L"Delete";
			this->buttonDelete->UseVisualStyleBackColor = true;
			this->buttonDelete->Click += gcnew System::EventHandler(this, &PasswordManager::buttonDelete_Click);
			// 
			// buttonShow
			// 
			this->buttonShow->Location = System::Drawing::Point(597, 44);
			this->buttonShow->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->buttonShow->Name = L"buttonShow";
			this->buttonShow->Size = System::Drawing::Size(100, 27);
			this->buttonShow->TabIndex = 22;
			this->buttonShow->Text = L"Show";
			this->buttonShow->UseVisualStyleBackColor = true;
			this->buttonShow->Click += gcnew System::EventHandler(this, &PasswordManager::buttonShow_Click);
			// 
			// buttonSave
			// 
			this->buttonSave->Location = System::Drawing::Point(597, 149);
			this->buttonSave->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->buttonSave->Name = L"buttonSave";
			this->buttonSave->Size = System::Drawing::Size(100, 27);
			this->buttonSave->TabIndex = 27;
			this->buttonSave->Text = L"Save";
			this->buttonSave->UseVisualStyleBackColor = true;
			this->buttonSave->Click += gcnew System::EventHandler(this, &PasswordManager::buttonSave_Click);
			// 
			// panel3
			// 
			this->panel3->Controls->Add(this->buttonShowShowPassword);
			this->panel3->Controls->Add(this->label7);
			this->panel3->Controls->Add(this->buttonShowClose);
			this->panel3->Controls->Add(this->buttonCopyPassword);
			this->panel3->Controls->Add(this->buttonCopyUsername);
			this->panel3->Controls->Add(this->textBoxShowPassword);
			this->panel3->Controls->Add(this->textBoxShowUsername);
			this->panel3->Location = System::Drawing::Point(171, 68);
			this->panel3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->panel3->Name = L"panel3";
			this->panel3->Size = System::Drawing::Size(385, 210);
			this->panel3->TabIndex = 22;
			this->panel3->Visible = false;
			// 
			// buttonShowShowPassword
			// 
			this->buttonShowShowPassword->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"buttonShowShowPassword.Image")));
			this->buttonShowShowPassword->Location = System::Drawing::Point(231, 114);
			this->buttonShowShowPassword->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->buttonShowShowPassword->Name = L"buttonShowShowPassword";
			this->buttonShowShowPassword->Size = System::Drawing::Size(27, 25);
			this->buttonShowShowPassword->TabIndex = 23;
			this->buttonShowShowPassword->UseVisualStyleBackColor = true;
			this->buttonShowShowPassword->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &PasswordManager::buttonShowShowPassword_MouseDown);
			this->buttonShowShowPassword->MouseLeave += gcnew System::EventHandler(this, &PasswordManager::buttonShowShowPassword_MouseLeave);
			this->buttonShowShowPassword->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &PasswordManager::buttonShowShowPassword_MouseUp);
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label7->Location = System::Drawing::Point(92, 10);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(197, 24);
			this->label7->TabIndex = 23;
			this->label7->Text = L"Selected Entry Display";
			// 
			// buttonShowClose
			// 
			this->buttonShowClose->Location = System::Drawing::Point(156, 162);
			this->buttonShowClose->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->buttonShowClose->Name = L"buttonShowClose";
			this->buttonShowClose->Size = System::Drawing::Size(92, 25);
			this->buttonShowClose->TabIndex = 4;
			this->buttonShowClose->Text = L"Close";
			this->buttonShowClose->UseVisualStyleBackColor = true;
			this->buttonShowClose->Click += gcnew System::EventHandler(this, &PasswordManager::buttonShowClose_Click);
			// 
			// buttonCopyPassword
			// 
			this->buttonCopyPassword->ImageAlign = System::Drawing::ContentAlignment::MiddleRight;
			this->buttonCopyPassword->Location = System::Drawing::Point(271, 108);
			this->buttonCopyPassword->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->buttonCopyPassword->Name = L"buttonCopyPassword";
			this->buttonCopyPassword->Size = System::Drawing::Size(75, 27);
			this->buttonCopyPassword->TabIndex = 3;
			this->buttonCopyPassword->Text = L"Copy";
			this->buttonCopyPassword->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			this->buttonCopyPassword->UseVisualStyleBackColor = true;
			this->buttonCopyPassword->Click += gcnew System::EventHandler(this, &PasswordManager::buttonCopyPassword_Click);
			// 
			// buttonCopyUsername
			// 
			this->buttonCopyUsername->Location = System::Drawing::Point(271, 64);
			this->buttonCopyUsername->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->buttonCopyUsername->Name = L"buttonCopyUsername";
			this->buttonCopyUsername->Size = System::Drawing::Size(75, 27);
			this->buttonCopyUsername->TabIndex = 2;
			this->buttonCopyUsername->Text = L"Copy";
			this->buttonCopyUsername->UseVisualStyleBackColor = true;
			this->buttonCopyUsername->Click += gcnew System::EventHandler(this, &PasswordManager::buttonCopyUsername_Click);
			// 
			// textBoxShowPassword
			// 
			this->textBoxShowPassword->Location = System::Drawing::Point(43, 113);
			this->textBoxShowPassword->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBoxShowPassword->Name = L"textBoxShowPassword";
			this->textBoxShowPassword->PasswordChar = '*';
			this->textBoxShowPassword->ReadOnly = true;
			this->textBoxShowPassword->Size = System::Drawing::Size(183, 22);
			this->textBoxShowPassword->TabIndex = 1;
			this->textBoxShowPassword->UseSystemPasswordChar = true;
			// 
			// textBoxShowUsername
			// 
			this->textBoxShowUsername->Location = System::Drawing::Point(43, 66);
			this->textBoxShowUsername->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBoxShowUsername->Name = L"textBoxShowUsername";
			this->textBoxShowUsername->ReadOnly = true;
			this->textBoxShowUsername->Size = System::Drawing::Size(207, 22);
			this->textBoxShowUsername->TabIndex = 0;
			// 
			// PasswordManager
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(757, 489);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->panel2);
			this->Controls->Add(this->panel3);
			this->Margin = System::Windows::Forms::Padding(4);
			this->Name = L"PasswordManager";
			this->Text = L"Password Manager";
			this->panel1->ResumeLayout(false);
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView))->EndInit();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->panel2->ResumeLayout(false);
			this->panel2->PerformLayout();
			this->panel3->ResumeLayout(false);
			this->panel3->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: 
		System::Void checkBoxNewUser_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
		void createUser(std::string username, std::string password);
		pair<vector<string>, vector<string>> PasswordManager::PasswordManager::getAllUserData();
		void loginUser(std::string username, std::string password);
		void putTrieInFile(ofstream &ofile, string input);
		string getTrieFromFile(ifstream &ifile, int fileSize);
		void search();

	private: System::Void buttonSign_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void button1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void button1_MouseLeave(System::Object^  sender, System::EventArgs^  e);
	private: System::Void button1_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void buttonRandomGenerate_Click(System::Object^  sender, System::EventArgs^  e);

	private: System::Void buttonShowNewPassword_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void buttonShowNewPassword_MouseLeave(System::Object^  sender, System::EventArgs^  e);
	private: System::Void buttonShowNewPassword_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void buttonAdd_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void buttonShowClose_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void buttonCopyUsername_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void buttonCopyPassword_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void buttonShowShowPassword_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void buttonShowShowPassword_MouseLeave(System::Object^  sender, System::EventArgs^  e);
	private: System::Void buttonShowShowPassword_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void buttonShow_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void buttonSignOut_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void buttonSearch_Click(System::Object^  sender, System::EventArgs^  e);	
	private: System::Void buttonSave_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void buttonDelete_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void textBoxSearch_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e);
	private: System::Void textBoxPassword_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e);
};
}
