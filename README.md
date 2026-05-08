# IPC - Inter-Process Communication Using fork() and Pipes

# Table of Contents

- [Problem](#problem)
- [Key OS Concepts Covered](#key-os-concepts-covered)
- [Concepts](#concepts)
  - [Process vs Thread](#process-vs-thread)
    - [Process](#process)
    - [Threads](#threads)
  - [fork, wait and pipes](#fork-wait-and-pipes)
    - [What is a process table](#what-is-a-process-table)
    - [fork](#fork)
    - [Wait](#wait)
    - [The Challenge: Communication Between Processes (pipes)](#the-challenge-communication-between-processes-pipes)
      - [Questions about pipes](#questions-about-pipes)
        - [In pipes Why can't we do buffer[3] or more?](#in-pipes-why-cant-we-do-buffer3-or-more)
        - [How does buffer[1] used for write and buffer[0] for read](#how-does-buffer1-used-for-write-and-buffer0-for-read)
        - [How does pipes fits with fork()](#how-does-pipes-fits-with-fork)
        - [Why do we need to close unused pipe ends in both parent and child processes? What would happen if we didn't close them](#why-do-we-need-to-close-unused-pipe-ends-in-both-parent-and-child-processes-what-would-happen-if-we-didnt-close-them)
        - [How many possible cases of read() in pipes:](#how-many-possible-cases-of-read-in-pipes)
        - [If read() do blocking, why I need to use wait () ?](#if-read-do-blocking-why-i-need-to-use-wait--)
- [Task Questions](#task-questions)


# Problem 

In this problem, you will explore process creation, inter-process communication (IPC), and parallel algorithm design by implementing a parallel sum calculator. The problem is deceptively simple: given a large array (vector) of floating-point numbers, compute the total sum. However, instead of computing the sum sequentially in a single loop, you will divide the work among multiple child processes, each summing a portion of the array in parallel.

     This problem touches on core operating system concepts including:
        ◦ Process creation and management (fork())
        ◦ Process synchronization (wait())
        ◦ Inter-process communication (pipes)
        ◦ Resource management (file descriptors)
        ◦ Process termination and cleanup

# Key OS Concepts Covered
- fork() → Process creation
- wait() → Process synchronization
- Pipes → IPC mechanism
- File descriptors → Resource handling
- Process lifecycle → creation → execution → cleanup

# Concepts

## Process vs Thread
### Process

- Has its **own memory space** (address space): **Address space** is basically the **range of memory addresses a program (process) is allowed to use**.
- 👉 It’s like each process thinks it owns the whole memory, even though the OS is managing everything behind the scenes.
- Own resources (files, variables, heap, etc.)
- Completely **isolated** from other processes
- Communication between processes is **slower** (uses IPC like pipes, sockets)

👉 Example: Running Chrome and Spotify — each is a separate process.



### Threads

A **thread** is a smaller unit of execution **inside a process**.

![https://images.openai.com/static-rsc-4/lxcbI7GJWhD4aJcdU_sXrzHVogOoKSzGEZBsNWBibZTS0wUPxZTHeg11TnvzYaZgYIy1mRQ1ihyvFGhL6ChDlaT9nT0XGua45r7KQqhV6_AjZJUtcQWJwF1Dje93BNHuFJP73OZG9JULXXJEWm5LeRu7yC2u48zYB_qrFBwuV8kvY3Uv75LmyAT3mSJKYeT6?purpose=fullsize](https://images.openai.com/static-rsc-4/FvDyEBK3xkFv_KP2OxN9XcbcAGNc-Yo9N6bVSunuYtEyjquFVbWHCY7Y2OYQjfBhSXzXWikKsnCEKEP4ftAPSTBHGd9Zp6tdkp2lCjQRbAVktug5l4JhXDaJmF88GYTPYrS16y0SetVnpXlUQEE-sfJKSHn9EOMROMBW3vDecKE?purpose=inline)

![https://images.openai.com/static-rsc-4/EPbydftk3Hx3KsFg8s-kqAPwKhqEiNgCOmWDs0li9d435kvWC0Nladx5J51AoSadVp29bZrFgbvDuzvljc3KRoCbm65YRc3ji38z_HLUpDp4w4-NR1eJvHVMsWiqGKhwimWqGWpJJwFMGy-oLhD-AhWKd1n1VcHB_YLDNPlD7sJwTRiOMoTgo_fKgblUrfkJ?purpose=fullsize](https://images.openai.com/static-rsc-4/XT8g-_y_IT1l8jUj5ina46up9JTPi6U8F7U9oRl_HD9kBdOychmKS_jFUwnLLJPhE9wDx_ye-zylUimHZO-yZ5p5mca9_rJc2HWvxStbhPVATRC5wqX0A0g8RmTQfGu0IAlhQ9iWXnC1S6vCtVE3736js9YX9IhHtax0h0b0ADA?purpose=inline)

![https://images.openai.com/static-rsc-4/TpvSaQ4qodhcLjNPlbMQagDFODYmzU2mq3YlF0KeLQJotzttVM4PN1ue5rhCgPGi_iwl3GBHNRyrQQR7aNLuL1yYAxErKD1XE3FoYCOPdOfy1Pdso8UyEF4azWVPHd5TNMP9eHiG6kcS3E1RsaAykCQTgz1tWnSfHlfLdZIFkLcru5G_O0z_ZzwLdBTpr60m?purpose=fullsize](https://images.openai.com/static-rsc-4/OOsyH4BFFOicyPorck0Y6W_nFC6Ci0USEMnq2eG31wcjfeiqzv4EjmNQ-CnOHFze1h8TOE65cElVX0j_ur7HC-F6OF4VZT48nr2uGGJ8iwBOdfY-_rBt3ok0N2Fu2fPPIY0_gBjg_J1SmCIOjfCD1dpNqYeVS_Q0g7ds-PDbXQg?purpose=inline)

![https://images.openai.com/static-rsc-4/AfPWlO0iOr3BAFPpHBn37B8M3blal1OgZleymyTMMpA3VXPp0vPXaK46DJpLxV9V3tq5pItZ8M2909lkZ1IuYkkcQMTOHX-2fgc8Q_xAFT5i79NhYT-5NDHKrIPutaH9SvDiPva405SvxKnPSvJZ69IL4nnCiag26DZ9eeyWJlatAxYZIGm4sj57jD4WdJhz?purpose=fullsize](https://images.openai.com/static-rsc-4/h0m8YZC2k7TJ_xwo_TwRN5O9ukzXFLGmvrKLBe_j3VrJ4GyuIf29tpnrAPpN2Ijz8A60kffFhAxK1l7yaR6xQsSWIz579Z-0LcLylSaMe45KfoYjnHqP75M8RCeY10PkcZwdlEiU7DaxvSPmReEyRzWjXCtPNGvMOKpT6eSoJW0?purpose=inline)

![https://images.openai.com/static-rsc-4/T2X8saOrAaaAmW8nD6jE_inUMTxnVesHqVJjHMipek4kpBqMhHWBqtV-A3TfvfBSy5g49l3hG16vYWwz2HguFwpWqNhkW1aKaVgEFrEJrUBMpY7ODPoWeKMnlWwZ2fGV9BilenCfI89ogLjFNAXax9P3zvnT57kGQmRQStIcyfmpHErw0n_pnUAp3Dt6eZws?purpose=fullsize](https://images.openai.com/static-rsc-4/sPPlZmdyp-5PrRDibSsZxhCD58M7ZShJt5PoZFWNUcg1UjYtOKjVowtqqo89YQXOxJrrjyr3uaInPa0dmo-C89jg99gpFhO9_YqAfXbT2x-u66RN22NtzW4yHLLuYJvTdlMidIlp3ONowSJPOeKOvq7VWKvKZ5NDZhq2jkxF7Go?purpose=inline)

![https://images.openai.com/static-rsc-4/PfLwUmzs6vl5fw2Y3-__aYgBraXyVgRzi5VWDJ9Bc0uHXshJI444O_Crtz-pMtDUJl3_0cNJwI22vqgZYskLG9uuDhVwx-WTdoGkt66x4i9Vw3NhncyMlyYgEg2UJFJ1_iFYUv-w8c2diKhgwvRUluFyegSLcSjhqTPr7IDz3L9eQyltulT9-DG4oTlEc6N-?purpose=fullsize](https://images.openai.com/static-rsc-4/kkBpGtYk9P8Hiag3U_K2LoNwpKXgzt9LLe2GbV2qOVaGgvT8Lxq051q3YxFEXiotB5Ry1DuoUbe85ZYOlrIh6eLli5i2Z_rG-iYx9HQVDxrdUseqqrHNFW3UlINMYWvntdYcn-53_PdFjM_f-9HQYfN-Z_lMr27hOxAk5_kxPJw?purpose=inline)

- Shares the **same memory** with other threads in the same process
- Has its own stack and registers
- **Lightweight** compared to processes
- Communication is **fast** (shared memory)

👉 Example: A browser tab loadingmages, scripts,  iand UI simultaneously — using threads.

So:

| Feature       | Process 🧠                 | Thread 🧵                       |
| ------------- | -------------------------- | ------------------------------- |
| Memory        | Separate memory            | Shared memory                   |
| Isolation     | Strong (safe)              | Weak (can affect each other)    |
| Communication | Slow (IPC)                 | Fast (shared data)              |
| Creation cost | Expensive                  | Cheap                           |
| Crash impact  | One process crash ≠ others | One thread crash can affect all |


## fork, wait and pipes  

### What is a process table  ?

The kernel keeps a big data structure (conceptually a table):

![process table](pic/process_table_ex.png)

Each entry = one process

🔹 What’s stored per process?

Each process has a structure (in Linux it's like task_struct) that includes:
```
PID → Process ID (unique)

PPID → Parent Process ID

State → running / sleeping / zombie

Registers / CPU state

Memory info

Open files

List of children
```

![mental mode](pic/fork-wait-sleep-C++.png)

## fork
* It's a system call (function), that creates a new process.
```C++
pid_t pid = fork();
```

→ The OS creates a child process.

→ It will executes after the fork() line, how ? the new process will copy the parent process memory space (stack, heap, Global/ static var, fd), by using a COW method (copy on write) .

* **Initially**: 
Parent and child share the same physical memory pages.
* **Only when one modifies memory**: The kernel creates a separate copy for that process.

So:

- The original process → called the parent

- The newly created process → called the child

>It's not guaranteed who will run first.


> Number of processes = **2 ^ n**, n = number of fork() in this code.

> Not checking fork() return value → Unpredictable behavior

## Wait 
wait() makes a parent process block until one of its child processes terminates, and then collects its exit status.

In other words suspends a parent process until a child finishes.

* It's useful because: 
  1) Synchronization:
        Parent waits for children to finish before continuing.

  2) Cleanup: (VERY important)
Removes zombie processes
Frees kernel resources
```C++

int main() {
  pid_t pid = fork();

  if (pid == 0) {
    std::cout << "Child running\n";
    sleep(2); // simulate work
    std::cout << "Child done\n";
  } else {
    std::cout << "Parent waiting...\n";
    wait(NULL); // wait for child to finish
    std::cout << "Parent resumes after child\n";
  }

  return 0;
}

```

What really happens (conceptually)

* When a process calls:

  ` wait(NULL);`

* The kernel already knows for each process its children (using the process table)
Checks:

  * Do I have any children?

    - ❌ No → return -1

    - ✅ Yes:

        If one already exited → return immediately


* You can make it wait for all children
```C++
while (wait(NULL) > 0);
```

> Not waiting for children → Zombie processes consume system resources

#### ⚠️ What happens if parent does NOT call wait()?

❌ 1. Zombie processes appear

* Child finishes → becomes zombie:

* Child: finished ❌ (still in process table)

If many children:

* You get many zombies
System resources get wasted


## The Challenge: Communication Between Processes (pipes)
How do child processes send their back to the parent?

Unlike threads, processes do not share memory space. Each process has its own private address space, so a child cannot simply write to a variable in the parent.

-> This is where **pipes** come in. A pipe is a unidirectional communication channel (one way only), that allows data to flow from one process to another. Think of it as a virtual tube:

  • One process **writes** data into one end of the tube, `write()`

  • Another process **reads** data from the other end, `read()`

![](pic/pipes_flow.png)

* Pipes should be made before fork()? 
  
  Because to make it shareable between them, we must make it before.



* Pipes is a one way communication between `parent -> child`, if we need otherwise we must open another pipe `child -> parent`.
```C++
buffer[1] ──> [ KERNEL PIPE BUFFER ] ──> buffer[0]
```

```
Process FD table        Kernel space

pipes[0] ─────────────► [ PIPE OBJECT ] ◄───────────── pipes[1]
   (read end)              (buffer)                     (write end)
```


## Questions about pipes
### In pipes Why can't we do buffer[3] or more?

Because:

The kernel only returns two file descriptors
There are only two roles:
read
write

If you do:

```C++
int buffer[3];
pipe(buffer);
```
👉 The kernel will:

`fill buffer[0] and buffer[1],
ignore buffer[2] completely`.

### How does buffer[1] used for write and buffer[0] for read 
* It’s defined behavior of the Unix API.

When you call: 
```C++
int buffer[2];
pipe(buffer)
```
    
The system guarantees: 

* `buffer[0]` -> read end 
* buffer[1] -> write end


### How does pipes fits with fork() 

* After fork():
  
  * Both parent and child inherit both file descriptors.
  * So each process must close the end it doesn’t use.

Child:
```C++
close(pipes[1]); // doesn’t write
read(pipes[0], buffer, sizeof(buffer));
```

> A read() returns EOF (0) only when ALL write ends of the pipe are closed.

Parent: 
```C++
close(pipes[0]); // doesn’t read
write(pipes[1], msg, strlen(msg) + 1);
```


> Forgetting to close pipe end -> Process hangs forever waiting for data.

>Using pipes after closing them → Results in "bad file descriptor" errors


### Why do we need to **close** unused pipe ends in both parent and child processes? What would happen if we didn't close them 
> read() returns 0 (EOF) only when pipe is empty and all write ends (from all processes in the program) are closed.

If you don’t close unused write ends:
The kernel sees: “there is still a writer alive”
So it assumes more data might come
👉 read() will block instead of returning EOF
Result:

Programs that read in a loop can **hang forever**


```C++
while (read(fd, buf, size) > 0) {
    // process data
}
/*

If any process (parent or child) still has the write end open:

read() never gets EOF
Loop never ends → 💥 hang
*/
```

Also:

* Each process inherits both ends after fork().

* If you don’t close:

  * A “reader” process still has a write end
  * A “writer” process still has a read end

This can cause:

- Confusing bugs.
- Hard-to-debug behavior.
- Incorrect assumptions about who is communicating.

### How many possible cases of read() in pipes:

1) Data is already available,
read() returns immediately
You get the data .

2) No data yet, but at least one process has the write fd.
* `read()`: blocks (waits) for other writes to finish (close).
*  Blocking here comes from `read()` itself, not from `wait()` or `sleep()`.

3) No data and no writers left.
* `read()` returns 0 -> EOF

4) There exist data but some processes still has the `write()` fd for the pipe -> return the available data and no blocking will happen unless you call the `read()` again and other processes still has `writer()` fd.

> Pipe works as a FIFO queue (First-In First-Out), write() data on top of the queue, read() remove from the top.

> `read()` removes data: reading consumes (removes) the data from the pipe


### If read() do blocking, why I need to use wait () ?

* They are used for different purposes :

| Function | Waits for…                     |
| -------- | ------------------------------ |
| `read()` | Block if empty and at least one write() still open. |
| `wait()` | Block if there is a **child process still** |

Getting the data ≠ the child has fully finished.


## Task Questions

1) **Explain what happens when fork() is called inside the loop. Why don't the child processes create grandchildren ?**

 *When fork() is called, the operating system creates a child process that is a copy of the parent process. Both parent and child continue execution from the line after fork(). The child receives a return value of 0, while the parent receives the child’s PID.*

*Child processes do not create grandchildren because the child executes:*
```C++
return 0;
```
 *Which terminates the child process before it can continue the loop and call `fork()` again.*

---
2) **Why do we need to close unused pipe ends in both parent and child processes? What would happen if we didn't close them?**

read() returns 0 (EOF) only when all write ends are closed.

If you don’t close unused write ends:

    • The kernel sees: “there is still a writer alive” 
    • So it assumes more data might come 
    • 👉 read() will block instead of returning EOF 
Result:

    • Programs that read in a loop can hang forever.
---
3) **Explain the purpose of the wait(NULL) function call. What would happen if the parent didn't wait for children?**

wait(NULL) makes the parent process wait for a child process to terminate. This allows the parent to properly collect the child’s exit status and prevents **zombie processes**.

If the parent does not wait for children, terminated child processes remain in the process table as zombie processes, wasting system resources.

--- 

4) **Why do we need a separate pipe for each child process? Could we use a single pipe for all children? Explain**

A separate pipe for each child simplifies communication because each child writes to its own dedicated pipe, avoiding interference between messages.

Yes, you can use same pipe for all of them but mixing of data will happened so no sync and Unpredictable behavior will happen, also race conditions.

--- 
5) **What is the purpose of the close(pipes[i][1]) call in the parent process before reading?**

The purpose of `close(pipes[i][1])` in the parent process is to indicate that the parent will not **write to the pipe**. This is important for correct **EOF** behavior, since a `read()` call only returns **EOF** when **all write ends of the pipe are closed**. If the parent **does not close its write end**, the kernel assumes that more data may still be written, which can cause the `read()` operation to **block indefinitely**. Additionally, closing unused pipe ends helps maintain proper resource management and prevents unintended behavior.

--- 
6) **Explain the blocking behavior of read(). What happens if a child process hasn't written its sum yet when the parent calls read()?**

`read()` is a blocking system call.

If the pipe is **empty** and **at least one write end is still open**, `read()` blocks and waits (freeze the process) until data becomes available.
## Next 

* ~~Implement the full solution code according to the template given.~~
* Make a video explaining how and why .

