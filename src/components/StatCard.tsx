import { motion } from "framer-motion";
import clsx from "clsx";
import { StatCardProps } from "../utils/props";

const StatCard = ({
  name,
  icon: IconComponent,
  value,
  color,
  iconSize = 22,
}: StatCardProps) => {
  // Animation configurations for hover
  const hoverEffect = {
    y: -5,
    boxShadow: "0 25px 50px -12px rgba(0, 0, 0, 0.1)",
    scale: 1.01,
  };

  return (
    <motion.div
      className={clsx(
        "bg-[#1A1A1A] bg-opacity-90 backdrop-blur-md overflow-hidden shadow-lg rounded-xl border border-gray-800 relative" // 'relative' for positioning the button
      )}
      whileHover={hoverEffect}
      transition={{ type: "spring", stiffness: 400 }}
    >
      <div className="px-6 py-6 sm:p-8">
        <span className="flex items-center text-sm font-medium text-gray-200">
          <div className="mr-2">
            <IconComponent
              size={iconSize}
              className="text-white"
              style={{ color }}
            />
          </div>
          {name}
        </span>
        <p className="mt-2 text-2xl font-semibold text-gray-200 text-start">
          {value}
        </p>
      </div>
    </motion.div>
  );
};

export default StatCard;
