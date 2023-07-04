#include "AI_Tool.h"



ScoreCtrl::ScoreCtrl() {
	CtrlLayout(*this);
	
	vsplit.Vert();
	vsplit << scores << part_list;
	
	mainsplit.Vert();
	mainsplit << plotter << list << presets;
	
	add <<= THISBACK(AddPattern);
	structure <<= THISBACK(SavePattern);
	reload <<= THISBACK(Reload);
	load_active_pattern <<= THISBACK(LoadActivePattern);
	scores <<= THISBACK(DataScores);
	part_list <<= THISBACK(DataList);
	//add_preset <<= THISBACK(AddPreset);
	
	scores.AddColumn(t_("Title"));
	
	Grouplist& g = Database::Single().groups;
	list.AddIndex();
	list.AddIndex();
	list.AddColumn(t_("Position"));
	presets.AddColumn(t_("Name"));
	for(Grouplist::ScoringType& t : g.scorings) {
		list.AddColumn(
			//g.Translate(t.klass) + ": " +
			g.Translate(t.axes0) + "/" +
			g.Translate(t.axes1)
			);
		presets.AddColumn(
			//g.Translate(t.klass) + ": " +
			g.Translate(t.axes0) + "/" +
			g.Translate(t.axes1)
			);
	}
	
	part_list.AddColumn(t_("Name"));
	
	list.WhenBar << THISBACK(ListMenu);
	presets.WhenBar << THISBACK(PresetMenu);
	presets.WhenLeftDouble << THISBACK(ApplyPreset);
	
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
	
	DataPresets();
}

void ScoreCtrl::DataPresets() {
	Database& db = Database::Single();
	for(int i = 0; i < db.attrscores.presets.GetCount(); i++) {
		String key = db.attrscores.presets.GetKey(i);
		const auto& values = db.attrscores.presets[i];
		presets.Set(i, 0, key);
		for(int j = 0; j < values.GetCount(); j++)
			presets.Set(i, 1+j, values[j]);
	}
	presets.SetCount(db.attrscores.presets.GetCount());
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
	//o.unique_parts.Clear();
	
	Vector<String> part_str = Split(o.structure, ",");
	Index<String> part_seen;
	
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
				PartScore& part = o.unique_parts[i];
				if (part.len != beats) {
					if (part_seen.Find(name) < 0) {
						part.len = beats;
						part.Realize();
					}
					else {
						PromptOK(DeQtf("error: part length mismatch"));
						return;
					}
				}
			}
			else {
				PartScore& part = o.unique_parts.GetAdd(name);
				part.len = beats;
				part.Realize();
			}
		}
		
		// Add part
		o.parts.Add(name);
		part_seen.Add(name);
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
		int total = 0;
		for(int i = 0; i < o.parts.GetCount(); i++) {
			String part_key = o.parts[i];
			int part_i = o.unique_parts.Find(part_key);
			if (part_i < 0) continue;
			PartScore& part = o.unique_parts[part_i];
			String part_name = db.groups.Translate(o.unique_parts.GetKey(part_i));
			
			for(int i = 0; i < part.len; i++) {
				list.Set(total, 0, part_i);
				list.Set(total, 1, i);
				list.Set(total, 2, part_name + ":" + IntStr(i));
				for(int j = 0; j < part.values.GetCount(); j++) {
					Vector<int>& d = part.values[j];
					int value = i < d.GetCount() ? d[i] : 0;
					int k1 = group_begin+j;
					int k2 = group_begin-2+j; // skip 2 index columns for visible Ctrls
					list.Set(total, k1, value);
					
					Ctrl* c = new EditIntNotNullSpin;
					*c <<= THISBACK2(ListValueChanged, total, j);
					list.SetCtrl(total, k2, c);
				}
				total++;
			}
		}
		list.SetCount(total);
	}
	else {
		int part_i = cursor - 1;
		
		if (part_i >= o.unique_parts.GetCount())
			return;
		
		PartScore& part = o.unique_parts[part_i];
		db.active_partscore = &part;
		plotter.SetPart(o.unique_parts.GetKey(part_i), part);
		String part_name = db.groups.Translate(o.unique_parts.GetKey(part_i));
		for(int i = 0; i < part.len; i++) {
			list.Set(i, 0, part_i);
			list.Set(i, 1, i);
			list.Set(i, 2, part_name + ":" + IntStr(i));
			for(int j = 0; j < part.values.GetCount(); j++) {
				Vector<int>& d = part.values[j];
				int value = i < d.GetCount() ? d[i] : 0;
				int k1 = group_begin+j;
				int k2 = group_begin-2+j; // skip 2 index columns for visible Ctrls
				list.Set(i, k1, value);
				
				Ctrl* c = new EditIntNotNullSpin;
				*c <<= THISBACK2(ListValueChanged, i, j);
				list.SetCtrl(i, k2, c);
			}
		}
		list.SetCount(part.len);
	}
}

