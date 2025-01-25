import { useEffect, useMemo, useState } from 'react';
import './App.css'
import { Flow } from './utils/props';
import { Route, Routes } from 'react-router-dom';
import HomePage from './pages/HomePage';
import InspectorPage from './pages/InspectorPage';
import Sidebar from './components/Sidebar';
import DashboardPage from './pages/DashboardPage';

function App() {  
    const [flows, setFlows] = useState<Flow[]>([]);
    const [status, setStatus] = useState("");
    const [uptime, setUptime] = useState(0);
    const [errorCount, setErrorCount] = useState(0);
  
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
      const res = await fetch("http://localhost:8080/api/v1/health");

      if (res.ok) {
        const data = await res.json();
        setStatus(data.status);
        setUptime(data.uptime);
        setErrorCount(data.error_count);
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

    return (
      <div className='flex h-screen bg-[#000814] text-gray-200 overflow-hidden'>

        {/* Background */}
        <div className='fixed inset-0 z-0'>
          <div className='absolute inset-0 bg-gradient-to-b from-gray-900 via-gray-800 to-gray-900 opacity-80'/>
          <div className='absolute inset-0 backdrop-blur-sm'/>
        </div>

        <Sidebar />

        <Routes>
          <Route path='/' element={<HomePage status={status} uptime={uptime} errorCount={errorCount}/>}/>
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
        </Routes>
      </div>
    )
}

export default App
