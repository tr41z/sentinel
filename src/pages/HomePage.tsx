import Header from '../components/Header'
import { motion } from 'framer-motion'
import ModuleStatCard from '../components/ModuleStatCard'
import { Bot, HeartPulse } from 'lucide-react'
import { HomePageProps } from '../utils/props'
import { CartesianGrid, Line, LineChart, ResponsiveContainer, Tooltip, XAxis, YAxis } from 'recharts'

const HomePage = ({ snifferStatus, snifferUptime, snifferErrorCount, flows, bandwidth}: HomePageProps) => {

  const data = [
    { hour: "00:00", snifferFlows: 34, aiThreats: 3 },
    { hour: "01:00", snifferFlows: 67, aiThreats: 5 },
    { hour: "02:00", snifferFlows: 54, aiThreats: 4 },
    { hour: "03:00", snifferFlows: 23, aiThreats: 2 },
    { hour: "04:00", snifferFlows: 89, aiThreats: 8 },
    { hour: "05:00", snifferFlows: 45, aiThreats: 4 },
    { hour: "06:00", snifferFlows: 78, aiThreats: 6 },
    { hour: "07:00", snifferFlows: 56, aiThreats: 3 },
    { hour: "08:00", snifferFlows: 34, aiThreats: 1 },
    { hour: "09:00", snifferFlows: 90, aiThreats: 9 },
    { hour: "10:00", snifferFlows: 87, aiThreats: 7 },
    { hour: "11:00", snifferFlows: 43, aiThreats: 2 },
    { hour: "12:00", snifferFlows: 21, aiThreats: 0 },
    { hour: "13:00", snifferFlows: 65, aiThreats: 4 },
    { hour: "14:00", snifferFlows: 76, aiThreats: 5 },
  ];

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
                        value: `${bandwidth.toFixed(1)} Mbps`
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
                <motion.div
                    className='bg-gray-800 bg-opacity-50 backdrop-blur-md shadow-lg rounded-xl p-6 border border-gray-700'
                    initial= {{ opacity: 0, y: 20 }}
                    animate= {{ opacity: 1, y: 0 }}
                    transition={{ delay: 0.2 }}
                    >
                        <h2 className='text-lg font-medium mb-4 text-gray-200'>
                            Flows Overview
                        </h2>
                
                        <div className='h-80'>
                        <ResponsiveContainer width={"100%"} height={"100%"}>
                          <LineChart data={data}>
                            <CartesianGrid strokeDasharray="3 3" stroke="#4B5563" />
                            <XAxis dataKey="hour" />
                            <YAxis stroke="#9ca3af" />
                            <Tooltip
                              contentStyle={{
                                backgroundColor: "rgba(31, 41, 55, 0.8)",
                                borderColor: "4B5563",
                              }}
                              itemStyle={{ color: "#E5E7EB" }}
                            />
                            <Line
                              type="monotone"
                              dataKey="snifferFlows"
                              name='Flows Processed'
                              stroke="#4cc9f0"
                              strokeWidth={3}
                              dot={{ fill: "#4cc9f0", strokeWidth: 2, r: 6 }}
                              activeDot={{ r: 8, strokeWidth: 2 }}
                            />
                            <Line
                              type="monotone"
                              dataKey="aiThreats"
                              name='Threats Detected'
                              stroke="#ef233c"
                              strokeWidth={3}
                              dot={{ fill: "#ef233c", strokeWidth: 2, r: 6 }}
                              activeDot={{ r: 8, strokeWidth: 2 }}
                            />
                          </LineChart>
                        </ResponsiveContainer>
                        </div>
                    </motion.div>
            </main>
    </div>
  )
}

export default HomePage