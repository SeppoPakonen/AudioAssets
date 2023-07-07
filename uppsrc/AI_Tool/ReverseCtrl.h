#ifndef _AI_Tool_ReverseCtrl_h_
#define _AI_Tool_ReverseCtrl_h_


struct TaskResult {
	int id = -1;
	double optimizer_score = 0;
	
};

struct ReverseTask {
	Array<TaskResult> results;
	Vector<double> result_values;
	Vector<double> scores;
	PatternSnap* snap = 0;
	int id = -1;
	int actual = 0, total = 1;
	double values_min = +DBL_MAX;
	double values_max = -DBL_MAX;
	bool active = false;
	RWMutex lock;
	
	Vector<SnapAttr> attrs;
	
};


class TaskPlotter : public Ctrl {
	ReverseTask* rtask = 0;
	Color clr, line_clr;
	
public:
	TaskPlotter();
	typedef TaskPlotter CLASSNAME;
	
	void Paint(Draw& draw) override;
	
	void Set(ReverseTask& t) {rtask = &t; Refresh();}
	
};


class TaskCtrl : public Ctrl {
	
public:
	TaskPlotter plotter;
	ProgressIndicator total_prog, snap_prog;
	
public:
	TaskCtrl();
	typedef TaskCtrl CLASSNAME;
	
	
};


class ReverseCtrl : public WithReverse<Ctrl> {
	Array<ReverseTask> tasks;
	
	ArrayCtrl tasklist, resultlist;
	TaskCtrl task;
	One<CoWork> co;
	TimeCallback tc;
	bool is_running = false, is_stopped = true;
	
	static const int gen_multiplier = 20;
	static const int gens = 200;
	inline static const int max_values = 12;
	
public:
	typedef ReverseCtrl CLASSNAME;
	ReverseCtrl();
	
	void Data();
	void DataWorker();
	void OnStart();
	void OnStop();
	void Stop();
	void LoadActivePatternScore();
	void TaskWorker(ReverseTask* task);
	void UpdateTaskStatus(ReverseTask* task);
	
	ToolApp* app = 0;
	
};


#endif
