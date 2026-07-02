"use client";

import { GanttInterval } from "@/types";
import { motion } from "framer-motion";
import { Activity } from "lucide-react";

interface GanttChartProps {
  intervals: GanttInterval[];
}

// Predictable colors for up to 20 processes
const colors = [
  "from-indigo-500 to-blue-500",
  "from-emerald-500 to-teal-500",
  "from-rose-500 to-pink-500",
  "from-amber-500 to-orange-500",
  "from-purple-500 to-fuchsia-500",
  "from-cyan-500 to-blue-400",
  "from-lime-500 to-green-500",
];

export default function GanttChart({ intervals }: GanttChartProps) {
  if (!intervals || intervals.length === 0) return null;

  const totalTime = intervals[intervals.length - 1].endTime;

  return (
    <div className="bg-white/5 border border-white/10 backdrop-blur-xl rounded-2xl p-6 shadow-2xl relative overflow-hidden">
      <div className="flex items-center gap-2 mb-6">
        <Activity className="text-emerald-400" size={20} />
        <h2 className="text-xl font-semibold text-white">Execution Timeline (Gantt Chart)</h2>
      </div>

      <div className="relative pt-8 pb-4">
        {/* Timeline Bar */}
        <div className="h-16 flex rounded-lg overflow-hidden bg-black/40 shadow-inner">
          {intervals.map((interval, i) => {
            const widthPct = ((interval.endTime - interval.startTime) / totalTime) * 100;
            const isIdle = interval.processId === -1;
            
            const colorClass = isIdle 
              ? "bg-stripes bg-black/40" 
              : `bg-gradient-to-r ${colors[interval.processId % colors.length]}`;

            return (
              <motion.div
                key={i}
                initial={{ width: 0, opacity: 0 }}
                animate={{ width: `${widthPct}%`, opacity: 1 }}
                transition={{ duration: 0.8, delay: i * 0.1, ease: "easeOut" }}
                className={`h-full flex items-center justify-center border-r border-black/20 relative group ${colorClass}`}
              >
                {!isIdle && widthPct > 3 && (
                  <span className="font-bold text-white text-sm drop-shadow-md">
                    P{interval.processId}
                  </span>
                )}
                
                {/* Tooltip */}
                <div className="absolute -top-10 opacity-0 group-hover:opacity-100 transition-opacity bg-gray-900 text-white text-xs px-2 py-1 rounded shadow-lg whitespace-nowrap z-20 pointer-events-none">
                  {isIdle ? "Idle" : `Process ${interval.processId}`} <br/>
                  Time: {interval.startTime} to {interval.endTime}
                </div>
              </motion.div>
            );
          })}
        </div>

        {/* Time markers */}
        <div className="absolute w-full flex h-4 top-[5rem]">
          {intervals.map((interval, i) => {
            const leftPct = (interval.startTime / totalTime) * 100;
            return (
              <div 
                key={`start-${i}`} 
                className="absolute text-xs text-gray-400 transform -translate-x-1/2"
                style={{ left: `${leftPct}%` }}
              >
                {interval.startTime}
              </div>
            );
          })}
          <div 
            className="absolute text-xs text-gray-400 transform -translate-x-1/2"
            style={{ left: `100%` }}
          >
            {totalTime}
          </div>
        </div>
      </div>
    </div>
  );
}
