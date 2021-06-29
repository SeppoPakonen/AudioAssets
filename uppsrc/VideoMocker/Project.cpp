#include "VideoMocker.h"

namespace VideoMockup {


String GetTextAlignString(TextAlign a) {
	switch (a) {
		case ALIGN_CENTER:	return "center";
		case ALIGN_TOP:		return "top";
		default:
		case ALIGN_BOTTOM:	return "bottom";
	}
}

TextAlign GetTextAlignFromString(String s) {
	if (s == "center")
		return ALIGN_CENTER;
	if (s == "top")
		return ALIGN_TOP;
	
	return ALIGN_BOTTOM;
}

void JsonizeAlign(JsonIO& json, TextAlign& a) {
	if (json.IsLoading()) {
		String s;
		json("align", s);
		a = GetTextAlignFromString(s);
	}
	else {
		String s = GetTextAlignString(a);
		json("align", s);
	}
}



void ImageFrame::Jsonize(JsonIO& json) {
	json
		("time", time)
		("title", title)
		("filename", filename)
		("keywords", keywords)
		("notes", notes)
		("thumbnail", thumbnail)
	;
	if (json.IsLoading()) {
		String b64;
		json ("thumbnail", b64);
		String s = Decode64(b64);
		thumbnail = StreamRaster::LoadStringAny(s);
	}
	else {
		JPGEncoder e;
		String s = e.SaveString(thumbnail);
		String b64 = Encode64(s);
		json ("thumbnail", b64); 
	}
}



void SubtitleFrame::Jsonize(JsonIO& json) {
	json
		("time", time)
		("line1", line1)
		("line2", line2)
	;
	JsonizeAlign(json, align);
}



void LyricsFrame::Jsonize(JsonIO& json) {
	json
		("time", time)
		("line1", line1)
		("line2", line2)
	;
	JsonizeAlign(json, align);
}




Project::Project() {
	
}

void Project::Jsonize(JsonIO& json) {
	json
		("images", imgs)
		("subtitles", subs)
		("lyrics", lyrics)
	;
}

void Project::Clear() {
	imgs.Clear();
	subs.Clear();
	lyrics.Clear();
	path.Clear();
}



}
