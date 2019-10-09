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
deque<bool> wasAnomalyMeasured;

long double averageWindow() {
    return running_sum / (long double)WINDOW_SIZE;
}

void takeInput(long long value, long long counter) {
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
    if (counter >= WINDOW_SIZE)
        wasAnomalyMeasured.push_back(isAnomaly(value));
    else
        wasAnomalyMeasured.push_back(false);
}

long double getPercentageOutside() {
    long long avg = (long long)averageWindow();
    long long r1 = running_values_set.order_of_key(avg + THRESHOLD),
              r2 = running_values_set.order_of_key(avg - THRESHOLD),
              elmsInMiddle = r1 - r2,
              elementsOutside = WINDOW_SIZE - elmsInMiddle;

    return elementsOutside / (long double)WINDOW_SIZE;
}

// needs to called at every time instant
bool isAnomaly(long long value) {
    long long avg = (long long)averageWindow(),
              maxLimit = (long long)((1 + THRESHOLD / 100.0) * avg),
              minLimit = (long long)((1 - THRESHOLD / 100.0) * avg);

    return (value >= maxLimit || value <= minLimit);
}

void sendResponse() {
    int anomalous = 0;
    for (int i = 0; i < SUB_WINDOW_SIZE; i++) {
        anomalous += wasAnomalyMeasured[i];
    }

    int percentage = anomalous / (long double)SUB_WINDOW_SIZE;

    if (percentage >= THRESHOLD)
        printf("1");
    else
        printf("0");
}

int main() {
    long long val, total;
    int counter = 0;
    scanf("%lld", &total);
    while (total--) {
        scanf("%lld", &val);
        takeInput(val, counter);
        sendResponse();
        counter++;
    }
}
