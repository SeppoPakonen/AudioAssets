#if 0
#include "AI_Tool.h"


PatternEditCtrl::PatternEditCtrl() {
	CtrlLayout(*this);
	
	add <<= THISBACK(AddPattern);
	add_line <<= THISBACK(AddLine);
	save <<= THISBACK(SavePattern);
	
	patterns.AddColumn("Pattern");
	patterns.NoHeader();
	patterns.WhenAction << THISBACK(DataPattern);
	
	lines.AddColumn("Line");
	lines.NoHeader();
	lines.WhenAction << THISBACK(DataLine);
}

void PatternEditCtrl::AddPattern() {
	String pattern_name;
	bool b = EditTextNotNull(
		pattern_name,
		"Pattern's name",
		"Please write pattern's name",
		0
	);
	if (b) {
		NewPattern(pattern_name);
	}
}

void PatternEditCtrl::AddLine() {
	app->pattern.lines.Add();
	
	DataPatternLines();
}

void PatternEditCtrl::NewPattern(const String& name) {
	String dir = app->GetPatternsDir();
	
	RealizeDirectory(dir);
	
	String path = dir + name + ".json";
	
	FileOut fout(path);
	fout << "\n";
	fout.Close();
	
	Data();
	
	int c = patterns.GetCount();
	if (c) patterns.SetCursor(c-1);
}

void PatternEditCtrl::Data() {
	String dir = app->GetPatternsDir();
	
	FindFile ff;
	
	String search = dir + "*.json";
	
	int file_i = 0;
	if (ff.Search(search)) do {
		if (ff.IsFile()) {
			String path = ff.GetPath();
			String title = GetFileTitle(path);
			patterns.Set(file_i, 0, title);
			file_i++;
		}
	}
	while (ff.Next());
	
	patterns.SetCount(file_i);
	
	if (file_i && !patterns.IsCursor())
		patterns.SetCursor(0);
	
	if (patterns.IsCursor())
		DataPattern();
	
}

void PatternEditCtrl::DataPattern() {
	if (!patterns.IsCursor())
		return;
	
	int cursor = patterns.GetCursor();
	String title = patterns.Get(cursor, 0);
	String dir = app->GetPatternsDir();
	json_path = dir + title + ".json";
	
	Pattern& o = app->pattern;
	o.Clear();
	LoadFromJsonFile(o, json_path);
	
	this->artist			.SetData(o.artist);
	this->title				.SetData(o.title);
	this->part				.SetData(o.part);
	
	DataPatternLines();
}

void PatternEditCtrl::DataPatternLines() {
	Pattern& o = app->pattern;
	
	for(int i = 0; i < o.lines.GetCount(); i++) {
		LinePattern& lp = o.lines[i];
		this->lines.Set(i, 0, lp.text);
	}
	this->lines.SetCount(o.lines.GetCount());
	
	if (this->lines.GetCount() && !lines.IsCursor())
		lines.SetCursor(0);
	
	if (lines.IsCursor())
		DataLine();
	
}

void PatternEditCtrl::DataLine() {
	if (!lines.IsCursor())
		return;
	
	int cursor = lines.GetCursor();
	
	Pattern& o = app->pattern;
	if (cursor < 0 || cursor >= o.lines.GetCount())
		return;
	
	LinePattern& lp = o.lines[cursor];
	
	#define IDX(x) Join(x.GetKeys(), ", ")
	this->text					.SetData(lp.text);
	this->pronouns				.SetData(IDX(lp.pronouns));
	this->explanation			.SetData(lp.explanation);
	this->elements				.SetData(IDX(lp.elements));
	this->interactions			.SetData(IDX(lp.interactions));
	this->interactions_with		.SetData(IDX(lp.interactions_with));
	
	this->moral_interactions	.SetData(IDX(lp.moral_interactions));
	this->acting_styles			.SetData(IDX(lp.acting_styles));
	this->tones					.SetData(IDX(lp.tones));
	this->voiceover				.SetData(IDX(lp.voiceover));
	this->comedic_scenarios		.SetData(IDX(lp.comedic_scenarios));
	
	this->dramatic_scenarios	.SetData(IDX(lp.dramatic_scenarios));
	this->comedic_attributes	.SetData(IDX(lp.comedic_attributes));
	this->humor_attributes		.SetData(IDX(lp.humor_attributes));
	this->type_of_sentence		.SetData(lp.type_of_sentence);
	#undef IDX
	
}

void LoadIndexData(Index<String>& idx, Ctrl& ctrl) {
	String data = ctrl.GetData();
	Vector<String> tokens = Split(data, ",");
	idx.Clear();
	for (String& s : tokens)
		idx.FindAdd(TrimBoth(s));
}

void PatternEditCtrl::SavePattern() {
	Pattern& o = app->pattern;
	o.artist						= artist.GetData();
	o.title							= title.GetData();
	o.part							= part.GetData();
	
	int line_cursor = lines.GetCursor();
	if (line_cursor < 0 ||line_cursor >= o.lines.GetCount())
		return;
	
	LinePattern& lp = o.lines[line_cursor];
	lp.text						= text.GetData();
	#define IDX(x) LoadIndexData(lp.x, this->x);
	IDX(pronouns)
	lp.explanation				= explanation.GetData();
	IDX(elements)
	IDX(interactions)
	IDX(interactions_with)
	IDX(moral_interactions)
	IDX(acting_styles)
	IDX(tones)
	IDX(voiceover)
	IDX(comedic_scenarios)
	IDX(dramatic_scenarios)
	IDX(comedic_attributes)
	IDX(humor_attributes)
	lp.type_of_sentence			= type_of_sentence.GetData();
	#undef IDX
	
	if (json_path.IsEmpty())
		return;
	
	String json = StoreAsJson(o, true);
	FileOut fout(json_path);
	fout << json;
	fout.Close();
}

#endif