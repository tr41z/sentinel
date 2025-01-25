import Header from "../components/Header";
import { motion } from "framer-motion";
import ModuleStatCard from "../components/ModuleStatCard";
import { Bot, HeartPulse } from "lucide-react";
import { HomePageProps } from "../utils/props";
import SnifferAiModChart from "../components/SnifferAiModChart";

const HomePage = ({
  snifferStatus,
  snifferUptime,
  snifferErrorCount,
  flows,
  bandwidth,
}: HomePageProps) => {
  
  return (
    <div className="flex-1 overflow-auto relative z-10">
      <Header title="Home" />

      <main className="max-w-7xl mx-auto py-6 px-4 lg:px-8">
        {/* Module Stat Cards Section */}
        <motion.div
          className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-2 gap-8 mb-8"
          initial={{ opacity: 0, y: 20 }}
          animate={{ opacity: 1, y: 0 }}
          transition={{ duration: 1 }}
        >
          <ModuleStatCard
            moduleName="Network Sniffer Module"
            icon={HeartPulse}
            color="#39FF14"
            statistics={[
              { name: "Status", value: snifferStatus },
              { name: "Uptime", value: snifferUptime },
              { name: "Flows Processed", value: flows.length },
              { name: "Bandwidth", value: `${bandwidth.toFixed(1)} kbps` },
              { name: "Error Count", value: snifferErrorCount },
            ]}
          />

          <ModuleStatCard
            moduleName="AI Module"
            icon={Bot}
            color="#ff0000"
            statistics={[
              { name: "Status", value: 0 },
              { name: "Uptime", value: 0 },
              { name: "IPs Flagged", value: 0 },
              { name: "Threats Detected", value: 0 },
              { name: "Error Count", value: 0 },
            ]}
          />
        </motion.div>

        {/* Flows Overview Chart */}
        <SnifferAiModChart flows={flows}/>
      </main>
    </div>
  );
};

export default HomePage;
