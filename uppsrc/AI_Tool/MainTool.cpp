#include "AI_Tool.h"


MainToolCtrl::MainToolCtrl() {
	CtrlLayout(*this);
	
	generate <<= THISBACK(Generate);
	copy     <<= THISBACK(CopyToClipboard);
	
	artists			<<= THISBACK(OnIndexChange);
	patterns		<<= THISBACK(OnIndexChange);
	stories			<<= THISBACK(OnIndexChange);
	compositions	<<= THISBACK(OnIndexChange);
	analyses		<<= THISBACK(OnIndexChange);
	
	
	// Set all active object pointers from here at the beginning
	PostCallback(THISBACK(Data));
	PostCallback(THISBACK(OnIndexChange));
}

void MainToolCtrl::OnIndexChange() {
	Database& db = Database::Single();
	
	#define SEL(x) (this->x.GetIndex() >= 0 && this->x.GetIndex() < db.x.GetCount()) ? &db.x[this->x.GetIndex()] : 0
	db.active_artist		= SEL(artists);
	db.active_pattern		= SEL(patterns);
	db.active_story			= SEL(stories);
	db.active_composition	= SEL(compositions);
	db.active_analysis		= SEL(analyses);
	
}

void MainToolCtrl::Data() {
	Database& db = Database::Single();
	
	artist		= max(0, db.GetActiveArtistIndex());
	pattern		= max(0, db.GetActivePatternIndex());
	story		= max(0, db.GetActiveStoryIndex());
	composition	= max(0, db.GetActiveCompositionIndex());
	analysis	= max(0, db.GetActiveAnalysisIndex());
	
	artists.Clear();
	for(int i = 0; i < db.artists.GetCount(); i++) {
		artists.Add(db.artists[i].file_title);
	}
	if (artist < artists.GetCount())
		artists.SetIndex(artist);
	
	
	patterns.Clear();
	for(int i = 0; i < db.patterns.GetCount(); i++) {
		patterns.Add(db.patterns[i].file_title);
	}
	if (pattern < patterns.GetCount())
		patterns.SetIndex(pattern);
	
	
	stories.Clear();
	for(int i = 0; i < db.stories.GetCount(); i++) {
		stories.Add(db.stories[i].file_title);
	}
	if (story < stories.GetCount())
		stories.SetIndex(story);
	
	
	compositions.Clear();
	for(int i = 0; i < db.compositions.GetCount(); i++) {
		compositions.Add(db.compositions[i].file_title);
	}
	if (composition < compositions.GetCount())
		compositions.SetIndex(composition);
	
	
	analyses.Clear();
	for(int i = 0; i < db.analyses.GetCount(); i++) {
		analyses.Add(db.analyses[i].file_title);
	}
	if (analysis < analyses.GetCount())
		analyses.SetIndex(analysis);
}

void MainToolCtrl::Generate() {
	
}

void MainToolCtrl::CopyToClipboard() {
	
}
