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
	Vector<String>* de = 0;
	int tgt;
	
	Songs() {
		rl   = &qt.AddPredictor("Rhyming level lyrics");
		fa   = &qt.AddPredictor("X-Centric video");
		ly   = &qt.AddPredictor("Lyrics as text");
		fad  = &qt.AddPredictor("Fading sequences in video");
		kids = &qt.AddPredictor("Youngest audience");
		g    = &qt.AddPredictor("Gender");
		st   = &qt.AddPredictor("Human strength");
		de   = &qt.AddPredictor("Decency");
		
	}
	void SetTarget(int i) {
		tgt = i;
		if (i == 0) {
			/*
			Total watch time: 4.8.2025:
			
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
		else if (i == 1) {
			/*
			Total likes: 4.8.2025:
			
			1: Around 16 likes
			2: Around 8 likes
			3: Around 4 likes
			4: Around 2 likes
			*/
			qt.AddTargetScore("1", 16);
			qt.AddTargetScore("2", 8);
			qt.AddTargetScore("3", 4);
			qt.AddTargetScore("4", 2);
			qt.AddTargetScore("5", 1);
		}
	}
	void Add(	String rhyming,
				String centric,
				String lyrics,
				String fading,
				String audience,
				String gender,
				String strength,
				String decency,
				String target0,
				String target1) {
		rl->Add(rhyming);
		fa->Add(centric);
		ly->Add(lyrics);
		fad->Add(fading);
		kids->Add(audience);
		g->Add(gender);
		st->Add(strength);
		de->Add(decency);
		if (tgt == 0)
			qt.AddTargetValue(target0);
		else
			qt.AddTargetValue(target1);
	}
};

CONSOLE_APP_MAIN
{
	for(int i = 0; i < 2; i++) {
		LOG("################### " << (i == 0 ? "TOTAL WATCH TIME" : "TOTAL LIKES") << " ###################");
		Songs s;
		s.SetTarget(i);
		
		/* EP 25 "Tärisen" */		s.Add("Begin & End",	"Dating",		"Text",			"Fade",	"Adults",	"Both",		"Weak",		"Dirty",	"2",	"2");
		/* EP 24 "Ujostus" */		s.Add("Begin & End",	"Married",		"Text",			"Fade",	"Adults",	"Both",		"Strong",	"Dirty",	"3",	"3");
		/* EP 23 "Sydämen varas" */	s.Add("End-ish",		"Married",		"Text",			"Cut",	"Adults",	"Female",	"Strong",	"Dirty",	"3",	"4");
		/* EP 22 "Tahdon sut" */	s.Add("End-ish",		"Dating",		"Text",			"Fade",	"Kids",		"Both",		"Strong",	"Clean",	"3",	"3");
		/* EP 21 "Talviyö" */		s.Add("End-ish",		"Breakup",		"Text",			"Fade",	"Adults",	"Both",		"Strong",	"Clean",	"4",	"4");
		/* EP 20 "Puuma" */			s.Add("End-ish",		"Dating",		"Text",			"Fade",	"Adults",	"Male",		"Weak",		"Dirty",	"3",	"4");
		/* EP 19 "Aamuisin" */		s.Add("Begin & End",	"Dating",		"Text",			"Fade",	"Adults",	"Both",		"Weak",		"Clean",	"1",	"1");
		/* EP 18 "Vanha sävel" */	s.Add("End-ish",		"Party",		"Audio only",	"Fade",	"Kids",		"Both",		"Neutral",	"Clean",	"4",	"3");
		/* EP 17 "Great escape" */	s.Add("End",			"Breakup",		"Audio only",	"Fade",	"Kids",		"Female",	"Strong",	"Clean",	"3",	"4");
		/* EP 16 "Mennään jkn." */	s.Add("End",			"Dating",		"Audio only",	"Fade",	"Kids",		"Both",		"Neutral",	"Clean",	"2",	"3");
		/* EP 15 "Laiva" */			s.Add("End",			"Married",		"Audio only",	"Fade",	"Kids",		"Male",		"Neutral",	"Clean",	"3",	"3");
		/* EP 14 "Mikä on" */		s.Add("End-ish",		"Loneliness",	"Audio only",	"Fade",	"Adults",	"Female",	"Weak",		"Clean",	"3",	"3");
		/* EP 13 "Invincible" */	s.Add("End",			"Empowerment",	"Audio only",	"Cut",	"Adults",	"Female",	"Strong",	"Dirty",	"3",	"3");
		/* EP 12 "Girl I n h" */	s.Add("End-ish",		"Loneliness",	"Audio only",	"Cut",	"Adults",	"Male",		"Weak",		"Clean",	"4",	"5");
		/* EP 11 "Pahuutta pah" */	s.Add("End-ish",		"Confusion",	"Audio only",	"Cut",	"Adults",	"Female",	"Weak",		"Dirty",	"3",	"4");
		/* EP 10 "Mitä tapahtuu" */	s.Add("End-ish",		"Confusion",	"Audio only",	"Cut",	"Adults",	"Female",	"Weak",		"Dirty",	"4",	"3");
		
		
		int j = s.qt.GetLargestInfoGainPredictor();
		DUMP(j);
		DUMPC(s.qt.GetInfoGains());
		LOG(s.qt.GetInfoString());
	}
}
