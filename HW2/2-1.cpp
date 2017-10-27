#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <sstream>
#include <string>
#include <cstdio>

int main(){
	int N;
	std::queue <int> q;
	std::vector <int> w, trard;
	std::fstream file_in;


	file_in.open("Q1.txt", std::ios::in);
	file_in >> N;
	//std::cin >> N;
	for(int i=0; i<N; ++i){
		int tmp;
		file_in >> tmp;
		//std::cin >> tmp;
		q.push(tmp);
		w.push_back(0);
		trard.push_back(0);
	}

	unsigned int t = 0;
	int pnum = 0;
	float avg_w = 0, avg_trard = 0;
	while(q.size() > 0){
		w.at(pnum) = t;
		trard.at(pnum) = q.front() + t;
		avg_w += w.at(pnum);
		avg_trard += trard.at(pnum);
		t += q.front();
		pnum++;
		q.pop();
	}

	std::cout << "Process    Waiting Time    Turnaround Time\n"; 
	for(int i=0; i<N; ++i){
		std::stringstream ss;
		std::string s;
		ss << "p[" << i+1 << "]";
		ss >> s;
		std::cout << s;
		if(s.length() < 11)
			for(int j=0; j<11-s.length(); ++j)
				std::cout << " ";
		printf("%-16d", w.at(i));
		printf("%-15d\n", trard.at(i));
	}
	printf("\nAverage waiting time : %.7f\n", avg_w/N);
	printf("Average turnaround time : %.7f\n", avg_trard/N);

	file_in.close();
	return 0;
}