import { motion } from "framer-motion"
import clsx from 'clsx';
import { Save } from "lucide-react";
import { ModuleStatCardProps } from "../utils/props";

const ModuleStatCard = ({ moduleName, icon: IconComponent, statistics, color }: ModuleStatCardProps) => {
  const hoverEffect = {
    y: -5,
    boxShadow: '0 25px 50px -12px rgba(0, 0, 0, 0.1)',
    scale: 1.01, 
  };
    
  return (
    <motion.div
    className={clsx(
      'bg-gray-800 bg-opacity-90 backdrop-blur-md overflow-hidden shadow-lg rounded-xl border border-gray-700 relative',  // 'relative' for positioning the button
    )}
    whileHover={hoverEffect}
  >
    {/* Save Button */}
    <motion.button
      whileHover={{ scale: 1.2 }}  // Button hover effect
      className="absolute top-3 right-3 p-2 rounded-full bg-gray-700 text-white"
      aria-label="Save Stat"
    >
      <Save size={20} />
    </motion.button>

    <div className="px-6 py-6 sm:p-8">
      <span className="flex items-center text-sm font-medium text-gray-200">
        <div
          className="mr-2"
        >
          <IconComponent size={24} className="text-white" style={{ color }} />
        </div>
        {moduleName}
      </span>
      {statistics.map((stat) => (
        <div className="mt-5 flex">
            <p className="">{stat.name}</p>
            <span>{stat.value}</span>
        </div>
      ))}
    </div>
  </motion.div>
  )
}

export default ModuleStatCard