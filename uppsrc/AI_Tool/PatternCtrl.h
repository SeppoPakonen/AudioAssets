#ifndef _AI_Tool_PatternCtrl_h_
#define _AI_Tool_PatternCtrl_h_


class ToolApp;


class PatternCtrl : public WithPattern<Ctrl> {
	
protected:
	friend class ToolApp;
	TreeCtrl tree;
	ArrayCtrl patterns;
	VectorMap<int, PatternSnap*> tree_snaps;
	Vector<PatternSnap*> level_snaps;
	AttrCtrl attr;
	ArrayCtrl list;
	PlotCtrl plot;
	
public:
	typedef PatternCtrl CLASSNAME;
	PatternCtrl();
	
	void Reload();
	void Data();
	void DataPattern();
	void DataPatternTree();
	void DataPatternTreeNode(PatternSnap& snap, int parent);
	void AddPattern();
	void SavePattern();
	void NewPattern(const String& name);
	void OnTreeSel();
	void OnListSel();
	void DataPatternSnap();
	void DataList();
	void FocusTree();
	void FocusList();
	void SwitchView();
	
	ToolApp* app = 0;
	Pattern* active_pattern = 0;
	PatternSnap* active_snap = 0;
	
};


#endif
