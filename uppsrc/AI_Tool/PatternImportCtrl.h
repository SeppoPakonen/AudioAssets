#ifndef _AI_Tool_PatternImportCtrl_h_
#define _AI_Tool_PatternImportCtrl_h_


class PatternImportCtrl : public WithPatternImport<Ctrl> {
	DocEdit input;
	ArrayCtrl output;
	
public:
	typedef PatternImportCtrl CLASSNAME;
	PatternImportCtrl();
	
	void Data();
	void DataSong();
	void AddSong();
	void OnValueChange();
	void CopyPrompt();
	void PasteResult();
	void CopyTest();
	void ResetSong();
	void RemoveLine();
	void OutputBar(Bar& b);
	
	
};


#endif
