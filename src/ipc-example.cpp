#include <iostream>
#include <sys/wait.h>
#include <vector>

int main() {
  // Configuration
  int N = 15; // Size of vector (divisible by 3 for simplicity)

  // Step 1: Parent initializes vector with random values
  std::vector<float> numbers(N);
  srand(time(NULL));

  std::cout << "Vector values: ";
  for (int i = 0; i < N; i++) {
    numbers[i] = (rand() % 100) / 10.0; // Random values 0.0 to 9.9
    std::cout << numbers[i] << " ";
  }
  std::cout << std::endl << std::endl;

  // Step 2: Create 3 pipes for 3 children to send results back
  int pipes[3][2];
  /*
   child 1 -> read, write
   child 2 -> read, write
   child 3 -> read, write

   * */
  for (int i = 0; i < 3; i++) {
    /*
      Create an unidirectional communication channel (a pipe)
      and give me two file descriptors to use it
    */
    pipe(pipes[i]);
  }

  // Step 3: Create 3 child processes
  int chunk_size = N / 3; // Each child gets 5 elements

  for (int i = 0; i < 3; i++) {
    pid_t pid = fork();

    if (pid == 0) {
      // Child process
      close(pipes[i][0]); // Close read end

      // Calculate start and end indices for this child
      int start = i * chunk_size;
      int end = (i + 1) * chunk_size;

      // Sum this chunk
      float sum = 0;
      for (int j = start; j < end; j++) {
        sum += numbers[j];
      }

      // Send sum to parent through pipe
      write(pipes[i][1], &sum, sizeof(float));
      close(pipes[i][1]);

      return 0; // Child exits
    }
  }

  // Step 4: Parent collects sums from all 3 children
  // Only parent will reach here
  float total = 0;
  for (int i = 0; i < 3; i++) {
    close(pipes[i][1]); // Close write end

    float child_sum;
    read(pipes[i][0], &child_sum, sizeof(float)); // blocking call
    close(pipes[i][0]);

    total += child_sum;

    // Wait for child to finish
    wait(NULL);

    std::cout << "Child " << i << " sum = " << child_sum << std::endl;
  }

  // Step 5: Display final result
  std::cout << "\nTotal sum = " << total << std::endl;

  return 0;
}
