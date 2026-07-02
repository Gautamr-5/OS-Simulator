"use client";

import { Process } from "@/types";
import { Plus, Trash2, Settings } from "lucide-react";
import { motion } from "framer-motion";

interface ProcessInputProps {
  processes: Process[];
  setProcesses: React.Dispatch<React.SetStateAction<Process[]>>;
}

export default function ProcessInput({ processes, setProcesses }: ProcessInputProps) {
  const handleAdd = () => {
    const nextId = processes.length > 0 ? Math.max(...processes.map((p) => p.id)) + 1 : 1;
    setProcesses([
      ...processes,
      { id: nextId, arrivalTime: 0, burstTime: 1, priority: 1 },
    ]);
  };

  const handleRemove = (id: number) => {
    setProcesses(processes.filter((p) => p.id !== id));
  };

  const handleChange = (id: number, field: keyof Process, value: string) => {
    const numValue = parseInt(value) || 0;
    setProcesses(
      processes.map((p) =>
        p.id === id ? { ...p, [field]: numValue >= 0 ? numValue : 0 } : p
      )
    );
  };

  const generateRandom = () => {
    const num = Math.floor(Math.random() * 4) + 3; // 3 to 6 processes
    const newProcs: Process[] = [];
    for (let i = 1; i <= num; i++) {
      newProcs.push({
        id: i,
        arrivalTime: Math.floor(Math.random() * 10),
        burstTime: Math.floor(Math.random() * 8) + 1,
        priority: Math.floor(Math.random() * 5) + 1,
      });
    }
    setProcesses(newProcs.sort((a, b) => a.arrivalTime - b.arrivalTime));
  };

  return (
    <div className="bg-white/5 border border-white/10 backdrop-blur-xl rounded-2xl p-6 shadow-2xl overflow-hidden relative">
      <div className="absolute inset-0 bg-gradient-to-br from-indigo-500/10 to-purple-500/10 z-0"></div>
      
      <div className="relative z-10">
        <div className="flex justify-between items-center mb-6">
          <h2 className="text-xl font-semibold text-white flex items-center gap-2">
            <Settings className="text-indigo-400" size={20} /> Process Configuration
          </h2>
          <div className="flex gap-2">
            <button
              onClick={generateRandom}
              className="px-4 py-2 bg-white/5 hover:bg-white/10 text-sm font-medium text-gray-300 rounded-lg transition-colors border border-white/5"
            >
              Randomize
            </button>
            <button
              onClick={handleAdd}
              className="flex items-center gap-1 px-4 py-2 bg-indigo-500 hover:bg-indigo-600 text-sm font-medium text-white rounded-lg transition-all shadow-[0_0_15px_rgba(99,102,241,0.4)]"
            >
              <Plus size={16} /> Add Process
            </button>
          </div>
        </div>

        <div className="overflow-x-auto">
          <table className="w-full text-left border-collapse">
            <thead>
              <tr className="border-b border-white/10 text-gray-400 text-sm">
                <th className="pb-3 font-medium">Process ID</th>
                <th className="pb-3 font-medium">Arrival Time</th>
                <th className="pb-3 font-medium">Burst Time</th>
                <th className="pb-3 font-medium">Priority (Lower=Higher)</th>
                <th className="pb-3 font-medium text-center">Action</th>
              </tr>
            </thead>
            <tbody>
              {processes.map((p) => (
                <motion.tr 
                  initial={{ opacity: 0, y: 10 }}
                  animate={{ opacity: 1, y: 0 }}
                  exit={{ opacity: 0, scale: 0.9 }}
                  key={p.id} 
                  className="border-b border-white/5 hover:bg-white/5 transition-colors"
                >
                  <td className="py-3">
                    <div className="flex items-center justify-center w-8 h-8 rounded-full bg-indigo-500/20 text-indigo-300 font-bold text-sm">
                      P{p.id}
                    </div>
                  </td>
                  <td className="py-3 pr-4">
                    <input
                      type="number"
                      min="0"
                      value={p.arrivalTime}
                      onChange={(e) => handleChange(p.id, "arrivalTime", e.target.value)}
                      className="w-full bg-black/20 border border-white/10 rounded-lg px-3 py-2 text-white focus:outline-none focus:ring-2 focus:ring-indigo-500 transition-all"
                    />
                  </td>
                  <td className="py-3 pr-4">
                    <input
                      type="number"
                      min="1"
                      value={p.burstTime}
                      onChange={(e) => handleChange(p.id, "burstTime", e.target.value)}
                      className="w-full bg-black/20 border border-white/10 rounded-lg px-3 py-2 text-white focus:outline-none focus:ring-2 focus:ring-indigo-500 transition-all"
                    />
                  </td>
                  <td className="py-3 pr-4">
                    <input
                      type="number"
                      min="1"
                      value={p.priority}
                      onChange={(e) => handleChange(p.id, "priority", e.target.value)}
                      className="w-full bg-black/20 border border-white/10 rounded-lg px-3 py-2 text-white focus:outline-none focus:ring-2 focus:ring-indigo-500 transition-all"
                    />
                  </td>
                  <td className="py-3 text-center">
                    <button
                      onClick={() => handleRemove(p.id)}
                      className="p-2 text-gray-400 hover:text-red-400 hover:bg-red-400/10 rounded-lg transition-colors"
                    >
                      <Trash2 size={18} />
                    </button>
                  </td>
                </motion.tr>
              ))}
            </tbody>
          </table>
          {processes.length === 0 && (
            <div className="text-center py-8 text-gray-400">
              No processes added yet. Click "Add Process" or "Randomize".
            </div>
          )}
        </div>
      </div>
    </div>
  );
}
