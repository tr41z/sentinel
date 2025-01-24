import { BarChart2, Check, HeartPulse, Menu, SearchCheck, Star, User } from 'lucide-react'
import { useState } from 'react'
import { AnimatePresence, motion } from 'framer-motion'
import { Link } from 'react-router-dom'

const SIDEBAR_ITEMS = [
    { name: "Dashboard", icon: BarChart2, color: "#6366f1", path: "/" },
    { name: "Inspector", icon: SearchCheck, color: "#EC4899", path: "/flows/inspector"},
    { name: "Dummy", icon: Check, color: "#c77dff", path: "/flows/inspector"},
    { name: "Dummy", icon: Star, color: "#2ec4b6", path: "/flows/inspector"},
    { name: "Dummy", icon: HeartPulse, color: "#ffe066", path: "/flows/inspector"},
    { name: "Dummy", icon: User, color: "#c1121f", path: "/flows/inspector"},
]

const Sidebar = () => {
  const [isSidebarOpen, setIsSidebarOpen] = useState(false);
  return (
    <motion.div className={`relative z-10 transition-all duration-300 ease-in-out flex-shrink-0 ${isSidebarOpen ? 'w-64' : 'w-20'}`}
    animate={{ width: isSidebarOpen ? 256 : 80 }}
    >
        <div className='h-full bg-gray-800 bg-opacity-50 backdrop-blur-md p-4 flex flex-col border-r border-gray-700'>
            <motion.button
                whileHover={{ scale: 1.1 }}
                whileTap={{ scale: 0.9 }}
                onClick={() => setIsSidebarOpen(!isSidebarOpen)}
                className='p-2 rounded-full hover:bg-gray-700 transition-colors max-w-fit'
            >
                <Menu size={24}/>
            </motion.button>

            <nav className='mt-8 flex-grow'>
                {SIDEBAR_ITEMS.map((item, _index) => (
                    <Link key={item.path} to={item.path}>
                        <motion.div
                        className='flex items-center p-4 text-sm font-medium rounded-lg hover:bg-gray-700 transition-colors mb-2'
                        >
                            <item.icon size={20} style={{ color: item.color, minWidth: "20px" }}/>

                            <AnimatePresence>
                                {isSidebarOpen && (
                                    <motion.span
                                    className="ml-4 whitespace-nowrap"
                                    initial={{ opacity: 0 }}
                                    animate={{ opacity: 1 }}
                                    exit={{ opacity: 0 }}
                                    transition={{ duration: 0.2, delay: 0.3 }}
                                    >
                                        {item.name}
                                    </motion.span>
                                )}
                            </AnimatePresence>
                        </motion.div>
                    </Link>
                ))}
            </nav>
        </div>
    </motion.div>
  )
}

export default Sidebar