#include "AI_Tool.h"

String Database::GetArtistsDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "artists" DIR_SEPS;
}

String Database::GetStoriesDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "stories" DIR_SEPS;
}

String Database::GetPatternsDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "patterns" DIR_SEPS;
}

String Database::GetCompositionsDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "compositions" DIR_SEPS;
}

String Database::GetAnalysesDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "analyses" DIR_SEPS;
}

void Database::Save() {
	{
		String dir = GetArtistsDir();
		RealizeDirectory(dir);
		for (Artist& o : artists) {
			String json_path = dir + o.file_title + ".json";
			String json = StoreAsJson(o, true);
			FileOut fout(json_path);
			fout << json;
			fout.Close();
		}
	}
	{
		String dir = GetPatternsDir();
		RealizeDirectory(dir);
		for (Pattern& o : patterns) {
			String json_path = dir + o.file_title + ".json";
			String json = StoreAsJson(o, true);
			FileOut fout(json_path);
			fout << json;
			fout.Close();
		}
	}
	{
		String dir = GetStoriesDir();
		RealizeDirectory(dir);
		for (Story& o : stories) {
			String json_path = dir + o.file_title + ".json";
			String json = StoreAsJson(o, true);
			FileOut fout(json_path);
			fout << json;
			fout.Close();
		}
	}
	{
		String dir = GetCompositionsDir();
		RealizeDirectory(dir);
		for (Composition& o : compositions) {
			String json_path = dir + o.file_title + ".json";
			String json = StoreAsJson(o, true);
			FileOut fout(json_path);
			fout << json;
			fout.Close();
		}
	}
	{
		String dir = GetAnalysesDir();
		RealizeDirectory(dir);
		for (Analysis& o : analyses) {
			String json_path = dir + o.file_title + ".json";
			String json = StoreAsJson(o, true);
			FileOut fout(json_path);
			fout << json;
			fout.Close();
		}
	}
}

void Database::Load() {
	{
		artists.Clear();
		String dir = GetArtistsDir();
		String search = dir + "*.json";
		
		FindFile ff;
		if (ff.Search(search)) do {
			if (ff.IsFile()) {
				String path = ff.GetPath();
				Artist& a = artists.Add();
				a.file_title = GetFileTitle(path);
				Load(a);
			}
		}
		while (ff.Next());
		Sort(artists, Artist());
	}
	
	{
		stories.Clear();
		String dir = GetStoriesDir();
		String search = dir + "*.json";
		
		FindFile ff;
		if (ff.Search(search)) do {
			if (ff.IsFile()) {
				String path = ff.GetPath();
				Story& s = stories.Add();
				s.file_title = GetFileTitle(path);
				Load(s);
				DUMP(s.year);
			}
		}
		while (ff.Next());
		Sort(stories, Story());
	}
	
	{
		patterns.Clear();
		String dir = GetPatternsDir();
		String search = dir + "*.json";
		
		FindFile ff;
		if (ff.Search(search)) do {
			if (ff.IsFile()) {
				String path = ff.GetPath();
				Pattern& p = patterns.Add();
				p.file_title = GetFileTitle(path);
				Load(p);
			}
		}
		while (ff.Next());
		Sort(patterns, Pattern());
	}
	
	{
		compositions.Clear();
		String dir = GetCompositionsDir();
		String search = dir + "*.json";
		
		FindFile ff;
		if (ff.Search(search)) do {
			if (ff.IsFile()) {
				String path = ff.GetPath();
				Composition& c = compositions.Add();
				c.file_title = GetFileTitle(path);
				Load(c);
			}
		}
		while (ff.Next());
		Sort(compositions, Composition());
	}
	
	{
		analyses.Clear();
		String dir = GetAnalysesDir();
		String search = dir + "*.json";
		
		FindFile ff;
		if (ff.Search(search)) do {
			if (ff.IsFile()) {
				String path = ff.GetPath();
				Analysis& a = analyses.Add();
				a.file_title = GetFileTitle(path);
				Load(a);
			}
		}
		while (ff.Next());
		Sort(analyses, Analysis());
	}
}

