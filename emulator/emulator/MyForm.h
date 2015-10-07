#include "pdp\emulator.c"

#pragma once

namespace emulator {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// —водка дл€ MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// ќсвободить все используемые ресурсы.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  display;
	protected:
	private: System::Windows::Forms::ListView^  assembler;
	private: System::Windows::Forms::TextBox^  registers;
	private: System::Windows::Forms::Button^  reset;


	private: System::Windows::Forms::Button^  run;
	private: System::Windows::Forms::Button^  step;

	private:
		/// <summary>
		/// ќб€зательна€ переменна€ конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// “ребуемый метод дл€ поддержки конструктора Ч не измен€йте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->display = (gcnew System::Windows::Forms::PictureBox());
			this->assembler = (gcnew System::Windows::Forms::ListView());
			this->registers = (gcnew System::Windows::Forms::TextBox());
			this->reset = (gcnew System::Windows::Forms::Button());
			this->run = (gcnew System::Windows::Forms::Button());
			this->step = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->display))->BeginInit();
			this->SuspendLayout();
			// 
			// display
			// 
			this->display->BackColor = System::Drawing::SystemColors::WindowFrame;
			this->display->Location = System::Drawing::Point(22, 12);
			this->display->Name = L"display";
			this->display->Size = System::Drawing::Size(260, 225);
			this->display->TabIndex = 0;
			this->display->TabStop = false;
			this->display->Click += gcnew System::EventHandler(this, &MyForm::display_Click);
			// 
			// assembler
			// 
			this->assembler->Location = System::Drawing::Point(352, 12);
			this->assembler->Name = L"assembler";
			this->assembler->Size = System::Drawing::Size(312, 225);
			this->assembler->TabIndex = 1;
			this->assembler->UseCompatibleStateImageBehavior = false;
			// 
			// registers
			// 
			this->registers->Location = System::Drawing::Point(352, 263);
			this->registers->Multiline = true;
			this->registers->Name = L"registers";
			this->registers->Size = System::Drawing::Size(312, 106);
			this->registers->TabIndex = 2;
			this->registers->Text = L"R1:\r\nR2:\r\nR3:\r\nR4:\r\nR5:\r\nSP:\r\nPC:";
			// 
			// reset
			// 
			this->reset->Location = System::Drawing::Point(22, 243);
			this->reset->Name = L"reset";
			this->reset->Size = System::Drawing::Size(51, 21);
			this->reset->TabIndex = 3;
			this->reset->Text = L"Reset";
			this->reset->UseVisualStyleBackColor = true;
			this->reset->Click += gcnew System::EventHandler(this, &MyForm::reset_Click);
			// 
			// run
			// 
			this->run->Location = System::Drawing::Point(174, 243);
			this->run->Name = L"run";
			this->run->Size = System::Drawing::Size(51, 21);
			this->run->TabIndex = 5;
			this->run->Text = L"Run";
			this->run->UseVisualStyleBackColor = true;
			this->run->Click += gcnew System::EventHandler(this, &MyForm::run_Click);
			// 
			// step
			// 
			this->step->Location = System::Drawing::Point(231, 243);
			this->step->Name = L"step";
			this->step->Size = System::Drawing::Size(51, 21);
			this->step->TabIndex = 6;
			this->step->Text = L"Step";
			this->step->UseVisualStyleBackColor = true;
			this->step->Click += gcnew System::EventHandler(this, &MyForm::step_Click);
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
		emu_reset();
	}
	private: System::Void run_Click(System::Object^  sender, System::EventArgs^  e) {
		emu_run();
	}
	private: System::Void step_Click(System::Object^  sender, System::EventArgs^  e) {
		emu_step();
	}
	private: System::Void display_Click(System::Object^  sender, System::EventArgs^  e) {
		
	}
};
}
