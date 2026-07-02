# OS Scheduling Simulator 🚀

A comprehensive, production-ready CPU Scheduling Simulator built with a high-performance **C++17** core backend and a stunning, animated **Next.js (React + Tailwind)** frontend.

## Features

- **8 Classic Algorithms**:
  - First Come First Serve (FCFS)
  - Shortest Job First (SJF)
  - Shortest Remaining Time First (SRTF)
  - Round Robin (RR)
  - Priority (Preemptive & Non-Preemptive)
  - Highest Response Ratio Next (HRRN)
  - Multilevel Queue (MLQ)
- **High-Performance Core**: Written entirely in modern C++ utilizing Object-Oriented patterns, the `Scheduler` interface allows easy extensions.
- **Beautiful Visualizations**: A modern UI built with Next.js and Framer Motion visualizes the exact execution time via an animated Gantt Chart.
- **Vercel Ready**: The Next.js frontend has been optimized with native TypeScript algorithms in its API route to allow instant serverless deployment without a standalone backend server.

---

## Architecture Overview

The repository is divided into two distinct parts to maintain a clean separation of concerns:

### 1. `/backend` (C++ Engine)
Contains the core C++ codebase. This is a standalone project that can be compiled and used directly from the CLI.
- `include/`: Header files defining the `Scheduler` interface and `Process` state.
- `src/`: Implementations of all 8 algorithms.
- `tests/`: Comprehensive Google Test suites ensuring the mathematical accuracy of every algorithm.

### 2. `/frontend` (Next.js UI)
A modern React application that provides an interactive dashboard to input processes and visualize the scheduling algorithms in real-time.
- `src/app/api/schedule/route.ts`: Contains the Vercel-optimized TypeScript ports of the algorithms for serverless execution.
- `src/components/`: Reusable React components (`GanttChart`, `StatisticsPanel`, etc.) powered by Tailwind CSS.

---

## Getting Started

### Running the Frontend locally
1. Navigate into the frontend folder:
   ```bash
   cd frontend
   ```
2. Install dependencies:
   ```bash
   npm install
   ```
3. Start the development server:
   ```bash
   npm run dev
   ```
4. Open [http://localhost:3000](http://localhost:3000) in your browser.

### Compiling the C++ Backend (Optional)
If you want to use the native C++ CLI wrappers instead of the web UI:
1. Navigate to the backend folder:
   ```bash
   cd backend
   ```
2. Build the project using CMake or directly with g++:
   ```bash
   g++ -std=c++14 -I include src/core/*.cpp src/algorithms/*.cpp src/utils/*.cpp src/server.cpp -o OSServer.exe
   ```

## Documentation

Full testing and implementation walkthroughs can be found in the Git history. The project was constructed in 4 phases, focusing first on core infrastructure, advanced preemptive algorithms, data serialization (CSV), and finally the visual web bridge.
