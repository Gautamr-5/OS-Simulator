import { NextResponse } from "next/server";
import { Process, GanttInterval, Statistics } from "@/types";

// Deep copy helper for resetting process state during algorithms
function cloneProcesses(processes: Process[]): Process[] {
  return processes.map(p => ({
    ...p,
    remainingTime: p.burstTime,
    startTime: -1,
    completionTime: 0,
    turnaroundTime: 0,
    waitingTime: 0,
    responseTime: -1
  }));
}

// Calculate Statistics
function calculateStats(processes: Process[], gantt: GanttInterval[], totalExecutionTime: number, idleTime: number, contextSwitches: number): Statistics {
  let totalWait = 0;
  let totalTurnaround = 0;

  for (const p of processes) {
    totalWait += p.waitingTime || 0;
    totalTurnaround += p.turnaroundTime || 0;
  }

  const n = processes.length;
  const avgWait = n > 0 ? totalWait / n : 0;
  const avgTurn = n > 0 ? totalTurnaround / n : 0;
  const cpuUtil = totalExecutionTime > 0 ? ((totalExecutionTime - idleTime) / totalExecutionTime) * 100 : 0;
  const throughput = totalExecutionTime > 0 ? n / totalExecutionTime : 0;

  return {
    averageWaitingTime: avgWait,
    averageTurnaroundTime: avgTurn,
    cpuUtilization: cpuUtil,
    contextSwitches: contextSwitches,
    throughput: throughput
  };
}

// 1. FCFS
function runFCFS(processes: Process[]) {
  const procs = cloneProcesses(processes);
  procs.sort((a, b) => a.arrivalTime === b.arrivalTime ? a.id - b.id : a.arrivalTime - b.arrivalTime);
  
  const gantt: GanttInterval[] = [];
  let currentTime = 0;
  let idleTime = 0;
  let contextSwitches = 0;

  for (let i = 0; i < procs.length; i++) {
    const p = procs[i];
    if (currentTime < p.arrivalTime) {
      gantt.push({ processId: -1, startTime: currentTime, endTime: p.arrivalTime });
      idleTime += (p.arrivalTime - currentTime);
      currentTime = p.arrivalTime;
    }

    p.startTime = currentTime;
    p.responseTime = p.startTime - p.arrivalTime;
    p.waitingTime = p.startTime - p.arrivalTime;

    gantt.push({ processId: p.id, startTime: currentTime, endTime: currentTime + p.burstTime });
    currentTime += p.burstTime;
    
    p.completionTime = currentTime;
    p.turnaroundTime = p.completionTime - p.arrivalTime;
    p.remainingTime = 0;

    if (i > 0) contextSwitches++;
  }

  return { procs, gantt, currentTime, idleTime, contextSwitches };
}

// 2. SJF
function runSJF(processes: Process[]) {
  const procs = cloneProcesses(processes);
  procs.sort((a, b) => a.arrivalTime === b.arrivalTime ? a.burstTime - b.burstTime : a.arrivalTime - b.arrivalTime);

  const gantt: GanttInterval[] = [];
  let currentTime = 0;
  let completed = 0;
  let idleTime = 0;
  let contextSwitches = 0;
  let lastProcessId = -1;
  const n = procs.length;
  const isCompleted = new Array(n).fill(false);

  while (completed < n) {
    let idx = -1;
    let minBurst = Infinity;

    for (let i = 0; i < n; i++) {
      if (procs[i].arrivalTime <= currentTime && !isCompleted[i]) {
        if (procs[i].burstTime < minBurst) {
          minBurst = procs[i].burstTime;
          idx = i;
        } else if (procs[i].burstTime === minBurst) {
          if (idx === -1 || procs[i].arrivalTime < procs[idx].arrivalTime) idx = i;
        }
      }
    }

    if (idx !== -1) {
      const p = procs[idx];
      p.startTime = currentTime;
      p.responseTime = p.startTime - p.arrivalTime;
      p.waitingTime = p.startTime - p.arrivalTime;

      gantt.push({ processId: p.id, startTime: currentTime, endTime: currentTime + p.burstTime });
      currentTime += p.burstTime;

      p.completionTime = currentTime;
      p.turnaroundTime = p.completionTime - p.arrivalTime;
      p.remainingTime = 0;

      isCompleted[idx] = true;
      completed++;

      if (lastProcessId !== -1 && lastProcessId !== p.id) contextSwitches++;
      lastProcessId = p.id;
    } else {
      let nextArrival = Infinity;
      for (let i = 0; i < n; i++) {
        if (!isCompleted[i] && procs[i].arrivalTime > currentTime) {
          nextArrival = Math.min(nextArrival, procs[i].arrivalTime);
        }
      }
      if (nextArrival !== Infinity) {
        gantt.push({ processId: -1, startTime: currentTime, endTime: nextArrival });
        idleTime += (nextArrival - currentTime);
        currentTime = nextArrival;
      }
    }
  }
  return { procs, gantt, currentTime, idleTime, contextSwitches };
}

