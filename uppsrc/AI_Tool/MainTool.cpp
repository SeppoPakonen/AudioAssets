#include "AI_Tool.h"


MainToolCtrl::MainToolCtrl() {
	CtrlLayout(*this);
	
	generate <<= THISBACK(Generate);
	copy     <<= THISBACK(CopyToClipboard);
	
	artists			<<= THISBACK(OnIndexChange);
	patterns		<<= THISBACK(OnIndexChange);
	stories			<<= THISBACK(OnIndexChange);
	compositions	<<= THISBACK(OnIndexChange);
	analyses		<<= THISBACK(OnIndexChange);
	
	
	// Set all active object pointers from here at the beginning
	PostCallback(THISBACK(Data));
	PostCallback(THISBACK(OnIndexChange));
}

void MainToolCtrl::OnIndexChange() {
	Database& db = Database::Single();
	
	#define SEL(x) (this->x.GetIndex() >= 0 && this->x.GetIndex() < db.x.GetCount()) ? &db.x[this->x.GetIndex()] : 0
	db.active_artist		= SEL(artists);
	db.active_pattern		= SEL(patterns);
	db.active_story			= SEL(stories);
	db.active_composition	= SEL(compositions);
	db.active_analysis		= SEL(analyses);
	
}

void MainToolCtrl::Data() {
	Database& db = Database::Single();
	
	artist		= max(0, db.GetActiveArtistIndex());
	pattern		= max(0, db.GetActivePatternIndex());
	story		= max(0, db.GetActiveStoryIndex());
	composition	= max(0, db.GetActiveCompositionIndex());
	analysis	= max(0, db.GetActiveAnalysisIndex());
	
	artists.Clear();
	for(int i = 0; i < db.artists.GetCount(); i++) {
		artists.Add(db.artists[i].file_title);
	}
	if (artist < artists.GetCount())
		artists.SetIndex(artist);
	
	
	patterns.Clear();
	for(int i = 0; i < db.patterns.GetCount(); i++) {
		patterns.Add(db.patterns[i].file_title);
	}
	if (pattern < patterns.GetCount())
		patterns.SetIndex(pattern);
	
	
	stories.Clear();
	for(int i = 0; i < db.stories.GetCount(); i++) {
		stories.Add(db.stories[i].file_title);
	}
	if (story < stories.GetCount())
		stories.SetIndex(story);
	
	
	compositions.Clear();
	for(int i = 0; i < db.compositions.GetCount(); i++) {
		compositions.Add(db.compositions[i].file_title);
	}
	if (composition < compositions.GetCount())
		compositions.SetIndex(composition);
	
	
	analyses.Clear();
	for(int i = 0; i < db.analyses.GetCount(); i++) {
		analyses.Add(db.analyses[i].file_title);
	}
	if (analysis < analyses.GetCount())
		analyses.SetIndex(analysis);
}

const char* example_conv = R"TXT(
Example structured lyrics:
part verse1{line(0:3) {line(0:1) {pronouns {i (m);}types of sentences {observations;}contrast and unexpected elements {exaggeration and surreal situations;}acting styles {funny;}tones {melancholic;}dramatic scenarios {tragic death;}voiceover tones {casual;}comedic sentences {satirical;}comedic scenarios {absurd and exaggerated scenarios;}humorous expressions {irony;}moral interactions {respect;}religiously moral {playful ia. with god;}interactions {social;}interactions with {man;}place {my bed (m);}place's visibility {is mentioned;}verbs {wish;}idea {a free spirited individual who is optimistic;playful;and joyful;yet realistic and accepting of imperfection.;}setting {world;}}line(1:2) {line(1:1) {pronouns {you (m);}types of sentences {statements;}contrast and unexpected elements {subversion of expectations;}acting styles {dramatic;}tones {pleading;}dramatic scenarios {a marriage in crisis;}voiceover tones {contrasting;}comedic sentences {pun;}comedic scenarios {physical comedy;}humorous expressions {playful wordplay;}moral interactions {honesty;}religiously moral {mutual antagonism ia.;}interactions {atheistic;}interactions with {woman;}place {my bed (f);}place's visibility {is said between the lines;}verbs {say;}idea {building strong relationships;}setting {space;}}line(2:1) {pronouns {i (m);}types of sentences {declarative;}contrast and unexpected elements {exaggeration and surreal situations;}moral interactions {respect;}religiously moral {playful ia. with god;}interactions {social;}acting styles {funny;}tones {melancholic;}dramatic scenarios {tragic death;}voiceover tones {casual;}comedic sentences {puns;}comedic scenarios {absurd and exaggerated scenarios;}humorous expressions {irony;}moral interactions mode {promote someone's;}place {my bed (m);}place's visibility {is mentioned;}verbs {wish;}idea {a free spirited individual who is optimistic;playful;and joyful;yet realistic and accepting of imperfection.;}setting {world;}}}}}

