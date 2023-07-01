#include "AI_Tool.h"


Plotter::Plotter() {
	
}

void Plotter::Paint(Draw& d) {
	Size sz = GetSize();
	
	d.DrawRect(sz, White());
	
}





PlotCtrl::PlotCtrl() {
	AddFrame(InsetFrame());
	Add(plotter.HSizePos().VSizePos(0,30));
	Add(choices.BottomPos(0,30).HSizePos());
	
	
	choices.Add(t_("Mood: joyful/melancholic"));
	choices.Add(t_("Mood: playful/serious"));
	choices.Add(t_("Mood: uplifting/heavy"));
	choices.Add(t_("Mood: lighthearted/somber"));
	choices.Add(t_("Mood: humorous/dramatic"));
	choices.Add(t_("Social: authoritarian/liberatrian"));
	choices.Add(t_("Economic: liberal/conservative"));
	choices.Add(t_("Culture: individualism/collective"));
	choices.Add(t_("Human strength: strong/weak"));
	choices.Add(t_("Motivation: rewarding/punishing"));
	choices.Add(t_("Sexualization: sexual/non-sexual"));
	choices.Add(t_("Attitude: hopeful/despair"));
	choices.Add(t_("Attitude: optimistic/pessimistic"));
	choices.Add(t_("Attitude: open/closed"));
	choices.Add(t_("Beliefs: spiritual/secular"));
	choices.Add(t_("Expectations: perfection/acceptance"));
	choices.SetIndex(0);
	
}
