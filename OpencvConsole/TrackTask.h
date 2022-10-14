#pragma once
#include <Windows.h>
#include <any>
#include <mutex>
#include <future>
#include <opencv2/opencv.hpp>

#define log(str) std::cout << __FUNCTION__ <<":"<< __LINE__<<" ||=>> " << str << std::endl;

template <typename T_in, typename T_out>
class TrackTask
{
	T_in* in_object;
	T_out* out_object;
	T_out(*func_ptr)(T_in&) = nullptr;

	bool is_run = false;
	std::mutex m;
	std::thread* track_thread = nullptr;
public:
	TrackTask()
	{
		in_object = new T_in();
		out_object = new T_out();
	}
	~TrackTask()
	{
		if (track_thread != nullptr)
		{
			track_thread->join();
			delete track_thread;
		}
		delete in_object;
		delete out_object;
	}
	bool check_wait()
	{
		return !is_run;
	}
	void set_input(T_in& in)
	{
		// TODO �ǿ���
		*in_object = in;
	}
	T_out get_output()
	{
		// TODO �ǿ���
		std::lock_guard<std::mutex> lk(m);
		return *out_object;
	}
	void task(T_out(*task_func_ptr)(T_in&))
	{
		func_ptr = task_func_ptr;
		//is_run = true;
	}
	void work()
	{
		if (track_thread == nullptr)
		{
			track_thread = new std::thread(&TrackTask<T_in,T_out>::run,this);
		}
		else
		{
			track_thread->join();
			delete track_thread;
			track_thread = new std::thread(&TrackTask<T_in, T_out>::run, this);
		}
	}
	void run()
	{
		if (func_ptr != nullptr)
		{
			is_run = true;
			T_out m_out_object = func_ptr(*in_object);
			// TODO ��ȷ��Ϊ����
			// �˴���Ҫ��ԭ�Ӳ���
			std::lock_guard<std::mutex> lk(m);
			*out_object = m_out_object;

			is_run = false;
		}
	}
};

class TrackManage
{
	bool is_run = false;
	bool is_exit = false;

	std::thread* track_thread = nullptr;
	std::condition_variable* is_run_cv = nullptr;
	std::mutex* is_run_m = nullptr;
	TrackTask<cv::Mat, cv::Mat>* task_screen = nullptr;
	TrackTask<cv::Mat, cv::Point2d>* task_positon = nullptr;
	TrackTask<cv::Mat, bool>* task_is_on_mainview = nullptr;

	//std::vector<Task*> task_list;
public:
	TrackManage()
	{
		is_run_cv = new std::condition_variable();
		is_run_m = new std::mutex();
		
		auto func_screen = [](cv::Mat& in) -> cv::Mat
		{
			static cv::Mat out = cv::imread("C:\\Users\\GengG\\source\\repos\\Cv����\\OpencvConsole\\img.png",-1);
			log("���� func_screen");
			//cv::cvtColor(out, out, cv::COLOR_BGRA2GRAY);
			return out;
		};
		auto func_position = [](cv::Mat& in)->cv::Point2d
		{
			static cv::Point2d pos;
			log("���� func_position");
			return pos;
		};
		auto func_is_on_mainview = [](cv::Mat& in)->bool
		{
			static bool is_on_mainview;
			log("���� func_is_on_mainview");
			Sleep(8500);
			log("���н��� func_is_on_mainview");
			return is_on_mainview;
		};
		task_screen = new TrackTask<cv::Mat, cv::Mat>();
		task_screen->task(func_screen);
		task_positon = new TrackTask<cv::Mat, cv::Point2d>();
		task_positon->task(func_position);
		task_is_on_mainview = new TrackTask<cv::Mat, bool>();
		task_is_on_mainview->task(func_is_on_mainview);

		//TaskScreen* task_screen = new TaskScreen();
		//task_list.push_back(task_screen);

	}
	~TrackManage() 
	{
		is_exit = true;
		
		is_run_cv->notify_all();
		track_thread->join();
		delete track_thread;

		delete task_screen;
		delete task_positon;
		delete task_is_on_mainview;

		delete is_run_cv;
		delete is_run_m;
	}
	void Start()
	{
		if (track_thread == nullptr)
		{
			log("���� TrackManage �߳�");
			track_thread = new std::thread(&TrackManage::server,this);
		}
		log("���� TrackManage �߳�");
		is_run_cv->notify_all();
		is_run = true;
	}
	void Stop()
	{
		log("ֹͣ TrackManage �߳�");
		is_run = false;
	}
	void server()
	{
		log("���� TrackManage �߳�");
		while (!is_exit)
		{
			if (!is_run)
			{
				log("TrackManage �̵߳ȴ�");
				// is_run Ϊ false ʱ������cv�ȴ�1s
				std::unique_lock<std::mutex> lk(*is_run_m);
				is_run_cv->wait_for(lk, std::chrono::seconds(1), [this] { return is_run; });
				continue;
			}
			
			// ҵ���߼�
			// TODO
			log("TrackManage �߳�����ҵ���߼�");
			check_wait_any();
			Sleep(100);

		}
		log("�˳� TrackManage �߳�");
	}
	
	//TrackResult& GetResult();
private:
	bool check_wait_any()
	{
		// ����task��״̬
		std::map<std::string, std::any> wait_state_list;

		
		if (task_screen->check_wait())
		{
			task_screen->work();
			wait_state_list["task_screen"] = std::any(task_screen);
		}
		
		if (task_positon->check_wait())
		{
			cv::Mat in = task_screen->get_output();
			if (in.empty())
			{
				log("task_positon ���� task_screen");
			}
			else
			{
				task_positon->set_input(in);
				task_positon->work();
				wait_state_list["task_positon"] = std::any(task_positon);
				log(wait_state_list["task_positon"].type().name());
			}

		}
		if (task_is_on_mainview->check_wait())
		{
			cv::Mat in;
			task_is_on_mainview->set_input(in);
			task_is_on_mainview->work();
			wait_state_list["task_is_on_mainview"] = std::any(task_is_on_mainview);
		}

		// ��������һ��task���
		if (wait_state_list.empty())
		{
			return true;
		}
		return false;
	}
};


int main_genshin()
{
	TrackManage trackManage;
	trackManage.Start();

	Sleep(300);
	trackManage.Stop();
	Sleep(3000);
	trackManage.Start();
	Sleep(1000);
	trackManage.Stop();
	Sleep(1000);
	trackManage.Start();
	Sleep(10000);
	trackManage.Stop();
	Sleep(1000);
	return 0;
}
int main_track()
{
	//return main_test();
	return main_genshin();
}