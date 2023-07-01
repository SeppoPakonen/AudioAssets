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
	Maximize();
	
	AddFrame(menu);
	Add(tabs.SizePos());
	
	tabs.Add(main.SizePos(),				t_("Main"));
	tabs.Add(artist_ctrl.SizePos(),			t_("Artist"));
	tabs.Add(story_ctrl.SizePos(),			t_("Story"));
	tabs.Add(pattern_ctrl.SizePos(),		t_("Pattern"));
	tabs.Add(attrscore_ctrl.SizePos(),		t_("Attribute score"));
	tabs.Add(score_ctrl.SizePos(),			t_("Score"));
	tabs.Add(composition_ctrl.SizePos(),	t_("Composition"));
	tabs.Add(analysis_ctrl.SizePos(),		t_("Analysis"));
	
	main.app				= this;
	artist_ctrl.app			= this;
	story_ctrl.app			= this;
	pattern_ctrl.app		= this;
	composition_ctrl.app	= this;
	analysis_ctrl.app		= this;
	score_ctrl.app			= this;
	attrscore_ctrl.app		= this;
	
	tabs.WhenSet << THISBACK(Data);
	menu.Set(THISBACK(MainMenu));
	
	
	PostCallback(THISBACK(Data));
	
}

ToolApp::~ToolApp() {
	Save();
}

void ToolApp::Save() {
	Database::Single().Save();
}

void ToolApp::Data() {
	int tab = tabs.Get();
	switch (tab) {
		case 0: main.Data(); break;
		case 1: artist_ctrl.Data(); break;
		case 2: story_ctrl.Data(); break;
		case 3: pattern_ctrl.Data(); break;
		case 4: attrscore_ctrl.Data(); break;
		case 5: score_ctrl.Data(); break;
		case 6: composition_ctrl.Data(); break;
		case 7: analysis_ctrl.Data(); break;
		default: break;
	}
}

void ToolApp::MainMenu(Bar& bar) {
	
	bar.Sub(t_("App"), [this](Bar& bar) {
		bar.Add(t_("Save"), THISBACK(Save)).Key(K_CTRL_S);
		{
			typedef TopWindow CLASSNAME;
			bar.Add(t_("Exit"), THISBACK(Close));
		}
	});
	
	bar.Sub(t_("View"), [this](Bar& bar) {
		//bar.Add(t_("Switch view"), THISBACK(SwitchView)).Key(K_F5);
		/*bar.Add(t_("Move to previous in level"), THISBACK(MovePrevLevel)).Key(K_F7);
		bar.Add(t_("Move to next in level"), THISBACK(MoveNextLevel)).Key(K_F8);
		bar.Separator();
		bar.Add(t_("Move to previous in total"), THISBACK(MovePrevTotal)).Key(K_F7);
		bar.Add(t_("Move to next in total"), THISBACK(MoveNextTotal)).Key(K_F8);
		bar.Separator();*/
		bar.Add(t_("Move to owner"), THISBACK(MoveOwner)).Key(K_F4);
		//bar.Add(t_("Move to previous"), THISBACK(MovePrevious)).Key(K_F3);
	});
	
}

void ToolApp::SwitchView() {
	int tab = tabs.Get();
	if (tab == 3) {
		pattern_ctrl.SwitchView();
	}
}

void ToolApp::MovePrevLevel() {
	
}

void ToolApp::MoveNextLevel() {
	
}

void ToolApp::MovePrevTotal() {
	
}

void ToolApp::MoveNextTotal() {
	
}

void ToolApp::MoveOwner() {
	int tab = tabs.Get();
	if (tab == 3) {
		int c = pattern_ctrl.tree.IsCursor() ? pattern_ctrl.tree.GetCursor() : 0;
		if (c) {
			c = pattern_ctrl.tree.GetParent(c);
			pattern_ctrl.tree.SetCursor(c);
		}
	}
}

void ToolApp::MovePrevious() {
	
}
