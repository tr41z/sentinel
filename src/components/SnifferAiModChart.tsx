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
  const threatsByHour =
    aiStats?.threats_detected.data && Array.isArray(aiStats.threats_detected.data)
      ? aiStats.threats_detected.data.reduce((acc: Record<string, number>, threat) => {
          const date = new Date(threat.timestamp);
          const hour = date.getHours();
          const hourString = hour < 10 ? `0${hour}:00` : `${hour}:00`;

          acc[hourString] = (acc[hourString] || 0) + 1;
          return acc;
        }, {})
      : {};

  // Transform flow data
  const data = flows.reduce(
    (
      acc: { hour: string; snifferFlows: number; aiPredictions: number }[],
      flow
    ) => {
            
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

  // Determine dynamic Y-axis domains
  const maxSnifferFlows = Math.max(...data.map((d) => d.snifferFlows), 1);
  const maxAiPredictions = Math.max(...data.map((d) => d.aiPredictions), 1);

  return (
    <motion.div
      className="bg-[#1A1A1A] bg-opacity-50 backdrop-blur-md shadow-lg rounded-xl p-8 border border-gray-800"
      initial={{ opacity: 0, y: 20 }}
      animate={{ opacity: 1, y: 0 }}
      transition={{ delay: 0.2 }}
    >
      <h2 className="text-lg font-medium mb-4 text-gray-200">
        Flows & AI Predictions
      </h2>

      <div className="h-80">
        <ResponsiveContainer width="100%" height="100%">
          <LineChart data={data}>
            <CartesianGrid strokeDasharray="3 3" stroke="#4B5563" />
            <XAxis dataKey="hour" />
            
            {/* Left Y-Axis for Sniffer Flows */}
            <YAxis 
              stroke="#9ca3af" 
              yAxisId="left" 
              domain={[0, Math.ceil(maxSnifferFlows * 1.05)]} // Rounds up
              allowDecimals={false} // Ensures only whole numbers
            />
            
            {/* Right Y-Axis for AI Predictions */}
            <YAxis 
              stroke="#9ca3af" 
              yAxisId="right" 
              orientation="right" 
              domain={[0, Math.ceil(maxAiPredictions * 5.5)]} // Rounds up
              allowDecimals={false} // Ensures only whole numbers
            />

            <Tooltip
              contentStyle={{
                backgroundColor: "rgba(31, 41, 55, 0.8)",
                borderColor: "#4B5563",
              }}
              itemStyle={{ color: "#E5E7EB" }}
            />

            {/* Sniffer Flows Line (Left Y-Axis) */}
            <Line
              type="monotone"
              dataKey="snifferFlows"
              name="Flows Captured"
              stroke="#39FF14"
              strokeWidth={3}
              dot={{ fill: "#39FF14", strokeWidth: 2, r: 6 }}
              activeDot={{ r: 8, strokeWidth: 2 }}
              yAxisId="left"
            />

            {/* AI Threat Predictions Line (Right Y-Axis) */}
            <Line
              type="monotone"
              dataKey="aiPredictions"
              name="Threats Detected"
              stroke="#ef233c"
              strokeWidth={3}
              dot={{ fill: "#ef233c", strokeWidth: 2, r: 6 }}
              activeDot={{ r: 8, strokeWidth: 2 }}
              yAxisId="right"
            />
          </LineChart>
        </ResponsiveContainer>
      </div>
    </motion.div>
  );
};

export default SnifferAiModChart;
