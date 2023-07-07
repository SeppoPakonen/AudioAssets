#include "AI_Tool.h"

/*
List of topics what makes the listener to think while listening song's lyrics:

1. Life choices
2. Changing the world
3. Overcoming adversity
4. Friendship
5. Love
6. Finding purpose
7. Loyalty
8. Journeys
9. Faith
10. Loss


List of topics what makes the listener to think subconsciously while listening song's lyrics:
1. Love 
2. Loss and longing
3. Memories 
4. Coming of age
5. Happiness and joy 
6. Heartache and sadness
7. Intimacy and connection
8. Self-esteem and identity
9. Hope and optimism
10. Fear and despair
11. Redemption and reconciliation
12. Social justice and intervention
13. Gratitude and appreciation
14. Rejection and abandonment 
15. Longing for belongingness
16. Creativity and expression
17. Hard work and resilience
18. Impermanence and interconnectedness
19. Courage and resilience
20. Awe and wonder
*/

AnalysisEditCtrl* last;

void EditData(One<Ctrl>& ctrl)
{
	ctrl.Create<DocEdit>() <<= callback(last, &AnalysisEditCtrl::SaveAnalysis);
}


AnalysisEditCtrl::AnalysisEditCtrl() {
	last = this;
	CtrlLayout(*this);
	
	add   <<= THISBACK(AddAnalysis);
	parse <<= THISBACK(ParseAnalysis);
	
	data.AddColumn("Key");
	data.AddColumn("Value").Ctrls(EditData);
	data.ColumnWidths("1 7");
	
	analyses.AddColumn("Analyses");
	analyses.NoHeader();
	analyses.WhenAction << THISBACK(DataAnalysis);
	
	#define ITEM(a,b,c) data.Add(AttrText(#a).NormalPaper(#c[1] == '_' ? Color(242, 255, 231) : Color(211, 238, 255)));
	ANALYSIS_KEY_LIST
	#undef ITEM
	
}

void AnalysisEditCtrl::AddAnalysis() {
	String analysis_name;
	bool b = EditTextNotNull(
		analysis_name,
		"Analysis's name",
		"Please write analysis's name",
		0
	);
	if (b) {
		NewAnalysis(analysis_name);
	}
}

void AnalysisEditCtrl::Data() {
	Database& db = Database::Single();
	for(int i = 0; i < db.analyses.GetCount(); i++) {
		Analysis& a = db.analyses[i];
		analyses.Set(i, 0, a.file_title);
	}
	analyses.SetCount(db.analyses.GetCount());
	
	int cursor = max(0, db.GetActiveAnalysisIndex());
	if (cursor >= 0 && cursor < db.analyses.GetCount())
		analyses.SetCursor(cursor);
	
	if (analyses.IsCursor())
		DataAnalysis();
	
}

void AnalysisEditCtrl::NewAnalysis(const String& name) {
	Database::Single().CreateAnalysis(name);
	
	Data();
	
	int c = analyses.GetCount();
	if (c) analyses.SetCursor(c-1);
}

void AnalysisEditCtrl::DataAnalysis() {
	if (!analyses.IsCursor())
		return;
	
	int cursor = analyses.GetCursor();
	String title = analyses.Get(cursor, 0);
	
	Database& db = Database::Single();
	Analysis& o = db.analyses[cursor];
	db.active_analysis = &o;
	
	int i = 0;
	#define ITEM(a,b,c) data.Set(i++, 1, o.c##b);
	ANALYSIS_KEY_LIST
	#undef ITEM
}

void AnalysisEditCtrl::SaveAnalysis() {
	Database& db = Database::Single();
	if (!db.active_analysis)
		return;
	
	Analysis& o = *db.active_analysis;
	
	int i = 0;
	#define ITEM(a,b,c) o.c##b = data.Get(i++, 1);
	ANALYSIS_KEY_LIST
	#undef ITEM
}

void AnalysisEditCtrl::ParseAnalysis() {
	Database& db = Database::Single();
	if (!db.active_analysis)
		return;
	
	String txt = ReadClipboardText();
	
	txt.Replace("\r", "");
	Vector<String> lines = Split(txt, "\n");
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		l = TrimBoth(l);
		if (l.IsEmpty()) {
			lines.Remove(i--);
		}
	}
	
	if (lines.GetCount() != 60) {
		PromptOK("Error: expected 60 lines, got " + IntStr(lines.GetCount()));
		return;
	}
	
	int i = 1, j = 0;
	#define ITEM(a, b, c) \
		db.active_analysis->c##b = TrimBoth(lines[i]); \
		i += 2; j++;
	ANALYSIS_KEY_LIST
	#undef ITEM
	
	DataAnalysis();
}
