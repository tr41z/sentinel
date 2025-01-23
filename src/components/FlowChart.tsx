import { XAxis, YAxis, CartesianGrid, Tooltip, ResponsiveContainer, AreaChart, Area } from 'recharts';
import { Flow } from '../utils/props';

interface FlowListProps {
  flows: Flow[];
}

const FlowChart = ({ flows }: FlowListProps) => {

const data = [
  { name: 'TCP', uv: flows.filter((flow) => flow.protocol === 6).length },
  { name: 'UDP', uv: flows.filter((flow) => flow.protocol === 17).length },
];

  return (
    <div className="w-full h-96">
      <ResponsiveContainer>
        <AreaChart
            data={data}
            margin={{
              top: 10,
              right: 30,
              left: 0,
              bottom: 0,
            }}
          >
            <CartesianGrid strokeDasharray="3 3" />
            <XAxis dataKey="name" />
            <YAxis />
            <Tooltip />
            <Area type="monotone" dataKey="uv" stroke="#8884d8" fill="#8884d8" />
        </AreaChart>
      </ResponsiveContainer>
    </div>
  );
};

export default FlowChart;
