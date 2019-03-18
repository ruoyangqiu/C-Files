// hw3.cpp 
// Ruoyang Qiu, Hao Li
// 
// This is a simulator that will simulate the execution of
// different CPU scheduling algorithms:
// SJF: Shortest Job First 
// SRTF: Shortest Remaining Time First
// NP: Nonpreemptive Priority
// PP: Preemptive Priority
// 
// A file which store the information of several process 
// will be provided. The information include 4 integers in
// a line:
// 1. Process id
// 2. Arrival time
// 3. Burst time
// 4. Priority
//
// The user will also the algorithm they want tot use to 
// simulate.
//
// The program will simulate the execution of the chosen
// algorithm and print running infomation
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iterator>
#include <sstream>
#include <iomanip>
#include <math.h>

using namespace std;


// Class store process information
// id, arrival time, burst time, priority, and remaining burst time

class myProcess
{
public:
	int  arrival_time, burst_time, _priority;

	myProcess(int id, int arrivalTime, int burstTime, int ipriority)
	{
		_id = id;
		arrival_time = arrivalTime;
		burst_time = burstTime;
		_priority = ipriority;
		remain_time = burstTime;
	}

	// set remaining burst time by subtruct a certain time
	void setRemain(int _time)
	{
		remain_time -= _time;
	}

	// get process id
	int getId()
	{
		return _id;
	}

	// get the remaining burst time
	int getremain() {
		return remain_time;
	}

private:
	int _id;
	int remain_time;
};


static const string _SJF = "SJF";
static const string _SRTF = "SRTF";
static const string _NP = "NP";
static const string _PP = "PP";
static const int _size = 4;
using namespace std;

// Find all the process arrive the waiting queue at a certain time
// And return them in a vector
vector<myProcess> findByArrivalTime(vector<myProcess> ps, int time)
{
	vector<myProcess> t;
	for (int i = 0; i < ps.size(); i++)
	{
		if (ps[i].arrival_time == time) {
			t.push_back(ps[i]);
		}
	}
	return t;
}

// Find the process with the minimal remaining time in a list of process
int minTime(vector<myProcess> ps)
{
	int min = ps[0].getremain();
	int index = 0;

	for (int i = 0; i < ps.size(); i++)
	{
		if (ps[i].getremain() < min)
		{
			index = i;
		}
	}
	return index;
}

// Find the process with the highest priority in a list of process
int minPriority(vector<myProcess> ps)
{
	int min = ps[0]._priority;
	int index = 0;

	for (int i = 0; i < ps.size(); i++)
	{
		if (ps[i]._priority < min)
		{
			index = i;
		}
	}
	return index;
}

// Return total burst time of the process in a list of process
int totalProcessTime(vector<myProcess> ps)
{
	int sum = 0;
	for (int i = 0; i < ps.size(); i++)
	{
		sum += ps[i].burst_time;
	}
	return sum;
}

// Sort a vector of process by remaining burst time in ascending order
// And return them in a vector
vector<myProcess> sortByTime(vector<myProcess> ps)
{
	vector<myProcess> t;
	int s = ps.size();
	while (!ps.empty())
	{
		int min = minTime(ps);
		myProcess p = ps[min];
		t.push_back(p);
		ps.erase(ps.begin() + min);
	}
	return t;
}

// Sort a vector of process by priority in descending order
// And return them in a vector
vector<myProcess> sortByPriority(vector<myProcess> ps)
{
	vector<myProcess> t;
	int s = ps.size();
	while (!ps.empty())
	{
		int min = minPriority(ps);
		myProcess p = ps[min];
		t.push_back(p);
		ps.erase(ps.begin() + min);
	}
	return t;
}

// Simulate in Shortest Job First Algorithm

void displaySJF(vector<myProcess> ps)
{
	int time = 0;
	int totalTime = totalProcessTime(ps);
	int total = ps.size();
	int running = 0;
	int idle = 0;
	int TotalIdleTime = 0;
	int idleTime = 0;
	int max = 0;
	int waiting_time = 0;
	vector<myProcess> done;
	vector<myProcess> ready;
	myProcess currentProcess = ps[0];
	while (done.size() < total) {
		vector<myProcess> arrival = findByArrivalTime(ps, time);
		for (int i = 0; i < arrival.size(); i++)
		{
			ready.push_back(arrival[i]);
		}
		ready = sortByTime(ready);
		if (running == 1) {
		

				currentProcess.setRemain(1);
				if (currentProcess.getremain() == 0)
				{
					done.push_back(currentProcess);
					int wt = time - currentProcess.arrival_time - currentProcess.burst_time +1;

					running = 0;
					waiting_time += wt;
					if (max < wt) max = wt;
				}
			time++;
			continue;
		}
		if (ready.empty()) {
			if (idleTime == 0) {
				cout << "Time " << time << " Idle" << endl;
			}
			idle = 1;
			time++;
			idleTime++;
			continue;
		}
		TotalIdleTime += idleTime;
		idleTime = 0;
		running = 1;
		currentProcess = ready[0];
		ready.erase(ready.begin() + 0);
		currentProcess.setRemain(1);
		cout << "Time " << time << " Process " << currentProcess.getId() <<  endl;
		time ++;
	}
	float cpuUtil = (float)(totalTime * 100) / (totalTime + TotalIdleTime);
	cpuUtil = round(cpuUtil);
	cout << "CPU Utilization: " << cpuUtil << "%" << endl;
	float awt = (float)waiting_time / total;
	cout << "Average waiting time: ";
	cout << fixed << setprecision(2) << awt << endl;
	cout << "Worst-cast waiting time: " << max;
}

// Simulate in Shortest Remaining Time First

