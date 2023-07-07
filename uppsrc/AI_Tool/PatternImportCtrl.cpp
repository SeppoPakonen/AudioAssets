#include "AI_Tool.h"


PatternImportCtrl::PatternImportCtrl() {
	CtrlLayout(*this);
	
	list.AddIndex();
	list.AddColumn(t_("Artist"));
	list.AddColumn(t_("Title"));
	list.AddColumn(t_("Year"));
	list.ColumnWidths("2 2 1");
	
	output.AddColumn(t_("Line"));
	output.AddColumn(t_("Attributes"));
	output.ColumnWidths("3 8");
	output.WhenBar << THISBACK(OutputBar);
	
	add		<<= THISBACK(AddSong);
	reset	<<= THISBACK(ResetSong);
	input	<<= THISBACK(OnValueChange);
	artist	<<= THISBACK(OnValueChange);
	title	<<= THISBACK(OnValueChange);
	year	<<= THISBACK(OnValueChange);
	list	<<= THISBACK(DataSong);
	copy_prompt		<<= THISBACK(CopyPrompt);
	paste_result	<<= THISBACK(PasteResult);
	copy_test		<<= THISBACK(CopyTest);
	
	mainsplit.Horz() << input << output;
	mainsplit.SetPos(3333);
}

void PatternImportCtrl::AddSong() {
	Database& db = Database::Single();
	db.active_archivedsong = &db.archived_songs.Add();
	Data();
}

void PatternImportCtrl::ResetSong() {
	Database& db = Database::Single();
	if (!db.active_archivedsong)
		return;
	ArchivedSong& song = *db.active_archivedsong;
	song.line_attrs.Clear();
	DataSong();
}

void PatternImportCtrl::OutputBar(Bar& b) {
	b.Add(t_("Remove line"), THISBACK(RemoveLine));
}

void PatternImportCtrl::RemoveLine() {
	Database& db = Database::Single();
	if (!db.active_archivedsong || !output.IsCursor())
		return;
	
	int cursor = output.GetCursor();
	if (cursor >= 0 && cursor < db.active_archivedsong->line_attrs.GetCount()) {
		db.active_archivedsong->line_attrs.Remove(cursor);
		DataSong();
	}
}

void PatternImportCtrl::OnValueChange() {
	Database& db = Database::Single();
	if (!db.active_archivedsong)
		return;
	
	ArchivedSong& a = *db.active_archivedsong;
	a.content = input.GetData();
	a.artist = artist.GetData();
	a.title = title.GetData();
	a.year = year.GetData();
	
	int i = list.GetCursor();
	list.Set(i, 1, a.artist);
	list.Set(i, 2, a.title);
	list.Set(i, 3, a.year);
	list.WhenSel << THISBACK(DataSong);
	
}

void PatternImportCtrl::Data() {
	Database& db = Database::Single();
	for(int i = 0; i < db.archived_songs.GetCount(); i++) {
		ArchivedSong& a = db.archived_songs[i];
		list.Set(i, 0, i);
		list.Set(i, 1, a.artist);
		list.Set(i, 2, a.title);
		list.Set(i, 3, a.year);
	}
	list.SetCount(db.archived_songs.GetCount());
	
	int cursor = max(0, db.GetActiveArchivedSongIndex());
	if (cursor >= 0 && cursor < db.archived_songs.GetCount()) {
		list.SetCursor(cursor);
		DataSong();
	}
	
}

void PatternImportCtrl::DataSong() {
	Database& db = Database::Single();
	if (!list.IsCursor())
		return;
	
	int cursor = list.GetCursor();
	int i = list.Get(cursor, 0);
	ArchivedSong& a = db.archived_songs[i];
	db.active_archivedsong = &a;
	
	input.SetData(a.content);
	artist.SetData(a.artist);
	title.SetData(a.title);
	year.SetData(a.year);
	
	for(int i = 0; i < a.line_attrs.GetCount(); i++) {
		ArchivedSong::Line& line = a.line_attrs[i];
		String s;
		for(int j = 0; j < line.attrs.GetCount(); j++) {
			ArchivedSong::Attr& attr = line.attrs[j];
			if (!s.IsEmpty()) s << ", ";
			s << attr.group << ": " << attr.item;
		}
		output.Set(i, 0, line.line);
		output.Set(i, 1, s);
	}
	output.SetCount(a.line_attrs.GetCount());
}

