#include <cstring>
#include <iostream>
#include <unistd.h>

int main() {
  int pipes[2];
  pipe(pipes);

  pid_t pid = fork();

  if (pid == 0) {
    // 🔹 Child (reader)
    close(pipes[1]); // close write end

    char buffer[100];
    read(pipes[0], buffer, sizeof(buffer));

    std::cout << "Child received: " << buffer << "\n";

    close(pipes[0]);
  } else {
    // 🔹 Parent (writer)
    close(pipes[0]); // close read end

    const char *msg = "Hello from parent!";
    write(pipes[1], msg, strlen(msg) + 1);

    close(pipes[1]);
  }

  return 0;
}
