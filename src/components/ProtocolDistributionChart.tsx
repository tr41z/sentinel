import { motion } from 'framer-motion';
import { Cell, Legend, Pie, PieChart, ResponsiveContainer, Tooltip } from 'recharts';
import { useMemo } from 'react';
import { Flow } from '../utils/props';

const COLORS = ["#4cc9f0", "#ef233c"];

interface FlowListProps {
    flows: Flow[];
}

const ProtocolDistributionChart = ({ flows }: FlowListProps) => {
    const data = useMemo(() => {
        if (!flows.length) {
            return [
                { name: 'TCP', value: 0 },
                { name: 'UDP', value: 0 }
            ];
        }

        const tcpCount = flows.filter((flow) => flow.protocol === 6).length;
        const udpCount = flows.filter((flow) => flow.protocol === 17).length;
        return [
            { name: 'TCP', value: tcpCount },
            { name: 'UDP', value: udpCount },
        ];
    }, [flows]);

    return (
        <motion.div
            className="bg-gray-800 bg-opacity-50 backdrop-blur-md shadow-lg rounded-xl p-6 border border-gray-700"
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ delay: 0.3 }}
        >
            <h2 className="text-lg font-medium mb-4 text-gray-200">
                Protocol Distribution
            </h2>
            <div className="h-80">
                <ResponsiveContainer width="100%" height="100%">
                    <PieChart aria-label="Protocol Distribution Chart">
                        <Pie
                            data={data}
                            cx="50%"
                            cy="50%"
                            labelLine={false}
                            outerRadius="80%" // Adjust radius dynamically for better responsiveness
                            fill="#8884d8"
                            dataKey="value"
                        >
                            {data.map((_entry, index) => (
                                <Cell key={`cell-${index}`} fill={COLORS[index % COLORS.length]} />
                            ))}
                        </Pie>
                        <Tooltip
                            contentStyle={{
                                backgroundColor: "rgba(31, 41, 55, 0.8)",
                                borderColor: "#4B5563",
                                borderRadius: '8px',
                            }}
                            itemStyle={{ color: "#E5E7EB" }}
                            labelStyle={{ color: '#E5E7EB', fontWeight: 'bold' }} // Improve tooltip label contrast
                        />
                        <Legend
                            iconType="circle"
                            layout="horizontal"
                            verticalAlign="bottom"
                            align="center"
                            wrapperStyle={{
                                paddingTop: 20,
                                paddingBottom: 0,
                            }}
                        />
                    </PieChart>
                </ResponsiveContainer>
            </div>
        </motion.div>
    );
};

export default ProtocolDistributionChart;
