import Header from '../components/Header'
import { motion } from 'framer-motion'
import ModuleStatCard from '../components/ModuleStatCard'
import { Bot, HeartPulse } from 'lucide-react'
import { HomePageProps } from '../utils/props'

const HomePage = ({ snifferStatus, snifferUptime, snifferErrorCount, flows }: HomePageProps) => {

  return (
    <div className='flex-1 overflow-auto relative z-10'>
            <Header title="Home"/>

            <main className='max-w-7xl mx-auto py-6 px-4 lg:px-8'>
                {/* Module Stat Cards Section */}
                <motion.div
                    className='grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-2 gap-8 mb-8'
                    initial={{ opacity: 0, y: 20 }}
                    animate={{ opacity: 1, y: 0 }}
                    transition={{ duration: 1 }}
                >
                 <ModuleStatCard 
                    moduleName='Sniffer Module' 
                    icon={HeartPulse} 
                    color='#4cc9f0'
                    statistics={[
                      {
                        name: "Status",
                        value: snifferStatus
                      },
                      {
                        name: "Uptime",
                        value: snifferUptime
                      },
                      {
                        name: "Flows Processed",
                        value: flows.length
                      },
                      {
                        name: "Bandwidth",
                        value: "10 Mbps"
                      },
                      {
                        name: "Error Count",
                        value: snifferErrorCount
                      }
                    ]}
                 />

                 <ModuleStatCard 
                    moduleName='AI Module' 
                    icon={Bot} 
                    color='#ef233c'
                    statistics={[
                      {
                        name: "Status",
                        value: 0
                      },
                      {
                        name: "Uptime",
                        value: 0
                      },
                      {
                        name: "IPs Flagged",
                        value: 0
                      },
                      {
                        name: "Threats Detected",
                        value: 0
                      },
                      {
                        name: "Error Count",
                        value: 0
                      }
                    ]}
                 />
                </motion.div>
            </main>
    </div>
  )
}

export default HomePage