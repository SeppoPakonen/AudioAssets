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
	{
		String dir = GetAttributesDir();
		RealizeDirectory(dir);
		{
			Grouplist& o = groups;
			String json_path = dir + o.file_title + ".json";
			String json = StoreAsJson(groups, true);
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
	
	{
		RealizeDirectory(dir);
		groups.Clear();
		String dir = GetAttributesDir();
		String json_path = dir + groups.file_title + ".json";
		ASSERT(FileExists(json_path));
		LoadFromJsonFile(groups, json_path);
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





int Grouplist::trans_i = -1;

Grouplist::Grouplist() {
	file_title = "default";
}

void Grouplist::DumbFix() {
	groups.Clear();
	
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

}

String Grouplist::Translate(const String& s) {
	if (trans_i < 0)
		return s;
	Translation& t = this->translation[trans_i];
	int i = t.data.Find(s);
	String o;
	if (i >= 0)
		o = t.data[i];
	if (i < 0 || o.IsEmpty()) {
		i = t.data.Find(ToLower(s));
		if (i >= 0)
			o = t.data[i];
		else
			t.data.Add(ToLower(s));
	}
	return o.IsEmpty() ? s : o;
}
