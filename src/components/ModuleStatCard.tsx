import { motion } from "framer-motion";
import clsx from "clsx";
import { Dot, Pause } from "lucide-react";
import { ModuleStatCardProps } from "../utils/props";

const ModuleStatCard = ({ moduleName, icon: IconComponent, statistics, color }: ModuleStatCardProps) => {
  const hoverEffect = {
    y: -5,
    boxShadow: "0 25px 50px -12px rgba(0, 0, 0, 0.2)",
    scale: 1.02,
  };

  return (
    <motion.div
      className={clsx(
        "relative bg-gray-800 bg-opacity-90 backdrop-blur-md shadow-2xl rounded-2xl border border-gray-700 overflow-hidden transition-transform duration-300"
      )}
      whileHover={hoverEffect}
    >
      {/* Save Button */}
      <motion.button
        whileHover={{ scale: 1.2 }}
        className="absolute top-3 right-3 p-2 rounded-full bg-gray-700 hover:bg-gray-600 text-white shadow-md z-10"
        aria-label="Save Stat"
      >
        <Pause size={20} />
      </motion.button>

      {/* Card Content */}
      <div className="px-6 py-6 sm:p-8 relative">
        {/* Header */}
        <div className="flex items-center space-x-3">
            <IconComponent size={24} color={color}/>
          <h2 className="text-lg font-semibold text-gray-200">{moduleName}</h2>
        </div>

        {/* Statistics */}
        <div className="space-y-4 mt-6">
          {statistics.map((stat, index) => (
            <div key={index} className="relative">
              <div className="flex justify-between items-center">
                <p className="text-sm font-medium text-gray-300">{stat.name}</p>
                <span className="text-sm font-semibold text-gray-100">{index !== 0 ? stat.value : null}</span>
              </div>

              {/* Dot for status */}
              {index === 0 && (
                <Dot
                  size={45}
                  className={clsx(
                    "absolute -right-4 -top-3",
                    stat.value === "running" ? "text-green-500" : "text-red-500"
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
