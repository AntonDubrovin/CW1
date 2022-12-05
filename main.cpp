#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <ctime>
#include <parlay/parallel.h>
#include <parlay/primitives.h>
#include <parlay/sequence.h>

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


void parallel_quicksort(vector<int> &array, int left, int right) {
    if (left >= right) {
        return;
    }

    if (right - left < 1000) {
        quicksort(array, left, right);
        return;
    }

    int x_ind = partition(array, left, right);

    parlay::par_do([&] {parallel_quicksort(array, left, x_ind - 1);},
                   [&] {parallel_quicksort(array, x_ind + 1, right); });
}


int main() {
    float average_seq_time = 0;
    float average_par_time = 0;
    for (int j = 0; j < 5; j++) {
        random_device rd;
        mt19937 gen(rd());

        int n = 1e8;
        uniform_int_distribution<> dist(1, n);
        vector<int> seq_array;
        vector<int> par_array;
        for (int i = 0; i < n; i++) {
            int el = dist(gen);
            seq_array.push_back(el);
            par_array.push_back(el);
        }

        // seq algo
        time_t now = time(nullptr);
        tm *start_seq_time = localtime(&now);
        int start_seq_seconds_time =
                start_seq_time->tm_hour * 3600 + start_seq_time->tm_min * 60 + start_seq_time->tm_sec;
        cout << "Start " << j << "Seq Time: " << start_seq_time->tm_hour << ":" << start_seq_time->tm_min << ":"
             << start_seq_time->tm_sec << endl;

        quicksort(seq_array, 0, n - 1);

        now = time(nullptr);
        tm *end_seq_time = localtime(&now);
        cout << "End " << j << "Seq Time: " << end_seq_time->tm_hour << ":" << end_seq_time->tm_min << ":"
             << end_seq_time->tm_sec
             << endl;

        int end_seq_second_time = end_seq_time->tm_hour * 3600 + end_seq_time->tm_min * 60 + end_seq_time->tm_sec;
        int work_seq_time = end_seq_second_time - start_seq_seconds_time;
        cout << "Work " << j << "Seq Time: " << work_seq_time << endl;

        average_seq_time += float(work_seq_time);
        // seq algo
        // par algo
        now = time(nullptr);
        tm *start_par_time = localtime(&now);
        int start_par_seconds_time =
                start_par_time->tm_hour * 3600 + start_par_time->tm_min * 60 + start_par_time->tm_sec;
        cout << "Start " << j << "Par Time: " << start_par_time->tm_hour << ":" << start_par_time->tm_min << ":"
             << start_par_time->tm_sec << endl;

        parallel_quicksort(par_array, 0, n - 1);

        now = time(nullptr);
        tm *end_par_time = localtime(&now);
        cout << "End " << j << "Par Time: " << end_par_time->tm_hour << ":" << end_par_time->tm_min << ":"
             << end_par_time->tm_sec
             << endl;

        int end_pars_second_time = end_par_time->tm_hour * 3600 + end_par_time->tm_min * 60 + end_par_time->tm_sec;
        int work_par_time = end_pars_second_time - start_par_seconds_time;
        cout << "Work " << j << " Par Time: " << work_par_time << endl;

        average_par_time += float(work_par_time);
        //par algo

    }

    cout << "Average seq time: " << average_seq_time / 5 << endl;
    cout << "Average par time: " << average_par_time / 5 << endl;

    return 0;
}
