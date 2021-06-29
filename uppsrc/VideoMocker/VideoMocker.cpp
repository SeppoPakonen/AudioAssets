#include "VideoMocker.h"



VideoMocker::VideoMocker()
{
	Title("VideoMocker");
	Add(hsplit.SizePos());
	hsplit.Horz() << panel << vsplit;
	hsplit.SetPos(2000);
	vsplit.Vert() << renderer << timeline;
	vsplit.SetPos(8000);
	Maximize();
	MinimizeBox().MaximizeBox();
	Sizeable();
	
	AddFrame(menu);
	menu.Set(THISBACK(MainMenu));
	
	panel.time << THISBACK(DataChanged);
	panel.title << THISBACK(DataChanged);
	panel.filename << THISBACK(DataChanged);
	panel.keywords << THISBACK(DataChanged);
	panel.notes << THISBACK(DataChanged);
	
	panel.lyr_time << THISBACK(DataChanged);
	panel.lyr_line1 << THISBACK(DataChanged);
	panel.lyr_line2 << THISBACK(DataChanged);
	panel.lyr_pos << THISBACK(DataChanged);
	
	panel.sub_time << THISBACK(DataChanged);
	panel.sub_line1 << THISBACK(DataChanged);
	panel.sub_line2 << THISBACK(DataChanged);
	panel.sub_pos << THISBACK(DataChanged);
	
}

void VideoMocker::MainMenu(Bar& bar) {
	bar.Sub("File", [this](Bar& bar) {
		bar.Add("New", THISBACK(NewFile));
		bar.Add("Open", THISBACK(OpenFile));
		bar.Add("Save", THISBACK(SaveFile));
		bar.Add("Save As", THISBACK(SaveAsFile));
	});
	
}

void VideoMocker::NewFile() {
	prj.Clear();
	Data();
}

void VideoMocker::SaveAsFile() {
	
}

void VideoMocker::SaveFile() {
	
}

void VideoMocker::OpenFile() {
	
}

void VideoMocker::DataChanged() {
	if (img_frame) {
		img_frame->time = panel.time.GetData();
		img_frame->title = panel.title.GetData();
		img_frame->filename = panel.filename.GetData();
		img_frame->keywords = panel.keywords.GetData();
		img_frame->notes = panel.notes.GetData();
		
	}
}

void VideoMocker::Data() {
	
}




SidePanel::SidePanel()
{
	CtrlLayout(*this);
	
}


void RendererCtrl::Paint(Draw& d) {
	Size sz = GetSize();
	
	d.DrawRect(sz, White());
	
	
}




void TimelineCtrl::Paint(Draw& d) {
	Size sz = GetSize();
	
	d.DrawRect(sz, White());
	
	
}
