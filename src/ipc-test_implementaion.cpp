#include <iostream>
#include <numeric>
#include <sys/wait.h>
#include <vector>
using namespace std;

int main() {
  int N = 20;
  vector<int> arr(N);
  srand(time(NULL));
  for (int i = 0; i < N; ++i) {
    arr[i] = (rand() % 100); // 0- 99
    printf("%d ", arr[i]);
  }
  printf("\n");
  printf("Total numbers before running = %d\n",
         accumulate(arr.begin(), arr.end(), 0));

  int children = N / 5;
  int pipes[children][2];

  for (int i = 0; i < children; ++i) {
    pipe(pipes[i]);
  }

  for (int i = 0; i < children; ++i) {

    int pid = fork();

    if (pid == 0) {
      close(pipes[i][0]);

      int sum = 0;
      int start = i * 5;
      int end = (i + 1) * 5;
      for (int j = start; j < end; ++j) {
        sum += arr[j];
      }

      write(pipes[i][1], &sum, sizeof(sum));
      close(pipes[i][1]);
      return 0;
    }
  }

  int total_sum = 0;
  for (int i = 0; i < children; ++i) {
    close(pipes[i][1]);
    int current_sum = 0;
    read(pipes[i][0], &current_sum, sizeof(current_sum));
    close(pipes[i][0]);
    total_sum += current_sum;
    int status;
    wait(&status);
    std::cout << "Child " << i << " It's status " << status
              << " sum = " << current_sum << std::endl;
  }

  printf("Total summation from all children = %d", total_sum);
}
