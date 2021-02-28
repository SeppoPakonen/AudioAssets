#ifndef _WebsiteGenerator_WebsiteGenerator_h
#define _WebsiteGenerator_WebsiteGenerator_h

#include <Core/Core.h>

using namespace Upp;



struct Data : Moveable<Data> {
	String                    filename;
	String                    description;
	String                    tags;
	Date                      date;
	int                       date_vis = 0;
	int                       listens = 0;
	bool                      is_hidden = false;
	
	
	Time                      last_edit;
	int                       idx = -1;
	
	void Jsonize(JsonIO& json) {
		json
			("Filename", filename)
			("Description", description)
			("Tags", tags)
			("Date", date)
			("Date-accuracy", date_vis)
			("Listens", listens)
			("Hidden", is_hidden)
		;
	}
	void Xmlize(XmlIO& xio)           { XmlizeByJsonize(xio, *this); }
	
	bool operator()(const Data& a, const Data& b) const {return a.date < b.date;}
	
};

struct DataEditSort {
	bool operator()(const Data& a, const Data& b) const {return a.last_edit > b.last_edit;}
};




struct Generator {
	Vector<String> files;
	Vector<Data> data;
	Vector<int> fresh_list;
	
	String author;
	String export_dir;
	String root_addr;
	String description;
	String more_url;
	String more_msg;
	String song_dir_to_root;
	String song_dir;
	String upphub_dir;
	String src_tg_dir;
	
	Generator();
	
	void Export();
	String GetCleanName(const Data& d);
	String GetTitleName(const Data& d);
	String GetRoundedDate(const Data& d);
	String GetUrl(const Data& d) {return root_addr + "/song/" + GetCleanName(d) + ".html";}
	String GetTGUrl(const Data& d) {return root_addr + "/song/" + GetCleanName(d) + ".tg";}
	
};


#endif
