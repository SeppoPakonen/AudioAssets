#include <AICore/AICore.h>

using namespace Upp;

struct Songs {
	QueryTable<String> qt;
	Vector<String>* rl = 0;
	Vector<String>* fa = 0;
	Vector<String>* ly = 0;
	Vector<String>* fad = 0;
	Vector<String>* kids = 0;
	Vector<String>* g = 0;
	Vector<String>* st = 0;
	
	
	Songs() {
		rl   = &qt.AddPredictor("Rhyming level lyrics");
		fa   = &qt.AddPredictor("X-Centric video");
		ly   = &qt.AddPredictor("Lyrics as text");
		fad  = &qt.AddPredictor("Fading sequences in video");
		kids = &qt.AddPredictor("Youngest audience");
		g    = &qt.AddPredictor("Gender");
		st   = &qt.AddPredictor("Human strength");
		
		/*
		
		Target value classes 4.8.2025:
		
		1: Around 3 hours
		2: Around 2 hours
		3: Around 1 hour
		4: Around 30 mins
		*/
		qt.AddTargetScore("1", 8);
		qt.AddTargetScore("2", 4);
		qt.AddTargetScore("3", 2);
		qt.AddTargetScore("4", 1);
	}
	void Add(	String rhyming,
				String centric,
				String lyrics,
				String fading,
				String audience,
				String gender,
				String strength,
				String target) {
		rl->Add(rhyming);
		fa->Add(centric);
		ly->Add(lyrics);
		fad->Add(fading);
		kids->Add(audience);
		g->Add(gender);
		st->Add(strength);
		qt.AddTargetValue(target);
	}
};

CONSOLE_APP_MAIN
{
	Songs s;
	
	/* EP 24 "Ujostus" */		s.Add("Begin & End",	"Married",		"Text",			"Fade",	"Adults",	"Both",		"Strong",	"3");
	/* EP 23 "Sydämen varas" */	s.Add("End-ish",		"Married",		"Text",			"Cut",	"Adults",	"Female",	"Strong",	"3");
	/* EP 22 "Tahdon sut" */	s.Add("End-ish",		"Dating",		"Text",			"Fade",	"Kids",		"Both",		"Strong",	"3");
	/* EP 21 "Talviyö" */		s.Add("End-ish",		"Breakup",		"Text",			"Fade",	"Adults",	"Both",		"Strong",	"4");
	/* EP 20 "Puuma" */			s.Add("End-ish",		"Dating",		"Text",			"Fade",	"Adults",	"Male",		"Weak",		"3");
	/* EP 19 "Aamuisin" */		s.Add("Begin & End",	"Dating",		"Text",			"Fade",	"Adults",	"Both",		"Weak",		"1");
	/* EP 18 "Vanha sävel" */	s.Add("End-ish",		"Party",		"Audio only",	"Fade",	"Kids",		"Both",		"Neutral",	"4");
	/* EP 17 "Great escape" */	s.Add("End",			"Breakup",		"Audio only",	"Fade",	"Kids",		"Female",	"Strong",	"3");
	/* EP 16 "Mennään jkn." */	s.Add("End",			"Dating",		"Audio only",	"Fade",	"Kids",		"Both",		"Neutral",	"2");
	/* EP 15 "Laiva" */			s.Add("End",			"Married",		"Audio only",	"Fade",	"Kids",		"Male",		"Neutral",	"3");
	/* EP 14 "Mikä on" */		s.Add("End-ish",		"Loneliness",	"Audio only",	"Fade",	"Adults",	"Female",	"Weak",		"3");
	/* EP 13 "Invincible" */	s.Add("End",			"Empowerment",	"Audio only",	"Cut",	"Adults",	"Female",	"Strong",	"3");
	/* EP 12 "Girl I n h" */	s.Add("End-ish",		"Loneliness",	"Audio only",	"Cut",	"Adults",	"Male",		"Weak",		"4");
	/* EP 11 "Pahuutta pah" */	s.Add("End-ish",		"Confusion",	"Audio only",	"Cut",	"Adults",	"Female",	"Weak",		"3");
	/* EP 10 "Mitä tapahtuu" */	s.Add("End-ish",		"Confusion",	"Audio only",	"Cut",	"Adults",	"Female",	"Weak",		"4");
	
	
	int i = s.qt.GetLargestInfoGainPredictor();
	DUMP(i);
	DUMPC(s.qt.GetInfoGains());
	LOG(s.qt.GetInfoString());
	
}
