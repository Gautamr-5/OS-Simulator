export type Process = {
  id: number;
  arrivalTime: number;
  burstTime: number;
  priority: number;
  completionTime?: number;
  turnaroundTime?: number;
  waitingTime?: number;
  responseTime?: number;
  startTime?: number;
  remainingTime?: number;
};

export type GanttInterval = {
  processId: number; // -1 for idle
  startTime: number;
  endTime: number;
};

export type Statistics = {
  averageWaitingTime: number;
  averageTurnaroundTime: number;
  cpuUtilization: number;
  contextSwitches: number;
  throughput: number;
};

export type ScheduleResult = {
  algorithm: string;
  processes: Process[];
  gantt: GanttInterval[];
  statistics: Statistics;
};
