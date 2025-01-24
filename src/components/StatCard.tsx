import { motion } from 'framer-motion';
import { LucideIcon } from 'lucide-react';
import clsx from 'clsx';
import { Save } from 'lucide-react';

interface StatCardProps {
  name: string;
  icon: LucideIcon;
  value: number | string;
  color: string;
  iconSize?: number; // Make icon size customizable
}

const StatCard = ({ name, icon: IconComponent, value, color, iconSize = 22 }: StatCardProps) => {
  // Animation configurations for hover
  const hoverEffect = {
    y: -5,
    boxShadow: '0 25px 50px -12px rgba(0, 0, 0, 0.1)',
    scale: 1.05,  // Subtle scale increase
  };

  return (
    <motion.div
      className={clsx(
        'bg-gray-800 bg-opacity-90 backdrop-blur-md overflow-hidden shadow-lg rounded-xl border border-gray-700 relative',  // 'relative' for positioning the button
      )}
      whileHover={hoverEffect}
      transition={{ type: 'spring', stiffness: 400 }}
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
            <IconComponent size={iconSize} className="text-white" style={{ color }} />
          </div>
          {name}
        </span>
        <p className="mt-2 text-2xl font-semibold text-gray-200 text-start">{value}</p>
      </div>
    </motion.div>
  );
};

export default StatCard;