const char* patimp_base = R"TEXT(
Groups of attributes:
- Pronouns
- Types of sentences
- Contrast and Unexpected Elements
- Moral interactions
- Religiously moral
- Interactions
- Interactions with
- Acting Styles
- Tones
- Dramatic scenarios
- Voiceover Tones
- Comedic sentences
- Comedic scenarios
- Humorous expressions
- Moral interactions mode
- Place
- Place's visibility
- Verbs
- Idea

Some, but not all, groups and attributes in the format "Group: Attribute":
- Pronouns: They (m)
- Pronouns: She
- Types of sentences: Affirmative
- Types of sentences: Declarative
- Contrast and Unexpected Elements: Incongruous pairings of opposites
- Contrast and Unexpected Elements: Incongruous settings and environments
- Moral interactions: Right to privacy
- Moral interactions: Integrity
- Religiously moral: Judgmental ia. with god 
- Religiously moral: Mutual antagonism ia.
- Interactions: Dialogue between people
- Interactions: Creative
- Interactions with: Sea
- Interactions with: Music
- Acting Styles: Funny
- Acting Styles: Light-hearted
- Tones: Encouraging
- Tones: Crooning
- Dramatic scenarios: A redemption story
- Dramatic scenarios: A moral dilemma
- Voiceover Tones: Low, calm
- Voiceover Tones: Detail focus
- Comedic sentences: Self-deprecating
- Comedic sentences: Pun
- Comedic scenarios: Satire
- Comedic scenarios: Self-deprecation
- Humorous expressions: Exaggeration
- Humorous expressions: Mustache twirling
- Moral interactions mode: Ask open ended question about
- Moral interactions mode: Promote someone's
- Place: Your bed (m)
- Place: The beach
- Place's visibility: Is not revealed
- Place's visibility: Is mentioned
- Verbs: Champion
- Idea: Creative and free thinking
- Idea: Sex, drugs, and rock n roll

)TEXT";

