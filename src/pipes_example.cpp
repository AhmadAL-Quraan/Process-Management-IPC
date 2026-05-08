#include <cstring>
#include <iostream>
#include <unistd.h>

int main() {
  int fd[2];
  pipe(fd);

  pid_t pid = fork();

  if (pid == 0) {
    // 🔹 Child (reader)
    close(fd[1]); // close write end

    char buffer[100];
    read(fd[0], buffer, sizeof(buffer));

    std::cout << "Child received: " << buffer << "\n";

    close(fd[0]);
  } else {
    // 🔹 Parent (writer)
    close(fd[0]); // close read end

    const char *msg = "Hello from parent!";
    write(fd[1], msg, strlen(msg) + 1);

    close(fd[1]);
  }

  return 0;
}