// 3. Priority
function runPriority(processes: Process[]) {
  const procs = cloneProcesses(processes);
  procs.sort((a, b) => a.arrivalTime === b.arrivalTime ? a.priority - b.priority : a.arrivalTime - b.arrivalTime);

  const gantt: GanttInterval[] = [];
  let currentTime = 0;
  let completed = 0;
  let idleTime = 0;
  let contextSwitches = 0;
  let lastProcessId = -1;
  const n = procs.length;
  const isCompleted = new Array(n).fill(false);

  while (completed < n) {
    let idx = -1;
    let maxPri = Infinity;

    for (let i = 0; i < n; i++) {
      if (procs[i].arrivalTime <= currentTime && !isCompleted[i]) {
        if (procs[i].priority < maxPri) {
          maxPri = procs[i].priority;
          idx = i;
        } else if (procs[i].priority === maxPri) {
          if (idx === -1 || procs[i].arrivalTime < procs[idx].arrivalTime) idx = i;
        }
      }
    }

    if (idx !== -1) {
      const p = procs[idx];
      p.startTime = currentTime;
      p.responseTime = p.startTime - p.arrivalTime;
      p.waitingTime = p.startTime - p.arrivalTime;

      gantt.push({ processId: p.id, startTime: currentTime, endTime: currentTime + p.burstTime });
      currentTime += p.burstTime;

      p.completionTime = currentTime;
      p.turnaroundTime = p.completionTime - p.arrivalTime;
      p.remainingTime = 0;

      isCompleted[idx] = true;
      completed++;

      if (lastProcessId !== -1 && lastProcessId !== p.id) contextSwitches++;
      lastProcessId = p.id;
    } else {
      let nextArrival = Infinity;
      for (let i = 0; i < n; i++) {
        if (!isCompleted[i] && procs[i].arrivalTime > currentTime) {
          nextArrival = Math.min(nextArrival, procs[i].arrivalTime);
        }
      }
      if (nextArrival !== Infinity) {
        gantt.push({ processId: -1, startTime: currentTime, endTime: nextArrival });
        idleTime += (nextArrival - currentTime);
        currentTime = nextArrival;
      }
    }
  }
  return { procs, gantt, currentTime, idleTime, contextSwitches };
}

