#include "AI_Tool.h"


MainToolCtrl::MainToolCtrl() {
	CtrlLayout(*this);
	
	generate <<= THISBACK(Generate);
	copy     <<= THISBACK(CopyToClipboard);
	
}

void MainToolCtrl::Data() {
	Database& db = Database::Single();
	
	
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
	if (story < compositions.GetCount())
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
