#ifndef _AI_Tool_TimelineCtrl_h_
#define _AI_Tool_TimelineCtrl_h_


class TimelineCtrl : public WithTimeline<Ctrl> {
	ArrayCtrl albumlist, songlist;
	
	
public:
	typedef TimelineCtrl CLASSNAME;
	TimelineCtrl();
	
	void Data();
	void DataAlbum();
	void DataSong();
	void AddAlbum();
	void AddSong();
	void RemoveSong();
	void OnValueChange();
	void SongBar(Bar& bar);
	
	ToolApp* app = 0;
	
};


#endif
