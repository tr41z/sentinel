import React from 'react'
import Sidebar from '../components/sidebar/Sidebar';
import Homepage from '../components/homepage/Homepage';

const Home = () => {
  return (
    <div className="flex h-screen">
      {/* Main Content Section */}
      <div className="flex-1">
        <Homepage />
      </div>
    </div>
  );
}

export default Home