void ScoreCtrl::ListValueChanged(int pos, int scoring) {
	Database& db = Database::Single();
	if (!db.active_patternscore)
		return;
	
	int part_i = list.Get(pos, 0);
	int i = list.Get(pos, 1);
	
	int value = list.Get(pos, group_begin+scoring);
	
	PatternScore& o = *db.active_patternscore;
	PartScore& part = o.unique_parts[part_i];
	
	if (scoring >= part.values.GetCount())
		part.values.SetCount(scoring+1);
	
	auto& v = part.values[scoring];
	if (i >= v.GetCount())
		v.SetCount(i+1, 0);
	
	v[i] = value;
	
	// Refresh duplicate values (for whole song)
	for(int j = 0; j < list.GetCount(); j++) {
		int part_i0 = list.Get(j, 0);
		int i0 = list.Get(j, 1);
		if (j != pos && part_i0 == part_i && i == i0) {
			list.Set(j, group_begin+scoring, value);
		}
	}
	
	plotter.Refresh();
}

void ScoreCtrl::LoadActivePattern() {
	Database& db = Database::Single();
	if (!db.active_pattern)
		return;
	
	Pattern& p = *db.active_pattern;
	Grouplist& g = db.groups;
	
	db.active_patternscore = 0;
	for(PatternScore& ps : db.scores) {
		if (ps.file_title == p.file_title) {
			if (!PromptYesNo(DeQtf(Format(t_("Do you want to overwrite the file %s?"), ps.file_title))))
				return;
			db.active_patternscore = &ps;
			break;
		}
	}
	
	if (!db.active_patternscore) {
		PatternScore& ps = db.scores.Add();
		ps.file_title = p.file_title;
		db.active_patternscore = &ps;
	}
	
	PatternScore& ps = *db.active_patternscore;
	ps.Clear();
	ps.structure = p.structure;
	
	Reload();
	
	
	Vector<int> scores;
	for(int i = 0; i < p.unique_parts.GetCount(); i++) {
		String key = p.unique_parts.GetKey(i);
		Part& f = p.unique_parts[i];
		PartScore& d = ps.unique_parts.GetAdd(key);
		
		d.values.Clear();
		d.values.SetCount(g.scorings.GetCount());
		for (auto& v : d.values)
			v.SetCount(f.len, 0);
		
		Vector<PatternSnap*> level_snaps;
		f.snap.GetSnapsLevel(0, level_snaps);
		if (level_snaps.GetCount() != f.len) {
			PromptOK(DeQtf(t_("error: snap count and length mismatch")));
			return;
		}
		
		for(int i = 0; i < level_snaps.GetCount(); i++) {
			PatternSnap* snap = level_snaps[i];
			GetScores(*snap, scores);
			for(int j = 0; j < g.scorings.GetCount(); j++) {
				d.values[j][i] = scores[j];
			}
		}
	}
	
	
	Data();
}

