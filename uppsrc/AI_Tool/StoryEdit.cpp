#include "AI_Tool.h"

/*
 - muisto
 - reality
 - tarina
 - novelli
*/

/*
Important topics to know about the lyrics of a song:
-The meaning or message behind the lyrics
-The use of literary devices, such as alliteration, metaphor, or imagery 
-The emotion that the songwriter is trying to convey
-The cultural references used in the song
-The musical structure or elements of the song, such as the chord progression, rhythm, and melody
-The history of the song and its significance in a particular era
-The storyline and narrative of the song
-The social and political implications of the song
*/

StoryEditCtrl::StoryEditCtrl() {
	CtrlLayout(*this);
	
	add <<= THISBACK(AddStory);
	
	year <<= THISBACK(SaveStory);
	title <<= THISBACK(SaveStory);
	meaning <<= THISBACK(SaveStory);
	devices <<= THISBACK(SaveStory);
	emotion <<= THISBACK(SaveStory);
	references <<= THISBACK(SaveStory);
	structure <<= THISBACK(SaveStory);
	history <<= THISBACK(SaveStory);
	storyline <<= THISBACK(SaveStory);
	implications <<= THISBACK(SaveStory);
	
	stories.AddColumn("Year");
	stories.AddColumn("Story");
	stories.ColumnWidths("1 5");
	stories.NoHeader();
	stories.WhenAction << THISBACK(DataStory);
}

void StoryEditCtrl::AddStory() {
	String story_name;
	bool b = EditTextNotNull(
		story_name,
		"Story's name",
		"Please write story's name",
		0
	);
	if (b) {
		NewStory(story_name);
	}
}

void StoryEditCtrl::NewStory(const String& name) {
	Database::Single().CreateStory(name);
	
	Data();
	
	int c = stories.GetCount();
	if (c) stories.SetCursor(c-1);
}

void StoryEditCtrl::Data() {
	Database& db = Database::Single();
	for(int i = 0; i < db.stories.GetCount(); i++) {
		Story& s = db.stories[i];
		stories.Set(i, 0, s.year);
		stories.Set(i, 1, s.file_title);
	}
	stories.SetCount(db.stories.GetCount());
	
	int cursor = max(0, db.GetActiveStoryIndex());
	if (cursor >= 0 && cursor < db.stories.GetCount())
		stories.SetCursor(cursor);
	
	if (stories.IsCursor())
		DataStory();
	
}

void StoryEditCtrl::DataStory() {
	if (!stories.IsCursor())
		return;
	
	int cursor = stories.GetCursor();
	Database& db = Database::Single();
	Story& o = db.stories[cursor];
	db.active_story = &o;
	
	this->year							.SetData(o.year);
	this->title							.SetData(o.title);
	this->meaning						.SetData(o.meaning);
	this->devices						.SetData(o.devices);
	this->emotion						.SetData(o.emotion);
	this->references					.SetData(o.references);
	this->structure						.SetData(o.structure);
	this->history						.SetData(o.history);
	this->storyline						.SetData(o.storyline);
	this->implications					.SetData(o.implications);
}

void StoryEditCtrl::SaveStory() {
	Database& db = Database::Single();
	if (!db.active_story)
		return;
	
	Story& o = *db.active_story;
	
	o.year							= this->year.GetData();
	o.title							= this->title.GetData();
	o.meaning						= this->meaning.GetData();
	o.devices						= this->devices.GetData();
	o.emotion						= this->emotion.GetData();
	o.references					= this->references.GetData();
	o.structure						= this->structure.GetData();
	o.history						= this->history.GetData();
	o.storyline						= this->storyline.GetData();
	o.implications					= this->implications.GetData();
	
}