// 4. HRRN
function runHRRN(processes: Process[]) {
  const procs = cloneProcesses(processes);
  procs.sort((a, b) => a.arrivalTime === b.arrivalTime ? a.id - b.id : a.arrivalTime - b.arrivalTime);

  const gantt: GanttInterval[] = [];
  let currentTime = 0;
  let completed = 0;
  let idleTime = 0;
  let contextSwitches = 0;
  let lastProcessId = -1;
  const n = procs.length;
  const isCompleted = new Array(n).fill(false);

  while (completed < n) {
    let idx = -1;
    let maxRR = -1;

    for (let i = 0; i < n; i++) {
      if (procs[i].arrivalTime <= currentTime && !isCompleted[i]) {
        const waitTime = currentTime - procs[i].arrivalTime;
        const rr = (waitTime + procs[i].burstTime) / procs[i].burstTime;
        if (rr > maxRR) {
          maxRR = rr;
          idx = i;
        } else if (rr === maxRR) {
          if (idx === -1 || procs[i].arrivalTime < procs[idx].arrivalTime) idx = i;
        }
      }
    }

    if (idx !== -1) {
      const p = procs[idx];
      p.startTime = currentTime;
      p.responseTime = p.startTime - p.arrivalTime;
      p.waitingTime = p.startTime - p.arrivalTime;

      gantt.push({ processId: p.id, startTime: currentTime, endTime: currentTime + p.burstTime });
      currentTime += p.burstTime;

      p.completionTime = currentTime;
      p.turnaroundTime = p.completionTime - p.arrivalTime;
      p.remainingTime = 0;

      isCompleted[idx] = true;
      completed++;

      if (lastProcessId !== -1 && lastProcessId !== p.id) contextSwitches++;
      lastProcessId = p.id;
    } else {
      let nextArrival = Infinity;
      for (let i = 0; i < n; i++) {
        if (!isCompleted[i] && procs[i].arrivalTime > currentTime) {
          nextArrival = Math.min(nextArrival, procs[i].arrivalTime);
        }
      }
      if (nextArrival !== Infinity) {
        gantt.push({ processId: -1, startTime: currentTime, endTime: nextArrival });
        idleTime += (nextArrival - currentTime);
        currentTime = nextArrival;
      }
    }
  }
  return { procs, gantt, currentTime, idleTime, contextSwitches };
}

// 5. Round Robin
function runRR(processes: Process[], quantum: number) {
  const procs = cloneProcesses(processes);
  procs.sort((a, b) => a.arrivalTime === b.arrivalTime ? a.id - b.id : a.arrivalTime - b.arrivalTime);

  const gantt: GanttInterval[] = [];
  let currentTime = 0;
  let completed = 0;
  let idleTime = 0;
  let contextSwitches = 0;
  let lastProcessId = -1;
  const n = procs.length;

  const isInQueue = new Array(n).fill(false);
  const queue: number[] = [];

  let currentProcessIndex = 0;
  while (currentProcessIndex < n && procs[currentProcessIndex].arrivalTime <= currentTime) {
    queue.push(currentProcessIndex);
    isInQueue[currentProcessIndex] = true;
    currentProcessIndex++;
  }

  while (completed < n) {
    if (queue.length > 0) {
      const idx = queue.shift()!;
      const p = procs[idx];

      if (p.startTime === -1) {
        p.startTime = currentTime;
        p.responseTime = p.startTime - p.arrivalTime;
      }

      const executeTime = Math.min(quantum, p.remainingTime!);
      const nextEventTime = currentTime + executeTime;

      if (gantt.length > 0 && gantt[gantt.length - 1].processId === p.id && gantt[gantt.length - 1].endTime === currentTime) {
        gantt[gantt.length - 1].endTime = nextEventTime;
      } else {
        gantt.push({ processId: p.id, startTime: currentTime, endTime: nextEventTime });
        if (lastProcessId !== -1 && lastProcessId !== p.id) contextSwitches++;
      }

      currentTime = nextEventTime;
      p.remainingTime! -= executeTime;
      lastProcessId = p.id;

      while (currentProcessIndex < n && procs[currentProcessIndex].arrivalTime <= currentTime) {
        queue.push(currentProcessIndex);
        isInQueue[currentProcessIndex] = true;
        currentProcessIndex++;
      }

      if (p.remainingTime! > 0) {
        queue.push(idx);
      } else {
        p.completionTime = currentTime;
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;
        completed++;
      }
    } else {
      const nextArrival = procs[currentProcessIndex].arrivalTime;
      gantt.push({ processId: -1, startTime: currentTime, endTime: nextArrival });
      idleTime += (nextArrival - currentTime);
      currentTime = nextArrival;

      while (currentProcessIndex < n && procs[currentProcessIndex].arrivalTime <= currentTime) {
        queue.push(currentProcessIndex);
        isInQueue[currentProcessIndex] = true;
        currentProcessIndex++;
      }
    }
  }
  return { procs, gantt, currentTime, idleTime, contextSwitches };
}

