#include "BatchDraftTool.h"

#define IMAGECLASS Imgs
#define IMAGEFILE <BatchDraftTool/BatchDraftTool.iml>
#include <Draw/iml_source.h>


BatchDraftTool::BatchDraftTool() {
	Title("Batch Draft Tool");
	MaximizeBox().MinimizeBox().Sizeable();
	Icon(Imgs::icon());
	
	Add(hsplit.SizePos());
	
	hsplit.Horz() << files << edit;
	hsplit.SetPos(2000);
	
	files.AddColumn("File");
	files.WhenCursor = THISBACK(DataFile);
	
	edit.WhenAction = [this] {
		if (next_file_on_enter) {
			int len = edit.GetLength();
			if (len > 0 && len == edit.GetCursor() && edit.GetLineLength(edit.GetLineCount()-1) == 0) {
				this->NextFile();
			}
		}
		tc.Set(1000, [this]{this->SaveFile();});
	};
	
	AddFrame(menu);
	menu.Set([this](Bar& bar) {
		bar.Sub("App", [this](Bar& bar) {
			bar.Add("New file", [this]{
				if (!HasDir()) return;
				String filetitle;
				if (!EditText(filetitle, "Title of the file", "Title")) return;
				String path = AppendFileName(dir, filetitle + ".txt");
				if (FileExists(path)) return;
				FileOut fout(path);
				fout.Flush();
				fout.Close();
				PostCallback([this]{this->DataDirectory();});
			}).Key(K_CTRL_N);
			bar.Add("Set Directory", [this]{
				String newdir;
				if (!EditText(newdir, "Active directory", "Directory")) return;
				if (!DirectoryExists(newdir)) return;
				dir = newdir;
				StoreThis();
				PostCallback([this]{this->DataDirectory();});
			}).Key(K_CTRL_O);
			bar.Add("Open next file", [this]{this->NextFile();}).Key(K_F1);
			bar.Add("Next file on new-last-line", [this]{
				next_file_on_enter = !next_file_on_enter;
			}).Key(K_F2).Check(next_file_on_enter);
			bar.Add("Open directory in file manager", [this]{
				if (dir.IsEmpty()) return;
				#ifdef flagWIN32
				system("explorer.exe \"" + dir + "\"");
				#endif
			}).Key(K_F4);
			bar.Add("Refresh files", [this]{this->DataDirectory();}).Key(K_F5);
			bar.Add("Batch create files", [this]{this->BatchCreate();}).Key(K_F7);
			bar.Add("Quit", [this]{this->Close();}).Key(K_CTRL_Q);
		});
		bar.Sub("Editor", [this](Bar& bar) {
			bar.Add("Increase font size", [this]{
				edit.SetFont(Arial(++fnt_size));
				StoreThis();
			}).Key(K_F11);
			bar.Add("Decrease font size", [this]{
				edit.SetFont(Arial(--fnt_size));
				StoreThis();
			}).Key(K_F10);
		});
	});
	LoadThis();
	edit.SetFont(Arial(fnt_size));
	
	PostCallback([this]{this->DataDirectory();});
	PostCallback([this]{this->EndCursor();});
}

void BatchDraftTool::NextFile() {
	if (!files.IsCursor()) return;
	int cur = (files.GetCursor() + 1) % files.GetCount();
	if (cur < files.GetCount()) {
		files.SetCursor(cur);
		PostCallback([this]{this->EndCursor();});
	}
}

void BatchDraftTool::EndCursor() {
	int lines = this->edit.GetLineCount();
	int chars = this->edit.GetLineLength(lines-1);
	int c = this->edit.GetLength();
	if (chars > 0)
		this->edit.Insert(c++,"\n");
	this->edit.SetCursor(c);
	this->edit.SetFocus();
}


void BatchDraftTool::DataDirectory() {
	if (!HasDir()) return;
	String search = AppendFileName(dir, "*.txt");
	FindFile ff;
	Vector<String> titles;
	if (ff.Search(search)) do {
		String title = ff.GetName();
		titles << GetFileTitle(title);
	}
	while (ff.Next());
	Sort(titles, StdLess<String>());
	
	for(int i = 0; i < titles.GetCount(); i++) {
		files.Set(i, 0, titles[i]);
	}
	files.SetCount(titles.GetCount());
	
	if (!files.IsCursor() && files.GetCount())
		files.SetCursor(0);
	else
		DataFile();
}

void BatchDraftTool::SaveFile() {
	if (activepath.IsEmpty()) return;
	if (active_exists && !FileExists(activepath)) {
		if (!PromptYesNo(DeQtf("File doesn't exist anymore. Do you want to save anyway?")))
			return;
	}
	String txt = edit.GetData();
	FileOut fout(activepath);
	fout << txt;
	fout.Close();
}

void BatchDraftTool::LoadFile() {
	if (activepath.IsEmpty()) return;
	active_exists = FileExists(activepath);
	if (active_exists) {
		String txt = UPP::LoadFile(activepath);
		edit.SetData(txt);
	}
	else
		edit.Clear();
}

void BatchDraftTool::DataFile() {
	if (!files.IsCursor())
		return;
	
	String title = files.Get(0);
	String path = AppendFileName(dir, title + ".txt");
	if (!FileExists(path)) {
		PromptOK("File doesn't exist: " + path);
		return;
	}
	
	if (activepath != path && !path.IsEmpty()) {
		SaveFile();
		tc.Kill();
		activepath = path;
		LoadFile();
	}
}

bool BatchDraftTool::HasDir() const {
	return dir.GetCount() && DirectoryExists(dir);
}

void BatchDraftTool::BatchCreate() {
	if (!HasDir()) return;
	String title = "Batch Create";
	WithBatchCreate<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, title);
	dlg.count.SetData(8);
	if(dlg.Execute() == IDOK) {
		int n = dlg.count;
		n = min(100,n);
		String title = dlg.title.GetData();
		for(int i = 1; i <= n; i++) {
			String path = AppendFileName(dir, title + " " + IntStr(i) + ".txt");
			if (!FileExists(path)) {
				FileOut fout(path);
			}
		}
		this->DataDirectory();
	}
}
