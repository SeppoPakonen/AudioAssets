#ifndef _AI_Tool_Data_h_
#define _AI_Tool_Data_h_

struct DataFile {
	String file_title;
	
};

struct Grouplist : DataFile {
	struct Translation : Moveable<Translation> {
		VectorMap<String, String> data;
		
		Translation& Add(String key, String value) {data.GetAdd(key) = value; return *this;}
		void Jsonize(JsonIO& json) {
			if (json.IsStoring())
				SortByKey(data, StdLess<String>());
			json
				("data", data)
				;
			if (json.IsLoading()) {
				VectorMap<String, String> tmp;
				for(int i = 0; i < data.GetCount(); i++)
					tmp.Add(ToLower(data.GetKey(i)), ToLower(data[i]));
				Swap(tmp, data);
			}
		}
	};
	
	struct Group : Moveable<Group> {
		Vector<String> values;
		Color clr;
		String description;
		
		Group& SetDescription(String s) {description = s; return *this;}
		Group& SetColor(Color c) {clr = c; return *this;}
		Group& SetColor(int r, int g, int b) {clr = Color(r,g,b); return *this;}
		Group& operator<<(String s) {values.Add(s); return *this;}
		void Jsonize(JsonIO& json) {
			json
				("description", description)
				("color", clr)
				("values", values)
				;
		}
		bool HasValue(String v) const {
			for (const String& s : values)
				if (s == v)
					return true;
			return false;
		}
	};
	struct ScoringType : Moveable<ScoringType> {
		String klass;
		String axes0, axes1;
		
		void Jsonize(JsonIO& json) {
			json
				("class", klass)
				("axes0", axes0)
				("axes1", axes1)
				;
		}
	};
	
	VectorMap<String, Group> groups;
	VectorMap<String, Translation> translation;
	Vector<ScoringType> scorings;
	
	
	Grouplist();
	
	int GetCount() const {return groups.GetCount();}
	int GetItemCount() const {
		int i = 0;
		for (const Group& g : groups.GetValues())
			i += g.values.GetCount();
		return i;
	}
	String Translate(const String& s);
	
	void Clear() {groups.Clear(); translation.Clear();}
	void Jsonize(JsonIO& json) {
		json
			("groups", groups)
			("translation", translation)
			("scorings", scorings)
			;
		if (json.IsLoading()) {
			//DumbFix();
			String lng = GetCurrentLanguageString().Left(5);
			trans_i = translation.Find(lng);
		}
	}
	void DumbFix();
	void AddScoring(String s, Vector<Grouplist::ScoringType>& scorings);
	
	static const int group_limit = 1024;
	static int trans_i;
	
};

struct Artist : DataFile {
	String name;
	int year_of_birth = 0;
	int year_of_career_begin = 0;
	String biography;
	String musical_style;
	String vibe_of_voice;
	String acoustic_instruments;
	String electronic_instruments;
	
	void Clear() {
		name.Clear();
		year_of_birth = 0;
		year_of_career_begin = 0;
		biography.Clear();
		musical_style.Clear();
		vibe_of_voice.Clear();
		acoustic_instruments.Clear();
		electronic_instruments.Clear();
	}
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("year_of_birth", year_of_birth)
			("year_of_career_begin", year_of_career_begin)
			("biography", biography)
			("musical_style", musical_style)
			("vibe_of_voice", vibe_of_voice)
			("acoustic_instruments", acoustic_instruments)
			("electronic_instruments", electronic_instruments);
	}
	
	bool operator()(const Artist& a, const Artist& b) const {
		if (a.year_of_birth != b.year_of_birth) return a.year_of_birth < b.year_of_birth;
		return a.name < b.name;
	}
	
};

struct Story : DataFile {
	int year = 0;
	String title;
	String meaning;
	String devices;
	String emotion;
	String references;
	String structure;
	String history;
	String storyline;
	String implications;
	
	void Clear() {
		year = 0;
		title.Clear();
		meaning.Clear();
		devices.Clear();
		emotion.Clear();
		references.Clear();
		structure.Clear();
		history.Clear();
		storyline.Clear();
		implications.Clear();
	}
	void Jsonize(JsonIO& json) {
		json
			("year", year)
			("title", title)
			("meaning", meaning)
			("devices", devices)
			("emotion", emotion)
			("references", references)
			("structure", structure)
			("history", history)
			("storyline", storyline)
			("implications", implications)
			;
	}
	bool operator()(const Story& a, const Story& b) const {
		if (a.year != b.year) return a.year < b.year;
		return a.title < b.title;
	}
	
};

struct LinePattern {
	String text;
	Index<String> pronouns;
	String explanation;
	Index<String> elements;
	Index<String> interactions;
	Index<String> interactions_with;
	
