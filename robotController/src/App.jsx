import { useState, useEffect, useRef } from 'react'
import './App.css'
import { sendBuzzer, sendCode } from './serverCommands'
import { Joystick } from 'react-joystick-component';

function App() {

    const [speed, setSpeed] = useState(0)
    const [angle, setAngle] = useState(0)
    const [joyStickCode, setJoyStickCode] = useState(0)
    const [isBuzzerSpinning, setIsBuzzerSpinning] = useState(false)
    const [serverIP, setServerIP] = useState('')

    const handleMove = (e) => {
        // setspeed(Math.round((e.x+1) * 3))
        // setangle(Math.round((e.y+1) * 3.5))
        sendCode(calculateCode(e.x, e.y), serverIP)
    }

    const sqrt2 = Math.sqrt(2);

    const calculateCode = (x, y) => {
        //a number from 0 to 63
        const speed = Math.sqrt((x * x) + (y * y))/sqrt2;
        const speed_scaled = Math.min(Math.round(speed * 4), 4);
        const angle = Math.atan2(x, -y);
        const percentage_angle = (angle-Math.PI)/(2*Math.PI) * -1;
        const scaled_angle = Math.min(Math.round(percentage_angle * 16), 16);

        const code =  speed_scaled * 16 + scaled_angle
        setJoyStickCode(code)
        setSpeed(speed_scaled)
        setAngle(scaled_angle)
        return code
    }

    const handleStop = () => {
        setSpeed(0)
        setAngle(0)
        sendCode(0, serverIP)   
    }

    const handleBuzzer = () => {
        setIsBuzzerSpinning(true)
        console.log("Buzzer")
        sendBuzzer(serverIP)   
        speakText("Hello this is ISA many time call no answer. You have one urgent message. Please press one for enlish, or 2 for chinese")
        setTimeout(() => {
            setIsBuzzerSpinning(false)
        }, 10000)
    }

    const speakText = (text) => {
        const utterance = new SpeechSynthesisUtterance(text);
        window.speechSynthesis.speak(utterance);
    };
      
    const handleIPChange = (e) => {
        setServerIP(e.target.value)
    }

    return (
        <>
            <div className='bg-yellow-500 h-screen w-screen p-16 flex flex-col justify-center items-center'>
                <input 
                    type="text" 
                    value={serverIP} 
                    onChange={handleIPChange} 
                    placeholder="Enter server IP address"
                    className='mb-4 p-2 text-lg rounded-md'
                />
                <div className='flex justify-center items-center h-fit'>
                    <Joystick size={200} sticky={false} baseColor="red" stickColor="blue" move={handleMove} stop={handleStop}></Joystick>
                </div>
                <div className='text-white text-7xl text-center'>
                    s: {speed} a: {angle} code: {joyStickCode}
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
