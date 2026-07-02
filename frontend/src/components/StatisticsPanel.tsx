"use client";

import { Statistics } from "@/types";
import { Clock, Zap, Cpu, RefreshCw, BarChart } from "lucide-react";
import { motion } from "framer-motion";

interface StatCardProps {
  title: string;
  value: string | number;
  icon: React.ReactNode;
  delay: number;
}

function StatCard({ title, value, icon, delay }: StatCardProps) {
  return (
    <motion.div 
      initial={{ opacity: 0, y: 20 }}
      animate={{ opacity: 1, y: 0 }}
      transition={{ duration: 0.5, delay }}
      className="bg-white/5 border border-white/10 rounded-xl p-5 flex items-center gap-4 hover:bg-white/10 transition-colors"
    >
      <div className="p-3 bg-white/5 rounded-lg border border-white/5">
        {icon}
      </div>
      <div>
        <p className="text-gray-400 text-sm">{title}</p>
        <p className="text-2xl font-bold text-white">{value}</p>
      </div>
    </motion.div>
  );
}

export default function StatisticsPanel({ stats }: { stats: Statistics }) {
  if (!stats) return null;

  return (
    <div className="bg-white/5 border border-white/10 backdrop-blur-xl rounded-2xl p-6 shadow-2xl relative overflow-hidden">
      <div className="absolute -right-20 -top-20 w-64 h-64 bg-fuchsia-500/10 rounded-full blur-3xl z-0 pointer-events-none"></div>
      
      <div className="relative z-10">
        <div className="flex items-center gap-2 mb-6">
          <BarChart className="text-fuchsia-400" size={20} />
          <h2 className="text-xl font-semibold text-white">Performance Analytics</h2>
        </div>

        <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-4 gap-4">
          <StatCard 
            title="Avg Waiting Time" 
            value={`${stats.averageWaitingTime.toFixed(2)} ms`} 
            icon={<Clock className="text-amber-400" size={24} />} 
            delay={0.1}
          />
          <StatCard 
            title="Avg Turnaround Time" 
            value={`${stats.averageTurnaroundTime.toFixed(2)} ms`} 
            icon={<Zap className="text-emerald-400" size={24} />} 
            delay={0.2}
          />
          <StatCard 
            title="CPU Utilization" 
            value={`${stats.cpuUtilization.toFixed(1)}%`} 
            icon={<Cpu className="text-blue-400" size={24} />} 
            delay={0.3}
          />
          <StatCard 
            title="Context Switches" 
            value={stats.contextSwitches} 
            icon={<RefreshCw className="text-rose-400" size={24} />} 
            delay={0.4}
          />
        </div>
      </div>
    </div>
  );
}
