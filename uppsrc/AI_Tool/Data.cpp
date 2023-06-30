#include "AI_Tool.h"

String Database::GetArtistsDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "artists" DIR_SEPS;
}

String Database::GetStoriesDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "stories" DIR_SEPS;
}

String Database::GetPatternsDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "patterns" DIR_SEPS;
}

String Database::GetCompositionsDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "compositions" DIR_SEPS;
}

String Database::GetAnalysesDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "analyses" DIR_SEPS;
}

void Database::Save() {
	{
		String dir = GetArtistsDir();
		RealizeDirectory(dir);
		for (Artist& o : artists) {
			String json_path = dir + o.file_title + ".json";
			String json = StoreAsJson(o, true);
			FileOut fout(json_path);
			fout << json;
			fout.Close();
		}
	}
	{
		String dir = GetPatternsDir();
		RealizeDirectory(dir);
		for (Pattern& o : patterns) {
			String json_path = dir + o.file_title + ".json";
			String json = StoreAsJson(o, true);
			FileOut fout(json_path);
			fout << json;
			fout.Close();
		}
	}
	{
		String dir = GetStoriesDir();
		RealizeDirectory(dir);
		for (Story& o : stories) {
			String json_path = dir + o.file_title + ".json";
			String json = StoreAsJson(o, true);
			FileOut fout(json_path);
			fout << json;
			fout.Close();
		}
	}
	{
		String dir = GetCompositionsDir();
		RealizeDirectory(dir);
		for (Composition& o : compositions) {
			String json_path = dir + o.file_title + ".json";
			String json = StoreAsJson(o, true);
			FileOut fout(json_path);
			fout << json;
			fout.Close();
		}
	}
	{
		String dir = GetAnalysesDir();
		RealizeDirectory(dir);
		for (Analysis& o : analyses) {
			String json_path = dir + o.file_title + ".json";
			String json = StoreAsJson(o, true);
			FileOut fout(json_path);
			fout << json;
			fout.Close();
		}
	}
}

void Database::Load() {
	{
		artists.Clear();
		String dir = GetArtistsDir();
		String search = dir + "*.json";
		
		FindFile ff;
		if (ff.Search(search)) do {
			if (ff.IsFile()) {
				String path = ff.GetPath();
				Artist& a = artists.Add();
				a.file_title = GetFileTitle(path);
				Load(a);
			}
		}
		while (ff.Next());
		Sort(artists, Artist());
	}
	
	{
		stories.Clear();
		String dir = GetStoriesDir();
		String search = dir + "*.json";
		
		FindFile ff;
		if (ff.Search(search)) do {
			if (ff.IsFile()) {
				String path = ff.GetPath();
				Story& s = stories.Add();
				s.file_title = GetFileTitle(path);
				Load(s);
				DUMP(s.year);
			}
		}
		while (ff.Next());
		Sort(stories, Story());
	}
	
	{
		patterns.Clear();
		String dir = GetPatternsDir();
		String search = dir + "*.json";
		
		FindFile ff;
		if (ff.Search(search)) do {
			if (ff.IsFile()) {
				String path = ff.GetPath();
				Pattern& p = patterns.Add();
				p.file_title = GetFileTitle(path);
				Load(p);
			}
		}
		while (ff.Next());
		Sort(patterns, Pattern());
	}
	
	{
		compositions.Clear();
		String dir = GetCompositionsDir();
		String search = dir + "*.json";
		
		FindFile ff;
		if (ff.Search(search)) do {
			if (ff.IsFile()) {
				String path = ff.GetPath();
				Composition& c = compositions.Add();
				c.file_title = GetFileTitle(path);
				Load(c);
			}
		}
		while (ff.Next());
		Sort(compositions, Composition());
	}
	
	{
		analyses.Clear();
		String dir = GetAnalysesDir();
		String search = dir + "*.json";
		
		FindFile ff;
		if (ff.Search(search)) do {
			if (ff.IsFile()) {
				String path = ff.GetPath();
				Analysis& a = analyses.Add();
				a.file_title = GetFileTitle(path);
				Load(a);
			}
		}
		while (ff.Next());
		Sort(analyses, Analysis());
	}
}

