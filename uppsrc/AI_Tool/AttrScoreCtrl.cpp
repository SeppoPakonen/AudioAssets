#include "AI_Tool.h"


AttrScoreCtrl::AttrScoreCtrl() {
	CtrlLayout(*this);
	
	vsplit.Vert();
	vsplit << attrscores << scoregroup_data << entries;
	
	mainsplit.Vert();
	mainsplit << entrygroup_split << pattern_errors;
	
	entrygroup_split.Horz();
	entrygroup_split << src_entrygroups << src_entries;
	
	attrscores.AddColumn(t_("Scoring"));
	attrscores.AddColumn(t_("Entry count"));
	attrscores.ColumnWidths("4 1");
	entries.AddColumn(t_("Group"));
	entries.AddColumn(t_("Entry"));
	src_entrygroups.AddColumn(t_("Group"));
	src_entrygroups.AddColumn(t_("Entry count"));
	src_entries.AddColumn(t_("Entry"));
	src_entries.AddColumn(t_("Scoring"));
	src_entrygroups.ColumnWidths("4 1");
	
	//pattern_errors.AddColumn(t_("Undefined entry"));
	pattern_errors.Columns(4);
	
	scoregroup_data.SetEditable(false);
	
	prompt_scores <<= THISBACK(OpenPromptScores);
	attrscores <<= THISBACK(DataAttrScore);
	add_scoregroup <<= THISBACK(AddAttrScoreGroup);
	add_entry.WhenAction = [this](){AddAttrScore();};
	src_entrygroups <<= THISBACK(DataGroup);
	src_entries <<= THISBACK(OnEntrySel);
	make_prompt <<= THISBACK(MakePrompt);
	parse_prompt <<= THISBACK(ParsePrompt);
	src_entries.WhenBar << THISBACK(SrcEntryMenu);
}

void AttrScoreCtrl::SrcEntryMenu(Bar& b) {
	b.Add(t_("Add to scoring group"), THISBACK(AddSrcEntryToScoringGroup));
}

void AttrScoreCtrl::OpenPromptScores() {
	
}

void AttrScoreCtrl::Data() {
	Database& db = Database::Single();
	for(int i = 0; i < db.attrscores.groups.GetCount(); i++) {
		AttrScoreGroup& a = db.attrscores.groups[i];
		attrscores.Set(i, 0, a.GetName());
		attrscores.Set(i, 1, a.attrs.GetCount());
	}
	attrscores.SetCount(db.attrscores.groups.GetCount());
	
	int cursor = max(0, db.GetActiveScoreGroupIndex());
	if (cursor >= 0 && cursor < db.attrscores.groups.GetCount())
		attrscores.SetCursor(cursor);
	
	DataAttrList();
	CheckErrors();
	
	if (attrscores.IsCursor())
		DataAttrScore();
	
}

void AttrScoreCtrl::DataAttrList() {
	Database& db = Database::Single();
	
	db.attrscores.RealizeTemp();
	
	Grouplist& g = db.groups;
	for(int i = 0; i < g.groups.GetCount(); i++) {
		Grouplist::Group& gg = db.groups.groups[i];
		src_entrygroups.Set(i, 0, Capitalize(g.Translate(gg.description)));
		src_entrygroups.Set(i, 1, gg.values.GetCount());
		
	}
	
	if (!src_entrygroups.IsCursor() && src_entrygroups.GetCount())
		src_entrygroups.SetCursor(0);
	
	DataGroup();
}

void AttrScoreCtrl::DataGroup() {
	if (!src_entrygroups.IsCursor())
		return;
	
	Database& db = Database::Single();
	Grouplist& g = db.groups;
	int cursor = src_entrygroups.GetCursor();
	Grouplist::Group& gg = db.groups.groups[cursor];
	
	for(int i = 0; i < gg.values.GetCount(); i++) {
		String v = gg.values[i];
		src_entries.Set(i, 0, Capitalize(g.Translate(v)));
		
		String score_txt;
		int score_i = db.attrscores.attr_to_score[cursor][i];
		if (score_i >= 0)
			score_txt = db.attrscores.groups[score_i].GetName();
		src_entries.Set(i, 1, Capitalize(score_txt));
	}
	src_entries.SetCount(gg.values.GetCount());
	
}

