#ifndef _AI_Tool_AttrScoreCtrl_h_
#define _AI_Tool_AttrScoreCtrl_h_


class AttrScoreCtrl : public WithAttrScore<Ctrl> {
	
	
public:
	typedef AttrScoreCtrl CLASSNAME;
	AttrScoreCtrl();
	
	void Data();
	
	ToolApp* app = 0;
	
};

#endif
