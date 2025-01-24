interface HeaderProps {
    title: string;
}

const Header = ({ title }: HeaderProps) => {
    return (
        <header className='bg-gray-800 bg-opacity-90 backdrop-blur-md shadow-lg border-b border-gray-700'>
            <div className='max-w-7xl mx-auto py-6 sm:px-6 lg:px-8'>
                <h1 className='uppercase tracking-wider text-3xl font-thin text-gray-200 hover:text-gray-50 transition duration-300'>
                    {title}
                </h1>
            </div>
        </header>
    );
};

export default Header;
