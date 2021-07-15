#pragma once
#include"Source.cpp"

namespace CppCLRWinformsProjekt {
	vector<string> xml_data;
	vector<string> error_messages;
	string ResultOfFix;
	string sourceFilePath;
	string modifiedSourceFilePath;
	string binaryResultFilePath="C:/Users/hp/Desktop/BinaryCompressed.txt";
	string binaryDecodedFilePath;
	string formtattedXMLfilePath;
	string JSONfilePath;
	int linesNum;
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	/// <summary>
	/// Zusammenfassung für Form1
	/// </summary>
	
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Konstruktorcode hier hinzufügen.
			//
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::RichTextBox^ richTextBox1;
	protected:
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog1;
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::RichTextBox^ richTextBox2;
	private: System::Windows::Forms::Button^ button3;
	private: System::Windows::Forms::Button^ button4;
	private: System::Windows::Forms::Button^ button5;
	private: System::Windows::Forms::Button^ button6;
	private: System::Windows::Forms::Button^ button7;
	private: System::Windows::Forms::Button^ button8;
	private: System::Windows::Forms::Button^ button9;
	private: System::Windows::Forms::SaveFileDialog^ saveFileDialog1;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		
		void InitializeComponent(void)
		{
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->richTextBox2 = (gcnew System::Windows::Forms::RichTextBox());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->button7 = (gcnew System::Windows::Forms::Button());
			this->button8 = (gcnew System::Windows::Forms::Button());
			this->button9 = (gcnew System::Windows::Forms::Button());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->SuspendLayout();
			// 
			// richTextBox1
			// 
			this->richTextBox1->Location = System::Drawing::Point(12, 27);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->Size = System::Drawing::Size(860, 381);
			this->richTextBox1->TabIndex = 0;
			this->richTextBox1->Text = L"";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(12, 414);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(105, 46);
			this->button1->TabIndex = 1;
			this->button1->Text = L"Read File";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::readFile_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(123, 414);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(105, 46);
			this->button2->TabIndex = 2;
			this->button2->Text = L"Detect Errors";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::Detect_Errors_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12));
			this->label1->Location = System::Drawing::Point(131, -1);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(81, 25);
			this->label1->TabIndex = 3;
			this->label1->Text = L"Xml_file";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12));
			this->label2->Location = System::Drawing::Point(989, -1);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(64, 25);
			this->label2->TabIndex = 4;
			this->label2->Text = L"Errors";
			// 
			// richTextBox2
			// 
			this->richTextBox2->Location = System::Drawing::Point(917, 27);
			this->richTextBox2->Name = L"richTextBox2";
			this->richTextBox2->Size = System::Drawing::Size(305, 381);
			this->richTextBox2->TabIndex = 5;
			this->richTextBox2->Text = L"";
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(234, 414);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(105, 46);
			this->button3->TabIndex = 6;
			this->button3->Text = L"Fix Errors";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Form1::fix_error_Click);
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(345, 414);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(105, 46);
			this->button4->TabIndex = 7;
			this->button4->Text = L"Format";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &Form1::format_Click);
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(456, 414);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(105, 46);
			this->button5->TabIndex = 8;
			this->button5->Text = L"Compress";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &Form1::Compress_Click);
			// 
			// button6
			// 
			this->button6->Location = System::Drawing::Point(678, 414);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(105, 46);
			this->button6->TabIndex = 9;
			this->button6->Text = L"Minify";
			this->button6->UseVisualStyleBackColor = true;
			this->button6->Click += gcnew System::EventHandler(this, &Form1::Minify_Click);
			// 
			// button7
			// 
			this->button7->Location = System::Drawing::Point(567, 414);
			this->button7->Name = L"button7";
			this->button7->Size = System::Drawing::Size(105, 46);
			this->button7->TabIndex = 10;
			this->button7->Text = L"Decompress";
			this->button7->UseVisualStyleBackColor = true;
			this->button7->Click += gcnew System::EventHandler(this, &Form1::Decomperss_Click);
			// 
			// button8
			// 
			this->button8->Location = System::Drawing::Point(789, 414);
			this->button8->Name = L"button8";
			this->button8->Size = System::Drawing::Size(105, 46);
			this->button8->TabIndex = 11;
			this->button8->Text = L"Json";
			this->button8->UseVisualStyleBackColor = true;
			this->button8->Click += gcnew System::EventHandler(this, &Form1::json_Click);
			// 
			// button9
			// 
			this->button9->Location = System::Drawing::Point(976, 414);
			this->button9->Name = L"button9";
			this->button9->Size = System::Drawing::Size(182, 46);
			this->button9->TabIndex = 12;
			this->button9->Text = L"Save changes in new file";
			this->button9->UseVisualStyleBackColor = true;
			this->button9->Click += gcnew System::EventHandler(this, &Form1::SaveNewFile_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1234, 472);
			this->Controls->Add(this->button9);
			this->Controls->Add(this->button8);
			this->Controls->Add(this->button7);
			this->Controls->Add(this->button6);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->richTextBox2);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->richTextBox1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
		void MarshalString(String^ s, string& os) {
			using namespace Runtime::InteropServices;
			const char* chars =
				(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
			os = chars;
			Marshal::FreeHGlobal(IntPtr((void*)chars));
		}
		void vector_to_string(vector<string> data, string& s) {
			for (int i = 0; i <data.size(); i++) {
				s += data[i] + '\n';
			}
		}
		void get_source_dest_files(string& source, string& dest) {
			String^ strfilename;
			if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				strfilename = openFileDialog1->FileName;
			}
			MarshalString(strfilename, source);
			saveFileDialog1->ShowDialog();
			String^ filename = saveFileDialog1->FileName;
			MarshalString(filename, dest);
			StreamWriter^ writer = gcnew StreamWriter(File::Create(filename));
			writer->Close();
		}
