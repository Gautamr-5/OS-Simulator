"use client";

import { useState } from "react";
import ProcessInput from "@/components/ProcessInput";
import GanttChart from "@/components/GanttChart";
import StatisticsPanel from "@/components/StatisticsPanel";
import { Process, ScheduleResult } from "@/types";
import { Cpu, Play, Loader2 } from "lucide-react";
import { motion } from "framer-motion";

const algorithms = [
  { id: "fcfs", name: "First Come First Serve" },
  { id: "sjf", name: "Shortest Job First" },
  { id: "srtf", name: "Shortest Remaining Time First" },
  { id: "priority", name: "Priority (Non-Preemptive)" },
  { id: "preemptive_priority", name: "Priority (Preemptive)" },
  { id: "hrrn", name: "Highest Response Ratio Next" },
  { id: "rr", name: "Round Robin" },
  { id: "mlq", name: "Multilevel Queue" },
];

export default function Home() {
  const [processes, setProcesses] = useState<Process[]>([
    { id: 1, arrivalTime: 0, burstTime: 5, priority: 2 },
    { id: 2, arrivalTime: 1, burstTime: 3, priority: 1 },
    { id: 3, arrivalTime: 2, burstTime: 8, priority: 4 },
  ]);

  const [algorithm, setAlgorithm] = useState("fcfs");
  const [quantum, setQuantum] = useState(2);
  const [result, setResult] = useState<ScheduleResult | null>(null);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);

  const handleSimulate = async () => {
    setLoading(true);
    setError(null);
    setResult(null);

    try {
      const res = await fetch("/api/schedule", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ algorithm, processes, quantum }),
      });

      if (!res.ok) {
        throw new Error("Failed to simulate. Please make sure OSServer.exe is built.");
      }

      const data = await res.json();
      setResult(data);
    } catch (err: any) {
      setError(err.message || "An unknown error occurred.");
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="min-h-screen bg-gray-950 text-white font-sans selection:bg-indigo-500/30">
      {/* Background Gradient Orbs */}
      <div className="fixed top-0 left-0 w-full h-full overflow-hidden pointer-events-none z-0">
        <div className="absolute top-[-10%] left-[-10%] w-[40%] h-[40%] rounded-full bg-indigo-600/20 blur-[120px]"></div>
        <div className="absolute bottom-[-10%] right-[-10%] w-[40%] h-[40%] rounded-full bg-fuchsia-600/20 blur-[120px]"></div>
      </div>

      <main className="relative z-10 max-w-6xl mx-auto p-6 space-y-8 pb-20">
        
        {/* Header */}
        <header className="py-8 border-b border-white/10 flex flex-col md:flex-row justify-between items-start md:items-center gap-6">
          <div>
            <h1 className="text-4xl font-bold bg-clip-text text-transparent bg-gradient-to-r from-indigo-400 via-purple-400 to-fuchsia-400 flex items-center gap-3">
              <Cpu className="text-indigo-400" size={36} />
              OS Simulator
            </h1>
            <p className="text-gray-400 mt-2">Production-Level CPU Scheduling Simulator</p>
          </div>

          <div className="flex flex-col sm:flex-row gap-4 items-end sm:items-center bg-white/5 p-4 rounded-xl border border-white/10">
            <div className="flex flex-col">
              <label className="text-xs text-gray-400 mb-1">Algorithm</label>
              <select 
                value={algorithm} 
                onChange={(e) => setAlgorithm(e.target.value)}
                className="bg-black/40 border border-white/10 rounded-lg px-3 py-2 text-white outline-none focus:ring-2 focus:ring-indigo-500 transition-all"
              >
                {algorithms.map(algo => (
                  <option key={algo.id} value={algo.id}>{algo.name}</option>
                ))}
              </select>
            </div>

            {algorithm === "rr" && (
              <div className="flex flex-col w-24">
                <label className="text-xs text-gray-400 mb-1">Quantum</label>
                <input 
                  type="number" 
                  min="1" 
                  value={quantum}
                  onChange={(e) => setQuantum(parseInt(e.target.value) || 1)}
                  className="bg-black/40 border border-white/10 rounded-lg px-3 py-2 text-white outline-none focus:ring-2 focus:ring-indigo-500 transition-all"
                />
              </div>
            )}

            <button 
              onClick={handleSimulate}
              disabled={loading || processes.length === 0}
              className="flex items-center gap-2 bg-gradient-to-r from-indigo-500 to-purple-500 hover:from-indigo-600 hover:to-purple-600 text-white px-6 py-2 rounded-lg font-semibold transition-all shadow-[0_0_20px_rgba(99,102,241,0.4)] disabled:opacity-50 disabled:cursor-not-allowed mt-4 sm:mt-0 h-[42px]"
            >
              {loading ? <Loader2 size={18} className="animate-spin" /> : <Play size={18} />}
              {loading ? "Simulating..." : "Run Simulator"}
            </button>
          </div>
        </header>

        {error && (
          <motion.div initial={{opacity:0}} animate={{opacity:1}} className="bg-red-500/10 border border-red-500/20 text-red-400 p-4 rounded-xl">
            {error}
          </motion.div>
        )}

        {/* Top Section: Process Config */}
        <ProcessInput processes={processes} setProcesses={setProcesses} />

        {/* Results Section */}
        {result && !loading && (
          <motion.div 
            initial={{ opacity: 0, y: 30 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ duration: 0.6 }}
            className="space-y-8 mt-12"
          >
            <div className="flex items-center gap-4">
              <div className="h-px bg-white/10 flex-1"></div>
              <h2 className="text-xl font-light text-gray-400 tracking-widest uppercase">Simulation Results</h2>
              <div className="h-px bg-white/10 flex-1"></div>
            </div>
            
            <div className="text-center mb-8">
              <span className="px-4 py-1.5 rounded-full bg-fuchsia-500/10 border border-fuchsia-500/20 text-fuchsia-300 font-medium text-sm tracking-wide">
                {result.algorithm}
              </span>
            </div>

            <StatisticsPanel stats={result.statistics} />
            
            <GanttChart intervals={result.gantt} />
            
          </motion.div>
        )}

      </main>
    </div>
  );
}
