#include <iostream>
#include <algorithm>

using namespace std;

// Lomuto scheme
int partition(int array[], int left, int right) {
    int x = array[right];
    int x_index = left;

    for (int i = left; i < right; i++) {
        if (array[i] <= x) {
            swap(array[i], array[x_index]);
            x_index++;
        }
    }

    swap(array[x_index], array[right]);

    return x_index;
}


void quicksort(int array[], int left, int right) {
    if (left >= right) {
        return;
    }

    int x_ind = partition(array, left, right);

    quicksort(array, left, x_ind - 1);
    quicksort(array, x_ind + 1, right);

    return;
}


int main() {
    int array[] = {9, -3, 5, 2, 6, 8, -6, 1, 3};
    int n = sizeof(array);

    quicksort(array, 0, n - 1);

    for (int i = 0; i < n; i++) {
        cout << array[i] << " ";
    }

    return 0;
}
