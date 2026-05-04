#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int n = 1;
  pid_t pid = fork();
  pid_t pid2 = fork();

  if (pid == 0 || pid2 == 0) {
    std::cout << "Child running\n";
    sleep(2); // simulate work
    std::cout << "Child done\n";
  } else {
    std::cout << "Parent waiting...\n";
    while (wait(NULL) > 0)
      ; // wait for child to finish
    std::cout << "Parent resumes after child\n";
  }

  return 0;
}
