#include "ChordTool.h"

ChordTool::ChordTool()
{
	CtrlLayout(*this, "Chord Tool");
	Sizeable().MaximizeBox().MinimizeBox();
	SetRect(Size(800,600));
	
	AddFrame(menu);
	menu.Set(THISBACK(MainMenu));
	
	list.AddColumn("Label");
	list.AddColumn("#");
	list.AddColumn("Function"); // Repeat "call", Goto, etc.
	list.AddColumn("Kind"); // Section, Key change, New key signature, Chord, Notes, Ending, Linked node, etc.
	list.AddColumn("Variable"); // If a chord has a variable name for linking
	list.AddColumn("Chord (abs.)"); // Absolute chord
	list.AddColumn("Chord (rel.)"); // Relative chord (in Major C scale + root semitone difference)
	list.AddColumn("Chord extensions"); // e.g. C -> Cm7 = C + MINOR + 7
	list.AddColumn("Chord (fine)"); // Relative chord in finer notation, e.g. I IV III etc.
	list.AddColumn("Beats"); // default: 4 for 4/4
	list.AddColumn("Binder"); // where listener's chord expectations meets the current actual chord
	list.AddColumn("Special flags"); // e.g. trioles (6/4 in 4/4)
	list.AddColumn("Rule"); // if some rule/assertion is used: e.g. 4th chord is same as 1st
	
}

void ChordTool::MainMenu(Bar& bar) {
	bar.Sub("File", [this](Bar& b) {
		b.Add("New", [this]{});
		b.Add("Open", [this]{});
		b.Add("Save", [this]{});
		b.Add("Save As", [this]{});
		b.Separator();
		b.Add("Quit", [this]{});
	});
	bar.Sub("Help", [this](Bar& b) {
		b.Add("About", [this]{});
	});
	
}

