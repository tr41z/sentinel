import { useEffect, useMemo, useState } from 'react';
import './App.css'
import { Flow } from './utils/props';
import { Route, Routes } from 'react-router-dom';
import HomePage from './pages/HomePage';
import InspectorPage from './pages/InspectorPage';
import Sidebar from './components/Sidebar';
import DashboardPage from './pages/DashboardPage';
import SettingsPage from './pages/SettingsPage';

function App() {  
    const [flows, setFlows] = useState<Flow[]>([]);
    const [snifferStatus, setSnifferStatus] = useState("");
    const [snifferUptime, setSnifferUptime] = useState(0);
    const [snifferErrorCount, setSnifferErrorCount] = useState(0);
  
    const fetchFlows = async () => {
      const res = await fetch("http://localhost:8080/api/v1/flows");
      
      if (res.ok) {
          const data = await res.json();
          setFlows(data);
      } else {
          console.error("There was an error while fetching flows!");
      }
    }

    const fetchStats = async () => {
      const res = await fetch("http://localhost:8080/api/v1/sniffer/health");

      if (res.ok) {
        const data = await res.json();
        setSnifferStatus(data.status);
        setSnifferUptime(data.uptime);
        setSnifferErrorCount(data.error_count);
      } else {
        console.error("There was an error while fetching health data!");
      }
    }

    useEffect(() => {
      const flowsInterval = setInterval(() => {
        fetchFlows();
      }, 4000);

      const healthInterval = setInterval(() => {
        fetchStats();
      }, 1000);

      return () => {
        clearInterval(flowsInterval);
        clearInterval(healthInterval);
      };
    }, [])

    // Calculate totalBytes and other derived values
    const totalFlows = flows.length;
    const totalBytes = useMemo(() => 
        flows.reduce((sum, flow) => sum + flow.total_bytes, 0), 
        [flows]
    );

    const totalDuration = useMemo(() => 
      flows.reduce((sum, flow) => sum + flow.duration, 0),
      [flows]
    )

    const avgFlowRate = useMemo(() => {
      const { totalData, totalDuration } = flows.reduce(
          (acc, flow) => {
              acc.totalData += flow.rate * flow.duration; // Total data transferred
              acc.totalDuration += flow.duration;        // Total duration
              return acc;
          },
          { totalData: 0, totalDuration: 0 } // Initial accumulator values
      );
      return totalDuration > 0 ? totalData / totalDuration : 0; // Avoid divide by zero
  }, [flows]);

    const avgFlowSize = totalFlows > 0 ? totalBytes / totalFlows : 0;
    const bandwidth = totalDuration > 0 ? (totalBytes * 8.0) / (totalDuration * 1000) : 0;

    return (
      <div className='flex h-screen bg-[#040404] text-gray-200 overflow-hidden'>

        {/* Background */}
        <div className='fixed inset-0 z-0'>
          <div className='absolute inset-0 bg-gradient-to-b from-[#040404] via-[#121212] to-gray-[#040404] opacity-80'/>
          <div className='absolute inset-0 backdrop-blur-sm'/>
        </div>

        <Sidebar />

        <Routes>
          <Route path='/' element={
            <HomePage 
                snifferStatus={snifferStatus} 
                snifferUptime={snifferUptime} 
                snifferErrorCount={snifferErrorCount} 
                flows={flows}
                bandwidth={bandwidth}
              />
          }/>
          <Route path='/dashboard' element={
            <DashboardPage 
                totalFlows={totalFlows} 
                avgFlowSize={avgFlowSize.toFixed(2)} 
                avgFlowRate={avgFlowRate.toFixed(2)}
                totalBytes={totalBytes}
                flows={flows}
              />
          }/>
          <Route path='/flows/inspector' element={<InspectorPage/>}/>
          <Route path='/settings' element={<SettingsPage/>}/>
        </Routes>
      </div>
    )
}

export default App
