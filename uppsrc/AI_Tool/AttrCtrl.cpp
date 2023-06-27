#include "AI_Tool.h"

/*
Vector<String> elements;
Vector<String> interactions;
Vector<String> with;
Vector<String> moral_interactions;
Vector<String> acting_styles;
Vector<String> tones;
Vector<String> voiceover_tones;
Vector<String> comedic_scenarios;
Vector<String> dramatic_scenarios;
Vector<String> types_of_sentences;
Vector<String> comedic_sentences;
Vector<String> humorous_expressions;
*/


AttrCtrl::AttrCtrl() {
	active.SetCount(group_count * group_limit, false);
	
	for (bool& b : active)
		b = Randomf() < 0.15;
	
	pronouns_clr = Color(255, 0, 212);
	pronouns
		<< t_("I (m)")
		<< t_("I (f)")
		<< t_("you (m)")
		<< t_("you (f)")
		<< t_("he")
		<< t_("he")
		<< t_("she")
		<< t_("we")
		<< t_("we (m)")
		<< t_("we (f)")
		<< t_("they")
		<< t_("they (m)")
		<< t_("they (f)")
		<< t_("plural you")
		<< t_("plural you (m)")
		<< t_("plural you (f)")
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
		<< t_("Usage of Unexpected Symbolism");
	
	moral_interactions_clr = Color(226, 42, 0);
	moral_interactions
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
		<< t_("Digital");
	
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
		<< t_("beach");
	
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
		<< t_("wistful");
	
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
		<< t_("Warbling");
	
	voiceover_tones_clr = Color(28, 255, 200);
	voiceover_tones
		<< t_("casual")
		<< t_("contrasting")
		<< t_("conversational")
		<< t_("deep")
		<< t_("detail focus")
		<< t_("educational")
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
		<< t_("witty");
	
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
		<< t_("Silliness");
	
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
		<< t_("fighting against the odds");
	
	types_of_sentences_clr = Color(0, 150, 246);
	types_of_sentences
		<< t_("observations")
		<< t_("statements")
		<< t_("questions");
	
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
		<< t_("Musical Comedy");
	
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

void AttrCtrl::Paint(Draw& d) {
	Color bg = Color(203, 203, 203);
	Size sz = GetSize();
	
	d.DrawRect(sz, bg);
	
	
	/*
	
	12 Contrast and Unexpected Elements
	15 Interactions
	24 (Interactions) with
	24 Moral interactions
	9  Acting Styles
	16 Tones
	19 Voiceover Tones
	17 Types of comedic scenarios
	19 Types of dramatic scenarios
	3  Types of sentences
	25 Comedic sentences
	23 Humorous expression
	
	*/
	
	int tab;
	int div = 5;
	int cy, cx = sz.cx / div;
	float h;
	
	int font_h = 0.75 * (float)sz.cy / (acting_styles.GetCount() + tones.GetCount() + dramatic_scenarios.GetCount());
	Font fnt = SansSerif(font_h);
	
	rects.SetCount(0);
	
	cy = 0;
	h = (float)sz.cy / (3 + types_of_sentences.GetCount() + elements.GetCount() + moral_interactions.GetCount());
	
	PaintKeys(d, t_("Types of sentences"), types_of_sentences, types_of_sentences_clr, tab * cx, cx, cy, h, fnt, 0);
	PaintKeys(d, t_("Contrast and Unexpected Elements"), elements, elements_clr, 0, cx, cy, h, fnt, 1);
	PaintKeys(d, t_("Moral interactions"), moral_interactions, moral_interactions_clr, 0, cx, cy, h, fnt, 2);
	
	cy = 0;
	tab = 1;
	h = (float)sz.cy / (2 + interactions.GetCount() + with.GetCount());
	PaintKeys(d, t_("Interactions"), interactions, interactions_clr, tab * cx, cx, cy, h, fnt, 3);
	PaintKeys(d, t_("(Interactions) with"), with, with_clr, tab * cx, cx , cy, h, fnt, 4);
	
	cy = 0;
	tab = 2;
	h = (float)sz.cy / (3 + acting_styles.GetCount() + tones.GetCount() + dramatic_scenarios.GetCount());
	PaintKeys(d, t_("Acting Styles"), acting_styles, acting_styles_clr, tab * cx, cx, cy, h, fnt, 5);
	PaintKeys(d, t_("Tones"), tones, tones_clr, tab * cx, cx, cy, h, fnt, 6);
	PaintKeys(d, t_("Types of dramatic scenarios"), dramatic_scenarios, dramatic_scenarios_clr, tab * cx, cx, cy, h, fnt, 7);
	
	cy = 0;
	tab = 3;
	h = (float)sz.cy / (2 + voiceover_tones.GetCount() + comedic_sentences.GetCount());
	PaintKeys(d, t_("Voiceover Tones"), voiceover_tones, voiceover_tones_clr, tab * cx, cx, cy, h, fnt, 8);
	PaintKeys(d, t_("Comedic sentences"), comedic_sentences, comedic_sentences_clr, tab * cx, cx, cy, h, fnt, 9);
	
	cy = 0;
	tab = 4;
	h = (float)sz.cy / (2 + comedic_scenarios.GetCount() + humorous_expressions.GetCount());
	PaintKeys(d, t_("Comedic scenarios"), comedic_scenarios, comedic_scenarios_clr, tab * cx, cx, cy, h, fnt, 10);
	PaintKeys(d, t_("Humorous expressions"), humorous_expressions, humorous_expressions_clr, tab * cx, cx, cy, h, fnt, 11);
	
	
}

void AttrCtrl::PaintKeys(Draw& d, String title, const Vector<String>& keys, Color clr, int x, int cx, int& cy, float h, Font fnt, int group) {
	int x0 = x;
	int y0 = cy;
	int x1 = x + cx;
	int ht = keys.GetCount() * h;
	int y1 = cy + ht;
	const int off = 1;
	
	
	
	for(int i = 0; i <= keys.GetCount(); i++) {
		int y2 = y0 + i * h;
		String txt;
		Rect r = RectC(x0,y2,cx,h+1);
		d.Clip(r);
		Color bg_clr;
		Font txt_fnt = fnt;
		int xoff = 4;
		if (!i) {
			bg_clr = Black(); //clr;
			txt = title;
			txt_fnt.Bold();
			Size sz = GetTextSize(txt, txt_fnt);
			xoff = (cx - sz.cx) / 2;
		}
		else {
			int j = i-1;
			const String& key = keys[j];
			bool active = this->active[group * group_limit + j];
			
			// Mouse hovers
			if (group == sel_group && j == sel_i)
				bg_clr = clr;
			else if (active)
				bg_clr = Blend(clr, GrayColor(64), 0.3*255);
			else
				bg_clr = Blend(clr, GrayColor(64), 0.97*255);
			txt = key;
			
			RectId& rid = rects.Add();
			rid.a = r;
			rid.b = group;
			rid.c = j;
		}
		
		d.DrawRect(r, bg_clr);
		d.DrawText(xoff+ x0 + off + 1, y2 + off + 1, txt, txt_fnt, Black());
		d.DrawText(xoff+ x0 + off, y2 + off, txt, txt_fnt, White());
		d.End();
	}
	
	
	Color line_clr = GrayColor();
	d.DrawLine(x0, y0, x0, y1, 1, line_clr);
	d.DrawLine(x1, y0, x1, y1, 1, line_clr);
	for(int i = 0; i <= keys.GetCount() + 1; i++) {
		int y2 = y0 + i * h;
		d.DrawLine(x0, y2, x1, y2, 1, line_clr);
	}
	
	cy += (1 + keys.GetCount()) * h;
}

void AttrCtrl::MouseMove(Point p, dword keyflags) {
	for(RectId& rid : rects) {
		if (rid.a.Contains(p)) {
			bool r = sel_group != rid.b || sel_i != rid.c;
			sel_group = rid.b;
			sel_i = rid.c;
			if (r)
				Refresh();
			break;
		}
	}
}
