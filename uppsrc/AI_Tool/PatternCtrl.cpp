#include "AI_Tool.h"


PatternCtrl::PatternCtrl() {
	CtrlLayout(*this);
	vsplit.Vert();
	vsplit << tree << plot << patterns;
	
	add <<= THISBACK(AddPattern);
	
	patterns.AddColumn("Pattern");
	patterns.NoHeader();
	patterns.WhenAction << THISBACK(DataPattern);
	
	structure <<= THISBACK(SavePattern);
	reload <<= THISBACK(Reload);
	
	tree.WhenSel << THISBACK(OnTreeSel);
	list.WhenSel << THISBACK(OnListSel);
	
	attr.WhenUpdate << THISBACK(DataList);
	
	list.Hide();
	
	list.AddColumn(t_("Position"));
	Database& db = Database::Single();
	Grouplist& g = db.groups;
	for(int i = 0; i < db.groups.groups.GetCount(); i++) {
		Grouplist::Group& gg = db.groups.groups[i];
		String key = g.Translate(gg.description);
		list.AddColumn(key);
	}
	
	
	mainsplit.Vert();
	mainsplit << list << attr;
	mainsplit.SetPos(1000);
}

void PatternCtrl::SwitchView() {
	/*attr.Hide();
	list.Hide();
	
	view = (view + 1) % 2;
	
	switch (view) {
		case 0: attr.Show(); break;
		case 1: list.Show(); break;
	}*/
}

void PatternCtrl::Reload() {
	if (!active_pattern)
		return;
	Pattern& o = *active_pattern;
	
	if (o.structure.IsEmpty())
		return;
	
	active_snap = 0;
	attr.active_snap = 0;
	
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
				Part& part = o.unique_parts.Add(name);
				part.len = beats;
			}
		}
		
		// Add part
		o.parts.Add(name);
	}
	
	DUMPM(o.unique_parts);
	DUMPC(o.parts);
	
	for (Part& part : o.unique_parts) {
		part.snap.Init(0, part.len);
		part.FixPtrs();
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
	
	
	DataPatternSnap();
	DataList();
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
		Part& part = o.unique_parts[i];
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
		FocusList();
		DataPatternSnap();
		DataList();
	}
}

void PatternCtrl::OnListSel() {
	int cursor = list.GetCursor();
	if (cursor >= 0 && cursor < level_snaps.GetCount()) {
		PatternSnap& snap = *level_snaps[cursor];
		active_snap = &snap;
		FocusTree();
		DataPatternSnap();
	}
}

void PatternCtrl::FocusTree() {
	for(int i = 0; i < tree_snaps.GetCount(); i++) {
		if (tree_snaps[i] == active_snap) {
			tree.SetCursor(tree_snaps.GetKey(i));
			break;
		}
	}
}

void PatternCtrl::FocusList() {
	for(int i = 0; i < level_snaps.GetCount(); i++) {
		if (level_snaps[i] == active_snap) {
			list.SetCursor(i);
			break;
		}
	}
}

void PatternCtrl::DataPatternSnap() {
	attr.active_snap = active_snap;
	attr.Load();
	attr.Refresh();
}

void PatternCtrl::DataList() {
	if (!active_pattern || !active_snap)
		return;
	
	Database& d = Database::Single();
	Grouplist& g = d.groups;
	
	Pattern& o = *active_pattern;
	PatternSnap& s = *active_snap;
	int level = s.GetLevel();
	
	level_snaps.SetCount(0);
	o.GetSnapsLevel(level, level_snaps);
	
	list.SetCount(level_snaps.GetCount());
	Index<String> skip_list;
	for(int i = 0; i < level_snaps.GetCount(); i++) {
		PatternSnap& s0 = *level_snaps[i];
		String pos = s0.part->name + " :" + IntStr(s0.pos);
		if (s0.len > 1)
			pos += ":" + IntStr(s0.len);
		list.Set(i, 0, pos);
		
		for(int j = 0; j < g.GetCount(); j++) {
			skip_list.Clear();
			String s = GetSnapGroupString(s0, j, skip_list);
			PatternSnap* owner = s0.owner;
			String inherited;
			while (owner) {
				String s2 = GetSnapGroupString(*owner, j, skip_list);
				if (s2.GetCount()) {
					if (!inherited.IsEmpty()) inherited << ", ";
					inherited += s2;
				}
				owner = owner->owner;
			}
			if (inherited.GetCount()) {
				if (s.GetCount()) s << ", ";
				s << "[" << inherited << "]";
			}
			list.Set(i, 1 + j, s);
		}
		
	}
	
	if (!list.IsCursor())
		list.SetCursor(0);
	
}

String GetSnapGroupString(PatternSnap& snap, int group, Index<String>& skip_list) {
	Grouplist& g = Database::Single().groups;
	Grouplist::Group& gg = g.groups[group];
	
	String s;
	for(const SnapAttr& a : snap.attributes.GetKeys()) {
		if (a.group != group)
			continue;
		
		String item = g.Translate(gg.values[a.item]);
		
		if (skip_list.Find(item) < 0) {
			if (!s.IsEmpty())
				s << ", ";
			skip_list.Add(item);
			s << item;
		}
	}
	return s;
}