void AttrScoreCtrl::DataAttrScore() {
	if (!attrscores.IsCursor())
		return;
	
	int cursor = attrscores.GetCursor();
	String title = attrscores.Get(cursor, 0);
	
	Database& db = Database::Single();
	AttrScoreGroup& o = db.attrscores.groups[cursor];
	db.active_scoregroup = &o;
	
	//structure.SetData(o.structure);
	scoregroup_data.SetData(o.ToString());
	
	for(int i = 0; i < o.attrs.GetCount(); i++) {
		const SnapAttr& a = o.attrs[i];
		const auto& gr = db.groups.groups[a.group];
		entries.Set(i, 0, Capitalize(db.groups.Translate(gr.description)));
		entries.Set(i, 1, Capitalize(db.groups.Translate(gr.values[a.item])));
	}
	entries.SetCount(o.attrs.GetCount());
	
	//DataPatternTree();
}

void AttrScoreCtrl::AddAttrScoreGroup() {
	Database& db = Database::Single();
	int exp_count = db.groups.scorings.GetCount();
	
	String score_str;
	bool b = EditTextNotNull(
		score_str,
		t_("Score string of the group"),
		t_("Score string"),
		0
	);
	if (!b) return;
	
	Vector<String> scores = Split(score_str, " ");
	if (scores.GetCount() != exp_count) {
		PromptOK(DeQtf(Format(t_("error: got %d scores (expected %d)"), scores.GetCount(), exp_count)));
		return;
	}
	
	Vector<int> score_ints;
	for (String& s : scores) {
		int i = StrInt(s);
		if (i < -1 || i > +1) {
			PromptOK(DeQtf(Format(t_("error: expected values between -1 and +1 (got %d)"), i)));
			return;
		}
		score_ints.Add(i);
	}
	
	AttrScoreGroup& g = db.attrscores.GetAdd(score_ints);
	
	db.active_scoregroup = &g;
	
	Data();
}

bool AttrScoreCtrl::AddAttrScore() {
	Database& db = Database::Single();
	
	if (!db.active_scoregroup)
		return false;
	
	String entry_str;
	bool b = EditTextNotNull(
		entry_str,
		t_("Entry name"),
		t_("Name of the entry"),
		0
	);
	if (!b) return false;
	
	return AddAttrScoreEntry("", entry_str);
}

bool AttrScoreCtrl::AddAttrScoreEntry(String group, String entry_str) {
	group = ToLower(group);
	
	Database& db = Database::Single();
	
	bool found = false;
	SnapAttr a;
	Grouplist& g = db.groups;
	String lname = ToLower(entry_str);
	for(int i = 0; i < g.groups.GetCount(); i++) {
		Grouplist::Group& gg = g.groups[i];
		
		//LOG("'" << group << "' vs '" << ToLower(gg.description) << "'");
		if (group.GetCount() && ToLower(gg.description) != group)
			continue;
		
		for(int j = 0; j < gg.values.GetCount(); j++) {
			if (gg.values[j] == lname) {
				a.group = i;
				a.item = j;
				found = true;
				break;
			}
		}
		if (found) break;
	}
	
	if (!found) {
		PromptOK(DeQtf(Format(t_("Entry was not found ('%s')"), entry_str)));
		return false;
	}
	
	AddAttrScoreId(a);
	
	return true;
}

void AttrScoreCtrl::AddAttrScoreId(const SnapAttr& a) {
	Database& db = Database::Single();
	for (AttrScoreGroup& ag : db.attrscores.groups) {
		for (const SnapAttr& a0 : ag.attrs) {
			if (a0 == a) {
				PromptOK(DeQtf(Format(t_("Entry was already in the group '%s'"), ag.GetName())));
				return;
			}
		}
	}
	
	int active_idx = db.GetActiveScoreGroupIndex();
	db.active_scoregroup->attrs.Add(a);
	
	db.attrscores.RealizeTemp();
	db.attrscores.attr_to_score[a.group][a.item] = active_idx;
	
	DataAttrScore();
	DataGroup();
	CheckErrors();
}

void AttrScoreCtrl::AddSrcEntryToScoringGroup() {
	if (!src_entries.IsCursor() || !src_entrygroups.IsCursor())
		return;
	SnapAttr a;
	a.group = src_entrygroups.GetCursor();
	a.item = src_entries.GetCursor();
	AddAttrScoreId(a);
}

void AttrScoreCtrl::OnEntrySel() {
	
	
	
}

