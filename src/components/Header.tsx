import { Moon } from "lucide-react";
import { motion } from "framer-motion";

interface HeaderProps {
    title: string;
}

const Header = ({ title }: HeaderProps) => {
    return (
        <header className='bg-[#040404] bg-opacity-90 backdrop-blur-md shadow-lg border-b border-gray-800'>
            <div className='max-w-7xl mx-auto py-6 sm:px-6 lg:px-8'>
                <h1 className='uppercase tracking-wider text-3xl font-thin text-gray-200 hover:text-gray-50 transition duration-300'>
                    {title}
                </h1>
                <motion.button
                    whileHover={{ scale: 1.2 }}
                    className="absolute top-6 right-8 p-2 rounded-full"
                    aria-label="Save Stat"
                >
                    <Moon/>
                </motion.button>
            </div>
        </header>
    );
};

export default Header;