void Database::Load(Artist& a) {
	String json_path = GetArtistsDir() + a.file_title + ".json";
	LoadFromJsonFile(a, json_path);
}

void Database::Load(Pattern& p) {
	String json_path = GetPatternsDir() + p.file_title + ".json";
	LoadFromJsonFile(p, json_path);
}

void Database::Load(Story& s) {
	String json_path = GetStoriesDir() + s.file_title + ".json";
	LoadFromJsonFile(s, json_path);
}

void Database::Load(Composition& c) {
	String json_path = GetCompositionsDir() + c.file_title + ".json";
	LoadFromJsonFile(c, json_path);
}

void Database::Load(Analysis& a) {
	String json_path = GetAnalysesDir() + a.file_title + ".json";
	LoadFromJsonFile(a, json_path);
}

void Database::Create(Story& s) {
	ASSERT(!s.file_title.IsEmpty());
	String dir = GetStoriesDir();
	
	RealizeDirectory(dir);
	
	String path = dir + s.file_title + ".json";
	
	FileOut fout(path);
	fout << "\n";
	fout.Close();
}

void Database::Create(Artist& a) {
	ASSERT(!a.file_title.IsEmpty());
	String dir = GetArtistsDir();
	
	RealizeDirectory(dir);
	
	String path = dir + a.file_title + ".json";
	
	FileOut fout(path);
	fout << "\n";
	fout.Close();
}

void Database::Create(Pattern& p) {
	ASSERT(!p.file_title.IsEmpty());
	String dir = GetPatternsDir();
	
	RealizeDirectory(dir);
	
	String path = dir + p.file_title + ".json";
	
	FileOut fout(path);
	fout << "\n";
	fout.Close();
	
	p.FixPtrs();
}

void Database::Create(Composition& c) {
	ASSERT(!c.file_title.IsEmpty());
	String dir = GetCompositionsDir();
	
	RealizeDirectory(dir);
	
	String path = dir + c.file_title + ".json";
	
	FileOut fout(path);
	fout << "\n";
	fout.Close();
}

void Database::Create(Analysis& a) {
	ASSERT(!a.file_title.IsEmpty());
	String dir = GetAnalysesDir();
	
	RealizeDirectory(dir);
	
	String path = dir + a.file_title + ".json";
	
	FileOut fout(path);
	fout << "\n";
	fout.Close();
}

Story& Database::CreateStory(String name) {
	for (Story& s : stories)
		if (s.file_title == name)
			return s;
	
	Story& s = stories.Add();
	s.file_title = name;
	Create(s);
	return s;
}

Artist& Database::CreateArtist(String name) {
	for (Artist& a : artists)
		if (a.file_title == name)
			return a;
	
	Artist& a = artists.Add();
	a.file_title = name;
	Create(a);
	return a;
}

Pattern& Database::CreatePattern(String name) {
	for (Pattern& p : patterns)
		if (p.file_title == name)
			return p;
	
	Pattern& p = patterns.Add();
	p.file_title = name;
	Create(p);
	return p;
}

Composition& Database::CreateComposition(String name) {
	for (Composition& c : compositions)
		if (c.file_title == name)
			return c;
	
	Composition& c = compositions.Add();
	c.file_title = name;
	Create(c);
	return c;
}

Analysis& Database::CreateAnalysis(String name) {
	for (Analysis& a : analyses)
		if (a.file_title == name)
			return a;
	
	Analysis& a = analyses.Add();
	a.file_title = name;
	Create(a);
	return a;
}




void PatternSnap::Init(int pos, int len) {
	this->pos = pos;
	this->len = len;
	
	if (len > 1) {
		int len2 = len / 2;
		int len0 = len2;
		int len1 = len - len2;
		a.Create().Init(pos,       len0);
		b.Create().Init(pos + len0, len1);
	}
}