	Index<String> moral_interactions;
	Index<String> acting_styles;
	Index<String> tones;
	Index<String> voiceover;
	Index<String> comedic_scenarios;
	
	Index<String> dramatic_scenarios;
	Index<String> comedic_attributes;
	Index<String> humor_attributes;
	String type_of_sentence;
	
	
	void Jsonize(JsonIO& json) {
		json
			("text", text)
			("pronouns", pronouns)
			("explanation", explanation)
			("elements", elements)
			("interactions", interactions)
			("interactions_with", interactions_with)
			
			("moral_interactions", moral_interactions)
			("acting_styles", acting_styles)
			("tones", tones)
			("voiceover", voiceover)
			("comedic_scenarios", comedic_scenarios)
			
			("dramatic_scenarios", dramatic_scenarios)
			("comedic_attributes", comedic_attributes)
			("humor_attributes", humor_attributes)
			("type_of_sentence", type_of_sentence)
			;
	}
};

struct SnapAttr : Moveable<SnapAttr> {
	int group = 0, item = 0;
	
	bool operator==(const SnapAttr& a) const {return group == a.group && item == a.item;}
	void Clear() {
		group = 0;
		item = 0;
	}
	void Jsonize(JsonIO& json) {
		json
			("g", group)
			("i", item)
			;
	}
	String ToString() const {return IntStr(group) + ":" + IntStr(item);}
	hash_t GetHashValue() const {CombineHash c; c.Put(group); c.Put(item); return c;}
};

struct Part;

struct PatternSnap : DataFile {
	PatternSnap* owner = 0;
	Part* part = 0;
	
	One<PatternSnap> a, b;
	int pos = -1, len = 0;
	Index<SnapAttr> attributes;
	
	void Init(int pos, int len);
	void Clear() {
		a.Clear();
		b.Clear();
		attributes.Clear();
	}
	void Jsonize(JsonIO& json) {
		json
			("pos",  pos)
			("len",  len)
			("attributes",  attributes)
		;
		if (json.IsStoring()) {
			if (a) json("a", *a);
			if (b) json("b", *b);
		}
		else {
			a.Clear();
			b.Clear();
			if (len > 1) {
				json("a", a.Create());
				json("b", b.Create());
			}
		}
	}
	void FixPtrs() {
		if (a) {
			a->owner = this;
			a->part = part;
			a->FixPtrs();
		}
		if (b) {
			b->owner = this;
			b->part = part;
			b->FixPtrs();
		}
	}
	int GetLevel() const {
		if (!a) return 0;
		else return a->GetLevel() + 1;
	}
	void GetSnapsLevel(int level, Vector<PatternSnap*>& level_snaps) {
		if (GetLevel() == level)
			level_snaps.Add(this);
		else {
			if (a) a->GetSnapsLevel(level, level_snaps);
			if (b) b->GetSnapsLevel(level, level_snaps);
		}
	}
	void GetAttributes(Index<SnapAttr>& attrs) const {
		for (const SnapAttr& a : this->attributes.GetKeys())
			attrs.FindAdd(a);
		if (a) a->GetAttributes(attrs);
		if (b) b->GetAttributes(attrs);
	}
};

struct Part {
	String name;
	
	int len = 0;
	PatternSnap snap;
	
	void Clear() {
		len = 0;
		snap.Clear();
	}
	void Jsonize(JsonIO& json) {
		json
			("len", len)
			("snap", snap)
			;
	}
	String ToString() const {return "len=" + IntStr(len);}
	void FixPtrs() {
		snap.part = this;
		snap.FixPtrs();
	}
};

struct Pattern : DataFile {
	String					structure;
	Vector<String>			parts;
	ArrayMap<String, Part>	unique_parts;
	
	void Clear() {
		structure.Clear();
		parts.Clear();
		unique_parts.Clear();
	}
	void Jsonize(JsonIO& json) {
		json
			("structure", structure)
			("parts", parts)
			("unique_parts", unique_parts)
			;
		if (json.IsLoading()) {
			FixPtrs();
		}
	}
	bool operator()(const Pattern& a, const Pattern& b) const {
		return a.file_title < b.file_title;
	}
	void FixPtrs() {
		for(int i = 0; i < unique_parts.GetCount(); i++)
			unique_parts[i].name = unique_parts.GetKey(i);
		for (Part& p : unique_parts.GetValues())
			p.FixPtrs();
	}
	void GetSnapsLevel(int level, Vector<PatternSnap*>& level_snaps) {
		for (Part& p : unique_parts.GetValues())
			p.snap.GetSnapsLevel(level, level_snaps);
	}
	void GetAttributes(Index<SnapAttr>& attrs) const {
		for (const Part& p : unique_parts.GetValues())
			p.snap.GetAttributes(attrs);
	}
	
};

struct PartScore {
	int len = 0;
	Vector<Vector<int>> values;
	
