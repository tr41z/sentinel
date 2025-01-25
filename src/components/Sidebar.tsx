import { BarChart2, Home, Menu, SearchCheck, Settings } from 'lucide-react'
import { useState } from 'react'
import { AnimatePresence, motion } from 'framer-motion'
import { Link } from 'react-router-dom'

const SIDEBAR_ITEMS = [
    { name: "Home", icon: Home, color: "#39FF14", path: "/"},
    { name: "Dashboard", icon: BarChart2, color: "#39FF14", path: "/dashboard" },
    { name: "Inspector", icon: SearchCheck, color: "#39FF14", path: "/flows/inspector"},
    { name: "Settings", icon: Settings, color: "#39FF14", path: "/settings"},
]

const Sidebar = () => {
  const [isSidebarOpen, setIsSidebarOpen] = useState(false);
  return (
    <motion.div className={`relative z-10 transition-all duration-300 ease-in-out flex-shrink-0 ${isSidebarOpen ? 'w-64' : 'w-20'}`}
    animate={{ width: isSidebarOpen ? 256 : 80 }}
    >
        <div className='h-full bg-[#040404] bg-opacity-50 backdrop-blur-md p-4 flex flex-col border-r border-gray-900'>
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
                <AnimatePresence>
                    {isSidebarOpen && (
                        <motion.span
                            className="absolute bottom-7 right-[35%]"
                            initial={{ opacity: 0 }}
                            animate={{ opacity: 1 }}
                            exit={{ opacity: 0 }}
                            transition={{ duration: 0.2, delay: 0.3 }}
                            >
                                <p className='uppercase tracking-widest text-lg'>sentinel</p>
                        </motion.span>
                    )}
                </AnimatePresence>
            </nav>
        </div>
    </motion.div>
  )
}

export default Sidebar