Grouplist::Grouplist() {
	pronouns_clr = Color(255, 0, 212);
	pronouns
		<< t_("I (m)")
		<< t_("I (f)")
		<< t_("you (m)")
		<< t_("you (f)")
		<< t_("he")
		<< t_("she")
		<< t_("we")
		<< t_("we (m)")
		<< t_("we (f)")
		<< t_("plural you")
		<< t_("plural you (m)")
		<< t_("plural you (f)")
		<< t_("they")
		<< t_("they (m)")
		<< t_("they (f)")
		<< t_("one")
		<< t_("everyone")
		<< t_("singular pronoun")
		<< t_("person")
		<< t_("that")
		;
	
	elements_clr = Color(255, 212, 0);
	elements
		<< t_("Exaggeration and Surreal Situations")
		<< t_("Subversion of Expectations")
		<< t_("Absurd Concepts")
		<< t_("Juxtaposition")
		<< t_("Unexpected Combinations")
		<< t_("Repetition of Unusual Themes")
		<< t_("Props and Characters that are Unusual")
		<< t_("Utilization of Unconventional Format")
		<< t_("Incongruous Settings and Environments")
		<< t_("Flips of Gender Roles")
		<< t_("Flips of Social Constructs")
		<< t_("Incongruous Pairings of Opposites")
		<< t_("Usage of Unexpected Symbolism")
		<< t_("Hope for future without immediate action")
		<< t_("Unexpected turning of a positive thing into negativity")
		<< t_("Unexpected turning of a negative thing into a positive")
		<< t_("Using an abstract concept")
		<< t_("Unexpected comparison between two")
		<< t_("Serious tone but humorous expectation")
		<< t_("Unexpected emotional reaction to treatment received from others")
		;
	
	moral_interactions_clr = Color(246, 22, 0);
	moral_interactions
		<< t_("Respect")
		<< t_("Honesty")
		<< t_("Integrity")
		<< t_("Kindness")
		<< t_("Compassion")
		<< t_("Gratitude")
		<< t_("Forgiveness")
		<< t_("Trust")
		<< t_("Responsibility")
		<< t_("Patience")
		<< t_("Humility")
		<< t_("Loyalty")
		<< t_("Fairness")
		<< t_("right to privacy")
		<< t_("strength")
		<< t_("resilience")
		<< t_("spiritual meaningfulness")
		<< t_("judgement")
		<< t_("altruism")
		;
	
	moral_interaction_modes_clr = Color(22, 22, 246);
	moral_interaction_modes
		<< t_("promote someone's")
		<< t_("to bring up your own")
		<< t_("emphasize someone's")
		<< t_("ask open ended question about")
		<< t_("support someone's")
		<< t_("challenges assumptions about")
		<< t_("give feedback on")
		<< t_("to give encouragement of strength")
		<< t_("to invite a discussion about")
		<< t_("think about your own")
		<< t_("understand someone else's")
		<< t_("questioning")
		<< t_("themes around")
		<< t_("Suggests a sense of obligation to have ")
		;
	
	religious_moral_interactions_clr = Color(226, 42, 0);
	religious_moral_interactions
		<< t_("Playful ia. with God")
		<< t_("Mutual Antagonism ia.")
		<< t_("Negligent ia. with God")
		<< t_("Mechanical/Workaround ia. with God")
		<< t_("Transcendental ia. with God ")
		<< t_("Devotional ia. with God")
		<< t_("Conversational ia. with God")
		<< t_("Parental ia. with God")
		<< t_("Playful ia. with Personified Deities ")
		<< t_("Quasi-Moral Behavioural ia. ")
		<< t_("Judgmental ia. with God ")
		<< t_("Confrontational ia. with God ")
		<< t_("Sympathetic ia. with God ")
		<< t_("Reverential ia. with God")
		<< t_("Playful ia. with Angels")
		<< t_("Oblivious ia. with Angels")
		<< t_("Trustful ia. with Angels ")
		<< t_("Supernatural ia. with Angels ")
		<< t_("Playful ia. with Spirits ")
		<< t_("Playful ia. with the Dead ")
		<< t_("Playful ia. with Nature")
		<< t_("Conflict ia. with Animals ")
		<< t_("Mutual Respect ia. with Animals ")
		<< t_("Spiritual ia. with Animals");
	
	interactions_clr = Color(28, 85, 255);
	interactions
		<< t_("Social")
		<< t_("Atheistic")
		<< t_("Religious")
		<< t_("Playful")
		<< t_("Sexual")
		<< t_("Romantic")
		<< t_("Business")
		<< t_("Social")
		<< t_("Political")
		<< t_("Cultural")
		<< t_("Emotional")
		<< t_("Intellectual")
		<< t_("Creative")
		<< t_("Virtual")
		<< t_("Digital")
		<< t_("Serious")
		<< t_("Requesting for confirmation")
		<< t_("Suggesting actions and not just thoughts")
		<< t_("Dialogue between people")
		;
	
	with_clr = Color(85, 127, 0);
	with
		<< t_("man")
		<< t_("woman")
		<< t_("girl")
		<< t_("boy")
		<< t_("animal")
		<< t_("husband")
		<< t_("wife")
		<< t_("friend")
		<< t_("friends")
		<< t_("stranger")
		<< t_("nature")
		<< t_("technology")
		<< t_("music")
		<< t_("art")
		<< t_("sea")
		<< t_("forest")
		<< t_("river")
		<< t_("road")
		<< t_("path in forest")
		<< t_("car")
		<< t_("train")
		<< t_("night club")
		<< t_("bar")
		<< t_("restaurant")
		<< t_("beach")
		<< t_("mother")
		<< t_("father")
		;
	
	acting_styles_clr = Color(198, 42, 200);
	acting_styles
		<< t_("funny")
		<< t_("dramatic")
		<< t_("seductive")
		<< t_("devious")
		<< t_("mysterious")
		<< t_("passionate")
		<< t_("mischievous")
		<< t_("powerful")
		<< t_("wistful")
		<< t_("determined")
		<< t_("aggressive")
		<< t_("implying urgency")
		<< t_("light-hearted")
		<< t_("broody")
		;
	
	tones_clr = Color(28, 42, 150);
	tones
		<< t_("Melancholic")
		<< t_("Pleading")
		<< t_("Earnest")
		<< t_("Bittersweet")
		<< t_("Desperate")
		<< t_("Nostalgic")
		<< t_("Wistful")
		<< t_("Reflection")
		<< t_("Somber")
		<< t_("Crooning")
		<< t_("Comforting")
		<< t_("Hopeful")
		<< t_("Dreamy")
		<< t_("Pensive")
		<< t_("Restrained")
		<< t_("Warbling")
		<< t_("Direct")
		<< t_("Assertive")
		<< t_("Isolated")
		<< t_("Mournful")
		<< t_("Commanding")
		<< t_("Insistent")
		<< t_("Uplifting")
		<< t_("Inspirational")
		<< t_("Inquisitive")
		<< t_("Angry")
		<< t_("Pity")
		<< t_("Confused")
		<< t_("Unknowing")
		<< t_("Sarcastic")
		<< t_("Sad")
		<< t_("Disappointed")
		<< t_("Challenging")
		<< t_("Encouraging")
		;
	
	voiceover_tones_clr = Color(28, 255, 200);
	voiceover_tones
		<< t_("casual")
		<< t_("contrasting")
		<< t_("conversational")
		<< t_("deep")
		<< t_("detail focus")
		<< t_("educating")
		<< t_("energetic")
		<< t_("excited")
		<< t_("enthusiastic")
		<< t_("gentle")
		<< t_("laid-back")
		<< t_("mellow")
		<< t_("natural")
		<< t_("nostalgic")
		<< t_("optimistic")
		<< t_("sarcastic")
		<< t_("sophisticated")
		<< t_("suggestive")
		<< t_("witty")
		<< t_("confident")
		<< t_("assured")
		<< t_("motivating")
		<< t_("reflective")
		;
	
	comedic_scenarios_clr = Color(141, 42, 150);
	comedic_scenarios
		<< t_("Absurd and Exaggerated Scenarios")
		<< t_("Physical Comedy")
		<< t_("Tragedy Mocks Success")
		<< t_("Self-Deprecation")
		<< t_("Deadpan Humor")
		<< t_("Satire")
		<< t_("Working Against Cliche")
		<< t_("Repetitive Actions and Situations")
		<< t_("Role Reversal")
		<< t_("Slapstick")
		<< t_("Miscommunication and Puns")
		<< t_("Parody")
		<< t_("Quirky Characters")
		<< t_("Social Commentary")
		<< t_("Absurd Dialogue")
		<< t_("Playing With Expectations")
		<< t_("Silliness")
		;
	
	dramatic_scenarios_clr = Color(56, 42, 200);
	dramatic_scenarios
		<< t_("tragic death")
		<< t_("a marriage in crisis")
		<< t_("a serious illness")
		<< t_("a family in deep debt")
		<< t_("a crime")
		<< t_("a personal betrayal")
		<< t_("a difficult decision")
		<< t_("an addiction")
		<< t_("a forbidden romance")
		<< t_("a risk")
		<< t_("a moral dilemma")
		<< t_("conflicts between cultures")
		<< t_("political unrest")
		<< t_("a character's inner struggle")
		<< t_("a redemption story")
		<< t_("racism and discrimination")
		<< t_("embracing fate")
		<< t_("facing mortality")
		<< t_("fighting against the odds")
		<< t_("Conflict with someone")
		<< t_("Bring up a moral issue")
		<< t_("Existential crisis")
		;
	
	types_of_sentences_clr = Color(0, 150, 246);
	types_of_sentences
		<< t_("observations")
		<< t_("statements")
		<< t_("questions")
		<< t_("declarative")
		<< t_("imperative")
		<< t_("interrogative")
		<< t_("affirmative")
		;
	
	comedic_sentences_clr = Color(255, 170, 0);
	comedic_sentences
		<< t_("Satirical")
		<< t_("Pun")
		<< t_("Irony")
		<< t_("Sarcasm")
		<< t_("Hyperbole")
		<< t_("Slapstick")
		<< t_("Absurdist")
		<< t_("Situational")
		<< t_("Wordplay")
		<< t_("Self-Deprecating")
		<< t_("Parodical")
		<< t_("Caricature")
		<< t_("Puns")
		<< t_("Comic Hyperbole")
		<< t_("Absurdity")
		<< t_("Mockery")
		<< t_("Self-Mockery")
		<< t_("Play-on-Words")
		<< t_("Situational Irony")
		<< t_("Topical")
		<< t_("Improvisation")
		<< t_("Sight Gags")
		<< t_("Fish-Out-of-Water")
		<< t_("Hammy Acting")
		<< t_("Musical Comedy")
		<< t_("Suggests lightheartedness")
		;
	
	humorous_expressions_clr = Color(255, 42, 200);
	humorous_expressions
		<< t_("Irony")
		<< t_("Playful Wordplay")
		<< t_("Ironical statements")
		<< t_("Romantic Humor")
		<< t_("Sexual Humor")
		<< t_("Self-Deprecating Humor")
		<< t_("Dark Humor")
		<< t_("Casual and Conversational Tone")
		<< t_("Mockery Humor")
		<< t_("Self-Mockery Humor")
		<< t_("Sarcasm")
		<< t_("Slapstick")
		<< t_("Facetiousness")
		<< t_("Gallows Humor")
		<< t_("Sex Humor")
		<< t_("Surreal Humor")
		<< t_("Absurdist Humor")
		<< t_("One-liners")
		<< t_("Smartassery")
		<< t_("Improvisation")
		<< t_("Exaggeration")
		<< t_("Mustache Twirling")
		<< t_("Pratfalls");
	
}
