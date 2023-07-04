#ifndef _AI_Tool_ScoreCtrl_h_
#define _AI_Tool_ScoreCtrl_h_


class ScoreCtrl : public WithScore<Ctrl> {
	ArrayCtrl scores;
	Plotter plotter;
	ArrayCtrl list;
	ArrayCtrl part_list;
	ArrayCtrl presets;
	
	static const int group_begin = 3;
	
	void ListMenu(Bar& bar);
	void PresetMenu(Bar& bar);
	
public:
	typedef ScoreCtrl CLASSNAME;
	ScoreCtrl();
	
	void Data();
	void DataScores();
	void DataList();
	void DataPresets();
	void AddPreset();
	void AddPattern();
	void SavePattern();
	void NewPattern(const String& name);
	void Reload();
	void LoadActivePattern();
	void SavePreset();
	void UpdatePreset();
	void ApplyPreset();
	void RemovePreset();
	void ListValueChanged(int pos, int scoring);
	void GetScores(const PatternSnap& snap, Vector<int>& scores);
	
	ToolApp* app = 0;
	
};


#endif
