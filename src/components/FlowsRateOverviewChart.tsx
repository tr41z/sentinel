import { motion } from 'framer-motion'
import { CartesianGrid, Line, LineChart, ResponsiveContainer, Tooltip, XAxis, YAxis } from 'recharts'
import { Flow } from '../utils/props';

interface FlowsRateOverviewChartProps {
    flows: Flow[];
}

const FlowsRateOverviewChart = ({ flows }: FlowsRateOverviewChartProps) => {
  const data = flows.reduce((acc: { hour: string; totalBytes: number; flowCount: number }[], flow) => {
    // Convert the Unix timestamp (seconds) to milliseconds by multiplying by 1000
    const date = new Date(flow.last_updated_time * 1000);

    // Use the local hours for grouping
    const hour = date.getHours();
    const hourString = hour < 10 ? `0${hour}:00` : `${hour}:00`;

    // Check if the hour already exists in the accumulator
    const existing = acc.find(item => item.hour === hourString);

    if (existing) {
      existing.totalBytes += flow.total_bytes; // Add bytes to totalBytes for this hour
      existing.flowCount += 1; // Increment flow count for this hour
    } else {
      acc.push({ hour: hourString, totalBytes: flow.total_bytes, flowCount: 1 }); // Initialize with first flow
    }

    return acc;
  }, []);

  // Calculate rate as bytes per hour
  const finalData = data.map(item => ({
    ...item,
    rate: Number((item.totalBytes / 3600).toFixed(1)), // Ensure 'rate' is a number
  }));

  return (
    <motion.div
      className='bg-[#1A1A1A] bg-opacity-50 backdrop-blur-md shadow-lg rounded-xl p-6 border border-gray-900'
      initial={{ opacity: 0, y: 20 }}
      animate={{ opacity: 1, y: 0 }}
      transition={{ delay: 0.2 }}
    >
        <h2 className='text-lg font-medium mb-4 text-gray-200'>
            Flows Overview
        </h2>

        <div className='h-80'>
            <ResponsiveContainer width={"100%"} height={"100%"}>
                <LineChart data={finalData}>
                    <CartesianGrid strokeDasharray='3 3' stroke='#4B5563'/>
                    <XAxis dataKey={"hour"}/>
                    <YAxis stroke='#9ca3af' allowDataOverflow domain={['auto', 'auto']}/>
                    <Tooltip
                    contentStyle={{
                        backgroundColor: "rgba(31, 41, 55, 0.8)",
                        borderColor: "4B5563",
                    }}
                    itemStyle={{ color: "#E5E7EB"}}
                    />
                    <Line type='monotone' dataKey='rate' stroke='#39FF14' strokeWidth={3} dot={{ fill: "#39FF14", strokeWidth: 2, r: 6}} activeDot={{ r: 8, strokeWidth: 2 }} name='Flow Rate (Bytes per Hour)'/>
                </LineChart>
            </ResponsiveContainer>
        </div>
    </motion.div>
  );
}

export default FlowsRateOverviewChart;
