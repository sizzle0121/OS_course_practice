#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <string>
#include <sstream>
#include <cstdio>

struct process{
	int id;
	int arr, bt;
	int w, t;
	process(int _id, int _arr) : id(_id), arr(_arr), bt(0), w(0), t(0){}
	process(int _id, int _arr, int _bt) : id(_id), arr(_arr), bt(_bt), w(0), t(0){}
	bool operator<(const process& b) const{
		if(bt != b.bt)	return bt > b.bt;
		else if(arr != b.arr)	return arr > b.arr;
		else	return id > b.id;
	}
};

int main(){
	int N, _N;
	unsigned int counter = 0;
	std::fstream file_in;
	std::priority_queue <process> q;
	std::vector <process> p;

	file_in.open("Q2.txt", std::ios::in);
	file_in >> N;
	//std::cin >> N;
	_N = N;
	for(int i=0; i<N; ++i){
		int tmp;
		file_in >> tmp;
		//std::cin >> tmp;
		p.push_back(process(i+1, tmp));
	}
	for(int i=0; i<N; ++i){
		int tmp;
		file_in >> tmp;
		//std::cin >> tmp;
		p[i].bt = tmp;
	}

	int remain_bt = 0;
	int current_pid = -1;
	//bool first = true;
	float avg_w = 0, avg_t = 0;
	//empty or terminate
	while(_N){
		for(int i=0; i<N; ++i){
			if(p[i].arr == counter){
				q.push(p[i]);
			}
		}
		if(remain_bt == 0){
			if(current_pid >= 0){
				p[current_pid].t = counter - p[current_pid].arr;
				p[current_pid].w = p[current_pid].t - p[current_pid].bt;
				avg_w += p[current_pid].w;
				avg_t += p[current_pid].t;
				_N--;
				current_pid = -1;
			}
			if(_N > 0 && q.size() > 0){
				//choose_process
				remain_bt = q.top().bt;
				current_pid = q.top().id-1;
				q.pop();
			}
		}
		if(remain_bt != 0)	remain_bt--;
		counter++;
	}

	std::cout << "Process    Waiting Time    Turnaround Time\n";
	for(int i=0; i<N; ++i){
		std::stringstream ss;
		std::string s;
		ss << "p[" << i+1 << ']';
		ss >> s;
		std::cout << s;
		if(s.length() < 11)
			for(int j=0; j<11-s.length(); ++j)
				std::cout << " ";
		printf("%-16d", p[i].w);
		printf("%-15d\n", p[i].t);
	}
	printf("\nAverage waiting time : %.7f\n", avg_w/N);
	printf("Average turnaround time : %.7f\n", avg_t/N);
	file_in.close();
	return 0;
}