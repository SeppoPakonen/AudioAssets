#ifndef _AI_Tool_SongCtrl_h_
#define _AI_Tool_SongCtrl_h_


class ToolApp;


class SongCtrl : public WithSong<Ctrl> {
	
public:
	typedef SongCtrl CLASSNAME;
	SongCtrl();
	
	void Data();
	
	ToolApp* app = 0;
	
};


#endif
