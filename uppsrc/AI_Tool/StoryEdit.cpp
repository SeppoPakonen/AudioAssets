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
	save <<= THISBACK(SaveStory);
	
	stories.AddColumn("Story");
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
	String dir = app->GetStoriesDir();
	
	RealizeDirectory(dir);
	
	String path = dir + name + ".json";
	
	FileOut fout(path);
	fout << "\n";
	fout.Close();
	
	Data();
	
	int c = stories.GetCount();
	if (c) stories.SetCursor(c-1);
}

void StoryEditCtrl::Data() {
	String dir = app->GetStoriesDir();
	
	FindFile ff;
	
	String search = dir + "*.json";
	
	int file_i = 0;
	if (ff.Search(search)) do {
		if (ff.IsFile()) {
			String path = ff.GetPath();
			String title = GetFileTitle(path);
			stories.Set(file_i, 0, title);
			file_i++;
		}
	}
	while (ff.Next());
	
	stories.SetCount(file_i);
	
	if (file_i && !stories.IsCursor())
		stories.SetCursor(0);
	
	if (stories.IsCursor())
		DataStory();
	
}

void StoryEditCtrl::DataStory() {
	if (!stories.IsCursor())
		return;
	
	int cursor = stories.GetCursor();
	String title = stories.Get(cursor, 0);
	String dir = app->GetStoriesDir();
	json_path = dir + title + ".json";
	
	Story& o = app->story;
	o.Clear();
	LoadFromJsonFile(o, json_path);
	
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
	Story& o = app->story;
	o.title							= this->title.GetData();
	o.meaning						= this->meaning.GetData();
	o.devices						= this->devices.GetData();
	o.emotion						= this->emotion.GetData();
	o.references					= this->references.GetData();
	o.structure						= this->structure.GetData();
	o.history						= this->history.GetData();
	o.storyline						= this->storyline.GetData();
	o.implications					= this->implications.GetData();
	
	if (json_path.IsEmpty())
		return;
	
	String json = StoreAsJson(o, true);
	FileOut fout(json_path);
	fout << json;
	fout.Close();
}