// 6. SRTF
function runSRTF(processes: Process[]) {
  const procs = cloneProcesses(processes);
  procs.sort((a, b) => a.arrivalTime === b.arrivalTime ? a.id - b.id : a.arrivalTime - b.arrivalTime);

  const gantt: GanttInterval[] = [];
  let currentTime = 0;
  let completed = 0;
  let idleTime = 0;
  let contextSwitches = 0;
  let lastProcessId = -1;
  const n = procs.length;
  const isCompleted = new Array(n).fill(false);

  while (completed < n) {
    let idx = -1;
    let minRem = Infinity;

    for (let i = 0; i < n; i++) {
      if (procs[i].arrivalTime <= currentTime && !isCompleted[i]) {
        if (procs[i].remainingTime! < minRem) {
          minRem = procs[i].remainingTime!;
          idx = i;
        } else if (procs[i].remainingTime === minRem) {
          if (idx === -1 || procs[i].arrivalTime < procs[idx].arrivalTime) idx = i;
        }
      }
    }

    if (idx !== -1) {
      const p = procs[idx];
      if (p.startTime === -1) {
        p.startTime = currentTime;
        p.responseTime = p.startTime - p.arrivalTime;
      }

      let nextEventTime = currentTime + p.remainingTime!;
      for (let i = 0; i < n; i++) {
        if (procs[i].arrivalTime > currentTime && procs[i].arrivalTime < nextEventTime && !isCompleted[i]) {
          const remAtArrival = p.remainingTime! - (procs[i].arrivalTime - currentTime);
          if (procs[i].remainingTime! < remAtArrival) {
            nextEventTime = procs[i].arrivalTime;
            break;
          }
        }
      }

      const executeTime = nextEventTime - currentTime;

      if (gantt.length > 0 && gantt[gantt.length - 1].processId === p.id && gantt[gantt.length - 1].endTime === currentTime) {
        gantt[gantt.length - 1].endTime = nextEventTime;
      } else {
        gantt.push({ processId: p.id, startTime: currentTime, endTime: nextEventTime });
        if (lastProcessId !== -1 && lastProcessId !== p.id) contextSwitches++;
      }

      currentTime = nextEventTime;
      p.remainingTime! -= executeTime;
      lastProcessId = p.id;

      if (p.remainingTime === 0) {
        p.completionTime = currentTime;
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;
        isCompleted[idx] = true;
        completed++;
      }
    } else {
      let nextArrival = Infinity;
      for (let i = 0; i < n; i++) {
        if (!isCompleted[i] && procs[i].arrivalTime > currentTime) {
          nextArrival = Math.min(nextArrival, procs[i].arrivalTime);
        }
      }
      if (nextArrival !== Infinity) {
        gantt.push({ processId: -1, startTime: currentTime, endTime: nextArrival });
        idleTime += (nextArrival - currentTime);
        currentTime = nextArrival;
      }
    }
  }
  return { procs, gantt, currentTime, idleTime, contextSwitches };
}

