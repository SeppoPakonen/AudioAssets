#ifndef _AI_Tool_ProductionCtrl_h_
#define _AI_Tool_ProductionCtrl_h_


class ProductionCtrl : public WithProduction<Ctrl> {
	
	
public:
	typedef ProductionCtrl CLASSNAME;
	ProductionCtrl();
	
	void Data();
	
	ToolApp* app = 0;
	
};


#endif
