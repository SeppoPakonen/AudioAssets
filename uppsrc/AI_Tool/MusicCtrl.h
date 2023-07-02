#ifndef _AI_Tool_MusicCtrl_h_
#define _AI_Tool_MusicCtrl_h_


class MusicCtrl : public WithMusic<Ctrl> {
	
	
public:
	typedef MusicCtrl CLASSNAME;
	MusicCtrl();
	
	void Data();
	
	ToolApp* app = 0;
	
};


#endif
