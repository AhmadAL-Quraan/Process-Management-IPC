#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
using namespace std;

/**
 * Generates a random float between min and max
 */
float random_float(float min, float max) {
  float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
  return min + r * (max - min);
}
/**
 * Displays first 'count' elements of the vector
 */
void display_vector(const std::vector<float> &vec, int count = 10) {
  // IMPLEMENT THIS FUNCTION
  printf("Vector values (first %d): ", min((int)vec.size(), count));
  for (int i = 0; i < min((int)vec.size(), count); ++i) {
    cout << vec[i] << ' ';
  }
  printf("\n");
}

/**
 * Calculates sum sequentially (for verification)
 */
float sequential_sum(const std::vector<float> &vec) { // IMPLEMENT THIS FUNCTION
  return 1.0;
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  // Parse command line arguments
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <vector_size> <num_processes>"
              << std::endl;
    return 1;
  }

  int N = std::atoi(argv[1]); // Vector size
  int M = std::atoi(argv[2]); // Number of child processes
                              //
  if (N <= 0) {
    cerr << "N must be bigger than zero" << '\n';
    return 1;
  }
  if (M < 1 || M > 100) {
    cerr << "M must be between 1 and 100" << '\n';
    return 1;
  }

  vector<float> numbers;
  for (int i = 0; i < N; ++i) {
    numbers.push_back(random_float(-100, 100));
  }

  display_vector(numbers);

  int chunk_size;
  int remaining;

  // If N < M then give for each M one value to process

  if (N < M) {
    M = N;
  }

  chunk_size = N / M;
  remaining = N % M;

  printf("\nCreating %d child processes\n\n", M);

  int pipes[M][2];
  for (int i = 0; i < M; ++i) {
    pipe(pipes[i]);
  }
  int index_behind = numbers.size() - 1;
  for (int i = 0; i < M; ++i) {
    int pid_t = fork();

    if (pid_t == 0) {

      close(pipes[i][0]);
      float sum = 0;
      int start, end, processed_values = 0;
      start = i * chunk_size + min(i, remaining);
      end = start + (i < remaining ? chunk_size + 1 : chunk_size);

      for (int j = start; j < end; ++j) {
        processed_values++;
        sum += numbers[j];
      }
      // This logic is wrong becuase each process has it's own copy of memory (
      // they can't see each other modification)
      // if (remaining != 0) {
      //   processed_values++;
      //   sum += numbers[index_behind];
      //   remaining -= 1;
      //   index_behind--;
      // }

      write(pipes[i][1], &sum, sizeof(sum));
      printf("Process child number %d, It's PID= %d has partial sum %f\n and "
             "range "
             "of values indices [%d-%d], so total %d values.\n",
             i + 1, getpid(), sum, start, end, processed_values);
      close(pipes[i][1]);
      return 0;
    }
  }

  float total_sum = 0;
  for (int i = 0; i < M; ++i) {
    close(pipes[i][1]);

    float local_value;
    // If the pipe is empty (no data to read), and at least one write is still
    // open, then read() will block and wait.
    // If all write ends are closed, read() returns 0 (EOF).
    read(pipes[i][0], &local_value, sizeof(local_value));
    close(pipes[i][0]);
    total_sum += local_value;
    // This waits for process termination not data like read().
    // This will wait for all of them (processes)

    wait(NULL);
  }
  printf("\n\nParallel summation = %.2f\n", total_sum);
  float seq_sum = 0;
  for (int i = 0; i < (int)numbers.size(); ++i) {
    seq_sum += numbers[i];
  }
  printf("Sequential sum (no child processes, only parent) = %.2f", seq_sum);

  return 0;
}