void AttrScoreCtrl::CheckErrors() {
	Database& db = Database::Single();
	Grouplist& g = db.groups;
	if (!db.active_pattern)
		return;
	
	db.attrscores.RealizeTemp();
	
	Index<SnapAttr> attrs;
	
	Pattern& pattern = *db.active_pattern;
	pattern.GetAttributes(attrs);
	
	
	int i = 0;
	pattern_errors.Clear();
	for (const SnapAttr& a : attrs.GetKeys()) {
		int score = db.attrscores.attr_to_score[a.group][a.item];
		if (score >= 0)
			continue;
		
		const Grouplist::Group& gg = db.groups.groups[a.group];
		String key = gg.values[a.item];
		//pattern_errors.Set(i, 0, Capitalize(g.Translate(key)));
		pattern_errors.Add(Capitalize(g.Translate(key)));
		i++;
	}
	//pattern_errors.SetCount(i);
	
}

const char* attrscore_prompt = R"ATRSCROO(

List of axes:
-a Mood: joyful/melancholic
-b Mood: playful/serious
-c Mood: uplifting/heavy
-d Mood: lighthearted/somber
-e Mood: humorous/dramatic
-f Social: authoritarian/liberatrian
-g Economic: liberal/conservative
-h Culture: individualism/collective
-i Human strength: strong/weak
-j Motivation: rewarding/punishing
-k Sexualization: sexual/non-sexual
-l Attitude: hopeful/despair
-m Attitude: optimistic/pessimistic
-n Attitude: open/closed
-o Beliefs: spiritual/ secular
-p Expectations: expectations of perfection/acceptance

Combination string from results:
 - a (Mood: joyful/melancholic) b (Mood: playful/serious) c (Mood: uplifting/heavy) etc.

Entries:
- Pronoun: i (m)
${ENTRIES}


Scores per entry:
Pronoun: "i (m)":
-a Mood: joyful/melancholic: +1 Joyful
-b Mood: playful/serious: +1 Playful
-c Mood: uplifting/heavy: +1 Uplifting
-d Mood: lighthearted/somber: +1 Lighthearted
-e Mood: humorous/dramatic: +1 Humorous
-f Social: authoritarian/liberatrian: -1 Libertarian
-g Economic: liberal/conservative: 0
-h Culture: individualism/collective: +1 Individualism
-i Human strength: strong/weak: +1 Strong
-j Motivation: rewarding/punishing: +1 Rewarding
-k Sexualization: sexual/non-sexual: 0
-l Attitude: hopeful/despair: +1 Hopeful
-m Attitude: optimistic/pessimistic: +1 Optimistic
-n Attitude: open/closed: +1 Open
-o Beliefs: spiritual/ secular: 0
-p Expectations: expectations of perfection/acceptance: -1 Acceptance
Combination string: a+1 b+1 c+1 d+1 e+1 f-1 g0 h+1 i+1 j+1 k0 l+1 m+1 n+1 o0 p-1

Pronoun: "i (m)":
Combination string: a+1 b+1 c+1 d+1 e+1 f-1 g0 h+1 i+1 j+1 k0 l+1 m+1 n+1 o0 p-1

${FIRSTENTRY}:
Combination string:)ATRSCROO";

void AttrScoreCtrl::MakePrompt() {
	Database& db = Database::Single();
	Grouplist& g = db.groups;
	String prompt = attrscore_prompt;
	String entries;
	Index<SnapAttr> attrs;
	
	
	// Try making prompt with errors first
	Pattern& pattern = *db.active_pattern;
	pattern.GetAttributes(attrs);
	
	int entry_count = 0;
	for (const SnapAttr& a : attrs.GetKeys()) {
		int score = db.attrscores.attr_to_score[a.group][a.item];
		if (score >= 0)
			continue;
		
		const Grouplist::Group& gg = db.groups.groups[a.group];
		String key = gg.values[a.item];
		
		if (0)
			entries << "- " << key << "\n";
		else
			entries << "- " << gg.description << ": " << key << "\n";
		
		if (!entry_count)
			prompt.Replace("${FIRSTENTRY}", gg.description + ": \"" + key + "\"");
		
		entry_count++;
	}
	
	if (entry_count == 0) {
		db.attrscores.RealizeTemp();
		for(int i = 0; i < g.groups.GetCount(); i++) {
			Grouplist::Group& gg = g.groups[i];
			for(int j = 0; j < gg.values.GetCount(); j++) {
				String key = gg.values[j];
				int scr = db.attrscores.attr_to_score[i][j];
				if (scr >= 0)
					continue;
				entries << "- " << gg.description << ": " << key << "\n";
				
				if (!entry_count)
					prompt.Replace("${FIRSTENTRY}", gg.description + ": \"" + key + "\"");
				
				entry_count++;
			}
		}
	}
	
	if (entry_count == 0) {
		PromptOK(DeQtf(t_("Nothing to ask from AI here")));
		return;
	}
	
	prompt.Replace("${ENTRIES}", entries);
	
	LOG(prompt);
	WriteClipboardText(prompt);
}