#pragma endregion
	private: System::Void readFile_Click(System::Object^ sender, System::EventArgs^ e) {
		Stream^ myStream;
		
		string result="";
		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			if ((myStream = openFileDialog1->OpenFile()) != nullptr)
			{
				String^ strfilename = openFileDialog1->FileName;
				//richTextBox1->AppendText(strfilename);
				String^ Readfile = File::ReadAllText(strfilename);
				string s;
				MarshalString(strfilename, s);
				xml_data=read_xml_file(s);
				vector_to_string(xml_data, result);
				String^ str2 = gcnew String(result.c_str());
				richTextBox1->AppendText(str2);
				//richTextBox1->AppendText(Readfile);
				myStream->Close();
			}
		}
		
		//MessageBox::Show(FileName);
	}
	private: System::Void Detect_Errors_Click(System::Object^ sender, System::EventArgs^ e) {
		string errors="";
		 detect_syn_error(xml_data, error_messages);
		 for (int i = 0; i < error_messages.size(); i++) {
			 errors += error_messages[i] + '\n';
		 }
		 String^ Errors = gcnew String(errors.c_str());
		 richTextBox2->AppendText(Errors);
	}
private: System::Void fix_error_Click(System::Object^ sender, System::EventArgs^ e) {
	string result = "";
	richTextBox1->SelectAll();
	richTextBox1->SelectedText = "";
	xml_data = FixAll(xml_data);
	vector_to_string(xml_data, result);
	ResultOfFix = result;
	String^ str2 = gcnew String(result.c_str());
	richTextBox1->AppendText(str2);
}
private: System::Void SaveNewFile_Click(System::Object^ sender, System::EventArgs^ e) {

	saveFileDialog1->ShowDialog();
	String^ filename = saveFileDialog1->FileName;
	StreamWriter^ writer = gcnew StreamWriter(File::Create(filename));
	for (int i = 0; i < xml_data.size(); i++)

	{
		String^ str2 = gcnew String(xml_data[i].c_str());
		writer->WriteLine(str2);

	}
	writer->Close();
	
}
private: System::Void Minify_Click(System::Object^ sender, System::EventArgs^ e) {
	get_source_dest_files(sourceFilePath, modifiedSourceFilePath);
	minifyFile(sourceFilePath, modifiedSourceFilePath);
}

private: System::Void format_Click(System::Object^ sender, System::EventArgs^ e) {
	/*
	saveFileDialog1->ShowDialog();
	String^ filename = saveFileDialog1->FileName;
	MarshalString(filename, formtattedXMLfilePath);
	StreamWriter^ writer = gcnew StreamWriter(File::Create(filename));
	writer->Close();
	*/
	//get_source_dest_files(sourceFilePath, modifiedSourceFilePath);
	//minifyFile(sourceFilePath, modifiedSourceFilePath);
	Tagg* rootTag = buildParseTree(modifiedSourceFilePath);

	get_source_dest_files(modifiedSourceFilePath, formtattedXMLfilePath);
	formatXML(rootTag, formtattedXMLfilePath);
	
}

private: System::Void json_Click(System::Object^ sender, System::EventArgs^ e) {
	/*
	saveFileDialog1->ShowDialog();
	String^ filename = saveFileDialog1->FileName;
	MarshalString(filename, formtattedXMLfilePath);
	StreamWriter^ writer = gcnew StreamWriter(File::Create(filename));
	writer->Close();
	*/
	//get_source_dest_files(sourceFilePath, modifiedSourceFilePath);
	//minifyFile(sourceFilePath, modifiedSourceFilePath);
	Tagg* rootTag = buildParseTree(modifiedSourceFilePath);

	get_source_dest_files(modifiedSourceFilePath, JSONfilePath);
	formatJOSN(rootTag, JSONfilePath);

}

private: System::Void Decomperss_Click(System::Object^ sender, System::EventArgs^ e) {
	string binaryResultFilePath;
	string binaryDecodedFilePath;

	get_source_dest_files(binaryResultFilePath, binaryDecodedFilePath);

	Node* alphabetRoot = buildCodeTree(modifiedSourceFilePath);

	//int linesNum = compresstoBinaryFile(modifiedSourceFilePath, alphabetRoot);

	decompressBinaryFile(binaryResultFilePath, binaryDecodedFilePath, linesNum, alphabetRoot);

}
private: System::Void Compress_Click(System::Object^ sender, System::EventArgs^ e) {
	String^ str = gcnew String(modifiedSourceFilePath.c_str());
	richTextBox1->AppendText(str);
	Node* alphabetRoot = buildCodeTree(modifiedSourceFilePath);
	linesNum = compresstoBinaryFile(modifiedSourceFilePath, alphabetRoot);
	String^ Errors = gcnew String(("number of binary lines are "+to_string(linesNum)).c_str());
	richTextBox2->AppendText(Errors);
	MessageBox::Show("Done");
}
};
}
