#ifndef _GPT3Lyrics_Editor_h_
#define _GPT3Lyrics_Editor_h_

#define LAYOUTFILE <GPT3Lyrics/Editor.lay>
#include <CtrlCore/lay.h>


struct Song : Moveable<Song> {
	String name;
	int year;
	
	String topic;
	String genre;
	String subdir;
	String audience;
	String qualifier;
	String attributes;
	
	String GetPath() const;
	
	void Xmlize(XmlIO& xml);
	bool operator()(const Song& a, const Song& b) const {if (a.year != b.year) return a.year < b.year; return a.name < b.name;}
};

class Editor : public TopWindow {
	Splitter hsplit;
	ArrayCtrl songs;
	WithSongForm<ParentCtrl> edit;
	MenuBar menu;
	
	VectorMap<String,Song> songlist;
	
	Index<String> qualifier;
	Index<String> genre;
	Index<String> audience;
	Index<String> attributes;
	
public:
	typedef Editor CLASSNAME;
	Editor();
	~Editor();
	
	void MenuBar(Bar& bar);
	void Data();
	void DataChanged();
	void AttributeListChanged();
	void SongData();
	void FindSongs();
	void LoadThis();
	void StoreThis();
	void Xmlize(XmlIO& xml);
	
};


#endif