// 7. Preemptive Priority
function runPreemptivePriority(processes: Process[]) {
  const procs = cloneProcesses(processes);
  procs.sort((a, b) => a.arrivalTime === b.arrivalTime ? a.id - b.id : a.arrivalTime - b.arrivalTime);

  const gantt: GanttInterval[] = [];
  let currentTime = 0;
  let completed = 0;
  let idleTime = 0;
  let contextSwitches = 0;
  let lastProcessId = -1;
  const n = procs.length;
  const isCompleted = new Array(n).fill(false);

  while (completed < n) {
    let idx = -1;
    let maxPri = Infinity;

    for (let i = 0; i < n; i++) {
      if (procs[i].arrivalTime <= currentTime && !isCompleted[i]) {
        if (procs[i].priority < maxPri) {
          maxPri = procs[i].priority;
          idx = i;
        } else if (procs[i].priority === maxPri) {
          if (idx === -1 || procs[i].arrivalTime < procs[idx].arrivalTime) idx = i;
        }
      }
    }

    if (idx !== -1) {
      const p = procs[idx];
      if (p.startTime === -1) {
        p.startTime = currentTime;
        p.responseTime = p.startTime - p.arrivalTime;
      }

      let nextEventTime = currentTime + p.remainingTime!;
      for (let i = 0; i < n; i++) {
        if (procs[i].arrivalTime > currentTime && procs[i].arrivalTime < nextEventTime && !isCompleted[i]) {
          if (procs[i].priority < p.priority) {
            nextEventTime = procs[i].arrivalTime;
            break;
          }
        }
      }

      const executeTime = nextEventTime - currentTime;

      if (gantt.length > 0 && gantt[gantt.length - 1].processId === p.id && gantt[gantt.length - 1].endTime === currentTime) {
        gantt[gantt.length - 1].endTime = nextEventTime;
      } else {
        gantt.push({ processId: p.id, startTime: currentTime, endTime: nextEventTime });
        if (lastProcessId !== -1 && lastProcessId !== p.id) contextSwitches++;
      }

      currentTime = nextEventTime;
      p.remainingTime! -= executeTime;
      lastProcessId = p.id;

      if (p.remainingTime === 0) {
        p.completionTime = currentTime;
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;
        isCompleted[idx] = true;
        completed++;
      }
    } else {
      let nextArrival = Infinity;
      for (let i = 0; i < n; i++) {
        if (!isCompleted[i] && procs[i].arrivalTime > currentTime) {
          nextArrival = Math.min(nextArrival, procs[i].arrivalTime);
        }
      }
      if (nextArrival !== Infinity) {
        gantt.push({ processId: -1, startTime: currentTime, endTime: nextArrival });
        idleTime += (nextArrival - currentTime);
        currentTime = nextArrival;
      }
    }
  }
  return { procs, gantt, currentTime, idleTime, contextSwitches };
}

