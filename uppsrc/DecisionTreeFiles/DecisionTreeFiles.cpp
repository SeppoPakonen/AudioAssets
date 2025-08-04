#include <AICore/AICore.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	{
		QueryTable<String> qt;
		
		Vector<String>& rl = qt.AddPredictor("Rhyming level lyrics");
		Vector<String>& fa = qt.AddPredictor("X-Centric video");
		Vector<String>& ly = qt.AddPredictor("Lyrics as text");
		Vector<String>& fad = qt.AddPredictor("Fading sequences in video");
		Vector<String>& kids = qt.AddPredictor("Youngest audience");
		Vector<String>& g = qt.AddPredictor("Gender");
		
		/* Target value classes 4.8.2025:
		
		1: Around 3 hours
		2: Around 2 hours
		3: Around 1 hour
		4: Around 30 mins
		*/
		qt.AddTargetScore("1", 8);
		qt.AddTargetScore("2", 4);
		qt.AddTargetScore("3", 2);
		qt.AddTargetScore("4", 1);
		
		// EP 23 "Sydämen varas"
		rl.Add("End-ish");		fa.Add("Dating");		ly.Add("Text");			fad.Add("Cut");			kids.Add("Adults");		g.Add("Female");	qt.AddTargetValue("3");
		
		// EP 22 "Tahdon sut"
		rl.Add("End-ish");		fa.Add("Dating");		ly.Add("Text");			fad.Add("Fade");		kids.Add("Kids");		g.Add("Both");		qt.AddTargetValue("3");
		
		// EP 21 "Talviyö"
		rl.Add("End-ish");		fa.Add("Breakup");		ly.Add("Text");			fad.Add("Fade");		kids.Add("Adults");		g.Add("Both");		qt.AddTargetValue("4");
		
		// EP 20 "Puuma"
		rl.Add("End-ish");		fa.Add("Dating");		ly.Add("Text");			fad.Add("Fade");		kids.Add("Adults");		g.Add("Male");		qt.AddTargetValue("3");
		
		// EP 19 "Aamuisin"
		rl.Add("Begin & End");	fa.Add("Dating");		ly.Add("Text");			fad.Add("Fade");		kids.Add("Adults");		g.Add("Both");		qt.AddTargetValue("1");
		
		// EP 18 "Vanha sävel"
		rl.Add("End-ish");		fa.Add("Party");		ly.Add("Audio only");	fad.Add("Fade");		kids.Add("Kids");		g.Add("Both");		qt.AddTargetValue("4");
		
		// EP 17 "Great escape"
		rl.Add("End");			fa.Add("Breakup");		ly.Add("Audio only");	fad.Add("Fade");		kids.Add("Kids");		g.Add("Female");	qt.AddTargetValue("3");
		
		// EP 16 "Mennään jonnekkin kahdestaan"
		rl.Add("End");			fa.Add("Dating");		ly.Add("Audio only");	fad.Add("Fade");		kids.Add("Kids");		g.Add("Both");		qt.AddTargetValue("2");
		
		// EP 15 "Laiva"
		rl.Add("End");			fa.Add("Married");		ly.Add("Audio only");	fad.Add("Fade");		kids.Add("Kids");		g.Add("Male");		qt.AddTargetValue("3");
		
		// EP 14 "Mikä on"
		rl.Add("End-ish");		fa.Add("Loneliness");	ly.Add("Audio only");	fad.Add("Fade");		kids.Add("Adults");		g.Add("Female");	qt.AddTargetValue("3");
		
		// EP 13 "Invincible"
		rl.Add("End");			fa.Add("Empowerment");	ly.Add("Audio only");	fad.Add("Cut");			kids.Add("Adults");		g.Add("Female");	qt.AddTargetValue("3");
		
		// EP 12 "Girl I never had"
		rl.Add("End-ish");		fa.Add("Loneliness");	ly.Add("Audio only");	fad.Add("Cut");			kids.Add("Adults");		g.Add("Male");		qt.AddTargetValue("4");
		
		// EP 11 "Pahuutta pahempaa"
		rl.Add("End-ish");		fa.Add("Confusion");	ly.Add("Audio only");	fad.Add("Cut");			kids.Add("Adults");		g.Add("Female");	qt.AddTargetValue("3");
		
		// EP 10 "Mitä tapahtuu"
		rl.Add("End-ish");		fa.Add("Confusion");	ly.Add("Audio only");	fad.Add("Cut");			kids.Add("Adults");		g.Add("Female");	qt.AddTargetValue("4");
		
		int i = qt.GetLargestInfoGainPredictor();
		DUMP(i);
		DUMPC(qt.GetInfoGains());
		LOG(qt.GetInfoString());
	}
	
}
