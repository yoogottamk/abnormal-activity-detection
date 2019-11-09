#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <iostream>
#include <queue>
#include <set>
#include <vector>
#include <fstream>
#include <string>

int min(int x, int y) {
    if (x <= y)
        return x;
    return y;
}

using namespace std;

deque<int> runningValues(0);
const int SECONDS = 1;
const int SAMPLES_PER_SEC = 400;
const int WINDOW_SIZE = 100; // SECONDS * SAMPLES_PER_SEC;
const int THRESHOLD_PERCENT = 24;
const int THRESHOLD_ABS = 100;
int runningSum = 0;
bool calibrated = false;
deque<bool> wasAnomalyMeasured;
int anomalousCount = 0;
double averageWindow = 0;

// needs to called at every time instant
bool isAnomaly(int value) {
    double maxLimit = THRESHOLD_ABS + averageWindow,
           minLimit = -THRESHOLD_ABS + averageWindow;

    return (value >= maxLimit || value <= minLimit);
}

void takeInput(int value) {
    while ((int)(runningValues.size()) >= WINDOW_SIZE) {
        runningSum -= runningValues.front();
        anomalousCount -= wasAnomalyMeasured.front();
        wasAnomalyMeasured.pop_front();
        runningValues.pop_front();
    }

    runningSum += value;
    runningValues.push_back(value);
    averageWindow = runningSum / (double)WINDOW_SIZE;

    if (calibrated) {
        bool x = isAnomaly(value);
        wasAnomalyMeasured.push_back(x);
    } else
        wasAnomalyMeasured.push_back(false);

    anomalousCount += wasAnomalyMeasured.back();
}

int counter = 0;

void sendResponse() {
    int lim = min((int)wasAnomalyMeasured.size(), WINDOW_SIZE);

    double percentage = anomalousCount * 100 / (double)lim;

    cout << (percentage >= THRESHOLD_PERCENT ? 1 : 0);
}

int main() {
    int val;
    string fileName = "data";

    ifstream inp;
    inp.open(fileName);

    if (inp) {
        while (inp >> val) {
            if (val == -1)
                break;

            runningValues.push_back(val);
            runningSum += val;
            wasAnomalyMeasured.push_back(0);
        }

        while (runningValues.size() >= WINDOW_SIZE) {
            runningSum -= runningValues[0];
            wasAnomalyMeasured.pop_front();
            runningValues.pop_front();
        }

        inp.close();
    }

    while(true){
        string s;
        cin >> s;

        if(s.size() % 3 != 0) {
            cerr << -3;
            return 1;
        }

        for(int i = 0, len = s.size(); i < len; i+=3){
            int val = (s[i]-'0')*100 + (s[i+1]-'0')*10 + (s[i+2]-'0');
	    val *= 10;
	    takeInput(val);
            sendResponse();

            counter++;
            if (counter > WINDOW_SIZE)
                calibrated = true;
        }

        int val;
        cin >> val;

        ofstream oft;
        oft.open(fileName, ios::trunc);

        if (val == -1) {
            cout << endl;
            fflush(stdout);

            for (auto d : runningValues) {
                oft << d << " ";
            }

            oft << -1 << endl;
            exit(0);
        }else{
            oft << -2 << endl;
        }

    }
}
