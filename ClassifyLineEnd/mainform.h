#pragma once
#include "endlineclassifier.h"


namespace ClassifyLineEnd {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for mainform
	/// </summary>
	public ref class mainform : public System::Windows::Forms::Form
	{
	public:
		mainform(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~mainform()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  picMain;
	protected:
	private: System::Windows::Forms::Button^  btnImport;
	private: System::Windows::Forms::Button^  btnProcess;
	private: System::Windows::Forms::Button^  btnExit;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->picMain = (gcnew System::Windows::Forms::PictureBox());
			this->btnImport = (gcnew System::Windows::Forms::Button());
			this->btnProcess = (gcnew System::Windows::Forms::Button());
			this->btnExit = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picMain))->BeginInit();
			this->SuspendLayout();
			// 
			// picMain
			// 
			this->picMain->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->picMain->Location = System::Drawing::Point(12, 22);
			this->picMain->Name = L"picMain";
			this->picMain->Size = System::Drawing::Size(518, 474);
			this->picMain->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->picMain->TabIndex = 0;
			this->picMain->TabStop = false;
			// 
			// btnImport
			// 
			this->btnImport->Location = System::Drawing::Point(12, 506);
			this->btnImport->Name = L"btnImport";
			this->btnImport->Size = System::Drawing::Size(110, 46);
			this->btnImport->TabIndex = 1;
			this->btnImport->Text = L"Import";
			this->btnImport->UseVisualStyleBackColor = true;
			this->btnImport->Click += gcnew System::EventHandler(this, &mainform::btnImport_Click);
			// 
			// btnProcess
			// 
			this->btnProcess->Location = System::Drawing::Point(128, 506);
			this->btnProcess->Name = L"btnProcess";
			this->btnProcess->Size = System::Drawing::Size(110, 46);
			this->btnProcess->TabIndex = 2;
			this->btnProcess->Text = L"Process";
			this->btnProcess->UseVisualStyleBackColor = true;
			this->btnProcess->Click += gcnew System::EventHandler(this, &mainform::btnProcess_Click);
			// 
			// btnExit
			// 
			this->btnExit->Location = System::Drawing::Point(420, 506);
			this->btnExit->Name = L"btnExit";
			this->btnExit->Size = System::Drawing::Size(110, 46);
			this->btnExit->TabIndex = 3;
			this->btnExit->Text = L"Exit";
			this->btnExit->UseVisualStyleBackColor = true;
			// 
			// mainform
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(544, 564);
			this->Controls->Add(this->btnExit);
			this->Controls->Add(this->btnProcess);
			this->Controls->Add(this->btnImport);
			this->Controls->Add(this->picMain);
			this->Name = L"mainform";
			this->Text = L"mainform";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picMain))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void btnImport_Click(System::Object^  sender, System::EventArgs^  e) {
		OpenFileDialog ^ dlgOpenFile = gcnew OpenFileDialog();
		dlgOpenFile->Filter = "Image|*.jpg; *.png";
		dlgOpenFile->Title = "Select a Image File";

		// Show the Dialog.
		// If the user clicked OK in the dialog and
		// a .CUR file was selected, open it.
		if (dlgOpenFile->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			picMain->ImageLocation = dlgOpenFile->FileName;
			
		}
	}
	private: System::Void btnProcess_Click(System::Object^  sender, System::EventArgs^  e) {
		lineimage limg;
		limg.setOrigImg(picMain->ImageLocation, "");

		//Convert to grayscale
		Mat img = limg.getOrigImg();
		limg.setGrayImg(img);
		

		//convert grayscale to binary
		limg.setBinImg(limg.getGrayImg());
		


		//clean image;
		limg.setMorpImg(limg.getBinImg());
		

		//edge detection;
		//limg.setEdgeImg(limg.getMorpImg());
		//img = limg.getCannyEdge();

		//contouring and cleaning;
		limg.setContourImg(limg.getMorpImg());
		

		//thinning
		limg.setSkelImg(limg.getContourImg());
		
		//end Points

		limg.setEndPoints(limg.getSkelImg());

		//overlap;
		//limg.setOverlapImg(limg.getMorpImg(), limg.getSkelImg());
		
		//line End Area
		limg.setLineEndArea(limg.getBinImg(),limg.getEndPoints());


		OutputDebugString(TEXT("Done"));
	}
};
}
