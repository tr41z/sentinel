import Header from '../components/Header'
import { motion } from 'framer-motion'
import ModuleStatCard from '../components/ModuleStatCard'
import { HeartPulse } from 'lucide-react'
import { HomePageProps } from '../utils/props'

const HomePage = ({ status, uptime, errorCount }: HomePageProps) => {

  return (
    <div className='flex-1 overflow-auto relative z-10'>
            <Header title="Home"/>

            <main className='max-w-7xl mx-auto py-6 px-4 lg:px-8'>
                {/* Module Stat Cards Section */}
                <motion.div
                    className='grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-2 gap-6 mb-8'
                    initial={{ opacity: 0, y: 20 }}
                    animate={{ opacity: 1, y: 0 }}
                    transition={{ duration: 1 }}
                >
                 <ModuleStatCard 
                    moduleName='Sniffer Module' 
                    icon={HeartPulse} 
                    color='#ffffff'
                    statistics={[
                      {
                        name: "Status",
                        value: status
                      },
                      {
                        name: "Uptime",
                        value: uptime
                      }
                    ]}
                 />
                </motion.div>
            </main>
    </div>
  )
}

export default HomePage