Example lyrics (from structured lyrics):
verse1:
Scar tissue that I wish you saw
Sarcastic mister know-it-all
Close your eyes and I'll kiss you, 'cause with the birds I'll share (lonely view)

)TXT";

void MainToolCtrl::Generate() {
	Database& db = Database::Single();
	if (!db.active_pattern || !db.active_artist || !db.active_story || !db.active_composition || !db.active_analysis)
		return;
	
	Artist& a = *db.active_artist;
	Story& s = *db.active_story;
	Composition& c = *db.active_composition;
	Analysis& n = *db.active_analysis;
	Pattern& p = *db.active_pattern;
	
	String o;
	o	<< "Artist name: " << a.name << "\n"
		<< "Year of birth: " << a.year_of_birth << "\n"
		<< "Year of beginning of the career: " << a.year_of_career_begin << "\n"
		<< "Music genre: " << a.musical_style << "\n"
		<< "Voice: " << a.vibe_of_voice << "\n"
		<< "\n\n";
		
	o	<< "Title of lyrics: " << s.title << "\n"
		<< "Year: " << s.year << "\n"
		<< "Meaning: " << s.meaning << "\n"
		<< "Literary devices: " << s.devices << "\n"
		<< "Emotion: " << s.emotion << "\n"
		<< "References: " << s.references << "\n"
		<< "Structure: " << s.structure << "\n"
		<< "History: " << s.history << "\n"
		<< "Storyline: " << s.storyline << "\n"
		<< "Implications: " << s.implications << "\n"
		<< "\n\n";
	
	o	<< "Title of music composition: " << c.title << "\n"
		<< "Year: " << c.year << "\n"
		<< "Tempo: " << c.tempo << " bpm\n"
		<< "Beat/Rhythm: " << c.beat << "\n"
		<< "Melody: " << c.melody << "\n"
		<< "Chord progression: " << c.chord_progression << "\n"
		<< "Key and mode: " << c.key_and_mode << "\n"
		<< "Texture: " << c.texture << "\n"
		<< "Structure: " << c.structure << "\n"
		<< "Genre/Style: " << c.genre_style << "\n"
		<< "\n\n";
	
	o	<< example_conv << "\n\n\nStructured lyrics:\n";
	o	<< p.GetStructuredText(false) << "\n\n";
	o	<< "\nLyrics:\n";
	
	code.SetData(o);
	
	
	if (0) {
		LOG("Groups of attributes:");
		for(int i = 0; i < db.groups.groups.GetCount(); i++) {
			const Grouplist::Group& gg = db.groups.groups[i];
			String key = Capitalize(gg.description);
			LOG("- " << key);
		}
		LOG("");
		
		LOG("Example attributes:");
		for(int i = 0; i < db.groups.groups.GetCount(); i++) {
			const Grouplist::Group& gg = db.groups.groups[i];
			String key = Capitalize(gg.description);
			int c = min(2, gg.values.GetCount());
			Index<int> used;
			for(int j = 0; j < c; j++) {
				int k;
				while (1) {
					k = Random(gg.values.GetCount());
					if (used.Find(k) < 0)
						break;
				}
				used.Add(k);
				String value = Capitalize(gg.values[k]);
				LOG("- " << key << ": " << value);
			}
		}
	}
}

void MainToolCtrl::CopyToClipboard() {
	
}
