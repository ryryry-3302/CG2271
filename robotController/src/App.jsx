import { useState, useEffect, useRef } from 'react'
import './App.css'
import { moveForward, moveBackward, turnLeft, turnRight, stopMoving, stopTurning, stop } from './serverCommands.tsx.old'
import { Joystick } from 'react-joystick-component';

function App() {

    const [joyStickX, setJoyStickX] = useState(0)
    const [joyStickY, setJoyStickY] = useState(0)

    const handleMove = (e) => {
        setJoyStickX(Math.round((e.x+1) * 4))
        setJoyStickY(Math.round((e.y+1) * 4))
    }

    const handleStop = () => {
        setJoyStickX(8)
        setJoyStickY(8)
    }

  return (
    <>
        <div className='bg-yellow-500 h-screen w-screen p-16'>
            <div className='flex justify-center items-center h-fit'>
                <Joystick size={333} sticky={false} baseColor="red" stickColor="blue" move={handleMove} stop={handleStop}></Joystick>
            </div>
            <div className='text-white text-7xl text-center'>
                x:      {joyStickX} y: {joyStickY}
            </div>
        </div>
    </>
  )
}

export default App