void AttrScoreCtrl::ParsePrompt() {
	Database& db = Database::Single();
	Grouplist& g = db.groups;
	int exp_count = db.groups.scorings.GetCount();
	
	String txt = ReadClipboardText();
	txt.Replace("\r", "");
	txt.Replace("\n\n\n\n\n", "\n\n");
	txt.Replace("\n\n\n\n", "\n\n");
	txt.Replace("\n\n\n", "\n\n");
	
	Vector<String> parts = Split(txt, "\n\n");
	DUMPC(parts);
	bool add_entries = false;
	
	for (String& part : parts) {
		part.Replace("\n", "");
		
		int a = part.Find(": ");
		if (a < 0)
			continue;
		
		String group = part.Left(a);
		Index<String> keys;
		while (1) {
			a = part.Find("\"", a);
			if (a < 0)
				break;
			a++;
			int b = part.Find("\"", a);
			if (b < 0)
				break;
			String key = part.Mid(a, b-a);
			keys.FindAdd(key);
			a = b+1;
		}
		
		if (keys.IsEmpty())
			break;
		
		DUMPC(keys);
		
		String search = "Combination string:";
		a = part.Find(search);
		if (a < 0)
			continue;
		
		String score_str = TrimBoth(part.Mid(a + search.GetCount()));
		score_str.Replace(",", "");
		
		if (keys.GetCount() == 1 && keys[0] == "i (m)")
			continue;
		
		LOG(group << ": " << keys[0] << ": " << score_str);
		
		
		// Check that groups and entries exists.
		// If not, ask if groups and entries are added.
		bool fail = false;
		for (String key : keys.GetKeys()) {
			bool found = false;
			for(int i = 0; i < g.groups.GetCount(); i++) {
				Grouplist::Group& gg = g.groups[i];
				if (gg.description == group) {
					for(int j = 0; j < gg.values.GetCount(); j++) {
						String v = gg.values[j];
						if (v == key) {
							found = true;
						}
					}
					if (!found) {
						if (!add_entries)
							add_entries = PromptYesNo(DeQtf(t_("Do you want to add new words?")));
						if (!add_entries) {
							fail = true;
							break;
						}
						gg.values.Add(key);
						LOG("Added '" << key << "' to group '" << gg.description << "'");
						found = true;
					}
				}
			}
			if (fail)
				break;
			if (!found) {
				if (!add_entries)
					add_entries = PromptYesNo(DeQtf(t_("Do you want to add new groups and new words?")));
				if (!add_entries) {
					fail = true;
					break;
				}
				String codename = ToLower(group);
				codename.Replace(" ", "_");
				codename.Replace("'", "");
				codename.Replace(".", "");
				codename.Replace(",", "");
				Grouplist::Group& gg = g.groups.Add(codename);
				gg.description = group;
				gg.clr = Color(Random(256), Random(256), Random(256));
				gg.values.Add(key);
				LOG("Added group '" << gg.description << "'");
				LOG("Added '" << key << "' to group '" << gg.description << "'");
			}
		}
		if (fail) break;
		
		
		// Handle combination: get group
		Vector<String> scores = Split(score_str, " ");
		if (scores.GetCount() != exp_count) {
			PromptOK(DeQtf(Format(t_("error: '%s' got %d scores (expected %d)"), keys[0], scores.GetCount(), exp_count)));
			return;
		}
		
		Vector<int> score_ints;
		int chr = 'a';
		for (String& s : scores) {
			if (s.IsEmpty()) {fail = true; break;}
			if (s[0] != chr++) {fail = true; break;}
			int i = StrInt(s.Mid(1));
			if (i < -1 || i > +1) {
				PromptOK(DeQtf(Format(t_("error: expected values between -1 and +1 (got %d)"), i)));
				return;
			}
			score_ints.Add(i);
		}
		if (fail) {
			PromptOK(DeQtf(Format(t_("error: fail with key %s"), keys[0])));
			break;
		}
		AttrScoreGroup& g = db.attrscores.GetAdd(score_ints);
		db.active_scoregroup = &g;
		
		
		// Add key to the group
		fail = false;
		for (String key : keys.GetKeys()) {
			if (!AddAttrScoreEntry(group, key)) {
				fail = true;
				break;
			}
		}
		if (fail)
			break;
	}
	
	db.Save();
	Data();
}
