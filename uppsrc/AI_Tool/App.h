#ifndef _AI_Tool_App_h_
#define _AI_Tool_App_h_


class ToolApp;


class MainToolCtrl : public WithMain<Ctrl> {
	int artist = 0, story = 0, pattern = 0, analysis = 0, composition = 0;
	
public:
	typedef MainToolCtrl CLASSNAME;
	MainToolCtrl();
	
	void Data();
	void OnIndexChange();
	void Generate();
	void CopyToClipboard();
	
	ToolApp* app = 0;
	
};

class ArtistEditCtrl : public WithArtist<Ctrl> {
	
public:
	typedef ArtistEditCtrl CLASSNAME;
	ArtistEditCtrl();
	
	void Data();
	void DataArtist();
	void AddArtist();
	void SaveArtist();
	void NewArtist(const String& name);
	
	ToolApp* app = 0;
	
};

class StoryEditCtrl : public WithStory<Ctrl> {
	
public:
	typedef StoryEditCtrl CLASSNAME;
	StoryEditCtrl();
	
	void Data();
	void DataStory();
	void NewStory(const String& name);
	void AddStory();
	void SaveStory();
	
	ToolApp* app = 0;
	
};

class CompositionEditCtrl : public WithComposition<Ctrl> {
	
public:
	typedef CompositionEditCtrl CLASSNAME;
	CompositionEditCtrl();
	
	void Data();
	void DataComposition();
	void NewComposition(const String& name);
	void AddComposition();
	void SaveComposition();
	
	ToolApp* app = 0;
	
};

class AnalysisEditCtrl : public WithAnalysis<Ctrl> {
	
public:
	typedef AnalysisEditCtrl CLASSNAME;
	AnalysisEditCtrl();
	
	void Data();
	void DataAnalysis();
	void NewAnalysis(const String& name);
	void AddAnalysis();
	void SaveAnalysis();
	void ParseAnalysis();
	
	ToolApp* app = 0;
	
};


class ToolApp : public TopWindow {
	MenuBar			menu;
	TabCtrl			tabs;
	
public:
	MainToolCtrl		main;
	ArtistEditCtrl		artist_ctrl;
	StoryEditCtrl		story_ctrl;
	PatternCtrl			pattern_ctrl;
	CompositionEditCtrl	composition_ctrl;
	AnalysisEditCtrl	analysis_ctrl;
	ScoreCtrl			score_ctrl;
	AttrScoreCtrl		attrscore_ctrl;
	ProductionCtrl		prod_ctrl;
	ReverseCtrl			rev_ctrl;
	TimelineCtrl		timeline_ctrl;
	
public:
	typedef ToolApp CLASSNAME;
	
	ToolApp();
	~ToolApp();
	
	void Save();
	void Data();
	void MainMenu(Bar& bar);
	
	void SwitchView();
	void MovePrevLevel();
	void MoveNextLevel();
	void MovePrevTotal();
	void MoveNextTotal();
	void MoveOwner();
	void MovePrevious();
};


#endif
