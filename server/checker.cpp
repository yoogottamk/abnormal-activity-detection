#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <iostream>
#include <queue>
#include <set>
#include <vector>
#include <fstream>
int min(int x, int y) {
    if (x <= y)
        return x;
    return y;
}
using namespace std;
using namespace __gnu_pbds;

typedef tree<long long, null_type, less_equal<long long>, rb_tree_tag,
             tree_order_statistics_node_update>
    map_t;

deque<long long> runningValues(0);
map_t runningValuesSet;
const int WINDOW_SIZE = 100;
const int SUB_WINDOW_SIZE = WINDOW_SIZE * 0.2;
const int THRESHOLD_PERCENT = 30;
const int THRESHOLD_ABS = 100;
long long runningSum = 0;
bool calibrated = false;
deque<bool> wasAnomalyMeasured;

long double averageWindow() {
    return runningSum / (long double)WINDOW_SIZE;
}

// needs to called at every time instant
bool isAnomaly(long long value) {
    long double avg = averageWindow(), maxLimit = THRESHOLD_ABS + avg,
                minLimit = -THRESHOLD_ABS + avg;

    return (value >= maxLimit || value <= minLimit);
}

void takeInput(long long value) {
    while ((int)(runningValues.size()) >= WINDOW_SIZE) {
        long long last = runningValues[0];
        runningSum -= last;
        wasAnomalyMeasured.pop_front();
        runningValues.pop_front();
        runningValuesSet.erase(runningValuesSet.upper_bound(last));
    }

    runningSum += value;
    runningValues.push_back(value);
    runningValuesSet.insert(value);

    if (calibrated) {
        bool x = isAnomaly(value);
        wasAnomalyMeasured.push_back(x);
    } else
        wasAnomalyMeasured.push_back(false);
}
long long counter = 0;

void sendResponse() {
    int anomalousCount = 0,
        lim = min((int)wasAnomalyMeasured.size(), WINDOW_SIZE);

    for (int i = 0; i < lim; i++) {
        anomalousCount += wasAnomalyMeasured[i];
    }

    long double percentage = anomalousCount * 100 / (long double)lim;

    cout << (percentage >= THRESHOLD_PERCENT ? 1 : 0);
}

int main() {
    long long val;
    string fileName = "data";

    ifstream inp;
    inp.open(fileName);

    if (inp) {
        while (inp >> val) {
            if (val == -1)
                break;

            runningValues.push_back(val);
            wasAnomalyMeasured.push_back(0);
        }

        while (runningValues.size() >= WINDOW_SIZE) {
            wasAnomalyMeasured.pop_front();
            runningValues.pop_front();
        }

        inp.close();
    }

    while (cin >> val) {
        if (val == -1) {
            cout << endl;
            fflush(stdout);

            ofstream oft;
            oft.open(fileName, ios::trunc);

            for (auto d : runningValues) {
                oft << d << " ";
            }

            oft << -1 << endl;
            exit(0);
        }

        takeInput(val);
        sendResponse();

        counter++;
        if (counter > WINDOW_SIZE)
            calibrated = true;
    }
}
