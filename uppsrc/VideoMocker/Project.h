#ifndef _VideoMocker_Project_h_
#define _VideoMocker_Project_h_

namespace VideoMockup {

typedef enum {
	ALIGN_BOTTOM,
	ALIGN_CENTER,
	ALIGN_TOP,
} TextAlign;

String GetTextAlignString(TextAlign a);
TextAlign GetTextAlignFromString(String s);
void JsonizeAlign(JsonIO& json, TextAlign& a);

class ImageFrame {
public:
	int time;
	String title, filename, keywords, notes;
	Image thumbnail;
	
	// Temporary
	Image img;
	
	
	void Jsonize(JsonIO& json);
	
};


class SubtitleFrame {
	
public:
	int time;
	String line1, line2;
	TextAlign align = ALIGN_BOTTOM;
	
	void Jsonize(JsonIO& json);
	
};


class LyricsFrame {
	
public:
	int time;
	String line1, line2;
	TextAlign align = ALIGN_BOTTOM;
	
	
	void Jsonize(JsonIO& json);
	
};


class Project {
	
	// Persistent
	Array<ImageFrame> imgs;
	Array<SubtitleFrame> subs;
	Array<LyricsFrame> lyrics;
	
	// Temporary
	String path;
	
public:
	typedef Project CLASSNAME;
	Project();
	
	void Clear();
	void Jsonize(JsonIO& json);
	
	void StoreThis() {
		ASSERT(!path.IsEmpty());
		StoreAsJsonFile(*this, path, true);
	}
	void LoadThis() {
		ASSERT(!path.IsEmpty());
		LoadFromJsonFile(*this, path);
	}
	
};

}

#endif
