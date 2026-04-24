# Process vs Threads

# Problem 

In this problem, you will explore process creation, inter-process communication (IPC), and parallel algorithm design by implementing a parallel sum calculator. The problem is deceptively simple: given a large array (vector) of floating-point numbers, compute the total sum. However, instead of computing the sum sequentially in a single loop, you will divide the work among multiple child processes, each summing a portion of the array in parallel.

     This problem touches on core operating system concepts including:
        ◦ Process creation and management (fork())
        ◦ Process synchronization (wait())
        ◦ Inter-process communication (pipes)
        ◦ Resource management (file descriptors)
        ◦ Process termination and cleanup

# Concepts

## Process

- Has its **own memory space** (address space): **Address space** is basically the **range of memory addresses a program (process) is allowed to use**.
- 👉 It’s like each process thinks it owns the whole memory, even though the OS is managing everything behind the scenes.
- Own resources (files, variables, heap, etc.)
- Completely **isolated** from other processes
- Communication between processes is **slower** (uses IPC like pipes, sockets)

👉 Example: Running Chrome and Spotify — each is a separate process.



## Threads

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



## Difference

| Feature       | Process 🧠                 | Thread 🧵                       |
| ------------- | -------------------------- | ------------------------------- |
| Memory        | Separate memory            | Shared memory                   |
| Isolation     | Strong (safe)              | Weak (can affect each other)    |
| Communication | Slow (IPC)                 | Fast (shared data)              |
| Creation cost | Expensive                  | Cheap                           |
| Crash impact  | One process crash ≠ others | One thread crash can affect all |


## The Challenge: Communication Between Processes (pipes)
How do child processes send their back to the parent? Unlike threads, processes do not share memory space. Each process has its own private address space, so a child cannot simply write to a variable in the parent.

This is where **pipes** come in. A pipe is a unidirectional communication channel that allows data to flow from one process to another. Think of it as a virtual tube:
    • One process writes data into one end of the tube
    • Another process reads data from the other end

![](pic/pipes_flow.png)

Why pipes made before fork()? 
