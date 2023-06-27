#include "AI_Tool.h"

/*

Main tab:
	Select artist:
	Select pattern:
		
Artist tab:
	- add/rem
Story tab:
	- add/rem
	- parse
Pattern tab:
	- add/rem
	- parse



Contrast and Unexpected Elements:
- Exaggeration and Surreal Situations
- Subversion of Expectations
- Absurd Concepts
- Juxtaposition
- Unexpected Combinations
- Repetition of Unusual Themes
- Props and Characters that are Unusual
- Utilization of Unconventional Format
- Incongruous Settings and Environments
- Flips of Gender Roles and Social Constructs
- Incongruous Pairings of Opposites
- Usage of Unexpected Symbolism

Interactions:
- Social Interaction
- Atheistic Interaction
- Religious Interaction
- Playful Interaction
- Sexual Interaction
- Romantic Interaction
- Business Interaction
- Social Interaction
- Political Interaction
- Cultural Interaction
- Emotional Interaction
- Intellectual Interaction
- Creative Interaction
- Virtual Interaction
- Digital Interaction

(Interactions) with:
- man
- woman
- girl boy
- animal
- husband
- wife
- friend
- friends
- stranger
- nature
- technology
- music
- art
- sea
- forest
- river
- road
- path in forest
- car
- train
- night club
- bar
- restaurant
- beach

Moral interactions:
- Playful Interaction with God
- Mutual Antagonism Interaction
- Negligent Interaction with God
- Mechanical/Workaround Interaction with God
- Transcendental Interaction with God 
- Devotional Interaction with God
- Conversational Interaction with God
- Parental Interaction with God
- Playful Interaction with Personified Deities 
- Quasi-Moral Behavioural Interaction 
- Judgmental Interaction with God 
- Confrontational Interaction with God 
- Sympathetic Interaction with God 
- Reverential Interaction with God
- Playful Interaction with Angels
- Oblivious Interaction with Angels
- Trustful Interaction with Angels 
- Supernatural Interaction with Angels 
- Playful Interaction with Spirits 
- Playful Interaction with the Dead 
- Playful Interaction with Nature
- Conflict Interaction with Animals 
- Mutual Respect Interaction with Animals 
- Spiritual Interaction with Animals

Acting Styles:
- funny
- dramatic
- seductive
- devious
- mysterious
- passionate
- mischievous
- powerful
- wistful

Tones:
- Melancholic Tone
- Pleading Tone
- Earnest Tone
- Bittersweet Tone
- Desperate Tone
- Nostalgic Tone
- Wistful Tone
- Reflection Tone
- Somber Tone
- Crooning Tone
- Comforting Tone
- Hopeful Tone
- Dreamy Tone
- Pensive Tone
- Restrained Tone
- Warbling Tone

Voiceover Tones:
- casual
- contrasting
- conversational
- deep
- detail focus
- educational
- energetic
- excited
- enthusiastic
- gentle
- laid-back
- mellow
- natural
- nostalgic
- optimistic
- sarcastic
- sophisticated
- suggestive
- witty

Types of comedic scenarios:
- Absurd and Exaggerated Scenarios
- Physical Comedy 
- Tragedy Mocks Success 
- Self-Deprecation 
- Deadpan Humor 
- Satire 
- Working Against Cliche 
- Repetitive Actions and Situations 
- Role Reversal 
- Slapstick 
- Miscommunication and Puns 
- Parody 
- Quirky Characters 
- Social Commentary 
- Absurd Dialogue 
- Playing With Expectations 
- Silliness

Types of dramatic scenarios:
- tragic death
- a marriage in crisis
- a serious illness
- a family in deep debt
- a crime
- a personal betrayal
- a difficult decision
- an addiction
- a forbidden romance
- a risk
- a moral dilemma
- conflicts between cultures
- political unrest
- a character's inner struggle
- a redemption story
- racism and discrimination
- embracing fate
- facing mortality
- fighting against the odds

Types of sentences:
- observations
- statements
- questions

Comedic sentences:
- Satirical sentences
- Pun sentences
- Irony sentences
- Sarcasm sentences
- Hyperbole sentences
- Slapstick sentences
- Absurdist sentences
- Situational sentences
- Wordplay sentences
- Self-Deprecating sentences
- Parodical sentences
- Caricature sentences
- Puns sentences
- Comic Hyperbole sentences
- Absurdity sentences
- Mockery sentences
- Self-Mockery sentences
- Play-on-Words sentences
- Situational Irony sentences
- Topical sentences
- Improvisation sentences
- Sight Gags sentences
- Fish-Out-of-Water sentences
- Hammy Acting sentences
- Musical Comedy sentences


Humorous expression:
- Irony
- Playful Wordplay
- Ironical statements
- Romantic Humor
- Sexual Humor
- Self-Deprecating Humor
- Dark Humor
- Casual and Conversational Tone
- Mockery Humor
- Self-Mockery Humor
- Sarcasm
- Slapstick
- Facetiousness
- Gallows Humor
- Sex Humor
- Surreal Humor
- Absurdist Humor
- One-liners
- Smartassery
- Improvisation
- Exaggeration
- Mustache Twirling
- Pratfalls
*/

ToolApp::ToolApp() {
	Title("ToolApp");
	MaximizeBox().MinimizeBox().Sizeable();
	Icon(AppImg::icon());
	SetRect(Size(800,600));
	
	AddFrame(menu);
	Add(tabs.SizePos());
	
	tabs.Add(main.SizePos(),			t_("Main"));
	tabs.Add(artist_ctrl.SizePos(),		t_("Artist"));
	tabs.Add(story_ctrl.SizePos(),		t_("Story"));
	tabs.Add(pattern_ctrl.SizePos(),	t_("Pattern"));
	main.app			= this;
	artist_ctrl.app		= this;
	story_ctrl.app		= this;
	pattern_ctrl.app	= this;
	
	tabs.WhenSet << THISBACK(Data);
	menu.Set(THISBACK(MainMenu));
	
	
	#ifdef flagWIN32
	dir = "C:\\git\\AudioAssets";
	#else
	dir = GetHomeDirFile("AudioAssets");
	#endif
	
	if (!DirectoryExists(dir)) {
		PromptOK(DeQtf("Default path not found.\nSelect AudioAssets directory."));
		dir = SelectDirectory();
	}
	
}

void ToolApp::Data() {
	int tab = tabs.Get();
	switch (tab) {
		case 0: main.Data(); break;
		case 1: artist_ctrl.Data(); break;
		case 2: story_ctrl.Data(); break;
		case 3: pattern_ctrl.Data(); break;
		default: break;
	}
}

void ToolApp::MainMenu(Bar& bar) {
	
	bar.Sub(t_("App"), [this](Bar& bar) {
		{
			typedef TopWindow CLASSNAME;
			bar.Add(t_("Exit"), THISBACK(Close));
		}
	});
	
}

String ToolApp::GetArtistsDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "artists" DIR_SEPS;
}

String ToolApp::GetStoriesDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "stories" DIR_SEPS;
}

String ToolApp::GetPatternsDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "patterns" DIR_SEPS;
}
