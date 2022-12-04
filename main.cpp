#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <ctime>
#include <chrono>

using namespace std;

// Lomuto scheme
int partition(vector<int> &array, int left, int right) {
    int x = array[right];
    int x_index = left;
    int tmp;

    for (int i = left; i < right; i++) {
        if (array[i] <= x) {
            tmp = array[i];
            array[i] = array[x_index];
            array[x_index] = tmp;
            x_index++;
        }
    }

    tmp = array[x_index];
    array[x_index] = array[right];
    array[right] = tmp;

    return x_index;
}


void quicksort(vector<int> &array, int left, int right) {
    if (left >= right) {
        return;
    }

    int x_ind = partition(array, left, right);

    quicksort(array, left, x_ind - 1);
    quicksort(array, x_ind + 1, right);
}


int main() {
    float average_time = 0;
    for (int j = 0; j < 5; j++) {
        random_device rd;
        mt19937 gen(rd());

        int n = 1e8;
        uniform_int_distribution<> dist(1, n);
        vector<int> array;
        for (int i = 0; i < n; i++) {
            array.push_back(dist(gen));
        }

        time_t now = time(nullptr);
        tm *start_time = localtime(&now);
        int start_seconds_time = start_time->tm_hour * 3600 + start_time->tm_min * 60 + start_time->tm_sec;
        cout << "Start Time: " << start_time->tm_hour << ":" << start_time->tm_min << ":" << start_time->tm_sec << endl;

        quicksort(array, 0, n - 1);

        now = time(nullptr);
        tm *end_time = localtime(&now);
        cout << "End Time: " << end_time->tm_hour << ":" << end_time->tm_min << ":" << end_time->tm_sec << endl;

        int end_second_time = end_time->tm_hour * 3600 + end_time->tm_min * 60 + end_time->tm_sec;
        int work_time = end_second_time - start_seconds_time;
        cout << work_time << endl;

        average_time += float(work_time);
    }

    cout << "Average seq time: " << average_time / 5 << endl;

    return 0;
}
