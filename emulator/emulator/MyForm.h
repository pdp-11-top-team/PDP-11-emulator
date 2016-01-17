#include <stdio.h>
extern "C" {
	#include "pdp\emulator.h"
}

#pragma once

namespace emulator {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace std;

	/// <summary>
	/// Pdp-11 emulator
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			stop = false;
		}

	protected:
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  display;
	private: System::Windows::Forms::ListView^  assembler;
	private: System::Windows::Forms::TextBox^  registers;
	private: System::Windows::Forms::Button^  reset;
	private: System::Windows::Forms::Button^  run;
	private: System::Windows::Forms::Button^  step;
	private: System::Windows::Forms::Timer^  timer;
	private: System::Windows::Forms::ColumnHeader^  columnHeader1;
	private: System::ComponentModel::IContainer^  components;

	private: bool stop;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->display = (gcnew System::Windows::Forms::PictureBox());
			this->assembler = (gcnew System::Windows::Forms::ListView());
			this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
			this->registers = (gcnew System::Windows::Forms::TextBox());
			this->reset = (gcnew System::Windows::Forms::Button());
			this->run = (gcnew System::Windows::Forms::Button());
			this->step = (gcnew System::Windows::Forms::Button());
			this->timer = (gcnew System::Windows::Forms::Timer(this->components));
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->display))->BeginInit();
			this->SuspendLayout();
			// 
			// display
			// 
			this->display->BackColor = System::Drawing::SystemColors::WindowFrame;
			this->display->Location = System::Drawing::Point(22, 12);
			this->display->Name = L"display";
			this->display->Size = System::Drawing::Size(256, 256);
			this->display->TabIndex = 0;
			this->display->TabStop = false;
			// 
			// assembler
			// 
			this->assembler->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(1) { this->columnHeader1 });
			this->assembler->FullRowSelect = true;
			this->assembler->GridLines = true;
			this->assembler->Location = System::Drawing::Point(352, 12);
			this->assembler->MinimumSize = System::Drawing::Size(300, 4);
			this->assembler->MultiSelect = false;
			this->assembler->Name = L"assembler";
			this->assembler->Size = System::Drawing::Size(312, 225);
			this->assembler->TabIndex = 1;
			this->assembler->UseCompatibleStateImageBehavior = false;
			// 
			// columnHeader1
			// 
			this->columnHeader1->Text = L"Assembler";
			this->columnHeader1->Width = 300;
			// 
			// registers
			// 
			this->registers->Location = System::Drawing::Point(352, 263);
			this->registers->Multiline = true;
			this->registers->Name = L"registers";
			this->registers->Size = System::Drawing::Size(312, 135);
			this->registers->TabIndex = 2;
			this->registers->Text = L"R1:\r\nR2:\r\nR3:\r\nR4:\r\nR5:\r\nSP:\r\nPC:";
			// 
			// reset
			// 
			this->reset->Location = System::Drawing::Point(22, 274);
			this->reset->Name = L"reset";
			this->reset->Size = System::Drawing::Size(51, 21);
			this->reset->TabIndex = 3;
			this->reset->Text = L"Reset";
			this->reset->UseVisualStyleBackColor = true;
			this->reset->Click += gcnew System::EventHandler(this, &MyForm::reset_Click);
			// 
			// run
			// 
			this->run->Location = System::Drawing::Point(174, 274);
			this->run->Name = L"run";
			this->run->Size = System::Drawing::Size(51, 21);
			this->run->TabIndex = 5;
			this->run->Text = L"Run";
			this->run->UseVisualStyleBackColor = true;
			this->run->Click += gcnew System::EventHandler(this, &MyForm::run_Click);
			// 
			// step
			// 
			this->step->Location = System::Drawing::Point(231, 274);
			this->step->Name = L"step";
			this->step->Size = System::Drawing::Size(51, 21);
			this->step->TabIndex = 6;
			this->step->Text = L"Step";
			this->step->UseVisualStyleBackColor = true;
			this->step->Click += gcnew System::EventHandler(this, &MyForm::step_Click);
			// 
			// timer
			// 
			this->timer->Enabled = true;
			this->timer->Interval = 700;
			this->timer->Tick += gcnew System::EventHandler(this, &MyForm::timer_Tick);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(683, 484);
			this->Controls->Add(this->step);
			this->Controls->Add(this->run);
			this->Controls->Add(this->reset);
			this->Controls->Add(this->registers);
			this->Controls->Add(this->assembler);
			this->Controls->Add(this->display);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Name = L"MyForm";
			this->Text = L"PDP-11 Emulator";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->display))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}

#pragma endregion
		private: System::Void reset_Click(System::Object^  sender, System::EventArgs^  e) {
			emu_init();
			this->display->BackgroundImage = nullptr;
			this->display->BackColor = System::Drawing::SystemColors::WindowFrame;
			this->assembler->Items->Clear();
			this->registers->Text = L"R0:\r\nR1:\r\nR2:\r\nR3:\r\nR4:\r\nR5:\r\nSP:\r\nPC:\r\nState:";
		}

		private: void get_disas() {
			String^ step_disas = gcnew String(disas);
			String^ registers_state = gcnew String(reg);

			this->assembler->Items->Add(gcnew ListViewItem(step_disas));
			this->assembler->View = View::Details;
			this->registers->Text = registers_state;
		}
				 
		private: void update_components() {
			IntPtr scan = IntPtr(&memory.VRAM[0]);
			Bitmap ^picture = gcnew Bitmap(256, 256, 32, System::Drawing::Imaging::PixelFormat::Format1bppIndexed, scan);

			this->display->BackgroundImageLayout = ImageLayout::Center;
			this->display->BackgroundImage = picture;
			this->display->Update();
			this->assembler->Update();
			this->assembler->Items[this->assembler->Items->Count - 1]->EnsureVisible();
			this->registers->Update();
		}

		private: virtual System::Void run_Click(System::Object^  sender, System::EventArgs^  e) sealed {
			stop = false;
			while (stop == false) {
				step_Click(sender, e);
			}
		}

		private: System::Void step_Click(System::Object^  sender, System::EventArgs^  e) {
			if (emu_step() <= 0) {
				stop = true;
			}

			get_disas();
			update_components();
		}
	
		private: System::Void timer_Tick(System::Object^  sender, System::EventArgs^  e) {
		
		}
	};	
}
