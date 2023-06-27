#ifndef _AI_Tool_App_h_
#define _AI_Tool_App_h_


class ToolApp;


class MainToolCtrl : public WithMain<Ctrl> {
	
public:
	typedef MainToolCtrl CLASSNAME;
	MainToolCtrl();
	
	void Data();
	
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
	String json_path;
	
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
	String json_path;
	
};

#if 0
class PatternEditCtrl : public WithPattern<Ctrl> {
	
public:
	typedef PatternEditCtrl CLASSNAME;
	PatternEditCtrl();
	
	void Data();
	void DataPattern();
	void DataPatternLines();
	void DataLine();
	void NewPattern(const String& name);
	void AddPattern();
	void AddLine();
	void SavePattern();
	
	ToolApp* app = 0;
	String json_path;
	
};

class PatternWizardCtrl : public Ctrl {
	
public:
	typedef PatternWizardCtrl CLASSNAME;
	PatternWizardCtrl();
	
	
};
#endif

class ToolApp : public TopWindow {
	MenuBar			menu;
	TabCtrl			tabs;
	
public:
	MainToolCtrl		main;
	ArtistEditCtrl		artist_ctrl;
	StoryEditCtrl		story_ctrl;
	SongCtrl			pattern_ctrl;
	
	String			dir;
	Artist			artist;
	Story			story;
	Pattern			pattern;
	
public:
	typedef ToolApp CLASSNAME;
	
	ToolApp();
	
	void Data();
	void MainMenu(Bar& bar);
	
	String GetArtistsDir() const;
	String GetStoriesDir() const;
	String GetPatternsDir() const;
	
};


#endif
