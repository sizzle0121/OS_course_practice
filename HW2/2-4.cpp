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
	int BT;
	int w, t;
	process(int _id, int _arr) : id(_id), arr(_arr), BT(0), bt(0), w(0), t(0){}
	process(int _id, int _arr, int _bt) : id(_id), arr(_arr), bt(_bt), w(0), t(0){}
	bool operator<(const process& b) const{
		if(bt != b.bt)	return bt > b.bt;
		else if(arr != b.arr)	return arr > b.arr;
		else	return id > b.id;
	}
};
int N, _N;
int T1, T2;

unsigned int counter = 0;
bool first = true;
int current_pid = -1;
float avg_w = 0, avg_t = 0;
std::queue<process> q1;
std::queue<process> q2;
std::priority_queue<process> q3;
std::vector<process> p;

void high_q();
void mid_q();
void low_q();

int main(){
	std::fstream file_in;
	file_in.open("Q4.txt", std::ios::in);
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
		p[i].BT = tmp;
		p[i].bt = tmp;
	}
	file_in >> T1 >> T2;
	//std::cin >> T1 >> T2;

	high_q();
	//printf("\n");
	
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


void high_q(){
	//bt == 0,  T1 expired
	while(first || q1.size() > 0){
		if(first){
			for(int i=0; i<N; ++i){
				if(p[i].arr == counter){
					q1.push(p[i]);
				}
			}
		}

		if(q1.size() > 0){
			first = false;
			current_pid = q1.front().id-1;

			//std::cout << q1.front().id << " ";

			q1.pop();
			int timer = T1;
			while(timer){
				timer--;
				p[current_pid].bt--;
				counter++;
				//printf("timer: %d, pid: %d, bt: %d\n#########\n", timer, current_pid+1, p[current_pid].bt);
				if(p[current_pid].bt == 0){
					p[current_pid].t = counter - p[current_pid].arr;
					//printf("counter: %d, timer: %d\n", counter, timer);

					p[current_pid].w = p[current_pid].t - p[current_pid].BT;
					avg_w += p[current_pid].w;
					avg_t += p[current_pid].t;
					current_pid = -1;
					_N--;
					break;
				}

				for(int i=0; i<N; ++i){
					if(p[i].arr == counter){
						//printf("current pid: %d, counter: %d, timer: %d\n########\n", current_pid+1, counter, timer);
						q1.push(p[i]);
					}
				}
				
			}
			//if(timer == 0)	p[current_pid].bt--;
			//printf("out loop timer: %d, pid: %d, bt: %d\n############\n", timer, current_pid+1, p[current_pid].bt);

			if(p[current_pid].bt > 0){//have remain time
				q2.push(p[current_pid]);
			}
			//counter += (T1-timer);
			//printf("only counter: %d\n", counter);
		}
		else{
			counter++;
		}

	}
	if(_N > 0)	{
		/*for(int i=0; i<N; ++i)
			std::cout << p[i].bt << " ";
		std::cout << "###\n";*/
		/*for(int i=0; i<N; ++i)
			std::cout << p[i].w << "\t" << p[i].t << std::endl;
		std::cout << "###\n";*/

		mid_q();
	}
	else	return;


}

void mid_q(){
	bool back_to_q1 = false;
	while(q2.size() > 0){
		for(int i=0; i<N; ++i){
			if(p[i].arr == counter){
				q1.push(p[i]);
				back_to_q1 = true;
			}
		}

		current_pid = q2.front().id-1;

		//std::cout << q2.front().id << " ";

		q2.pop();
		int timer = T2;
		while(timer){
			timer--;
			p[current_pid].bt--;
			counter++;
			if(p[current_pid].bt == 0){
				p[current_pid].t = counter - p[current_pid].arr;
				p[current_pid].w = p[current_pid].t - p[current_pid].BT;
				avg_w += p[current_pid].w;
				avg_t += p[current_pid].t;
				current_pid = -1;
				_N--;
				break;
			}
			for(int i=0; i<N; ++i){
				if(p[i].arr == counter){
					q1.push(p[i]);
					back_to_q1 = true;
				}
			}
		}
		//if(timer == 0)	p[current_pid].bt--;
		if(p[current_pid].bt > 0){//have remain time
			q3.push(p[current_pid]);
		}
		if(back_to_q1)	high_q();
	}
	if(_N > 0){
		/*for(int i=0; i<N; ++i)
			std::cout << p[i].bt << " ";
		std::cout << "###\n";*/
		low_q();
	}
	else	return;
}

void low_q(){
	bool back_to_q1 = false;
	int remain_bt = 0;
	current_pid = -1;
	while(_N){
		for(int i=0; i<N; ++i){
			if(p[i].arr == counter){
				q1.push(p[i]);
				back_to_q1 = true;
			}
		}
		if(current_pid == -1 && back_to_q1)	high_q();

		if(remain_bt == 0){
			if(current_pid >= 0){
				p[current_pid].t = counter - p[current_pid].arr;
				p[current_pid].w = p[current_pid].t - p[current_pid].BT;
				avg_w += p[current_pid].w;
				avg_t += p[current_pid].t;
				_N--;
				current_pid = -1;
				if(back_to_q1)	high_q();
			}
			if(_N > 0 && q3.size() > 0){
				//choose_process
				remain_bt = q3.top().bt;
				current_pid = q3.top().id-1;

				//std::cout << q3.top().id << " ";

				q3.pop();
			}
		}
		if(remain_bt != 0)	remain_bt--;
		counter++;
	}
	return;
}