import { motion } from "framer-motion";
import { useEffect, useState } from "react";
import {
  CartesianGrid,
  Line,
  LineChart,
  ResponsiveContainer,
  Tooltip,
  XAxis,
  YAxis,
  Legend
} from "recharts";
import { AIStats, Flow } from "../utils/props";

interface SnifferAiModChartProps {
  flows: Flow[];
  aiStats: AIStats;
}

const SnifferAiModChart = ({ flows, aiStats }: SnifferAiModChartProps) => {
  const [aiPredictions, setAiPredictions] = useState(aiStats?.threatCount || 0);

  useEffect(() => {
    if (aiStats?.threatCount !== undefined && aiStats.threatCount > aiPredictions) {
      setAiPredictions(aiStats.threatCount);
    }
  }, [aiStats?.threatCount, aiPredictions]);

  const data = flows.reduce(
    (acc: { hour: string; snifferFlows: number; aiPredictions: number }[], flow) => {
      // Convert Unix timestamp (seconds) to milliseconds
      const date = new Date(flow.last_updated_time * 1000);

      // Group data by local hour
      const hour = date.getHours();
      const hourString = hour < 10 ? `0${hour}:00` : `${hour}:00`;

      // Check if the hour already exists in the accumulator
      const existing = acc.find((item) => item.hour === hourString);

      if (existing) {
        existing.snifferFlows += 1;
        existing.aiPredictions = aiPredictions; // Use stable state
      } else {
        acc.push({ hour: hourString, snifferFlows: 1, aiPredictions });
      }

      return acc;
    },
    []
  );

  return (
    <motion.div
      className="bg-[#1A1A1A] bg-opacity-50 backdrop-blur-md shadow-lg rounded-xl p-6 border border-gray-800"
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
            <Legend />
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