void PatternImportCtrl::CopyPrompt() {
	Database& db = Database::Single();
	if (!db.active_archivedsong)
		return;
	ArchivedSong& a = *db.active_archivedsong;
	
	String s;
	if (0) {
		s << patimp_base;
	}
	else {
		s << "Groups of attributes:\n";
		for(int i = 0; i < db.groups.groups.GetCount(); i++) {
			const Grouplist::Group& gg = db.groups.groups[i];
			String key = Capitalize(gg.description);
			s << "- " << key << "\n";
		}
		//s << "- etc.\n";
		s << "\n";
		
		if (1) {
			//s << "Following are example attributes in groups, but other exists too.\n\n";
			
			for(int i = 0; i < db.groups.groups.GetCount(); i++) {
				const Grouplist::Group& gg = db.groups.groups[i];
				String key = Capitalize(gg.description);
				//s << "\"" << key << "\" attributes:\n";
				s << "List of example " << ToLower(key) << ":\n";
				for(int j = 0; j < gg.values.GetCount(); j++) {
					String value = Capitalize(gg.values[j]);
					s << "- " << value << "\n";
				}
				//s << "- etc.\n";
				s << "\n";
			}
			s << "\n";
		}
	}
	
	// Pattern
	String file_title = a.title;	
	int pattern_i = -1;
	for(int i = 0; i < db.patterns.GetCount(); i++) {
		Pattern& p = db.patterns[i];
		if (p.file_title == file_title) {
			pattern_i = i;
			if (!PromptYesNo(DeQtf(t_("Do you want to overwrite previous file?"))))
				return;
			break;
		}
	}
	if (pattern_i < 0) {
		pattern_i = db.patterns.GetCount();
		db.patterns.Add().file_title = file_title;
	}
	Pattern& p = db.patterns[pattern_i];
	p.parts.Clear();
	p.unique_parts.Clear();
	a.tmp_pattern = &p;
	
	// Parse parts string
	String c = a.content;
	c.Replace("\r", "");
	Vector<String> parts = Split(c, "\n\n");
	// structure string
	p.structure.Clear();
	for(int i = 0; i < parts.GetCount(); i++) {
		Vector<String> lines = Split(parts[i], "\n");
		int lc = lines.GetCount()-1;
		String part_title = TrimBoth(lines[0]);
		if (part_title.Right(1) == ":")
			part_title = part_title.Left(part_title.GetCount()-1);
		if (!p.structure.IsEmpty())
			p.structure << ",";
		p.structure << part_title;
		if (lc) p.structure << ":" << lc;
	}
	DUMP(p.structure);
	p.ReloadStructure();
	// Other stuff
	bool found = false;
	Index<String> unique_lines;
	for(int i = 0; i < parts.GetCount(); i++) {
		Part* part = 0;
		Vector<String> lines = Split(parts[i], "\n");
		int lc = lines.GetCount()-1;
		if (lines.GetCount() >= 1) {
			String part_title = TrimBoth(lines[0]);
			if (part_title.Right(1) == ":")
				part_title = part_title.Left(part_title.GetCount()-1);
			part = &p.unique_parts.Get(part_title);
			if (lc) {
				Vector<PatternSnap*> level_snaps;
				part->snap.GetSnapsLevel(0, level_snaps);
				if (level_snaps.GetCount() != lc) {
					LOG(part_title);
					PromptOK(Format(t_("Unexpected mismatch: %d vs %d"), level_snaps.GetCount(), lc));
					return;
				}
				int j = 1;
				for (PatternSnap* snap : level_snaps)
					snap->txt = lines[j++];
			}
		}
		if (lines.GetCount() >= 2) {
			for(int j = 1; j < lines.GetCount(); j++) {
				String tl = TrimBoth(lines[j]);
				bool found = false;
				for (ArchivedSong::Line& l : a.line_attrs) {
					if (l.line == tl) {
						found = true;
						break;
					}
				}
				if (!found) {
					unique_lines.FindAdd(tl);
				}
			}
		}
	}
	p.FixPtrs();
	if (unique_lines.IsEmpty()) {
		WriteClipboardText("");
		return;
	}
	s << "Lyrics:\n";
	for(int i = 0; i < unique_lines.GetCount(); i++) {
		s << unique_lines[i] << "\n";
	}
	
	s << "\nOne attribute from every group is required (or N/A).\n\n";

	s << "\n\nAttributes (in format \"Group: Attribute\"):\nLine 1, \"" << unique_lines[0] << "\"\n- Pronouns:";
	//LOG(s);
	
	WriteClipboardText(s);
}