void ScoreCtrl::GetScores(const PatternSnap& snap, Vector<int>& scores) {
	Database& db = Database::Single();
	const PatternSnap* s = &snap;
	int c = db.groups.scorings.GetCount();
	scores.SetCount(c);
	for(auto& v : scores) v = 0;
	
	while (s) {
		for (const AttrScoreGroup& g : db.attrscores.groups) {
			SnapAttr a0;
			
			int match_count = 0;
			for (const SnapAttr& a0 : g.attrs) {
				for (const SnapAttr& a1 : s->attributes.GetKeys()) {
					if (a0 == a1)
						match_count++;
				}
			}
			
			if (match_count) {
				for(int i = 0; i < c; i++) {
					int sc = g.scores[i];
					if (0)
						sc = max(-1, min(+1, sc));
					scores[i] += match_count * sc;
				}
			}
		}
		
		s = s->owner;
	}
}

void ScoreCtrl::ListMenu(Bar& bar) {
	bar.Add(t_("Save as preset"), THISBACK(SavePreset));
	bar.Add(t_("Update focused preset"), THISBACK(UpdatePreset));
}

void ScoreCtrl::PresetMenu(Bar& bar) {
	bar.Add(t_("Apply preset"), THISBACK(ApplyPreset));
	bar.Add(t_("Remove preset"), THISBACK(RemovePreset));
	
}

void ScoreCtrl::SavePreset() {
	if (!list.IsCursor())
		return;
	
	Database& db = Database::Single();
	AttrScore& a = db.attrscores;
	String name;
	bool b = EditTextNotNull(
		name,
		t_("Name of the preset"),
		t_("Preset's name"),
		0
	);
	if (!b) return;
	
	if (a.presets.Find(name) >= 0) {
		PromptOK(DeQtf(Format(t_("Preset '%s' already exists"), name)));
		return;
	}
	
	int c = db.groups.scorings.GetCount();
	Vector<int>& scores = a.presets.Add(name);
	scores.SetCount(c);
	
	int row = list.GetCursor();
	for(int i = 0; i < c; i++) {
		int j = group_begin + i;
		scores[i] = list.Get(row, j);
	}
	
	DataPresets();
}

void ScoreCtrl::UpdatePreset() {
	if (!list.IsCursor() || !presets.IsCursor())
		return;
	
	Database& db = Database::Single();
	AttrScore& a = db.attrscores;
	
	int c = db.groups.scorings.GetCount();
	Vector<int>& scores = a.presets[presets.GetCursor()];
	scores.SetCount(c);
	
	int row = list.GetCursor();
	for(int i = 0; i < c; i++) {
		int j = group_begin + i;
		scores[i] = list.Get(row, j);
	}
	
	DataPresets();
}

void ScoreCtrl::ApplyPreset() {
	Database& db = Database::Single();
	if (!list.IsCursor() || !presets.IsCursor())
		return;
	if (!db.active_patternscore)
		return;
	
	AttrScore& a = db.attrscores;
	
	int c = db.groups.scorings.GetCount();
	Vector<int>& scores = a.presets[presets.GetCursor()];
	scores.SetCount(c);
	
	
	int cur = list.GetCursor();
	int part_i = list.Get(cur, 0);
	int pos = list.Get(cur, 1);
	
	PatternScore& o = *db.active_patternscore;
	PartScore& part = o.unique_parts[part_i];
	
	for(int i = 0; i < c; i++) {
		auto& vv = part.values[i];
		auto& v = vv[pos];
		v = scores[i];
	}
	
	DataList();
}

void ScoreCtrl::RemovePreset() {
	if (!presets.IsCursor())
		return;
	
	Database& db = Database::Single();
	AttrScore& a = db.attrscores;
	
	int cur = presets.GetCursor();
	
	a.presets.Remove(cur);
	
	DataPresets();
}

