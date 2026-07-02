# OS Scheduling Simulator 🚀

A comprehensive, production-ready CPU Scheduling Simulator. This project bridges a high-performance **C++17** backend engine with a stunning, animated **Next.js (React + Tailwind)** frontend to visualize how operating systems handle process scheduling.

---

## 📖 Project Overview
CPU scheduling is a process which allows one process to use the CPU while the execution of another process is on hold (in waiting state) due to unavailability of any resource like I/O etc, thereby making full use of CPU. The aim of CPU scheduling is to make the system efficient, fast, and fair.

This simulator models **8 classical OS scheduling algorithms** and provides interactive, animated **Gantt Charts** alongside execution statistics (Average Waiting Time, Turnaround Time, CPU Utilization, and Context Switches).

---

## 🛠️ Technology Stack & Architecture

This project is built using a decoupled architecture, ensuring the computational heavy-lifting is separated from the UI presentation layer.

### 1. The Engine: C++17 (Located in `/backend`)
The core algorithms and data structures are implemented entirely in object-oriented C++.
*   **Why C++?**: C++ provides the raw performance and low-level control required for accurate system-level simulations.
*   **Google Test**: Unit testing is heavily utilized via the `gtest` framework to ensure mathematical accuracy for edge cases (preemptions, idle times).
*   **CMake**: Used as the build system to manage compilation across different platforms.

### 2. The Visual Bridge: Next.js & React (Located in `/frontend`)
The presentation layer is a modern web application built on the Next.js framework.
*   **TypeScript**: Ensures type safety across the application.
*   **Tailwind CSS**: Utilized for rapid, utility-first styling to create a sleek dark-mode, glassmorphic aesthetic.
*   **Framer Motion**: Powers the buttery-smooth micro-animations, particularly the dynamic rendering of the Gantt chart timeline.
*   **Serverless Optimization**: For instant deployment to platforms like Vercel, the C++ algorithms have also been mapped natively into the Next.js API route (`app/api/schedule/route.ts`).

---

## ⚙️ Algorithms Implemented

| Algorithm | Preemptive? | Description |
| :--- | :---: | :--- |
| **First Come First Serve (FCFS)** | No | Processes are dispatched according to their arrival time. Simple but can suffer from the convoy effect. |
| **Shortest Job First (SJF)** | No | Selects the process with the smallest execution time. Optimal for minimizing waiting time. |
| **Shortest Remaining Time First (SRTF)** | Yes | The preemptive version of SJF. If a new process arrives with a shorter burst time than the currently running process, the current process is preempted. |
| **Round Robin (RR)** | Yes | Each process is assigned a fixed time slot (quantum) in a cyclic way. Excellent for time-sharing systems. |
| **Priority Scheduling** | No | Processes are executed based on priority. (Lower number = Higher priority). |
| **Preemptive Priority** | Yes | If a newly arriving process has a higher priority than the currently running process, it preempts it. |
| **Highest Response Ratio Next (HRRN)** | No | Mitigates starvation by factoring in how long a process has been waiting. `Ratio = (Wait + Burst) / Burst` |
| **Multilevel Queue (MLQ)** | Yes | Partitions the ready queue into multiple distinct queues (e.g., Foreground RR, Background FCFS) based on process properties. |

---

## 🚀 How to Run the Project

### Option A: Run the Web UI (Recommended)
This requires Node.js installed on your system.

1. Navigate to the frontend directory:
   ```bash
   cd frontend
   ```
2. Install the necessary dependencies:
   ```bash
   npm install
   ```
3. Start the Next.js development server:
   ```bash
   npm run dev
   ```
4. Open [http://localhost:3000](http://localhost:3000) in your web browser.

### Option B: Compile the C++ Engine Locally
If you wish to interact with the raw C++ engine via CLI or run the Google Tests:

1. Navigate to the backend directory:
   ```bash
   cd backend
   ```
2. Compile the server executable using `g++` (requires MinGW or similar compiler):
   ```bash
   g++ -std=c++14 -I include src/core/*.cpp src/algorithms/*.cpp src/utils/*.cpp src/server.cpp -o OSServer.exe
   ```
3. Execute with arguments (Algorithm, Process String):
   ```bash
   .\OSServer.exe srtf "1,0,4,2;2,1,3,3"
   ```

---

## 📊 Vercel Deployment
This repository is fully optimized for 1-click deployment on Vercel. 
Simply import this Git repository in your Vercel Dashboard, and **ensure you set the "Root Directory" to `frontend`**. Vercel will automatically detect the Next.js configuration and deploy the UI globally.
