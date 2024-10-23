import { useState, useEffect, useRef } from 'react'
import './App.css'
import { sendBuzzer, sendCoordinates } from './serverCommands'
import { Joystick } from 'react-joystick-component';

function App() {

    const [joyStickX, setJoyStickX] = useState(3)
    const [joyStickY, setJoyStickY] = useState(3                        )
    const [isBuzzerSpinning, setIsBuzzerSpinning] = useState(false)

    const handleMove = (e) => {
        setJoyStickX(Math.round((e.x+1) * 3))
        setJoyStickY(Math.round((e.y+1) * 3.5))
        sendCoordinates(Math.round((e.x+1) * 3), Math.round((e.y+1) * 3.5))
    }

    const handleStop = () => {
        setJoyStickX(3)
        setJoyStickY(3)
        sendCoordinates(3, 3)   
    }

    const handleBuzzer = () => {
        setIsBuzzerSpinning(true)
        console.log("Buzzer")
        sendBuzzer()   
        speakText("Hello this is ISA many time call no answer. You have one urgent message. Please press one for enlish, or 2 for chinese")
        setTimeout(() => {
            setIsBuzzerSpinning(false)
        }, 10000)
    }

    const speakText = (text) => {
        const utterance = new SpeechSynthesisUtterance(text);
        window.speechSynthesis.speak(utterance);
      };
      


  return (
    <>
        <div className='bg-yellow-500 h-screen w-screen p-16 flex flex-col justify-center items-center'>
            <div className='flex justify-center items-center h-fit'>
                <Joystick size={200} sticky={false} baseColor="red" stickColor="blue" move={handleMove} stop={handleStop}></Joystick>
            </div>
            <div className='text-white text-7xl text-center'>
                x:      {joyStickX} y: {joyStickY}
            </div>
            <button 
                className={`mt-10 bg-blue-500 text-white font-bold text-3xl p-2 rounded-md mx-auto ${isBuzzerSpinning ? 'animate-spin' : ''}`} 
                onClick={handleBuzzer}
            >
                Buzzer
            </button>
        </div>
    </>
  )
}

export default App
