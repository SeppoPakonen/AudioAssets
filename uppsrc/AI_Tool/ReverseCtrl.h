#ifndef _AI_Tool_ReverseCtrl_h_
#define _AI_Tool_ReverseCtrl_h_


class ReverseCtrl : public WithReverse<Ctrl> {
	
	
public:
	typedef ReverseCtrl CLASSNAME;
	ReverseCtrl();
	
	void Data();
	
	
	ToolApp* app = 0;
	
};


#endif
