import { motion } from "framer-motion";
import clsx from "clsx";
import { Dot, Pause, Play } from "lucide-react";
import { ModuleStatCardProps } from "../utils/props";

const ModuleStatCard = ({ moduleName, icon: IconComponent, statistics, color }: ModuleStatCardProps) => {
  const hoverEffect = {
    y: -5,
    boxShadow: "0 25px 50px -12px rgba(0, 0, 0, 0.2)",
    scale: 1.01,
  };

  const toggleSniffer = async (isRunning: boolean) => {
    const endpoint = isRunning ? "/api/v1/sniffer/stop" : "/api/v1/sniffer/start";
    const method = "POST";

    try {
      const response = await fetch(`http://localhost:8080${endpoint}`, {
        method,
        headers: { "Content-Type": "application/json" },
      });

      if (!response.ok) {
        const error = await response.json();
        alert(`Error: ${error.message}`);
      }
    } catch (error) {
      console.error("Failed to toggle sniffer:", error);
    }
  };

  return (
    <motion.div
      className={clsx(
        "relative bg-[#1A1A1A] bg-opacity-90 backdrop-blur-md shadow-2xl rounded-2xl border border-gray-800 overflow-hidden transition-transform duration-300"
      )}
      whileHover={hoverEffect}
    >
      {/* Play/Pause Button */}
      <motion.button
        whileHover={{ scale: 1.2 }}
        className="absolute top-3 right-3 p-2 rounded-full bg-gray-700 hover:bg-gray-600 text-white shadow-md z-10"
        aria-label="Save Stat"
        onClick={() => toggleSniffer(statistics[0]?.value === "running")}
      >
        {statistics.map((stat, index) => (
          <div key={index}>
            {index === 0 && (
              stat.value === "running" ? <Pause size={20} /> : <Play size={20} />
            )}
          </div>
        ))}
      </motion.button>

      {/* Card Content */}
      <div className="px-6 py-6 sm:p-8 relative">
        {/* Header */}
        <div className="flex items-center space-x-3">
          <IconComponent size={24} color={color} />
          <h2 className="text-lg font-semibold text-gray-200">{moduleName}</h2>
        </div>

        {/* Statistics */}
        <div className="space-y-6 mt-6">
          {statistics.map((stat, index) => (
            <div key={index} className="relative">
              <div className="flex justify-between items-center">
                <p className="text-sm font-medium text-gray-300">{stat.name}</p>
                <span className="text-sm font-semibold text-gray-100">
                  {index != 0 ? stat.value : null}
                </span>
              </div>

              {/* Dot for status */}
              {index === 0 && (
                <Dot
                  size={45}
                  className={clsx(
                    "absolute -right-4 -top-3",
                    stat.value === "running"
                      ? "text-green-500"
                      : stat.value === "stopped"
                      ? "text-orange-500"
                      : "text-red-500"
                  )}
                />
              )}
            </div>
          ))}
        </div>
      </div>
    </motion.div>
  );
};

export default ModuleStatCard;
