import Header from '../components/Header';
import { motion } from 'framer-motion';
import StatCard from '../components/StatCard';
import { Activity, Percent, Weight, Zap } from 'lucide-react';
import FlowsOverviewChart from '../components/FlowsOverviewChart';
import ProtocolDistributionChart from '../components/ProtocolDistributionChart';
import { Flow } from '../utils/props';

interface HomePageProps {
    totalFlows: number;
    avgFlowSize: string;
    avgFlowRate: string;
    totalBytes: number;
    flows: Flow[];
}

const HomePage = ({ totalFlows, avgFlowSize, avgFlowRate, totalBytes, flows }: HomePageProps) => {
    return (
        <div className='flex-1 overflow-auto relative z-10'>
            <Header title="Dashboard" />

            <main className='max-w-7xl mx-auto py-6 px-4 lg:px-8'>
                {/* Stat Cards Section */}
                <motion.div
                    className='grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-4 gap-6 mb-8'
                    initial={{ opacity: 0, y: 20 }}
                    animate={{ opacity: 1, y: 0 }}
                    transition={{ duration: 1 }}
                >
                    <StatCard name="Total Flows" icon={Zap} value={totalFlows} color='#4cc9f0'/>
                    <StatCard name="Average Flow Size" icon={Weight} value={`${avgFlowSize} bytes`} color='#ef233c'/>
                    <StatCard name="Average Flow Rate" icon={Activity} value={`${avgFlowRate} b/s`} color='#38b000'/>
                    <StatCard name="Total Bytes Exchanged" icon={Percent} value={`${totalBytes} bytes`} color='#EC4899'/>
                </motion.div>

                {/* Charts Section */}
                <motion.div
                    className='grid grid-cols-1 lg:grid-cols-2 gap-8 mb-8'
                    initial={{ opacity: 0, y: 20 }}
                    animate={{ opacity: 1, y: 0 }}
                    transition={{ delay: 0.2 }}
                >
                    <FlowsOverviewChart />
                    <ProtocolDistributionChart flows={flows} />
                </motion.div>
            </main>
        </div>
    );
}

export default HomePage;
