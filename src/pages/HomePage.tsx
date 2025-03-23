import Header from "../components/Header";
import { motion } from "framer-motion";
import ModuleStatCard from "../components/ModuleStatCard";
import { Bot, HeartPulse } from "lucide-react";
import { HomePageProps } from "../utils/props";
import SnifferAiModChart from "../components/SnifferAiModChart";
import { useEffect, useState } from "react";

const HomePage = ({
  snifferStatus,
  snifferUptime,
  snifferErrorCount,
  flows,
  bandwidth,
  aiStats: newAiStats, // New AI stats from props
}: HomePageProps) => {
  const [aiStats, setAiStats] = useState(newAiStats);

  useEffect(() => {  
    if (newAiStats) {
      setAiStats(prev => ({
        ...prev,
        ...newAiStats,
        threatCount: Math.max(prev?.threatCount ?? 0, newAiStats.threatCount ?? 0),
        ips_flagged: Math.max(prev?.ips_flagged ?? 0, newAiStats.ips_flagged ?? 0),
        error_count: Math.max(prev?.error_count ?? 0, newAiStats.error_count ?? 0),
      }));
    }
  }, [newAiStats])  

  return (
    <div className="flex-1 overflow-auto relative z-10">
      <Header title="Home" />

      <main className="max-w-7xl mx-auto py-6 px-4 lg:px-8">
        {/* Module Stat Cards Section */}
        <motion.div
          className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-2 gap-8 mb-8"
          initial={{ opacity: 0, y: 20 }}
          animate={{ opacity: 1, y: 0 }}
          transition={{ duration: 0.2 }}
        >
          <ModuleStatCard
            moduleName="Network Sniffer Module"
            moduleType="sniffer"            
            icon={HeartPulse}
            color="#39FF14"
            statistics={[
              { name: "Status", value: snifferStatus },
              { name: "Uptime", value: snifferUptime },
              { name: "Flows Processed", value: flows.length },
              { name: "Bandwidth", value: `${bandwidth.toFixed(2)} kbps` },
              { name: "Error Count", value: snifferErrorCount },
            ]}
          />

          <ModuleStatCard
            moduleName="AI Module"
            moduleType="ai"
            icon={Bot}
            color="#ff0000"
            statistics={[
              { name: "Status", value: aiStats?.status || aiStats?.status !== undefined ? aiStats?.status : "Loading..." }, 
              { name: "Uptime", value: aiStats?.uptime ?? "Loading..." }, 
              { name: "IPs Flagged", value: aiStats?.ips_flagged ?? "Loading..." },
              { name: "Threats Detected", value: Number.isInteger(aiStats?.threatCount) ? aiStats?.threatCount : 0 },
              { name: "Error Count", value: aiStats?.error_count ?? "Loading..." },
            ]}
          />
        </motion.div>

        {/* Flows Overview Chart */}
        <SnifferAiModChart flows={flows} aiStats={aiStats} />
      </main>
    </div>
  );
};

export default HomePage;
