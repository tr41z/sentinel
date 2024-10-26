import React from 'react'
import InspectorElement from '../components/packet/InspectorElement'

const PacketInspector = ({ flows }) => {
  return (
    <div>
      <InspectorElement flows={flows}/>
    </div>
  )
}

export default PacketInspector