void PatternImportCtrl::PasteResult() {
	Database& db = Database::Single();
	if (!db.active_archivedsong)
		return;
	ArchivedSong& song = *db.active_archivedsong;
	
	String s = ReadClipboardText();
	s.Replace("\r", "");
	
	int a = s.Find("Attributes (in format \"Group: Attribute\"):");
	if (a < 0) {
		PromptOK(t_("Didn't find 'Attributes' string"));
		return;
	}
	
	a = s.Find("\n", a);
	if (a < 0) return;
	s = s.Mid(a+1);
	
	VectorMap<String, Index<String>> groups;
	Vector<String> lines = Split(s, "\n\n");
	bool fail = false;
	
	for(int i = 0; i < lines.GetCount(); i++) {
		
		Vector<String> v = Split(lines[i], "\n");
		if (v.GetCount() < 2) break;
		
		String header = v[0];
		if (header.Left(5) != "Line ") {
			PromptOK(t_("Expected header string"));
			fail = true;
			break;
		}
		
		// Parse original line again
		a = header.Find("\"");
		int b = header.Find("\"", a+1);
		if (a < 0 || b < 0) {
			PromptOK(t_("Expected '\"'"));
			fail = true;
			break;
		}
		a += 1;
		String txt = header.Mid(a, b-a);
		int line_i = -1;
		for(int j = 0; j < song.line_attrs.GetCount(); j++) {
			ArchivedSong::Line& l = song.line_attrs[j];
			if (l.line == txt) {
				line_i = j;
				break;
			}
		}
		
		ArchivedSong::Line& l = line_i < 0 ? song.line_attrs.Add() : song.line_attrs[line_i];
		l.line = txt;
		
		// Parse attributes
		for(int j = 1; j < v.GetCount(); j++) {
			String& s = v[j];
			if (s.IsEmpty()) continue;
			if (s[0] != '-') {
				PromptOK(t_("Expected '-'"));
				fail = true;
				break;
			}
			
			s = TrimBoth(s.Mid(1));
			a = s.Find(":");
			if (a < 0) {
				PromptOK(t_("Expected ':'"));
				fail = true;
				break;
			}
			
			String item_str = TrimBoth(s.Mid(a+1));
			if (item_str == "N/A")
				continue;
			
			#if 0
			Vector<String> items = Split(item_str, ",");
			for (String item : items) {
				item = TrimBoth(item);
				
				ArchivedSong::Attr& attr = l.attrs.Add();
				attr.group = s.Left(a);
				attr.item = item;
				
				groups.GetAdd(attr.group).FindAdd(attr.item);
			}
			#else
			item_str = TrimBoth(item_str);
			
			ArchivedSong::Attr& attr = l.attrs.Add();
			attr.group = s.Left(a);
			attr.item = item_str;
			
			groups.GetAdd(attr.group).FindAdd(attr.item);
			#endif
		}
		if (fail) break;
		
		
	}
	
	DataSong();
}

void PatternImportCtrl::CopyTest() {
	Database& db = Database::Single();
	if (!db.active_archivedsong)
		return;
	ArchivedSong& song = *db.active_archivedsong;
	
	// Find unique groups and attrs
	VectorMap<String, Index<String>> groups;
	for (ArchivedSong::Line& l : song.line_attrs) {
		for (ArchivedSong::Attr& attr : l.attrs) {
			groups.GetAdd(attr.group).FindAdd(attr.item);
		}
	}
	DUMPM(groups);
	
	// Add new groups and items to Grouplist
	Grouplist& g = db.groups;
	for(int i = 0; i < groups.GetCount(); i++) {
		String lkey = ToLower(groups.GetKey(i));
		const Index<String>& items = groups[i];
		int group_i = -1;
		for(int j = 0; j < g.groups.GetCount(); j++) {
			Grouplist::Group& gg = g.groups[j];
			if (ToLower(gg.description) == lkey) {
				group_i = j;
				break;
			}
		}
		if (group_i < 0) {
			PromptOK(Format(t_("Group not found: %s"), lkey));
			return;
		}
		Grouplist::Group& gg = g.groups[group_i];
		
		bool found = false;
		for (const String& v0 : items.GetKeys()) {
			String lv0 = ToLower(v0);
			for (const String& v : gg.values) {
				String lv1 = ToLower(v);
				if (lv0 == lv1) {
					found = true;
					break;
				}
			}
			if (!found)
				gg.values.Add(v0);
		}
	}
	
	// Write to pattern
	if (song.tmp_pattern) {
		Pattern& p = *song.tmp_pattern;
		Vector<PatternSnap*> level_snaps;
		for (Part& part : p.unique_parts.GetValues()) {
			p.GetSnapsLevel(0, level_snaps);
			for (PatternSnap* snap : level_snaps) {
				bool found = false;
				for (ArchivedSong::Line& l : song.line_attrs) {
					if (l.line == snap->txt) {
						for (ArchivedSong::Attr& attr : l.attrs) {
							SnapAttr sa;
							if (g.FindAttr(attr.group, attr.item, sa)) {
								snap->attributes.FindAdd(sa);
							}
							else {
								LOG("error: not found: " << attr.group << ", " << attr.item);
							}
						}
						found = true;
						break;
					}
				}
			}
		}
	}
	
	
}
