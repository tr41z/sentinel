import { motion } from "framer-motion";
import {
  CartesianGrid,
  Line,
  LineChart,
  ResponsiveContainer,
  Tooltip,
  XAxis,
  YAxis,
} from "recharts";
import { AIStats, Flow } from "../utils/props";

interface SnifferAiModChartProps {
  flows: Flow[];
  aiStats: AIStats;
}

const SnifferAiModChart = ({ flows, aiStats }: SnifferAiModChartProps) => {
  // Group AI threats by hour
  const threatsByHour = aiStats?.threats_detected && Array.isArray(aiStats.threats_detected)
  ? aiStats.threats_detected.reduce((acc, threat) => {
      const date = new Date(threat.timestamp);
      const hour = date.getHours();
      const hourString = hour < 10 ? `0${hour}:00` : `${hour}:00`;

      acc[hourString] = (acc[hourString] || 0) + 1;
      return acc;
    }, {})
  : {};

  // Transform flow data
  const data = flows.reduce(
    (acc: { hour: string; snifferFlows: number; aiPredictions: number }[], flow) => {
      const date = new Date(flow.last_updated_time * 1000);
      const hour = date.getHours();
      const hourString = hour < 10 ? `0${hour}:00` : `${hour}:00`;
  
      const existing = acc.find((item) => item.hour === hourString);
      if (existing) {
        existing.snifferFlows += 1;
        existing.aiPredictions = threatsByHour[hourString] || 0;
      } else {
        acc.push({
          hour: hourString,
          snifferFlows: 1,
          aiPredictions: threatsByHour[hourString] || 0,
        });
      }
      return acc;
    },
    []
  );  

  // Assign AI threat predictions per hour
  data.forEach((item) => {
    item.aiPredictions = threatsByHour[item.hour] || 0;
  });

  return (
    <motion.div
      className="bg-[#1A1A1A] bg-opacity-50 backdrop-blur-md shadow-lg rounded-xl p-8 border border-gray-800"
      initial={{ opacity: 0, y: 20 }}
      animate={{ opacity: 1, y: 0 }}
      transition={{ delay: 0.2 }}
    >
      <h2 className="text-lg font-medium mb-4 text-gray-200">Flows & AI Predictions</h2>

      <div className="h-80">
        <ResponsiveContainer width="100%" height="100%">
          <LineChart data={data}>
            <CartesianGrid strokeDasharray="3 3" stroke="#4B5563" />
            <XAxis dataKey="hour" />
            <YAxis stroke="#9ca3af" />
            <Tooltip
              contentStyle={{
                backgroundColor: "rgba(31, 41, 55, 0.8)",
                borderColor: "#4B5563",
              }}
              itemStyle={{ color: "#E5E7EB" }}
            />
            <Line
              type="monotone"
              dataKey="snifferFlows"
              name="Sniffer Flows"
              stroke="#39FF14"
              strokeWidth={3}
              dot={{ fill: "#39FF14", strokeWidth: 2, r: 6 }}
              activeDot={{ r: 8, strokeWidth: 2 }}
            />
            <Line
              type="monotone"
              dataKey="aiPredictions"
              name="AI Threat Predictions"
              stroke="#ef233c"
              strokeWidth={3}
              dot={{ fill: "#ef233c", strokeWidth: 2, r: 6 }}
              activeDot={{ r: 8, strokeWidth: 2 }}
            />
          </LineChart>
        </ResponsiveContainer>
      </div>
    </motion.div>
  );
};

export default SnifferAiModChart;
