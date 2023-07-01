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

String Database::GetAttributesDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "attributes" DIR_SEPS;
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
	#if 0
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
		<< t_("every")
		<< t_("day")
		<< t_("night")
		<< t_("love")
		<< t_("moment")
		<< t_("passive")
		<< t_("my")
		<< t_("your")
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
		<< t_("Unexpected use of opposites")
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
		<< t_("navigation in life")
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
		<< t_("Suggests a sense of obligation to have")
		<< t_("Do ")
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
		<< t_("Spiritual ia. with Animals")
		<< t_("Baptism of Jesus")
		;
	
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
		<< t_("Personal")
		<< t_("Making a decision")
		<< t_("Taking a risk")
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
		<< t_("bold")
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
		<< t_("Neutral")
		<< t_("Poignant")
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
		<< t_("Perilious choices leads to a better outcome")
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
	
	#else
	
	
	groups.Add("pronouns")
		.SetDescription("Pronouns")
		.SetColor(255, 0, 212)
		<< ToLower("I (m)")
		<< ToLower("I (f)")
		<< ToLower("you (m)")
		<< ToLower("you (f)")
		<< ToLower("he")
		<< ToLower("she")
		<< ToLower("we")
		<< ToLower("we (m)")
		<< ToLower("we (f)")
		<< ToLower("plural you")
		<< ToLower("plural you (m)")
		<< ToLower("plural you (f)")
		<< ToLower("they")
		<< ToLower("they (m)")
		<< ToLower("they (f)")
		<< ToLower("one")
		<< ToLower("everyone")
		<< ToLower("singular pronoun")
		<< ToLower("person")
		<< ToLower("that")
		<< ToLower("every")
		<< ToLower("day")
		<< ToLower("night")
		<< ToLower("love")
		<< ToLower("moment")
		<< ToLower("passive")
		<< ToLower("my")
		<< ToLower("your")
		;
	
	groups.Add("elements")
		.SetDescription("Contrast and Unexpected Elements")
		.SetColor(255, 212, 0)
		<< ToLower("Exaggeration and Surreal Situations")
		<< ToLower("Subversion of Expectations")
		<< ToLower("Absurd Concepts")
		<< ToLower("Juxtaposition")
		<< ToLower("Unexpected Combinations")
		<< ToLower("Repetition of Unusual Themes")
		<< ToLower("Props and Characters that are Unusual")
		<< ToLower("Utilization of Unconventional Format")
		<< ToLower("Incongruous Settings and Environments")
		<< ToLower("Flips of Gender Roles")
		<< ToLower("Flips of Social Constructs")
		<< ToLower("Incongruous Pairings of Opposites")
		<< ToLower("Usage of Unexpected Symbolism")
		<< ToLower("Hope for future without immediate action")
		<< ToLower("Unexpected turning of a positive thing into negativity")
		<< ToLower("Unexpected turning of a negative thing into a positive")
		<< ToLower("Using an abstract concept")
		<< ToLower("Unexpected comparison between two")
		<< ToLower("Serious tone but humorous expectation")
		<< ToLower("Unexpected emotional reaction to treatment received from others")
		<< ToLower("Unexpected use of opposites")
		;
	
	groups.Add("moral_interactions")
		.SetDescription("Moral interactions")
		.SetColor(246, 22, 0)
		<< ToLower("Respect")
		<< ToLower("Honesty")
		<< ToLower("Integrity")
		<< ToLower("Kindness")
		<< ToLower("Compassion")
		<< ToLower("Gratitude")
		<< ToLower("Forgiveness")
		<< ToLower("Trust")
		<< ToLower("Responsibility")
		<< ToLower("Patience")
		<< ToLower("Humility")
		<< ToLower("Loyalty")
		<< ToLower("Fairness")
		<< ToLower("right to privacy")
		<< ToLower("strength")
		<< ToLower("resilience")
		<< ToLower("spiritual meaningfulness")
		<< ToLower("judgement")
		<< ToLower("altruism")
		<< ToLower("navigation in life")
		;
	
	groups.Add("moral_interaction_modes")
		.SetDescription("Moral interactions mode")
		.SetColor(22, 22, 246)
		<< ToLower("promote someone's")
		<< ToLower("to bring up your own")
		<< ToLower("emphasize someone's")
		<< ToLower("ask open ended question about")
		<< ToLower("support someone's")
		<< ToLower("challenges assumptions about")
		<< ToLower("give feedback on")
		<< ToLower("to give encouragement of strength")
		<< ToLower("to invite a discussion about")
		<< ToLower("think about your own")
		<< ToLower("understand someone else's")
		<< ToLower("questioning")
		<< ToLower("themes around")
		<< ToLower("Suggests a sense of obligation to have")
		<< ToLower("Do ")
		;
	
	groups.Add("religious_moral_interactions")
		.SetDescription("Religiously moral")
		.SetColor(226, 42, 0)
		<< ToLower("Playful ia. with God")
		<< ToLower("Mutual Antagonism ia.")
		<< ToLower("Negligent ia. with God")
		<< ToLower("Mechanical/Workaround ia. with God")
		<< ToLower("Transcendental ia. with God ")
		<< ToLower("Devotional ia. with God")
		<< ToLower("Conversational ia. with God")
		<< ToLower("Parental ia. with God")
		<< ToLower("Playful ia. with Personified Deities ")
		<< ToLower("Quasi-Moral Behavioural ia. ")
		<< ToLower("Judgmental ia. with God ")
		<< ToLower("Confrontational ia. with God ")
		<< ToLower("Sympathetic ia. with God ")
		<< ToLower("Reverential ia. with God")
		<< ToLower("Playful ia. with Angels")
		<< ToLower("Oblivious ia. with Angels")
		<< ToLower("Trustful ia. with Angels ")
		<< ToLower("Supernatural ia. with Angels ")
		<< ToLower("Playful ia. with Spirits ")
		<< ToLower("Playful ia. with the Dead ")
		<< ToLower("Playful ia. with Nature")
		<< ToLower("Conflict ia. with Animals ")
		<< ToLower("Mutual Respect ia. with Animals ")
		<< ToLower("Spiritual ia. with Animals")
		<< ToLower("Baptism of Jesus")
		;
	
	groups.Add("interactions")
		.SetDescription("Interactions")
		.SetColor(28, 85, 255)
		<< ToLower("Social")
		<< ToLower("Atheistic")
		<< ToLower("Religious")
		<< ToLower("Playful")
		<< ToLower("Sexual")
		<< ToLower("Romantic")
		<< ToLower("Business")
		<< ToLower("Social")
		<< ToLower("Political")
		<< ToLower("Cultural")
		<< ToLower("Emotional")
		<< ToLower("Intellectual")
		<< ToLower("Creative")
		<< ToLower("Virtual")
		<< ToLower("Digital")
		<< ToLower("Serious")
		<< ToLower("Requesting for confirmation")
		<< ToLower("Suggesting actions and not just thoughts")
		<< ToLower("Dialogue between people")
		<< ToLower("Personal")
		<< ToLower("Making a decision")
		<< ToLower("Taking a risk")
		;
	
	groups.Add("with")
		.SetDescription("(Interactions) with")
		.SetColor(85, 127, 0)
		<< ToLower("man")
		<< ToLower("woman")
		<< ToLower("girl")
		<< ToLower("boy")
		<< ToLower("animal")
		<< ToLower("husband")
		<< ToLower("wife")
		<< ToLower("friend")
		<< ToLower("friends")
		<< ToLower("stranger")
		<< ToLower("nature")
		<< ToLower("technology")
		<< ToLower("music")
		<< ToLower("art")
		<< ToLower("sea")
		<< ToLower("forest")
		<< ToLower("river")
		<< ToLower("road")
		<< ToLower("path in forest")
		<< ToLower("car")
		<< ToLower("train")
		<< ToLower("night club")
		<< ToLower("bar")
		<< ToLower("restaurant")
		<< ToLower("beach")
		<< ToLower("mother")
		<< ToLower("father")
		;
	
	groups.Add("acting_styles")
		.SetDescription("Acting Styles")
		.SetColor(198, 42, 200)
		<< ToLower("funny")
		<< ToLower("dramatic")
		<< ToLower("seductive")
		<< ToLower("devious")
		<< ToLower("mysterious")
		<< ToLower("passionate")
		<< ToLower("mischievous")
		<< ToLower("powerful")
		<< ToLower("wistful")
		<< ToLower("determined")
		<< ToLower("aggressive")
		<< ToLower("implying urgency")
		<< ToLower("light-hearted")
		<< ToLower("broody")
		<< ToLower("bold")
		;
	
	groups.Add("tones")
		.SetDescription("Tones")
		.SetColor(28, 42, 150)
		<< ToLower("Melancholic")
		<< ToLower("Pleading")
		<< ToLower("Earnest")
		<< ToLower("Bittersweet")
		<< ToLower("Desperate")
		<< ToLower("Nostalgic")
		<< ToLower("Wistful")
		<< ToLower("Reflection")
		<< ToLower("Somber")
		<< ToLower("Crooning")
		<< ToLower("Comforting")
		<< ToLower("Hopeful")
		<< ToLower("Dreamy")
		<< ToLower("Pensive")
		<< ToLower("Restrained")
		<< ToLower("Warbling")
		<< ToLower("Direct")
		<< ToLower("Assertive")
		<< ToLower("Isolated")
		<< ToLower("Mournful")
		<< ToLower("Commanding")
		<< ToLower("Insistent")
		<< ToLower("Uplifting")
		<< ToLower("Inspirational")
		<< ToLower("Inquisitive")
		<< ToLower("Angry")
		<< ToLower("Pity")
		<< ToLower("Confused")
		<< ToLower("Unknowing")
		<< ToLower("Sarcastic")
		<< ToLower("Sad")
		<< ToLower("Disappointed")
		<< ToLower("Challenging")
		<< ToLower("Encouraging")
		<< ToLower("Neutral")
		<< ToLower("Poignant")
		;
	
	groups.Add("voiceover_tones")
		.SetDescription("Voiceover Tones")
		.SetColor(28, 255, 200)
		<< ToLower("casual")
		<< ToLower("contrasting")
		<< ToLower("conversational")
		<< ToLower("deep")
		<< ToLower("detail focus")
		<< ToLower("educating")
		<< ToLower("energetic")
		<< ToLower("excited")
		<< ToLower("enthusiastic")
		<< ToLower("gentle")
		<< ToLower("laid-back")
		<< ToLower("mellow")
		<< ToLower("natural")
		<< ToLower("nostalgic")
		<< ToLower("optimistic")
		<< ToLower("sarcastic")
		<< ToLower("sophisticated")
		<< ToLower("suggestive")
		<< ToLower("witty")
		<< ToLower("confident")
		<< ToLower("assured")
		<< ToLower("motivating")
		<< ToLower("reflective")
		;
	
	groups.Add("comedic_scenarios")
		.SetDescription("Comedic scenarios")
		.SetColor(141, 42, 150)
		<< ToLower("Absurd and Exaggerated Scenarios")
		<< ToLower("Physical Comedy")
		<< ToLower("Tragedy Mocks Success")
		<< ToLower("Self-Deprecation")
		<< ToLower("Deadpan Humor")
		<< ToLower("Satire")
		<< ToLower("Working Against Cliche")
		<< ToLower("Repetitive Actions and Situations")
		<< ToLower("Role Reversal")
		<< ToLower("Slapstick")
		<< ToLower("Miscommunication and Puns")
		<< ToLower("Parody")
		<< ToLower("Quirky Characters")
		<< ToLower("Social Commentary")
		<< ToLower("Absurd Dialogue")
		<< ToLower("Playing With Expectations")
		<< ToLower("Silliness")
		;
	
	groups.Add("dramatic_scenarios")
		.SetDescription("Dramatic scenarios")
		.SetColor(56, 42, 200)
		<< ToLower("tragic death")
		<< ToLower("a marriage in crisis")
		<< ToLower("a serious illness")
		<< ToLower("a family in deep debt")
		<< ToLower("a crime")
		<< ToLower("a personal betrayal")
		<< ToLower("a difficult decision")
		<< ToLower("an addiction")
		<< ToLower("a forbidden romance")
		<< ToLower("a risk")
		<< ToLower("a moral dilemma")
		<< ToLower("conflicts between cultures")
		<< ToLower("political unrest")
		<< ToLower("a character's inner struggle")
		<< ToLower("a redemption story")
		<< ToLower("racism and discrimination")
		<< ToLower("embracing fate")
		<< ToLower("facing mortality")
		<< ToLower("fighting against the odds")
		<< ToLower("Conflict with someone")
		<< ToLower("Bring up a moral issue")
		<< ToLower("Existential crisis")
		<< ToLower("Perilious choices leads to a better outcome")
		;
	
	groups.Add("types_of_sentences")
		.SetDescription("Types of sentences")
		.SetColor(0, 150, 246)
		<< ToLower("observations")
		<< ToLower("statements")
		<< ToLower("questions")
		<< ToLower("declarative")
		<< ToLower("imperative")
		<< ToLower("interrogative")
		<< ToLower("affirmative")
		;
	
	groups.Add("comedic_sentences")
		.SetDescription("Comedic sentences")
		.SetColor(255, 170, 0)
		<< ToLower("Satirical")
		<< ToLower("Pun")
		<< ToLower("Irony")
		<< ToLower("Sarcasm")
		<< ToLower("Hyperbole")
		<< ToLower("Slapstick")
		<< ToLower("Absurdist")
		<< ToLower("Situational")
		<< ToLower("Wordplay")
		<< ToLower("Self-Deprecating")
		<< ToLower("Parodical")
		<< ToLower("Caricature")
		<< ToLower("Puns")
		<< ToLower("Comic Hyperbole")
		<< ToLower("Absurdity")
		<< ToLower("Mockery")
		<< ToLower("Self-Mockery")
		<< ToLower("Play-on-Words")
		<< ToLower("Situational Irony")
		<< ToLower("Topical")
		<< ToLower("Improvisation")
		<< ToLower("Sight Gags")
		<< ToLower("Fish-Out-of-Water")
		<< ToLower("Hammy Acting")
		<< ToLower("Musical Comedy")
		<< ToLower("Suggests lightheartedness")
		;
	
	groups.Add("humorous_expressions")
		.SetDescription("Humorous expressions")
		.SetColor(255, 42, 200)
		<< ToLower("Irony")
		<< ToLower("Playful Wordplay")
		<< ToLower("Ironical statements")
		<< ToLower("Romantic Humor")
		<< ToLower("Sexual Humor")
		<< ToLower("Self-Deprecating Humor")
		<< ToLower("Dark Humor")
		<< ToLower("Casual and Conversational Tone")
		<< ToLower("Mockery Humor")
		<< ToLower("Self-Mockery Humor")
		<< ToLower("Sarcasm")
		<< ToLower("Slapstick")
		<< ToLower("Facetiousness")
		<< ToLower("Gallows Humor")
		<< ToLower("Sex Humor")
		<< ToLower("Surreal Humor")
		<< ToLower("Absurdist Humor")
		<< ToLower("One-liners")
		<< ToLower("Smartassery")
		<< ToLower("Improvisation")
		<< ToLower("Exaggeration")
		<< ToLower("Mustache Twirling")
		<< ToLower("Pratfalls");



	Translation& fi_fi = translation.Add("FI_FI");
	
	#define TRANS(x) .Add(x, t_(x))
	fi_fi
		TRANS("I (m)")
		TRANS("I (f)")
		TRANS("you (m)")
		TRANS("you (f)")
		TRANS("he")
		TRANS("she")
		TRANS("we")
		TRANS("we (m)")
		TRANS("we (f)")
		TRANS("plural you")
		TRANS("plural you (m)")
		TRANS("plural you (f)")
		TRANS("they")
		TRANS("they (m)")
		TRANS("they (f)")
		TRANS("one")
		TRANS("everyone")
		TRANS("singular pronoun")
		TRANS("person")
		TRANS("that")
		TRANS("every")
		TRANS("day")
		TRANS("night")
		TRANS("love")
		TRANS("moment")
		TRANS("passive")
		TRANS("my")
		TRANS("your")
		TRANS("Exaggeration and Surreal Situations")
		TRANS("Subversion of Expectations")
		TRANS("Absurd Concepts")
		TRANS("Juxtaposition")
		TRANS("Unexpected Combinations")
		TRANS("Repetition of Unusual Themes")
		TRANS("Props and Characters that are Unusual")
		TRANS("Utilization of Unconventional Format")
		TRANS("Incongruous Settings and Environments")
		TRANS("Flips of Gender Roles")
		TRANS("Flips of Social Constructs")
		TRANS("Incongruous Pairings of Opposites")
		TRANS("Usage of Unexpected Symbolism")
		TRANS("Hope for future without immediate action")
		TRANS("Unexpected turning of a positive thing into negativity")
		TRANS("Unexpected turning of a negative thing into a positive")
		TRANS("Using an abstract concept")
		TRANS("Unexpected comparison between two")
		TRANS("Serious tone but humorous expectation")
		TRANS("Unexpected emotional reaction to treatment received from others")
		TRANS("Unexpected use of opposites")
		TRANS("Respect")
		TRANS("Honesty")
		TRANS("Integrity")
		TRANS("Kindness")
		TRANS("Compassion")
		TRANS("Gratitude")
		TRANS("Forgiveness")
		TRANS("Trust")
		TRANS("Responsibility")
		TRANS("Patience")
		TRANS("Humility")
		TRANS("Loyalty")
		TRANS("Fairness")
		TRANS("right to privacy")
		TRANS("strength")
		TRANS("resilience")
		TRANS("spiritual meaningfulness")
		TRANS("judgement")
		TRANS("altruism")
		TRANS("navigation in life")
		TRANS("promote someone's")
		TRANS("to bring up your own")
		TRANS("emphasize someone's")
		TRANS("ask open ended question about")
		TRANS("support someone's")
		TRANS("challenges assumptions about")
		TRANS("give feedback on")
		TRANS("to give encouragement of strength")
		TRANS("to invite a discussion about")
		TRANS("think about your own")
		TRANS("understand someone else's")
		TRANS("questioning")
		TRANS("themes around")
		TRANS("Suggests a sense of obligation to have")
		TRANS("Do ")
		TRANS("Playful ia. with God")
		TRANS("Mutual Antagonism ia.")
		TRANS("Negligent ia. with God")
		TRANS("Mechanical/Workaround ia. with God")
		TRANS("Transcendental ia. with God ")
		TRANS("Devotional ia. with God")
		TRANS("Conversational ia. with God")
		TRANS("Parental ia. with God")
		TRANS("Playful ia. with Personified Deities ")
		TRANS("Quasi-Moral Behavioural ia. ")
		TRANS("Judgmental ia. with God ")
		TRANS("Confrontational ia. with God ")
		TRANS("Sympathetic ia. with God ")
		TRANS("Reverential ia. with God")
		TRANS("Playful ia. with Angels")
		TRANS("Oblivious ia. with Angels")
		TRANS("Trustful ia. with Angels ")
		TRANS("Supernatural ia. with Angels ")
		TRANS("Playful ia. with Spirits ")
		TRANS("Playful ia. with the Dead ")
		TRANS("Playful ia. with Nature")
		TRANS("Conflict ia. with Animals ")
		TRANS("Mutual Respect ia. with Animals ")
		TRANS("Spiritual ia. with Animals")
		TRANS("Baptism of Jesus")
		TRANS("Social")
		TRANS("Atheistic")
		TRANS("Religious")
		TRANS("Playful")
		TRANS("Sexual")
		TRANS("Romantic")
		TRANS("Business")
		TRANS("Social")
		TRANS("Political")
		TRANS("Cultural")
		TRANS("Emotional")
		TRANS("Intellectual")
		TRANS("Creative")
		TRANS("Virtual")
		TRANS("Digital")
		TRANS("Serious")
		TRANS("Requesting for confirmation")
		TRANS("Suggesting actions and not just thoughts")
		TRANS("Dialogue between people")
		TRANS("Personal")
		TRANS("Making a decision")
		TRANS("Taking a risk")
		TRANS("man")
		TRANS("woman")
		TRANS("girl")
		TRANS("boy")
		TRANS("animal")
		TRANS("husband")
		TRANS("wife")
		TRANS("friend")
		TRANS("friends")
		TRANS("stranger")
		TRANS("nature")
		TRANS("technology")
		TRANS("music")
		TRANS("art")
		TRANS("sea")
		TRANS("forest")
		TRANS("river")
		TRANS("road")
		TRANS("path in forest")
		TRANS("car")
		TRANS("train")
		TRANS("night club")
		TRANS("bar")
		TRANS("restaurant")
		TRANS("beach")
		TRANS("mother")
		TRANS("father")
		TRANS("funny")
		TRANS("dramatic")
		TRANS("seductive")
		TRANS("devious")
		TRANS("mysterious")
		TRANS("passionate")
		TRANS("mischievous")
		TRANS("powerful")
		TRANS("wistful")
		TRANS("determined")
		TRANS("aggressive")
		TRANS("implying urgency")
		TRANS("light-hearted")
		TRANS("broody")
		TRANS("bold")
		TRANS("Melancholic")
		TRANS("Pleading")
		TRANS("Earnest")
		TRANS("Bittersweet")
		TRANS("Desperate")
		TRANS("Nostalgic")
		TRANS("Wistful")
		TRANS("Reflection")
		TRANS("Somber")
		TRANS("Crooning")
		TRANS("Comforting")
		TRANS("Hopeful")
		TRANS("Dreamy")
		TRANS("Pensive")
		TRANS("Restrained")
		TRANS("Warbling")
		TRANS("Direct")
		TRANS("Assertive")
		TRANS("Isolated")
		TRANS("Mournful")
		TRANS("Commanding")
		TRANS("Insistent")
		TRANS("Uplifting")
		TRANS("Inspirational")
		TRANS("Inquisitive")
		TRANS("Angry")
		TRANS("Pity")
		TRANS("Confused")
		TRANS("Unknowing")
		TRANS("Sarcastic")
		TRANS("Sad")
		TRANS("Disappointed")
		TRANS("Challenging")
		TRANS("Encouraging")
		TRANS("Neutral")
		TRANS("Poignant")
		TRANS("casual")
		TRANS("contrasting")
		TRANS("conversational")
		TRANS("deep")
		TRANS("detail focus")
		TRANS("educating")
		TRANS("energetic")
		TRANS("excited")
		TRANS("enthusiastic")
		TRANS("gentle")
		TRANS("laid-back")
		TRANS("mellow")
		TRANS("natural")
		TRANS("nostalgic")
		TRANS("optimistic")
		TRANS("sarcastic")
		TRANS("sophisticated")
		TRANS("suggestive")
		TRANS("witty")
		TRANS("confident")
		TRANS("assured")
		TRANS("motivating")
		TRANS("reflective")
		TRANS("Absurd and Exaggerated Scenarios")
		TRANS("Physical Comedy")
		TRANS("Tragedy Mocks Success")
		TRANS("Self-Deprecation")
		TRANS("Deadpan Humor")
		TRANS("Satire")
		TRANS("Working Against Cliche")
		TRANS("Repetitive Actions and Situations")
		TRANS("Role Reversal")
		TRANS("Slapstick")
		TRANS("Miscommunication and Puns")
		TRANS("Parody")
		TRANS("Quirky Characters")
		TRANS("Social Commentary")
		TRANS("Absurd Dialogue")
		TRANS("Playing With Expectations")
		TRANS("Silliness")
		TRANS("tragic death")
		TRANS("a marriage in crisis")
		TRANS("a serious illness")
		TRANS("a family in deep debt")
		TRANS("a crime")
		TRANS("a personal betrayal")
		TRANS("a difficult decision")
		TRANS("an addiction")
		TRANS("a forbidden romance")
		TRANS("a risk")
		TRANS("a moral dilemma")
		TRANS("conflicts between cultures")
		TRANS("political unrest")
		TRANS("a character's inner struggle")
		TRANS("a redemption story")
		TRANS("racism and discrimination")
		TRANS("embracing fate")
		TRANS("facing mortality")
		TRANS("fighting against the odds")
		TRANS("Conflict with someone")
		TRANS("Bring up a moral issue")
		TRANS("Existential crisis")
		TRANS("Perilious choices leads to a better outcome")
		TRANS("observations")
		TRANS("statements")
		TRANS("questions")
		TRANS("declarative")
		TRANS("imperative")
		TRANS("interrogative")
		TRANS("affirmative")
		TRANS("Satirical")
		TRANS("Pun")
		TRANS("Irony")
		TRANS("Sarcasm")
		TRANS("Hyperbole")
		TRANS("Slapstick")
		TRANS("Absurdist")
		TRANS("Situational")
		TRANS("Wordplay")
		TRANS("Self-Deprecating")
		TRANS("Parodical")
		TRANS("Caricature")
		TRANS("Puns")
		TRANS("Comic Hyperbole")
		TRANS("Absurdity")
		TRANS("Mockery")
		TRANS("Self-Mockery")
		TRANS("Play-on-Words")
		TRANS("Situational Irony")
		TRANS("Topical")
		TRANS("Improvisation")
		TRANS("Sight Gags")
		TRANS("Fish-Out-of-Water")
		TRANS("Hammy Acting")
		TRANS("Musical Comedy")
		TRANS("Suggests lightheartedness")
		TRANS("Irony")
		TRANS("Playful Wordplay")
		TRANS("Ironical statements")
		TRANS("Romantic Humor")
		TRANS("Sexual Humor")
		TRANS("Self-Deprecating Humor")
		TRANS("Dark Humor")
		TRANS("Casual and Conversational Tone")
		TRANS("Mockery Humor")
		TRANS("Self-Mockery Humor")
		TRANS("Sarcasm")
		TRANS("Slapstick")
		TRANS("Facetiousness")
		TRANS("Gallows Humor")
		TRANS("Sex Humor")
		TRANS("Surreal Humor")
		TRANS("Absurdist Humor")
		TRANS("One-liners")
		TRANS("Smartassery")
		TRANS("Improvisation")
		TRANS("Exaggeration")
		TRANS("Mustache Twirling")
		TRANS("Pratfalls")
		TRANS("Pronouns")
		TRANS("Moral interactions")
		TRANS("Moral interactions mode")
		TRANS("Types of sentences")
		TRANS("Contrast and Unexpected Elements")
		TRANS("Religiously moral")
		TRANS("Interactions")
		TRANS("(Interactions) with")
		TRANS("Acting Styles")
		TRANS("Types of dramatic scenarios")
		TRANS("Tones")
		TRANS("Voiceover Tones")
		TRANS("Comedic sentences")
		TRANS("Comedic scenarios")
		TRANS("Humorous expressions")
		;
		
	#endif
}

void Grouplist::StoreJson() {
	Database& d = Database::Single();
	String dir = d.GetAttributesDir();
	
	RealizeDirectory(dir);
	
	String json_path = dir + "default.json";
	String json = StoreAsJson(*this, true);
	FileOut fout(json_path);
	fout << json;
	fout.Close();
}

