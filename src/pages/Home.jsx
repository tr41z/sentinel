import React from 'react'
import Sidebar from '../components/sidebar/Sidebar';
import Homepage from '../components/homepage/Homepage';

const Home = ({ flows }) => {
  return (
    <div className="flex h-[100%]">
      <div className="flex-1">
        <Homepage flows={flows}/>
      </div>
    </div>
  );
}

export default Home
