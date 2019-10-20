#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <iostream>
#include <queue>
#include <set>
#include <vector>
#define min(x, y) ((x) >= (y) ? (y) : (x))
using namespace std;
using namespace __gnu_pbds;

typedef tree<long long, null_type, less_equal<long long>, rb_tree_tag,
             tree_order_statistics_node_update>
    map_t;

deque<long long> runningValues(0);  // use deque here
map_t runningValuesSet;
const int WINDOW_SIZE = 100;
const int SUB_WINDOW_SIZE = WINDOW_SIZE * 0.2;
const int THRESHOLD_PERCENT = 30;
long long runningSum = 0;
bool calibrated = false;
deque<bool> wasAnomalyMeasured;

long double averageWindow() {
    return runningSum / (long double)WINDOW_SIZE;
}

// needs to called at every time instant
bool isAnomaly(long long value) {
    long long avg = (long long)averageWindow(),
              maxLimit = (long long)((1 + THRESHOLD_PERCENT / 100.0) * avg),
              minLimit = (long long)((1 - THRESHOLD_PERCENT / 100.0) * avg);

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

    if (calibrated)
        wasAnomalyMeasured.push_back(isAnomaly(value));
    else
        wasAnomalyMeasured.push_back(false);
}
long long counter = 0;

void sendResponse() {
    int anomalousCount = 0,
        lim = min((int)wasAnomalyMeasured.size(), SUB_WINDOW_SIZE);

    for (int i = 0; i < lim; i++) {
        anomalousCount += wasAnomalyMeasured[i];
    }

    int percentage = anomalousCount * 100 / (long double)lim;

    if (percentage >= THRESHOLD_PERCENT)
        printf("1");
    else {
        printf("0");
    }
    fflush(stdout);
}

int main() {
    long long val;

    while (scanf("%lld ", &val) != EOF) {
        if(val == -1)
            printf("\n");

        takeInput(val);
        sendResponse();

        counter++;
        if (counter > WINDOW_SIZE)
            calibrated = true;
    }
}