	void Clear() {
		len = 0;
	}
	void Jsonize(JsonIO& json) {
		json
			("len", len)
			("values", values)
			;
		if (json.IsLoading())
			Realize();
	}
	void Realize();
};

struct AttrScoreGroup {
	Vector<SnapAttr> attrs;
	Vector<int> scores;
	String name;
	
	void Jsonize(JsonIO& json) {
		json
			("attrs", attrs)
			("scores", scores)
			("name", name)
			;
	}
	String GetName() const {
		if (!name.IsEmpty())
			return name;
		String s;
		for (const int& i : scores) {
			switch (i) {
				#if 0
				case 0: s.Cat('0'); break;
				case 1: s.Cat('+'); break;
				case -1: s.Cat('-'); break;
				#else
				case 0: s.Cat('-'); break;
				case 1: s.Cat('^'); break;
				case -1: s.Cat('_'); break;
				#endif
				default: s << IntStr(i); break;
			}
		}
		return s;
	}
	bool Is(const Vector<int>& s) const {
		if (s.GetCount() != scores.GetCount()) return false;
		for(int i = 0; i < s.GetCount(); i++)
			if (s[i] != scores[i])
				return false;
		return true;
	}
	String ToString() const;
	
};

struct AttrScore : DataFile {
	Array<AttrScoreGroup> groups;
	VectorMap<String, Vector<int>> presets;
	
	// Temp
	Vector<Vector<int>> attr_to_score;
	
	
	AttrScore();
	
	AttrScoreGroup& GetAdd(const Vector<int>& scores) {
		for (AttrScoreGroup& g : groups)
			if (g.Is(scores))
				return g;
		AttrScoreGroup& g = groups.Add();
		g.scores <<= scores;
		return g;
	}
	void Clear() {
		groups.Clear();
	}
	void Jsonize(JsonIO& json) {
		json
			("groups", groups)
			("presets", presets)
			;
	}
	void RealizeTemp();
};

struct PatternScore : DataFile {
	String						structure;
	Vector<String>				parts;
	ArrayMap<String, PartScore>	unique_parts;
	
	void Clear() {
		structure.Clear();
		parts.Clear();
		unique_parts.Clear();
	}
	void Jsonize(JsonIO& json) {
		json
			("structure", structure)
			("parts", parts)
			("unique_parts", unique_parts)
			;
	}
	bool operator()(const PatternScore& a, const PatternScore& b) const {
		return a.file_title < b.file_title;
	}
};

struct Composition : DataFile {
	int year = 0;
	String title;
	int tempo = 0;
	String beat;
	String melody;
	String chord_progression;
	String key_and_mode;
	String texture;
	String structure;
	String genre_style;
	
	void Clear() {
		year = 0;
		title.Clear();
		tempo = 0;
		beat.Clear();
		melody.Clear();
		chord_progression.Clear();
		key_and_mode.Clear();
		texture.Clear();
		structure.Clear();
		genre_style.Clear();
	}
	void Jsonize(JsonIO& json) {
		json
			("year", year)
			("title", title)
			("tempo", tempo)
			("beat", beat)
			("melody", melody)
			("chord_progression", chord_progression)
			("key_and_mode", key_and_mode)
			("texture", texture)
			("structure", structure)
			("genre_style", genre_style)
			;
	}
	bool operator()(const Composition& a, const Composition& b) const {
		if (a.year != b.year) return a.year < b.year;
		return a.title < b.title;
	}
	
};

#define ANALYSIS_KEY_LIST \
	ITEM(Life choices, life_choices, c_) \
	ITEM(Changing the world, changing_world, c_) \
	ITEM(Overcoming adversity, overcoming_adversity, c_) \
	ITEM(Friendship, friendship, c_) \
	ITEM(Love, love, c_) \
	ITEM(Finding purpose, finding_purpose, c_) \
	ITEM(Loyalty, loyalty, c_) \
	ITEM(Journeys, journeys, c_) \
	ITEM(Faith, faith, c_) \
	ITEM(Loss, loss, c_) \
	ITEM(Love, love, sc_) \
	ITEM(Loss and longing, loss_and_longing, sc_) \
	ITEM(Memories, memories, sc_) \
	ITEM(Coming of age, coming_of_age, sc_) \
	ITEM(Happiness and joy, happiness_and_joy, sc_) \
	ITEM(Heartache and sadness, heartache_and_sadness, sc_) \
	ITEM(Intimacy and connection, intimacy_and_connection, sc_) \
	ITEM(Self-esteem and identity, selfesteem_and_identity, sc_) \
	ITEM(Hope and optimism, hope_and_optimism, sc_) \
	ITEM(Fear and despair, fear_and_despair, sc_) \
	ITEM(Redemption and reconciliation, redemption_and_reconciliation, sc_) \
	ITEM(Social justice and intervention, social_justice_and_intervention, sc_) \
	ITEM(Gratitude and appreciation, gratitude_and_appreciation, sc_) \
	ITEM(Rejection and abandonment, rejection_and_abandonment, sc_) \
	ITEM(Longing for belongingness, longing_for_belongingness, sc_) \
	ITEM(Creativity and expression, creativity_and_expression, sc_) \
	ITEM(Hard work and resilience, hard_work_and_resilience, sc_) \
	ITEM(Impermanence and interconnectedness, impermanence_and_interconnectedness, sc_) \
	ITEM(Courage and resilience, courage_and_resilience, sc_) \
	ITEM(Awe and wonder, awe_and_wonder, sc_)


