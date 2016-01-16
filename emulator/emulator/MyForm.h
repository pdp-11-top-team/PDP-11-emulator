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
	/// —водка дл€ MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		//FILE *disas;
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
	private: System::Windows::Forms::Timer^  timer;
	private: System::Windows::Forms::ColumnHeader^  columnHeader1;




	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// ќб€зательна€ переменна€ конструктора.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// “ребуемый метод дл€ поддержки конструктора Ч не измен€йте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
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
			this->registers->Size = System::Drawing::Size(312, 115);
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
		this->timer->Stop();
		emu_init();
		this->display->BackgroundImage = nullptr;
		this->display->BackColor = System::Drawing::SystemColors::WindowFrame;
		this->assembler->Items->Clear();
		this->registers->Text = L"R0:\r\nR1:\r\nR2:\r\nR3:\r\nR4:\r\nR5:\r\nSP:\r\nPC:";
	}

	private: void get_disas() {
	String^ s = gcnew String(disas);
	String^ r = gcnew String(reg);// = gcnew String("");

	/*if (sscanf(disas, "%s\n", s) >= 0) {// ((s = get_string()) != nullptr) {
		this->assembler->Items->Add(gcnew ListViewItem(s));
		this->assembler->View = View::Details;
			 for (int i = 0; i < 7; i++) {
				sscanf(disas, "%s\n", r); // r += get_string();
				r += "\r\n";
				 this->registers->Text += r;
			 }
			 
		 }*/
	 //sscanf_s(disas, "%s", s);
	 this->assembler->Items->Add(gcnew ListViewItem(s));
	 this->assembler->View = View::Details;
	// sscanf_s(reg, "%s", r);
	 this->registers->Text = r;
	}

	private: virtual System::Void run_Click(System::Object^  sender, System::EventArgs^  e) sealed {
		this->timer->Start();
		stop = FALSE;
		while (stop == FALSE) {
			step_Click(sender, e);
		}
	}
	private: System::Void step_Click(System::Object^  sender, System::EventArgs^  e) {
		this->timer->Start();
		if (emu_step() == 0) {
			stop = TRUE;
			return;
		}

		get_disas();
		int index = 0;
		IntPtr scan = IntPtr(&memory.VRAM[0]);
		Bitmap ^picture = gcnew Bitmap(256, 256, 32, System::Drawing::Imaging::PixelFormat::Format1bppIndexed, scan);
		this->display->BackgroundImageLayout = ImageLayout::Center;
		this->display->BackgroundImage = picture;
		this->display->Invalidate();
		this->display->Update();
		//this->disas = fopen("pdp/log.txt", "r");
		//fclose(this->disas);
	}


	/*private: String ^get_string() {
		String^ s;
		char c[256];
	
		if (fgets(c, 256, disas) > 0) {
			s = gcnew String(c);
			s += "\n";
			return s;
		}

		return nullptr;
	}*/
	
	
private: System::Void timer_Tick(System::Object^  sender, System::EventArgs^  e) {
	int index = 0;
	IntPtr scan = IntPtr(&memory.VRAM[0]);
	Bitmap ^picture = gcnew Bitmap(256, 256, 32, System::Drawing::Imaging::PixelFormat::Format1bppIndexed, scan);
	/* int pixel;
	for (int Xcount = 0; Xcount < picture->Width; Xcount++) {
	for (int Ycount = 0; Ycount < picture->Height; Ycount++) {
	pixel = memory.VRAM[index++];
	picture->SetPixel(Xcount, Ycount, Color::FromArgb(pixel, pixel, pixel));
	}
	}*/
	this->display->BackgroundImageLayout = ImageLayout::Center;
	this->display->BackgroundImage = picture;
}
};
}
