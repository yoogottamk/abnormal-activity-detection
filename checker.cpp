#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

using namespace std;
using namespace __gnu_pbds;

typedef tree<long long, null_type, less_equal<long long>, rb_tree_tag,
             tree_order_statistics_node_update>
    map_t;

deque<long long> running_values(0);  // use deque here
map_t running_values_set;
const int WINDOW_SIZE = 100;
const int SUB_WINDOW_SIZE = WINDOW_SIZE * 0.2;
const int THRESHOLD = 75;
long long running_sum = 0;
bool calibrated = false;
deque<bool> wasAnomalyMeasured;

long double averageWindow() {
    return running_sum / (long double)WINDOW_SIZE;
}

// needs to called at every time instant
bool isAnomaly(long long value) {
    long long avg = (long long)averageWindow(),
              maxLimit = (long long)((1 + THRESHOLD / 100.0) * avg),
              minLimit = (long long)((1 - THRESHOLD / 100.0) * avg);

    return (value >= maxLimit || value <= minLimit);
}

void takeInput(long long value) {
    if ((int)(running_values.size()) == WINDOW_SIZE) {
        long long last = running_values[0];
        running_sum -= last;
        wasAnomalyMeasured.pop_front();
        running_values.pop_front();
        running_values_set.erase(running_values_set.upper_bound(last));
    }

    running_sum += value;
    running_values.push_back(value);
    running_values_set.insert(value);
    if (calibrated)
        wasAnomalyMeasured.push_back(isAnomaly(value));
    else
        wasAnomalyMeasured.push_back(false);
}

void sendResponse() {
    int anomalousCount = 0;
    for (int i = 0; i < SUB_WINDOW_SIZE; i++) {
        anomalousCount += wasAnomalyMeasured[i];
    }

    int percentage = anomalousCount / (long double)SUB_WINDOW_SIZE;

    if (percentage >= THRESHOLD)
        printf("1\n");
    else
        printf("0\n");
    fflush(stdout);
}

int main() {
    long long val;
    long long counter = 0;

    while (scanf("%lld", &val)) {
        takeInput(val);
        sendResponse();

        if (!calibrated && counter < WINDOW_SIZE)
            counter++;
        else
            calibrated = true;
    }
}