void displaySRTF(vector<myProcess> ps)
{
	int time = 0;
	int totalTime = totalProcessTime(ps);
	int total = ps.size();
	int running = 0;
	int idle = 0;
	int TotalIdleTime = 0;
	int idleTime = 0;
	int max = 0;
	int waiting_time = 0;
	vector<myProcess> done;
	vector<myProcess> ready;
	myProcess currentProcess = ps[0];
	while (done.size() < total) {
		vector<myProcess> arrival = findByArrivalTime(ps, time);
		for (int i = 0; i < arrival.size(); i++)
		{
			ready.push_back(arrival[i]);
		}
		ready = sortByTime(ready);
		if (running == 1) {
			
				if (!ready.empty() && ready[0].getremain() < currentProcess.getremain()) 
				{
					ready.push_back(currentProcess);
					currentProcess = ready[0];
					cout << "Time " << time << " Process " << currentProcess.getId() << endl;
					ready.erase(ready.begin() + 0);
				}
				currentProcess.setRemain(1);
				if (currentProcess.getremain() == 0)
				{
					done.push_back(currentProcess);
					int wt = time - currentProcess.arrival_time - currentProcess.burst_time + 1;

					running = 0;
					waiting_time += wt;
					if (max < wt) max = wt;
				}
			
			time++;
			continue;
		}
		if (ready.empty()) {
			if (idleTime == 0) {
				cout << "Time " << time << " Idle" << endl;
			}
			time++;
			idleTime++;
			continue;
		}
		TotalIdleTime += idleTime;
		idleTime = 0;
		running = 1;
		currentProcess = ready[0];
		currentProcess.setRemain(1);
		ready.erase(ready.begin() + 0);
		cout << "Time " << time << " Process " << currentProcess.getId() << endl;
		time++;
	}
	float cpuUtil = (float)(totalTime * 100) / (totalTime + TotalIdleTime);
	cpuUtil = round(cpuUtil);
	cout << "CPU Utilization: " << cpuUtil << "%" << endl;
	float awt = (float)waiting_time / total;
	cout << "Average waiting time: ";
	cout << fixed << setprecision(2) << awt << endl;
	cout << "Worst-cast waiting time: " << max;
}

// Simulate in Nonpreemptive Priority Algorithm

void displayNP(vector<myProcess> ps)
{
	int time = 0;
	int totalTime = totalProcessTime(ps);
	int total = ps.size();
	int running = 0;
	int idle = 0;
	int TotalIdleTime = 0;
	int idleTime = 0;
	int max = 0;
	int waiting_time = 0;
	vector<myProcess> done;
	vector<myProcess> ready;
	myProcess currentProcess = ps[0];
	while (done.size() < total) {
		vector<myProcess> arrival = findByArrivalTime(ps, time);
		for (int i = 0; i < arrival.size(); i++)
		{
			ready.push_back(arrival[i]);
		}
		ready = sortByPriority(ready);
		if (running == 1) {
			
			currentProcess.setRemain(1);
			if (currentProcess.getremain() == 0)
			{
				done.push_back(currentProcess);
				int wt = time - currentProcess.arrival_time - currentProcess.burst_time + 1;

				running = 0;
				waiting_time += wt;
				if (max < wt) max = wt;
			}
			time++;
			continue;
		}
		if (ready.empty()) {
			if (idleTime == 0) {
				cout << "Time " << time << " Idle" << endl;
			}
			time++;
			idleTime++;
			continue;
		}
		TotalIdleTime += idleTime;
		idleTime = 0;
		running = 1;
		currentProcess = ready[0];
		currentProcess.setRemain(1);
		ready.erase(ready.begin() + 0);
		cout << "Time " << time << " Process " << currentProcess.getId() << endl;
		time++;
	}
	float cpuUtil = (float)(totalTime * 100) / (totalTime + TotalIdleTime);
	cpuUtil = round(cpuUtil);
	cout << "CPU Utilization: " << cpuUtil << "%" << endl;
	float awt = (float)waiting_time / total;
	cout << "Average waiting time: ";
	cout << fixed << setprecision(2) << awt << endl;
	cout << "Worst-cast waiting time: " << max;
}

// Simulate in Preemptive Priority Algorithm

void displayPP(vector<myProcess> ps)
{
	// TODO


}

// Main function, it will read two argument from command line:
// File name and algorithm name
//
// If the arguments do not meet the requirements, it will 
// terminate and print an error message.
//
// Read the data in the file and store them in a vector
// Run a scheduling algorithm according to the argument

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		cerr << "Invalid argument! " << endl;
		return 1;
	}
	string scheduling = argv[2];
	string filename = argv[1];
	if (scheduling != _SJF && scheduling != _SRTF && scheduling != _NP && scheduling != _PP)
	{
		cerr << "Invalid argument! " << endl;
		return 1;
	}
	ifstream myFile;
	myFile.open(filename);
	if (!myFile.is_open())
	{
		cerr << "Fail to open the file! " << endl;
		return 1;
	}
	
	vector <myProcess> _process;
	string line;
	while (getline(myFile, line)) {
		int data[_size];
		int index = 0;
		stringstream sline(line);
		while (sline >> data[index]) {
			index++;
		}
		myProcess p(data[0], data[1], data[2], data[3]);
		_process.push_back(p);
	}

	
	if (scheduling == _SJF)
	{
		displaySJF(_process);
	}
	if (scheduling == _SRTF)
	{
		displaySRTF(_process);
	}
	if (scheduling == _NP)
	{
		displayNP(_process);
	}
	if (scheduling == _PP)
	{
		displayPP(_process);
	}

    return 0;
}

