import Header from '../components/Header';
import { motion } from 'framer-motion';
import StatCard from '../components/StatCard';
import { Activity, Percent, Weight, Zap } from 'lucide-react';
import FlowsRateOverviewChart from '../components/FlowsRateOverviewChart';
import ProtocolDistributionChart from '../components/ProtocolDistributionChart';
import { DashboardPageProps } from '../utils/props';

const DashboardPage = ({ totalFlows, avgFlowSize, avgFlowRate, totalBytes, flows }: DashboardPageProps) => {
    const totalBytesInMB = (totalBytes / (1024 * 1024)).toFixed(2);

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
                    <StatCard name="Total Flows" icon={Zap} value={totalFlows} color='#deff1c'/>
                    <StatCard name="Average Flow Size" icon={Weight} value={`${avgFlowSize} bytes`} color='#ff0000'/>
                    <StatCard name="Average Flow Rate" icon={Activity} value={`${avgFlowRate} b/s`} color='#39FF14'/>
                    <StatCard name="Total Size Exchanged" icon={Percent} value={`${totalBytesInMB} MB`} color='#00ffff'/>
                </motion.div>

                {/* Charts Section */}
                <motion.div
                    className='grid grid-cols-1 lg:grid-cols-2 gap-8 mb-8'
                    initial={{ opacity: 0, y: 20 }}
                    animate={{ opacity: 1, y: 0 }}
                    transition={{ delay: 0.2 }}
                >
                    <FlowsRateOverviewChart flows={flows}/>
                    <ProtocolDistributionChart flows={flows} />
                </motion.div>
            </main>
        </div>
    );
}

export default DashboardPage;
