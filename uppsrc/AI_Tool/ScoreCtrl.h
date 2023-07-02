#ifndef _AI_Tool_ScoreCtrl_h_
#define _AI_Tool_ScoreCtrl_h_


class ScoreCtrl : public WithScore<Ctrl> {
	ArrayCtrl scores;
	Plotter plotter;
	ArrayCtrl list;
	ArrayCtrl part_list;
	ArrayCtrl preset;
	
	
public:
	typedef ScoreCtrl CLASSNAME;
	ScoreCtrl();
	
	void Data();
	void DataScores();
	void DataList();
	void AddPreset();
	void AddPattern();
	void SavePattern();
	void NewPattern(const String& name);
	void Reload();
	void LoadActivePattern();
	void ListValueChanged(int pos, int scoring);
	
	ToolApp* app = 0;
	
};


#endif
