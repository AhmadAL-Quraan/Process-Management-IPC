#include <iostream>
#include <sys/types.h>
#include <unistd.h> // fork()
using namespace std;

int main() {
  cout << "Before execution" << '\n';
  pid_t pid = fork(); // create a new process
  cout << "After execution" << '\n';

  if (pid == 0) {
    cout << "I'm a child" << '\n';
  } else {
    cout << "I'm a parent" << '\n';
  }
  return 0;
}