struct Analysis : DataFile {
	int year = 0;
	String title;
	
	#define ITEM(a,b,c) String c##b;
	ANALYSIS_KEY_LIST
	#undef ITEM
	
	void Clear() {
		#define ITEM(a,b,c) c##b.Clear();
		ANALYSIS_KEY_LIST
		#undef ITEM
	}
	void Jsonize(JsonIO& json) {
		json
		#define ITEM(a,b,c) (#c #b, c##b)
		ANALYSIS_KEY_LIST
		#undef ITEM
			;
	}
	bool operator()(const Analysis& a, const Analysis& b) const {
		if (a.year != b.year) return a.year < b.year;
		return a.title < b.title;
		return false;
	}
	
};

struct Timeline : DataFile {
	struct Song {
		String title, prj_name;
		
		void Jsonize(JsonIO& json) {
			json
				("title", title)
				("prj_name", prj_name)
				;
		}
	};
	struct Album {
		String artist, title;
		Date date;
		Array<Song> songs;
		
		void Jsonize(JsonIO& json) {
			json
				("artist", artist)
				("title", title)
				("date", date)
				("songs", songs)
				;
		}
	};
	
	Array<Album> albums;
	
	
	
	void Clear() {
		albums.Clear();
	}
	void Jsonize(JsonIO& json) {
		json
			("albums", albums)
			;
	}
};

template <class T, class PTR>
int VectorFindPtr(PTR* p, T& arr) {
	int i = 0;
	for (auto& r : arr) {
		if (&r == p)
			return i;
		i++;
	}
	return -1;
}

struct Database {
	String				dir;
	Array<Story>		stories;
	Array<Artist>		artists;
	Array<Pattern>		patterns;
	Array<Composition>	compositions;
	Array<Analysis>		analyses;
	AttrScore			attrscores;
	Array<PatternScore>	scores;
	Grouplist			groups;
	Timeline			timeline;
	
	Artist*			active_artist = 0;
	Story*			active_story = 0;
	PatternSnap*	active_snap = 0;
	Pattern*		active_pattern = 0;
	PartScore*		active_partscore = 0;
	PatternScore*	active_patternscore = 0;
	Composition*	active_composition = 0;
	Analysis*		active_analysis = 0;
	AttrScoreGroup*	active_scoregroup = 0;
	
	int GetActiveArtistIndex() const {return VectorFindPtr(active_artist, artists);}
	int GetActivePatternIndex() const {return VectorFindPtr(active_pattern, patterns);}
	int GetActiveStoryIndex() const {return VectorFindPtr(active_story, stories);}
	int GetActiveCompositionIndex() const {return VectorFindPtr(active_composition, compositions);}
	int GetActiveAnalysisIndex() const {return VectorFindPtr(active_analysis, analyses);}
	int GetActiveScoreGroupIndex() const {return VectorFindPtr(active_scoregroup, attrscores.groups);}
	
	void Save();
	void Load();
	void Load(Story& s);
	void Load(Artist& a);
	void Load(Pattern& p);
	void Load(Composition& c);
	void Load(Analysis& c);
	void Load(PatternScore& c);
	void Create(Story& s);
	void Create(Artist& a);
	void Create(Pattern& p);
	void Create(Composition& c);
	void Create(Analysis& c);
	void Create(PatternScore& c);
	Story& CreateStory(String name);
	Artist& CreateArtist(String name);
	Pattern& CreatePattern(String name);
	Composition& CreateComposition(String name);
	Analysis& CreateAnalysis(String name);
	PatternScore& CreateScore(String name);
	
	String GetArtistsDir() const;
	String GetStoriesDir() const;
	String GetPatternsDir() const;
	String GetCompositionsDir() const;
	String GetAnalysesDir() const;
	String GetScoresDir() const;
	String GetAttributesDir() const;
	String GetAttrScoresDir() const;
	String GetTimelineDir() const;
	
	static Database& Single() {static Database db; return db;}
	
};

String GetSnapGroupString(PatternSnap& snap, int group, Index<String>& skip_list);
String Capitalize(String s);

#endif
