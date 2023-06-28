#include "AI_Tool.h"


PatternCtrl::PatternCtrl() {
	CtrlLayout(*this);
	vsplit.Vert(tree, patterns);
	
	add <<= THISBACK(AddPattern);
	
	patterns.AddColumn("Pattern");
	patterns.NoHeader();
	patterns.WhenAction << THISBACK(DataPattern);
	
	structure <<= THISBACK(SavePattern);
	reload <<= THISBACK(Reload);
	
	tree.WhenSel << THISBACK(OnTreeSel);
}

void PatternCtrl::Reload() {
	if (!active_pattern)
		return;
	Pattern& o = *active_pattern;
	
	if (o.structure.IsEmpty())
		return;
	
	active_snap = 0;
	
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
				Pattern::Part& part = o.unique_parts.Add(name);
				part.len = beats;
			}
		}
		
		// Add part
		o.parts.Add(name);
	}
	
	DUMPM(o.unique_parts);
	DUMPC(o.parts);
	
	for (Pattern::Part& part : o.unique_parts) {
		part.snap.Init(0, part.len);
	}
	
	DataPatternTree();
}

void PatternCtrl::AddPattern() {
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

void PatternCtrl::Data() {
	Database& db = Database::Single();
	for(int i = 0; i < db.patterns.GetCount(); i++) {
		Pattern& a = db.patterns[i];
		patterns.Set(i, 0, a.file_title);
	}
	patterns.SetCount(db.patterns.GetCount());
	
	if (db.patterns.GetCount() && !patterns.IsCursor())
		patterns.SetCursor(0);
	
	if (patterns.IsCursor())
		DataPattern();
	
}

void PatternCtrl::NewPattern(const String& name) {
	Database::Single().CreatePattern(name);
	
	Data();
	
	int c = patterns.GetCount();
	if (c) patterns.SetCursor(c-1);
}

void PatternCtrl::DataPattern() {
	if (!patterns.IsCursor())
		return;
	
	int cursor = patterns.GetCursor();
	String title = patterns.Get(cursor, 0);
	
	Database& db = Database::Single();
	Pattern& o = db.patterns[cursor];
	active_pattern = &o;
	
	structure.SetData(o.structure);
	
	DataPatternTree();
}

void PatternCtrl::SavePattern() {
	if (!active_pattern)
		return;
	
	Pattern& o = *active_pattern;
	
	o.structure = structure.GetData();
}

void PatternCtrl::DataPatternTree() {
	if (!active_pattern)
		return;
	
	Pattern& o = *active_pattern;
	
	tree_snaps.Clear();
	tree.Clear();
	tree.SetRoot(AppImg::Root(), "Root");
	
	for(int i = 0; i < o.unique_parts.GetCount(); i++) {
		Pattern::Part& part = o.unique_parts[i];
		String s = o.unique_parts.GetKey(i) + " :" + IntStr(part.len);
		int id = tree.Add(0, AppImg::Part(), s);
		DataPatternTreeNode(part.snap, id);
	}
	tree.OpenDeep(0);
	
	
}

void PatternCtrl::DataPatternTreeNode(PatternSnap& snap, int parent) {
	tree_snaps.Add(parent, &snap);
	
	if (snap.a) {
		String s = IntStr(snap.a->pos);
		if (snap.a->len > 1) s += ":" + IntStr(snap.a->len);
		int id = tree.Add(parent, AppImg::Snap(), s);
		DataPatternTreeNode(*snap.a, id);
	}
	if (snap.b) {
		String s = IntStr(snap.b->pos);
		if (snap.b->len > 1) s += ":" + IntStr(snap.b->len);
		int id = tree.Add(parent, AppImg::Snap(), s);
		DataPatternTreeNode(*snap.b, id);
	}
}

void PatternCtrl::OnTreeSel() {
	int cursor = tree.GetCursor();
	int i = tree_snaps.Find(cursor);
	if (i >= 0) {
		PatternSnap& snap = *tree_snaps[i];
		active_snap = &snap;
		DataPatternSnap();
	}
}

void PatternCtrl::DataPatternSnap() {
	
}
