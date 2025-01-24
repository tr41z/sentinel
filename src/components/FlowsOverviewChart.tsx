import { motion } from 'framer-motion'
import { CartesianGrid, Line, LineChart, ResponsiveContainer, Tooltip, XAxis, YAxis } from 'recharts'

const FlowsOverviewChart = () => {
    const data = [
        { hour: "00:00", flowCount: 34 },
        { hour: "01:00", flowCount: 67 },
        { hour: "02:00", flowCount: 54 },
        { hour: "03:00", flowCount: 23 },
        { hour: "04:00", flowCount: 89 },
        { hour: "05:00", flowCount: 45 },
        { hour: "06:00", flowCount: 78 },
        { hour: "07:00", flowCount: 56 },
        { hour: "08:00", flowCount: 34 },
        { hour: "09:00", flowCount: 90 },
        { hour: "10:00", flowCount: 87 },
        { hour: "11:00", flowCount: 43 },
        { hour: "12:00", flowCount: 21 },
        { hour: "13:00", flowCount: 65 },
        { hour: "14:00", flowCount: 76 },
    ]

  return (
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
                    <CartesianGrid strokeDasharray='3 3' stroke='#4B5563'/>
                    <XAxis dataKey={"hour"}/>
                    <YAxis stroke='#9ca3af'/>
                    <Tooltip
                    contentStyle={{
                        backgroundColor: "rgba(31, 41, 55, 0.8)",
                        borderColor: "4B5563",
                    }}
                    itemStyle={{ color: "#E5E7EB"}}
                    />
                    <Line type='monotone' dataKey='flowCount' stroke='#4cc9f0' strokeWidth={3} dot={{ fill: "#4cc9f0", strokeWidth: 2, r: 6}} activeDot={{ r: 8, strokeWidth: 2 }}/>
                </LineChart>
            </ResponsiveContainer>
        </div>
    </motion.div>
  )
}

export default FlowsOverviewChart