// 8. MLQ
function runMLQ(processes: Process[]) {
  const procs = cloneProcesses(processes);
  procs.sort((a, b) => a.arrivalTime === b.arrivalTime ? a.id - b.id : a.arrivalTime - b.arrivalTime);

  const gantt: GanttInterval[] = [];
  let currentTime = 0;
  let completed = 0;
  let idleTime = 0;
  let contextSwitches = 0;
  let lastProcessId = -1;
  const n = procs.length;

  const isInQueue = new Array(n).fill(false);
  const q1: number[] = [];
  const q2: number[] = [];
  const q3: number[] = [];
  let currentProcessIndex = 0;

  const addToCorrectQueue = (idx: number) => {
    if (procs[idx].priority <= 2) q1.push(idx);
    else if (procs[idx].priority <= 4) q2.push(idx);
    else q3.push(idx);
  };

  while (currentProcessIndex < n && procs[currentProcessIndex].arrivalTime <= currentTime) {
    addToCorrectQueue(currentProcessIndex);
    isInQueue[currentProcessIndex] = true;
    currentProcessIndex++;
  }

  while (completed < n) {
    let idx = -1;
    let maxExecuteTime = Infinity;
    let isRR = false;

    if (q1.length > 0) {
      idx = q1.shift()!;
      maxExecuteTime = 2;
      isRR = true;
    } else if (q2.length > 0) {
      idx = q2.shift()!;
      maxExecuteTime = 4;
      isRR = true;
    } else if (q3.length > 0) {
      idx = q3.shift()!;
      maxExecuteTime = procs[idx].remainingTime!;
      isRR = false;
    }

    if (idx !== -1) {
      const p = procs[idx];
      if (p.startTime === -1) {
        p.startTime = currentTime;
        p.responseTime = p.startTime - p.arrivalTime;
      }

      let executeTime = Math.min(maxExecuteTime, p.remainingTime!);
      let actualExecuteTime = executeTime;

      for (let t = 1; t <= executeTime; t++) {
        let checkTime = currentTime + t;
        let higherPriorityArrived = false;
        while (currentProcessIndex < n && procs[currentProcessIndex].arrivalTime === checkTime) {
          addToCorrectQueue(currentProcessIndex);
          isInQueue[currentProcessIndex] = true;
          
          if (procs[currentProcessIndex].priority <= 2 && p.priority > 2) higherPriorityArrived = true;
          else if (procs[currentProcessIndex].priority <= 4 && p.priority > 4) higherPriorityArrived = true;
          
          currentProcessIndex++;
        }
        if (higherPriorityArrived) {
          actualExecuteTime = t;
          break;
        }
      }

      if (gantt.length > 0 && gantt[gantt.length - 1].processId === p.id && gantt[gantt.length - 1].endTime === currentTime) {
        gantt[gantt.length - 1].endTime = currentTime + actualExecuteTime;
      } else {
        gantt.push({ processId: p.id, startTime: currentTime, endTime: currentTime + actualExecuteTime });
        if (lastProcessId !== -1 && lastProcessId !== p.id) contextSwitches++;
      }

      currentTime += actualExecuteTime;
      p.remainingTime! -= actualExecuteTime;
      lastProcessId = p.id;

      while (currentProcessIndex < n && procs[currentProcessIndex].arrivalTime <= currentTime && !isInQueue[currentProcessIndex]) {
        addToCorrectQueue(currentProcessIndex);
        isInQueue[currentProcessIndex] = true;
        currentProcessIndex++;
      }

      if (p.remainingTime! > 0) {
        if (isRR || p.priority > 2) addToCorrectQueue(idx);
        else q3.push(idx);
      } else {
        p.completionTime = currentTime;
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;
        completed++;
      }
    } else {
      const nextArrival = procs[currentProcessIndex].arrivalTime;
      gantt.push({ processId: -1, startTime: currentTime, endTime: nextArrival });
      idleTime += (nextArrival - currentTime);
      currentTime = nextArrival;

      while (currentProcessIndex < n && procs[currentProcessIndex].arrivalTime <= currentTime) {
        addToCorrectQueue(currentProcessIndex);
        isInQueue[currentProcessIndex] = true;
        currentProcessIndex++;
      }
    }
  }
  return { procs, gantt, currentTime, idleTime, contextSwitches };
}

// Map algorithm strings to names
const algoNames: Record<string, string> = {
  fcfs: "First Come First Serve",
  sjf: "Shortest Job First",
  srtf: "Shortest Remaining Time First",
  priority: "Priority (Non-Preemptive)",
  preemptive_priority: "Priority (Preemptive)",
  hrrn: "Highest Response Ratio Next",
  rr: "Round Robin",
  mlq: "Multilevel Queue"
};

export async function POST(req: Request) {
  try {
    const body = await req.json();
    const { algorithm, processes, quantum } = body;

    if (!algorithm || !processes || !Array.isArray(processes)) {
      return NextResponse.json({ error: "Invalid request payload" }, { status: 400 });
    }

    let result;
    switch (algorithm) {
      case "fcfs": result = runFCFS(processes); break;
      case "sjf": result = runSJF(processes); break;
      case "priority": result = runPriority(processes); break;
      case "hrrn": result = runHRRN(processes); break;
      case "rr": result = runRR(processes, quantum || 2); break;
      case "srtf": result = runSRTF(processes); break;
      case "preemptive_priority": result = runPreemptivePriority(processes); break;
      case "mlq": result = runMLQ(processes); break;
      default: result = runFCFS(processes); break;
    }

    const { procs, gantt, currentTime, idleTime, contextSwitches } = result;
    const stats = calculateStats(procs, gantt, currentTime, idleTime, contextSwitches);

    const algoName = algoNames[algorithm] || "Algorithm";

    return NextResponse.json({
      algorithm: algorithm === "rr" ? `${algoName} (Quantum: ${quantum || 2})` : algoName,
      processes: procs,
      gantt: gantt,
      statistics: stats
    });

  } catch (err: any) {
    console.error("API error:", err);
    return NextResponse.json({ error: "Internal Server Error" }, { status: 500 });
  }
}
