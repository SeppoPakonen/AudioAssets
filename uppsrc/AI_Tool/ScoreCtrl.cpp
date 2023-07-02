#include "AI_Tool.h"



ScoreCtrl::ScoreCtrl() {
	CtrlLayout(*this);
	
	vsplit.Vert();
	vsplit << scores << part_list;
	
	mainsplit.Vert();
	mainsplit << plotter << list << preset;
	
	add <<= THISBACK(AddPattern);
	structure <<= THISBACK(SavePattern);
	reload <<= THISBACK(Reload);
	load_active_pattern <<= THISBACK(LoadActivePattern);
	scores <<= THISBACK(DataScores);
	part_list <<= THISBACK(DataList);
	//add_preset <<= THISBACK(AddPreset);
	
	scores.AddColumn(t_("Title"));
	
	Grouplist& g = Database::Single().groups;
	list.AddColumn(t_("Position"));
	preset.AddColumn(t_("Name"));
	for(Grouplist::ScoringType& t : g.scorings) {
		list.AddColumn(
			//g.Translate(t.klass) + ": " +
			g.Translate(t.axes0) + "/" +
			g.Translate(t.axes1)
			);
		preset.AddColumn(
			//g.Translate(t.klass) + ": " +
			g.Translate(t.axes0) + "/" +
			g.Translate(t.axes1)
			);
	}
	preset.AddColumn(t_("Read-only"));
	
	part_list.AddColumn(t_("Name"));
	
}

void ScoreCtrl::AddPreset() {
	String preset_name;
	bool b = EditTextNotNull(
		preset_name,
		t_("Preset's name"),
		t_("Preset's name"),
		0
	);
	if (b) {
		DUMP(preset_name);
	}
}

void ScoreCtrl::AddPattern() {
	String pattern_name;
	bool b = EditTextNotNull(
		pattern_name,
		t_("Pattern's name"),
		t_("Please write pattern's name"),
		0
	);
	if (b) {
		NewPattern(pattern_name);
	}
}

void ScoreCtrl::SavePattern() {
	Database& db = Database::Single();
	if (!db.active_patternscore)
		return;
	
	PatternScore& o = *db.active_patternscore;
	
	o.structure = structure.GetData();
}

void ScoreCtrl::NewPattern(const String& name) {
	Database::Single().CreateScore(name);
	
	Data();
	
	int c = scores.GetCount();
	if (c) scores.SetCursor(c-1);
}

void ScoreCtrl::Data() {
	Database& db = Database::Single();
	for(int i = 0; i < db.scores.GetCount(); i++) {
		PatternScore& a = db.scores[i];
		scores.Set(i, 0, a.file_title);
	}
	scores.SetCount(db.scores.GetCount());
	
	if (db.scores.GetCount() && !scores.IsCursor())
		scores.SetCursor(0);
	
	if (scores.IsCursor())
		DataScores();
	
}

void ScoreCtrl::DataScores() {
	if (!scores.IsCursor())
		return;
	
	int cursor = scores.GetCursor();
	String title = scores.Get(cursor, 0);
	
	Database& db = Database::Single();
	PatternScore& o = db.scores[cursor];
	db.active_patternscore = &o;
	
	structure.SetData(o.structure);
	
	part_list.SetCount(o.unique_parts.GetCount() + 1);
	part_list.Set(0, 0, t_("Whole song"));
	for(int i = 0; i < o.unique_parts.GetCount(); i++) {
		part_list.Set(1+i, 0, db.groups.Translate(o.unique_parts.GetKey(i)));
	}
	if (!part_list.IsCursor() && part_list.GetCount())
		part_list.SetCursor(0);
	
	DataList();
}

void ScoreCtrl::Reload() {
	Database& db = Database::Single();
	if (!db.active_patternscore)
		return;
	PatternScore& o = *db.active_patternscore;
	
	if (o.structure.IsEmpty())
		return;
	
	o.parts.Clear();
	o.unique_parts.Clear();
	
	Vector<String> part_str = Split(o.structure, ",");
	
	for (String& p : part_str) {
		p = TrimBoth(p);
		
		// Split name and beat count
		int i = p.Find(":");
		String name;
		
		if (i < 0) {
			i = o.unique_parts.Find(p);
			if (i >= 0) {
				name = p; // ok
			}
			else {
				PromptOK(DeQtf("error: no ':' character and beat length"));
				return;
			}
		}
		else {
			name = p.Left(i);
			int beats = StrInt(p.Mid(i+1));
			
			// Check for beat length error
			i = o.unique_parts.Find(name);
			if (i >= 0) {
				if (o.unique_parts[i].len != beats) {
					PromptOK(DeQtf("error: part length mismatch"));
					return;
				}
			}
			else {
				PartScore& part = o.unique_parts.Add(name);
				part.len = beats;
				part.Realize();
			}
		}
		
		// Add part
		o.parts.Add(name);
	}
	
	//DUMPM(o.unique_parts);
	//DUMPC(o.parts);
	
	/*for (PartScore& part : o.unique_parts) {
		part.snap.Init(0, part.len);
		part.FixPtrs();
	}*/
	
	DataScores();
}

void ScoreCtrl::DataList() {
	Database& db = Database::Single();
	int cursor = part_list.GetCursor();
	if (cursor < 0)
		return;
	
	if (!db.active_patternscore)
		return;
	
	PatternScore& o = *db.active_patternscore;
	
	// Whole song
	if (cursor == 0) {
		list.SetCount(0);
		plotter.SetWholeSong(o);
	}
	else {
		int part_i = cursor - 1;
		
		if (part_i >= o.unique_parts.GetCount())
			return;
		
		PartScore& part = o.unique_parts[part_i];
		db.active_partscore = &part;
		plotter.SetPart(o.unique_parts.GetKey(part_i), part);
		
		for(int i = 0; i < part.len; i++) {
			list.Set(i, 0, i);
			for(int j = 0; j < part.values.GetCount(); j++) {
				Vector<int>& d = part.values[j];
				int value = i < d.GetCount() ? d[i] : 0;
				int k = 1+j;
				list.Set(i, k, value);
				
				Ctrl* c = new EditIntNotNullSpin;
				*c <<= THISBACK2(ListValueChanged, i, j);
				list.SetCtrl(i, k, c);
			}
		}
		list.SetCount(part.len);
	}
}

void ScoreCtrl::ListValueChanged(int pos, int scoring) {
	Database& db = Database::Single();
	if (!db.active_partscore)
		return;
	
	int value = list.Get(pos, 1+scoring);
	
	PartScore& part = *db.active_partscore;
	if (scoring >= part.values.GetCount())
		part.values.SetCount(scoring+1);
	
	auto& v = part.values[scoring];
	if (pos >= v.GetCount())
		v.SetCount(pos+1, 0);
	
	v[pos] = value;
	
	plotter.Refresh();
}

void ScoreCtrl::LoadActivePattern() {
	
	
	
}
