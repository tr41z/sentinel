import { useEffect, useMemo, useState } from 'react';
import './App.css'
import { AIStats, Flow } from './utils/props';
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
    const [aiStats, setAiStats] = useState<AIStats>({ 
      status: "", 
      uptime: 0, 
      error_count: 0, 
      ips_flagged: 0, 
      threatCount: 0, 
      threats_detected: { data: [] } 
    });
  
    const fetchFlows = async () => {
      const res = await fetch("http://localhost:8080/api/v1/flows");
      
      if (res.ok) {
          const response = await res.json();
          setFlows(response.data || []); // Extract 'data' field and ensure it's an array
      } else {
          console.error("There was an error while fetching flows!");
      }
    }

    const fetchStats = async () => {
      try {
        // Fetch Sniffer stats
        const snifferRes = await fetch("http://localhost:8080/api/v1/sniffer/health");
        if (snifferRes.ok) {
          const snifferData = await snifferRes.json();
          setSnifferStatus(snifferData.status);
          setSnifferUptime(snifferData.uptime);
          setSnifferErrorCount(snifferData.error_count);
        } else {
          console.error("Error fetching Sniffer health data");
        }
    
        // Fetch AI module stats
        const aiRes = await fetch("http://localhost:8080/api/v1/ai/health");
        if (aiRes.ok) {
          const aiData = await aiRes.json();
          setAiStats(prev => ({ ...prev, ...aiData }));
        } else {
          console.error("Error fetching AI health data");
        }
    
      } catch (error) {
        console.error("Failed to fetch health data:", error);
      }
    
      try {
        const threatRes = await fetch("http://localhost:8080/api/v1/threats");
        const flaggedRes = await fetch("http://localhost:8080/api/v1/flagged_ips");
    
        if (threatRes.ok && flaggedRes.ok) {
          const threatsData = await threatRes.json();
          const flaggedData = await flaggedRes.json();

          setAiStats(prev => ({
            ...prev,
            threats_detected: threatsData,
            ips_flagged: flaggedData.data.length,
            threatCount: Math.max(prev?.threatCount ?? 0, threatsData.data.length)
          }));
    
        } else {
          console.error("Error fetching threats or flagged IPs data");
        }
      } catch (error) {
        console.error("Failed to fetch threats or flagged IPs:", error);
      }
    };
    
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
        (flows || []).reduce((sum, flow) => sum + flow.total_bytes, 0), 
        [flows]
    );

    const totalDuration = useMemo(() => 
      (flows || []).reduce((sum, flow) => sum + flow.duration, 0),
      [flows]
    )

    const avgFlowRate = useMemo(() => {
      const { totalData, totalDuration } = (flows || []).reduce(
          (acc, flow) => {
              acc.totalData += flow.rate * flow.duration; // Total data transferred
              acc.totalDuration += flow.duration;        // Total duration
              return acc;
          },
          { totalData: 0, totalDuration: 0 } // Initial accumulator values
      );
      return totalDuration > 0 ? totalData / totalDuration : 0;
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
            aiStats && (
              <HomePage 
                  snifferStatus={snifferStatus} 
                  snifferUptime={snifferUptime} 
                  snifferErrorCount={snifferErrorCount} 
                  flows={flows}
                  bandwidth={bandwidth}
                  aiStats={aiStats}
                />
            )
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
          <Route path='/flows/inspector' element={<InspectorPage flows={(flows || []).map(flow => ({ 
            id: `${flow.source_ip} -> ${flow.destination_ip}`, 
            src_ip: flow.source_ip, 
            dst_ip: flow.destination_ip, 
            total_bytes: flow.total_bytes, 
            duration: flow.duration, 
            rate: flow.rate 
          }))} />} />
          <Route path='/settings' element={<SettingsPage/>}/>
        </Routes>
      </div>
    )
}

export default App
