#ifndef _AI_Tool_PlotCtrl_h_
#define _AI_Tool_PlotCtrl_h_


class Plotter : public Ctrl {
	
	
public:
	Plotter();
	
	void Paint(Draw& d);
	
};


class PlotCtrl : public Ctrl {
	Plotter plotter;
	DropList choices;
	
	
public:
	typedef PlotCtrl CLASSNAME;
	PlotCtrl();
	
	
};


#endif