void Database::Load(Artist& a) {
	String json_path = GetArtistsDir() + a.file_title + ".json";
	LoadFromJsonFile(a, json_path);
}

void Database::Load(Pattern& p) {
	String json_path = GetPatternsDir() + p.file_title + ".json";
	LoadFromJsonFile(p, json_path);
}

void Database::Load(Story& s) {
	String json_path = GetStoriesDir() + s.file_title + ".json";
	LoadFromJsonFile(s, json_path);
}

void Database::Load(Composition& c) {
	String json_path = GetCompositionsDir() + c.file_title + ".json";
	LoadFromJsonFile(c, json_path);
}

void Database::Load(Analysis& a) {
	String json_path = GetAnalysesDir() + a.file_title + ".json";
	LoadFromJsonFile(a, json_path);
}

void Database::Create(Story& s) {
	ASSERT(!s.file_title.IsEmpty());
	String dir = GetStoriesDir();
	
	RealizeDirectory(dir);
	
	String path = dir + s.file_title + ".json";
	
	FileOut fout(path);
	fout << "\n";
	fout.Close();
}

void Database::Create(Artist& a) {
	ASSERT(!a.file_title.IsEmpty());
	String dir = GetArtistsDir();
	
	RealizeDirectory(dir);
	
	String path = dir + a.file_title + ".json";
	
	FileOut fout(path);
	fout << "\n";
	fout.Close();
}

void Database::Create(Pattern& p) {
	ASSERT(!p.file_title.IsEmpty());
	String dir = GetPatternsDir();
	
	RealizeDirectory(dir);
	
	String path = dir + p.file_title + ".json";
	
	FileOut fout(path);
	fout << "\n";
	fout.Close();
}

void Database::Create(Composition& c) {
	ASSERT(!c.file_title.IsEmpty());
	String dir = GetCompositionsDir();
	
	RealizeDirectory(dir);
	
	String path = dir + c.file_title + ".json";
	
	FileOut fout(path);
	fout << "\n";
	fout.Close();
}

void Database::Create(Analysis& a) {
	ASSERT(!a.file_title.IsEmpty());
	String dir = GetAnalysesDir();
	
	RealizeDirectory(dir);
	
	String path = dir + a.file_title + ".json";
	
	FileOut fout(path);
	fout << "\n";
	fout.Close();
}

Story& Database::CreateStory(String name) {
	for (Story& s : stories)
		if (s.file_title == name)
			return s;
	
	Story& s = stories.Add();
	s.file_title = name;
	Create(s);
	return s;
}

Artist& Database::CreateArtist(String name) {
	for (Artist& a : artists)
		if (a.file_title == name)
			return a;
	
	Artist& a = artists.Add();
	a.file_title = name;
	Create(a);
	return a;
}

Pattern& Database::CreatePattern(String name) {
	for (Pattern& p : patterns)
		if (p.file_title == name)
			return p;
	
	Pattern& p = patterns.Add();
	p.file_title = name;
	Create(p);
	return p;
}

Composition& Database::CreateComposition(String name) {
	for (Composition& c : compositions)
		if (c.file_title == name)
			return c;
	
	Composition& c = compositions.Add();
	c.file_title = name;
	Create(c);
	return c;
}

Analysis& Database::CreateAnalysis(String name) {
	for (Analysis& a : analyses)
		if (a.file_title == name)
			return a;
	
	Analysis& a = analyses.Add();
	a.file_title = name;
	Create(a);
	return a;
}




void PatternSnap::Init(int pos, int len) {
	this->pos = pos;
	this->len = len;
	
	if (len > 1) {
		int len2 = len / 2;
		int len0 = len2;
		int len1 = len - len2;
		a.Create().Init(pos,       len0);
		b.Create().Init(pos + len0, len1);
	}
}
