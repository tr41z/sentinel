import { Flow } from '../utils/props';

interface FlowListProps {
  flows: Flow[];
}

const FlowList = ({ flows }: FlowListProps) => {
  return (
    <div>{flows.map((flow: any) => {
        return (
            <div key={flow.id} className='flex flex-row'>
                <p>{flow.id}</p>
                <p className='ml-10'>{flow.source_ip}</p>
                <p className='ml-10'>{flow.destination_ip}</p>
            </div>
        )
    })}</div>
  )
}

export default FlowList